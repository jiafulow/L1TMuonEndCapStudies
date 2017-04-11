#include <memory>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cstdint>

#include "TString.h"
#include "TFile.h"
//#include "TH1F.h"
//#include "TH2F.h"
//#include "TEfficiency.h"
#include "TTree.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

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
class RPCIntegration2 : public edm::EDAnalyzer {
public:
  explicit RPCIntegration2(const edm::ParameterSet& iConfig);
  ~RPCIntegration2();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
  virtual void endJob() override;

  // Main functions
  void makeDPhiPlots();  // make all the dPhi histograms

  // Aux functions
  void getHandles(const edm::Event& iEvent);

  void makeTree();
  void writeTree();

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

  // TTree
  TTree* tree;

  std::unique_ptr<int32_t>  v_dphi;
  std::unique_ptr<int32_t>  v_cdphi;
  std::unique_ptr<int16_t>  v_ipair;
  std::unique_ptr<int16_t>  v_ifr;
  std::unique_ptr<int16_t>  v_ieta;
  std::unique_ptr<int16_t>  v_itheta;
  std::unique_ptr<int16_t>  v_ipt;
  std::unique_ptr<float  >  v_eta;
  std::unique_ptr<float  >  v_theta;
  std::unique_ptr<float  >  v_invPt;
};


// _____________________________________________________________________________
RPCIntegration2::RPCIntegration2(const edm::ParameterSet& iConfig) :
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

RPCIntegration2::~RPCIntegration2() {}

// _____________________________________________________________________________
void RPCIntegration2::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  getHandles(iEvent);

  makeDPhiPlots();
}

// _____________________________________________________________________________
void RPCIntegration2::getHandles(const edm::Event& iEvent) {

  // EMTF hits and tracks
  edm::Handle<decltype(emuHits_)>   emuHits_handle;
  edm::Handle<decltype(emuTracks_)> emuTracks_handle;

  if (!emuHitToken_.isUninitialized()) {
    iEvent.getByToken(emuHitToken_, emuHits_handle);
  }
  if (!emuHits_handle.isValid()) {
    edm::LogError("RPCIntegration2") << "Cannot get the product: " << emuHitTag_;
    return;
  }

  if (!emuTrackToken_.isUninitialized()) {
    iEvent.getByToken(emuTrackToken_, emuTracks_handle);
  }
  if (!emuTracks_handle.isValid()) {
    edm::LogError("RPCIntegration2") << "Cannot get the product: " << emuTrackTag_;
    return;
  }

  // gen particles
  edm::Handle<decltype(genParts_)> genParts_handle;

  if (!iEvent.isRealData()) {
    if (!genPartToken_.isUninitialized()) {
      iEvent.getByToken(genPartToken_, genParts_handle);
    }
    if (!genParts_handle.isValid()) {
      edm::LogError("RPCIntegration2") << "Cannot get the product: " << genPartTag_;
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

auto get_eta_bin = [](float eta) {
  // 12 bins in [1.2,2.4] still using floating point
  double absEta = std::abs(eta);
  if (absEta <  1.2)  absEta = 1.2;
  if (absEta >= 2.4)  absEta = 2.4 * 0.99999999;
  return (static_cast<int>((absEta - 1.2) / (2.4 - 1.2) * 12));
};

auto get_theta_bin = [](int theta) {
  // 16 bins in [0,127] integer units
  return (theta/8);
};

// _____________________________________________________________________________
void RPCIntegration2::makeDPhiPlots() {
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
    //const double eta    = part.eta();

    // Keep the hits and tracks in a sector processor
    std::vector<l1t::EMTFHitExtra> myhits;
    std::vector<l1t::EMTFTrackExtra> mytracks;

    // Loop over sectors
    for (int sector = 1; sector <= 6; ++sector) {
      myhits.clear();
      mytracks.clear();

      int mode = 0;

      for (const auto& hit : emuHits_) {
        assert(1 <= hit.sector && hit.sector <= 6);
        assert(1 <= hit.station && hit.station <= 4);

        if (hit.sector == sector && (isCSC(hit) || isRPC(hit))) {
          mode |= (1<<(4-hit.station));
          myhits.push_back(hit);
        }
      }

      for (const auto& track : emuTracks_) {
        assert(1 <= track.sector && track.sector <= 6);

        if (track.sector == sector) {
          mytracks.push_back(track);
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

      // Loop over pairs (only when there is a track)
      for (int ipair = 0; (mytracks.size() > 0) && (ipair < 9); ++ipair) {
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

          // Pick the first track
          const l1t::EMTFTrackExtra& in_track = mytracks.front();
          int theta_int = in_track.theta_int;
          float theta_angle = l1t::calc_theta_rad_from_int( theta_int );
          float eta = l1t::calc_eta_from_theta_rad( theta_angle );
          //eta = (in_track.endcap == 1) ? eta : -eta;

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
          int ieta = get_eta_bin(eta);
          assert(ieta < 12);

          // Find itheta index
          int itheta = get_theta_bin(theta_int);
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

          const float common_dphi_ratio = 1.0;  //FIXME
          //const float common_dphi_ratio = table_common_dphi_ieta[ipair][ifr][ieta];
          //const float common_dphi_ratio = table_common_dphi_itheta[ipair][ifr][itheta];
          const int common_dphi = static_cast<int>(std::round(1.0 / common_dphi_ratio * dphi));

          *v_dphi   = dphi;
          *v_cdphi  = common_dphi;
          *v_ipair  = ipair;
          *v_ifr    = ifr;
          *v_ieta   = ieta;
          *v_itheta = itheta;
          *v_ipt    = ipt;
          *v_eta    = eta;
          *v_theta  = theta_angle;
          *v_invPt  = 1.0/pt;

          tree->Fill();

        }  // end if the pair is valid
      }  // end loop over pairs
    }  // end loop over sectors
  }  // end if keep_event

}

// _____________________________________________________________________________
void RPCIntegration2::beginJob() {
  makeTree();
}

void RPCIntegration2::endJob() {
  writeTree();
}

// _____________________________________________________________________________
void RPCIntegration2::makeTree() {
  // Create TTree
  edm::Service<TFileService> fs;
  tree = fs->make<TTree>("tree", "tree");

  v_dphi  .reset(new int32_t(0));
  v_cdphi .reset(new int32_t(0));
  v_ipair .reset(new int16_t(0));
  v_ifr   .reset(new int16_t(0));
  v_ieta  .reset(new int16_t(0));
  v_itheta.reset(new int16_t(0));
  v_ipt   .reset(new int16_t(0));
  v_eta   .reset(new float(0.) );
  v_theta .reset(new float(0.) );
  v_invPt .reset(new float(0.) );

  tree->Branch("dphi"  , &(*v_dphi  ));
  tree->Branch("cdphi" , &(*v_cdphi ));
  tree->Branch("ipair" , &(*v_ipair ));
  tree->Branch("ifr"   , &(*v_ifr   ));
  tree->Branch("ieta"  , &(*v_ieta  ));
  tree->Branch("itheta", &(*v_itheta));
  tree->Branch("ipt"   , &(*v_ipt   ));
  tree->Branch("eta"   , &(*v_eta   ));
  tree->Branch("theta" , &(*v_theta ));
  tree->Branch("invPt" , &(*v_invPt ));
}

void RPCIntegration2::writeTree() {
  // Handled by TFileService
}

// _____________________________________________________________________________
void RPCIntegration2::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

// _____________________________________________________________________________
// Define this as a plug-in
typedef RPCIntegration2 RPCIntegration2Sep2016;
DEFINE_FWK_MODULE(RPCIntegration2Sep2016);
