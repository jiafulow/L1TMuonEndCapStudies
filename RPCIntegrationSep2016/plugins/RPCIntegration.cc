#include <memory>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

#include "TString.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TEfficiency.h"
#include "TTree.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

//#include "DataFormats/L1TMuon/interface/EMTFHit.h"
//#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
//#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
//#include "DataFormats/L1TMuon/interface/EMTFTrackExtra.h"

//#include "DataFormatsSep2016/L1TMuon/interface/EMTFHit.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFHitExtra.h"
//#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrack.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrackExtra.h"

#include "L1TriggerSep2016/L1TMuonEndCap/interface/EMTFTrackTools.hh"


// From L1TriggerSep2016/L1TMuonEndCap/interface/MuonTriggerPrimitive.h
class TriggerPrimitive {
public:
  enum subsystem_type{kDT,kCSC,kRPC,kNSubsystems};
};

namespace l1t = L1TMuonEndCap;

#include "helper.hh"


// _____________________________________________________________________________
// Class declaration
class RPCIntegration : public edm::EDAnalyzer {
public:
  explicit RPCIntegration(const edm::ParameterSet& iConfig);
  ~RPCIntegration();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
  virtual void endJob() override;

  // Main functions
  void makeDPhiPlots();  // make all the dPhi histograms

  // Aux functions
  void getHandles(const edm::Event& iEvent);

  void bookHistograms();
  void writeHistograms();

  // Configurables
  const edm::InputTag emuHitTag_;
  const edm::InputTag emuTrackTag_;
  const edm::InputTag genPartTag_;
  const std::string outFileName_;
  int verbose_;

  // Member data
  edm::EDGetTokenT<l1t::EMTFHitExtraCollection>   emuHitToken_;
  edm::EDGetTokenT<l1t::EMTFTrackExtraCollection> emuTrackToken_;
  edm::EDGetTokenT<reco::GenParticleCollection>   genPartToken_;

  l1t::EMTFHitExtraCollection    emuHits_;
  l1t::EMTFTrackExtraCollection  emuTracks_;
  reco::GenParticleCollection    genParts_;

  std::map<TString, TH1F*> histograms_;
  std::map<TString, TH2F*> histogram2Ds_;
};

// The correction LUT, indexed by [ipair][ifr][ieta]
// Hardcoded
static const float table_common_dphi_ieta[9][16][12] = {{{0}}};

// The correction LUT, indexed by [ipair][ifr][itheta]
// Hardcoded
static const float table_common_dphi_itheta[9][16][16] = {{{0}}};


// _____________________________________________________________________________
RPCIntegration::RPCIntegration(const edm::ParameterSet& iConfig) :
    emuHitTag_    (iConfig.getParameter<edm::InputTag>("emuHitTag")),
    emuTrackTag_  (iConfig.getParameter<edm::InputTag>("emuTrackTag")),
    genPartTag_   (iConfig.getParameter<edm::InputTag>("genPartTag")),
    outFileName_  (iConfig.getParameter<std::string>  ("outFileName")),
    verbose_      (iConfig.getUntrackedParameter<int> ("verbosity"))
{
  emuHitToken_   = consumes<l1t::EMTFHitExtraCollection>  (emuHitTag_);
  emuTrackToken_ = consumes<l1t::EMTFTrackExtraCollection>(emuTrackTag_);
  genPartToken_ = consumes<reco::GenParticleCollection>(genPartTag_);
}

RPCIntegration::~RPCIntegration() {}

// _____________________________________________________________________________
void RPCIntegration::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  getHandles(iEvent);

  makeDPhiPlots();
}

// _____________________________________________________________________________
void RPCIntegration::getHandles(const edm::Event& iEvent) {

  // EMTF hits and tracks
  edm::Handle<decltype(emuHits_)>   emuHits_handle;
  edm::Handle<decltype(emuTracks_)> emuTracks_handle;

  if (!emuHitToken_.isUninitialized()) {
    iEvent.getByToken(emuHitToken_, emuHits_handle);
  }
  if (!emuHits_handle.isValid()) {
    edm::LogError("RPCIntegration") << "Cannot get the product: " << emuHitTag_;
    return;
  }

  if (!emuTrackToken_.isUninitialized()) {
    iEvent.getByToken(emuTrackToken_, emuTracks_handle);
  }
  if (!emuTracks_handle.isValid()) {
    edm::LogError("RPCIntegration") << "Cannot get the product: " << emuTrackTag_;
    return;
  }

  // gen particles
  edm::Handle<decltype(genParts_)> genParts_handle;

  if (!iEvent.isRealData()) {
    if (!genPartToken_.isUninitialized()) {
      iEvent.getByToken(genPartToken_, genParts_handle);
    }
    if (!genParts_handle.isValid()) {
      edm::LogError("RPCIntegration") << "Cannot get the product: " << genPartTag_;
      return;
    }
  }

  // object filters
  emuHits_.clear();
  for (const auto& hit : (*emuHits_handle)) {
    if (!(-1 <= hit.bx && hit.bx <= 1))  continue;  // only BX=[-1,+1]
    if (hit.endcap != 1)  continue;  // only positive endcap
    emuHits_.push_back(hit);
  }

  emuTracks_.clear();
  for (const auto& trk : (*emuTracks_handle)) {
    if (trk.bx != 0)      continue;  // only BX=0
    if (trk.endcap != 1)  continue;  // only positive endcap
    emuTracks_.push_back(trk);
  }

  genParts_.clear();
  for (const auto& part : (*genParts_handle)) {
    //if (!(part.pt() >= 2.))     continue;  // only pT > 2
    if (!(1.2 <= part.eta() && part.eta() <= 2.4))  continue;  // only positive endcap
    genParts_.push_back(part);
  }
}

// ___________________________________________________________________________
// adapted from RecoMuon/DetLayers/src/MuonCSCDetLayerGeometryBuilder.cc
//              RecoMuon/DetLayers/src/MuonRPCDetLayerGeometryBuilder.cc
auto isFront_detail = [](int subsystem, int station, int ring, int chamber, int subsector) {
  bool result = false;

  if (subsystem == TriggerPrimitive::kCSC) {
    bool isOverlapping = !(station == 1 && ring == 3);
    // not overlapping means back
    if(isOverlapping)
    {
      bool isEven = (chamber % 2 == 0);
      // odd chambers are bolted to the iron, which faces
      // forward in 1&2, backward in 3&4, so...
      result = (station < 3) ? isEven : !isEven;
    }
  } else if (subsystem == TriggerPrimitive::kRPC) {
    // 10 degree rings have odd subsectors in front
    result = (subsector % 2 == 0);
  }
  return result;
};

auto isFront = [](const l1t::EMTFHitExtra& hit) {
  return isFront_detail(hit.subsystem, hit.station, hit.ring, hit.chamber, hit.subsector);
};

auto isCSC = [](const l1t::EMTFHitExtra& hit) {
  return (hit.subsystem == TriggerPrimitive::kCSC);
};

auto isRPC = [](const l1t::EMTFHitExtra& hit) {
  return (hit.subsystem == TriggerPrimitive::kRPC);
};

auto get_eta_bin = [](const l1t::EMTFHitExtra& hit) {
  // 12 bins in [1.2,2.4] still using floating point
  double absEta = std::abs(hit.eta);
  if (absEta <  1.2)  absEta = 1.2;
  if (absEta >= 2.4)  absEta = 2.4 * 0.99999999;
  return (static_cast<int>((absEta - 1.2) / (2.4 - 1.2) * 12));
};

auto get_theta_bin = [](const l1t::EMTFHitExtra& hit) {
  // 16 bins in [0,127] integer units
  return (hit.theta_fp/8);
};

// _____________________________________________________________________________
void RPCIntegration::makeDPhiPlots() {
  TString hname;
  TH2F* h2;

  std::random_device rd;
  std::mt19937 genrd(rd());

  bool keep_event = true;

  if (genParts_.empty()) {
    keep_event = false;
  }

  if (keep_event) {
    // gen particle info
    const auto& part = genParts_.front();
    const int    charge = part.charge();
    const double pt     = part.pt();
    const double eta    = part.eta();

    // Keep the hits in a sector processor in myhits.
    std::vector<l1t::EMTFHitExtra> myhits;

    // Loop over sectors
    for (int sector = 1; sector <= 6; ++sector) {
      myhits.clear();
      int mode = 0;

      for (const auto& hit : emuHits_) {
        assert(1 <= hit.sector && hit.sector <= 6);
        assert(1 <= hit.station && hit.station <= 4);

        if (hit.sector == sector && (isCSC(hit) || isRPC(hit))) {
          mode |= (1<<(4-hit.station));
          myhits.push_back(hit);
        }
      }

      // Make deflection angles
      //   ipair 0: dphi(1/1,2)
      //   ipair 1: dphi(1/1,3)
      //   ipair 2: dphi(1/1,4)
      //   ipair 3: dphi(1/2,2)
      //   ipair 4: dphi(1/2,3)
      //   ipair 5: dphi(1/2,4)
      //   ipair 6: dphi(2,3)
      //   ipair 7: dphi(2,4)
      //   ipair 8: dphi(3,4)
      //
      // For each pair, keep the hits in the 'front' station in myhits1;
      // and the hits in the 'back' station in myhits2. 'front' station is
      // the first station in dphi(X,Y) as listed above.
      //
      std::vector<l1t::EMTFHitExtra> myhits1;
      std::vector<l1t::EMTFHitExtra> myhits2;

      // Loop over pairs
      for (int ipair = 0; ipair < 9; ++ipair) {
        myhits1.clear();
        myhits2.clear();

        if (ipair == 0) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 1) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 2) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 3) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 4) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 5) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 6) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 7) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 8) {
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
          std::copy_if(myhits.begin(), myhits.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else {
          assert(false && "invalid ipair");
        }

        // If the pair is valid
        if (myhits1.size() > 0 && myhits2.size() > 0) {

          // There can be more than one hits in a station. Pick one randomly
          std::uniform_int_distribution<> index1(0, myhits1.size()-1);
          std::uniform_int_distribution<> index2(0, myhits2.size()-1);
          l1t::EMTFHitExtra myhit1 = myhits1.at(index1(genrd));
          l1t::EMTFHitExtra myhit2 = myhits2.at(index2(genrd));
          assert(isCSC(myhit1) || isRPC(myhit1));
          assert(isCSC(myhit2) || isRPC(myhit2));

          // Find ifr index
          int ifr = 0;
          ifr |= isCSC(myhit1);
          ifr <<= 1;
          ifr |= isFront(myhit1);
          ifr <<= 1;
          ifr |= isCSC(myhit2);
          ifr <<= 1;
          ifr |= isFront(myhit2);
          assert(ifr < 16);

          // Find ieta index using the hit in 'front' station
          int ieta = get_eta_bin(myhit1);
          assert(ieta < 12);

          // Find itheta index
          int itheta = get_theta_bin(myhit1);
          assert(itheta < 16);

          // Find ipt index
          int ipt = -1;
          if ((1.0/2 - 0.01) < 1.0/pt && 1.0/pt <= (1.0/2)) {
            ipt = 0;
          } else if ((1.0/3 - 0.01) < 1.0/pt && 1.0/pt <= (1.0/3)) {
            ipt = 1;
          } else if ((1.0/5 - 0.01) < 1.0/pt && 1.0/pt <= (1.0/5)) {
            ipt = 2;
          } else if ((1.0/10 - 0.01) < 1.0/pt && 1.0/pt <= (1.0/10)) {
            ipt = 3;
          } else if ((1.0/20 - 0.01) < 1.0/pt && 1.0/pt <= (1.0/20)) {
            ipt = 4;
          } else if ((1.0/50 - 0.005) < 1.0/pt && 1.0/pt <= (1.0/50)) {
            ipt = 5;
          } else if ((1.0/100 - 0.002) < 1.0/pt && 1.0/pt <= (1.0/100)) {
            ipt = 6;
          } else if ((1.0/200 - 0.001) < 1.0/pt && 1.0/pt <= (1.0/200)) {
            ipt = 7;
          }

          // Find dphi
          //int dphi = myhit1.phi_fp - myhit2.phi_fp;
          int dphi = l1t::calc_phi_loc_int(myhit1.phi_glob_deg, sector) - l1t::calc_phi_loc_int(myhit2.phi_glob_deg, sector);

          // Reverse dphi if positive muon
          if (charge > 0)  dphi = -dphi;

          //const float common_dphi_ratio = table_common_dphi_ieta[ipair][ifr][ieta];
          //const float common_dphi_ratio = table_common_dphi_itheta[ipair][ifr][itheta];
          const float common_dphi_ratio = 1.0;  //FIXME
          const int common_dphi = static_cast<int>(std::round(1.0 / common_dphi_ratio * dphi));

          // dphi vs particle 1/pT
          hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_stp%i_frp%i_eta%i", ipair, 16, ieta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_stp%i_frp%i_theta%i", ipair, ifr, itheta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_stp%i_frp%i_theta%i", ipair, 16, itheta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);

          // corrected dphi vs particle 1/pT
          hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, 16, ieta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_stp%i_frp%i_theta%i", ipair, ifr, itheta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_stp%i_frp%i_theta%i", ipair, 16, itheta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);

          if (ipt != -1) {
            // dphi vs particle |eta|
            hname = Form("deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(eta), dphi);
            hname = Form("deflection_stp%i_frp%i_pt%i", ipair, 16, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(eta), dphi);

            // corrected dphi vs particle |eta|
            hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(eta), common_dphi);
            hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, 16, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(eta), common_dphi);
          }

        }  // end if the pair is valid
      }  // end loop over pairs
    }  // end loop over sectors
  }  // end if keep_event

}

// _____________________________________________________________________________
void RPCIntegration::beginJob() {
  bookHistograms();
}

void RPCIntegration::endJob() {
  writeHistograms();
}

// _____________________________________________________________________________
void RPCIntegration::bookHistograms() {
  TString hname;
  TH2F* h2;

  TString deflection_labels1[] = {
      "#phi(1/1) - #phi(2/n)",
      "#phi(1/1) - #phi(3/n)",
      "#phi(1/1) - #phi(4/n)",
      "#phi(1/2) - #phi(2/n)",
      "#phi(1/2) - #phi(3/n)",
      "#phi(1/2) - #phi(4/n)",
      "#phi(2/n) - #phi(3/n)",
      "#phi(2/n) - #phi(4/n)",
      "#phi(3/n) - #phi(4/n)"
  };

  TString deflection_labels2[] = {
      "RR+RR",
      "RR+RF",
      "RR+MR",
      "RR+MF",
      "RF+RR",
      "RF+RF",
      "RF+MR",
      "RF+MF",
      "MR+RR",
      "MR+RF",
      "MR+MR",
      "MR+MF",
      "RF+RR",
      "RF+RF",
      "RF+MR",
      "MF+MF",
      "all"
  };

  TString deflection_labels3[] = {
      "1.2#leq|#eta|<1.3",
      "1.3#leq|#eta|<1.4",
      "1.4#leq|#eta|<1.5",
      "1.5#leq|#eta|<1.6",
      "1.6#leq|#eta|<1.7",
      "1.7#leq|#eta|<1.8",
      "1.8#leq|#eta|<1.9",
      "1.9#leq|#eta|<2.0",
      "2.0#leq|#eta|<2.1",
      "2.1#leq|#eta|<2.2",
      "2.2#leq|#eta|<2.3",
      "2.3#leq|#eta|<2.4"
  };

  TString deflection_labels4[] = {
      "0#leq#theta_{FW}<8",
      "8#leq#theta_{FW}<16",
      "16#leq#theta_{FW}<24",
      "24#leq#theta_{FW}<32",
      "32#leq#theta_{FW}<40",
      "40#leq#theta_{FW}<48",
      "48#leq#theta_{FW}<56",
      "56#leq#theta_{FW}<64",
      "64#leq#theta_{FW}<72",
      "72#leq#theta_{FW}<80",
      "80#leq#theta_{FW}<88",
      "88#leq#theta_{FW}<96",
      "96#leq#theta_{FW}<104",
      "104#leq#theta_{FW}<112",
      "112#leq#theta_{FW}<120",
      "120#leq#theta_{FW}<128"
  };

  TString deflection_labels5[] = {
      "2#leqp_{T}<2.04",
      "3#leqp_{T}<3.09",
      "5#leqp_{T}<5.26",
      "10#leqp_{T}<11.1",
      "20#leqp_{T}<25.0",
      "50#leqp_{T}<66.7",
      "100#leqp_{T}<125",
      "200#leqp_{T}<250"
  };

  for (int ipair=0; ipair<9; ++ipair) {
    for (int ifr=0; ifr<(16+1); ++ifr) {

      // dphi vs 1/pT
      for (int ieta=0; ieta<12; ++ieta) {
        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = new TH2F(hname, TString("; 1/p_{T} [1/GeV]; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels3[ieta]+"}", 50, 0., 0.5, 808, -808, 808);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }

      for (int itheta=0; itheta<16; ++itheta) {
        hname = Form("deflection_stp%i_frp%i_theta%i", ipair, ifr, itheta);
        h2 = new TH2F(hname, TString("; 1/p_{T} [1/GeV]; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels4[itheta]+"}", 50, 0., 0.5, 808, -808, 808);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_stp%i_frp%i_theta%i", ipair, ifr, itheta);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }

      // dphi vs |eta|
      for (int ipt=0; ipt<8; ++ipt) {
        hname = Form("deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
        h2 = new TH2F(hname, TString("; |#eta|; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels5[ipt]+"}", 48, 1.2, 2.4, 808, -808, 808);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }
    }
  }
}

void RPCIntegration::writeHistograms() {
  TString hname;

  TFile* f = TFile::Open(outFileName_.c_str(), "RECREATE");
  for (const auto& kv : histograms_) {
    kv.second->SetDirectory(gDirectory);
  }
  for (const auto& kv : histogram2Ds_) {
    kv.second->SetDirectory(gDirectory);
  }
  f->Write();
  f->Close();
}

// _____________________________________________________________________________
void RPCIntegration::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

// _____________________________________________________________________________
// Define this as a plug-in
typedef RPCIntegration RPCIntegrationSep2016;
DEFINE_FWK_MODULE(RPCIntegrationSep2016);
