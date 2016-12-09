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
#include "TEfficiency.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "DataFormats/L1TMuon/interface/EMTFHit.h"
//#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
//#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
//#include "DataFormats/L1TMuon/interface/EMTFTrackExtra.h"

//#include "DataFormatsSep2016/L1TMuon/interface/EMTFHit.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFHitExtra.h"
//#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrack.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrackExtra.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


// From L1TriggerSep2016/L1TMuonEndCap/interface/MuonTriggerPrimitive.h
enum subsystem_type{kDT,kCSC,kRPC,kNSubsystems};

namespace l1t = L1TMuonEndCap;

#include "helper.hh"


// _____________________________________________________________________________
class RPCIntegration : public edm::EDAnalyzer {
public:
  explicit RPCIntegration(const edm::ParameterSet& iConfig);
  ~RPCIntegration();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
  virtual void endJob() override;

  // Member functions
  void getHandles(const edm::Event& iEvent);

  void makeEfficiency();

  void bookHistograms();
  void writeHistograms();

  // Options
  const edm::InputTag emuHitTag_;
  const edm::InputTag emuTrackTag_;

  const edm::InputTag genPartTag_;

  const std::string outFileName_;

  int verbose_;

  // Member data
  edm::EDGetTokenT<l1t::EMTFHitExtraCollection>   emuHitToken_;
  edm::EDGetTokenT<l1t::EMTFTrackExtraCollection> emuTrackToken_;

  edm::EDGetTokenT<reco::GenParticleCollection> genPartToken_;

  l1t::EMTFHitExtraCollection    emuHits_;
  l1t::EMTFTrackExtraCollection  emuTracks_;

  reco::GenParticleCollection genParts_;

  std::map<TString, TH1F*> histograms_;
};

// _____________________________________________________________________________
RPCIntegration::RPCIntegration(const edm::ParameterSet& iConfig) :
    emuHitTag_   (iConfig.getParameter<edm::InputTag>("emuHitTag")),
    emuTrackTag_ (iConfig.getParameter<edm::InputTag>("emuTrackTag")),
    genPartTag_  (iConfig.getParameter<edm::InputTag>("genPartTag")),
    outFileName_ (iConfig.getParameter<std::string>  ("outFileName")),
    verbose_     (iConfig.getUntrackedParameter<int> ("verbosity"))
{
    emuHitToken_   = consumes<l1t::EMTFHitExtraCollection>  (emuHitTag_);
    emuTrackToken_ = consumes<l1t::EMTFTrackExtraCollection>(emuTrackTag_);

    genPartToken_ = consumes<reco::GenParticleCollection>(genPartTag_);
}

RPCIntegration::~RPCIntegration() {}

// _____________________________________________________________________________
void RPCIntegration::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  getHandles(iEvent);

  makeEfficiency();
}

// _____________________________________________________________________________
void RPCIntegration::getHandles(const edm::Event& iEvent) {
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

  // Object filters
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
    double pt     = part.pt();
    double eta    = part.eta();
    if (!(pt >= 2.))     continue;                // only pT > 2
    if (!(1.24 <= eta && eta <= 2.4))  continue;  // only positive endcap
    genParts_.push_back(part);
  }
}

// _____________________________________________________________________________
void RPCIntegration::makeEfficiency() {
  TString hname;
  TH1F* h;

  std::random_device rd;
  std::mt19937 genrd(rd());

  auto get_mode_bin = [](const auto& trk) {
    int mode      = trk.mode;
    if (mode == 15)                              return 3;
    if (mode == 11 || mode == 13 || mode == 14)  return 2;
    if (mode ==  7 || mode == 10 || mode == 12)  return 1;
    if (mode ==  3 || mode ==  5 || mode ==  6 || mode == 9)  return 0;
    return -1;
  };

  auto get_l1pt_bin = [](const auto& trk) {
    float pt      = trk.pt;
    if (pt >= 100.)  return 3;
    if (pt >=  18.)  return 2;
    if (pt >=  12.)  return 1;
    if (pt >=   0.)  return 0;
    return -1;
  };

  auto get_gen_eta_bin = [](const auto& part) {
    double absEta = std::abs(part.eta());
    if (absEta < 1.24)  return -1;
    if (absEta < 1.60)  return 0;
    if (absEta < 2.00)  return 1;
    if (absEta < 2.40)  return 2;
    return -1;
  };

  auto get_gen_eta_any_bin = [](const auto& part) {
    return 3;
  };

  // ___________________________________________________________________________
  bool keep_event = true;

  if (verbose_ > 0) {
    int ipart = 0;
    for (const auto& part : genParts_) {
      //std::cout << "part " << ipart++ << " " << part.pdgId() << " " << part.status() << " " << part.pt() << " " << part.eta() << " " << part.phi() << std::endl;
      std::cout << "part " << ipart++ << " " << part.pt() << " " << part.eta() << " " << part.phi() << std::endl;
    }

    int itrack = 0;
    for (const auto& trk : emuTracks_) {
      std::cout << "trk " << itrack++ << " " << trk.pt << " " << trk.gmt_eta << " " << trk.gmt_phi << " " << trk.mode << std::endl;
    }

    int ihit = 0;
    for (const auto& hit : emuHits_) {
      if (hit.subsystem == kCSC) {
        std::cout << "CSC hit " << ihit++ << " " << hit.sector << " " << hit.station << " " << hit.chamber << " " << hit.bx << std::endl;
      }
      if (hit.subsystem == kRPC) {
        std::cout << "RPC hit " << ihit++ << " " << hit.sector << " " << hit.station << " " << hit.chamber << " " << hit.bx << std::endl;
      }
    }
  }

  if (genParts_.empty()) {
    keep_event = false;
  }

  if (keep_event) {
    const auto& part = genParts_.front();

    bool trigger = !emuTracks_.empty();

    if (genParts_.size() != 1) {
      std::cout << "[WARNING] perche non uno? num of genParts: " << genParts_.size() << std::endl;
    }

    if (trigger && emuTracks_.size() != 1) {
      std::cout << "[WARNING] perche non uno? num of emuTracks: " << emuTracks_.size() << std::endl;
    }

    int mode_bin    = trigger ? get_mode_bin(emuTracks_.front()) : -1;
    int l1pt_bin    = trigger ? get_l1pt_bin(emuTracks_.front()) : -1;
    int eta_bin     = get_gen_eta_bin(part);
    int eta_any_bin = get_gen_eta_any_bin(part);

    const int    charge = part.charge();
    const double pt     = part.pt();
    const double absEta = std::abs(part.eta());
    assert(charge == 1 || charge == -1);

    bool overwrite_with_ideal = false;
    if (overwrite_with_ideal) {
      l1t::EMTFTrackExtra tmp_track;

      tmp_track.mode = 0;
      for (const auto& hit : emuHits_) {
        assert(1 <= hit.station && hit.station <= 4);
        if (hit.subsystem == kCSC) {
          tmp_track.mode |= (1<<(4-hit.station));
        }
        //if (hit.subsystem == kRPC) {
        //  tmp_track.mode |= (1<<(4-hit.station));
        //}
      }
      assert(tmp_track.mode <= 15);

      if (tmp_track.mode != 15 && pt > 20. && (1.34 <= absEta && absEta <= 1.36)) {  // debug
        std::cout << "[WARNING] perche non quindici? mode: " << tmp_track.mode << std::endl;

        hname = Form("debug_eta1p3");
        h = histograms_[hname];
        if      (tmp_track.mode ==  7)  h->Fill(1);
        else if (tmp_track.mode == 11)  h->Fill(2);
        else if (tmp_track.mode == 13)  h->Fill(3);
        else if (tmp_track.mode == 14)  h->Fill(4);
      }

      trigger = true;
      l1pt_bin = 3;
      mode_bin = get_mode_bin(tmp_track);
    }

    // _________________________________________________________________________
    // Fill histograms

    // Efficiency vs pT
    for (int i=0; i<4; ++i) {  // mode
      for (int j=0; j<4; ++j) {  // eta
        if (!(j == eta_bin || j == eta_any_bin)) continue;  // gen eta binning

        hname = Form("denom_vs_pt_mode%i_eta%i", i, j);
        h = histograms_.at(hname);
        h->Fill(pt);

        if (trigger) {
          if (!(i <= mode_bin)) continue;
          //if (!(2 <= l1pt_bin)) continue;  // trigger pT cut

          hname = Form("num_vs_pt_mode%i_eta%i", i, j);
          h = histograms_.at(hname);
          h->Fill(pt);
        }
      }
    }

    // Efficiency vs eta
    for (int i=0; i<4; ++i) {  // mode
      for (int j=0; j<4; ++j) {  // pT
        //if (!(pt >= 2.)) continue;  // gen pT requirement
        if (!(pt >= 20.)) continue;  // gen pT requirement

        hname = Form("denom_vs_eta_mode%i_l1pt%i", i, j);
        h = histograms_.at(hname);
        h->Fill(absEta);

        if (trigger) {
          if (!(i <= mode_bin)) continue;
          if (!(j <= l1pt_bin)) continue;

          hname = Form("num_vs_eta_mode%i_l1pt%i", i, j);
          h = histograms_.at(hname);
          h->Fill(absEta);
        }
      }
    }

    // Deflection angles
    for (int i=0; i<4; i++) {
      std::vector<int> csc_phis;
      std::vector<int> rpc_phis;
      for (const auto& hit : emuHits_) {
        if (hit.station == i+1) {
          if (hit.subsystem == kCSC) {
            csc_phis.push_back(hit.phi_fp);
          }
          if (hit.subsystem == kRPC) {
            rpc_phis.push_back(hit.phi_fp);
          }
        }
      }

      // Only 120 < pT < 250
      if ((120. < pt && pt < 250.) && csc_phis.size() > 0 && rpc_phis.size() > 0) {
        std::uniform_int_distribution<> index1(0, csc_phis.size()-1);
        std::uniform_int_distribution<> index2(0, rpc_phis.size()-1);
        int csc_phi = csc_phis.at(index1(genrd));
        int rpc_phi = rpc_phis.at(index2(genrd));
        hname = Form("deflection_rpc_csc_st%i", i+1);
        h = histograms_.at(hname);
        h->Fill((csc_phi - rpc_phi) * charge);
        //h->Fill((((csc_phi + (1<<1))>>2) - ((rpc_phi + (1<<1))>>2)) * charge);
      }
    }

  }  // if keep_event
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
  TH1F* h;

  // Efficiency vs pT
  // Make [mode] x [eta] where
  //   mode 0,1,2,3 = MuOpen, DoubleMu, SingleMu, Mode15
  //   eta  0,1,2,3 = 1.2-1.6, 1.6-2.0, 2.0-2.4, inclusive

  const Double_t pt_bins[46] = {0., 1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 12., 14., 16., 18., 20., 22., 24., 26., 28., 30., 35., 40., 45., 50., 55., 60., 70., 80., 100., 120., 140., 160., 180., 200., 250., 300., 350., 400., 450., 500., 550., 600., 700., 800., 1000.};

  for (int i=0; i<4; ++i) {  // mode
    for (int j=0; j<4; ++j) {  // eta
      for (int k=0; k<2; ++k) {
        if (k == 0)
          hname = Form("denom_vs_pt_mode%i_eta%i", i, j);
        else
          hname = Form("num_vs_pt_mode%i_eta%i", i, j);
        h = new TH1F(hname, "; gen p_{T} [GeV]; entries", 46-1, pt_bins);
        histograms_[hname] = h;
      }
    }
  }

  // Efficiency vs eta (requiring gen pT >= 20)
  // Make [mode] x [pT] where
  //   mode 0,1,2,3 = MuOpen, DoubleMu, SingleMu, Mode15
  //   pT   0,1,2,3 = >=0, >=12, >=18, >=100

  for (int i=0; i<4; ++i) {  // mode
    for (int j=0; j<4; ++j) {  // pT
      for (int k=0; k<2; ++k) {
        if (k == 0)
          hname = Form("denom_vs_eta_mode%i_l1pt%i", i, j);
        else
          hname = Form("num_vs_eta_mode%i_l1pt%i", i, j);
        h = new TH1F(hname, "; gen |#eta|; entries", 70, 1.1, 2.5);
        histograms_[hname] = h;
      }
    }
  }

  // Deflection angles

  for (int i=0; i<4; ++i) {
    hname = Form("deflection_rpc_csc_st%i", i+1);
    h = new TH1F(hname, "; RPC #phi - CSC #phi [integer unit]", 121, -60.5, 60.5);
    histograms_[hname] = h;
  }

  // Debugging

  if (1) {
    hname = Form("debug_eta1p3");
    h = new TH1F(hname, "; missing station", 10, 0, 10);
    histograms_[hname] = h;
  }
}

void RPCIntegration::writeHistograms() {
  TString hname;
  TH1F* denom;
  TH1F* num;
  TEfficiency* eff;

  TFile* f = TFile::Open(outFileName_.c_str(), "RECREATE");
  for (const auto& kv : histograms_) {
    kv.second->SetDirectory(gDirectory);

    // Make TEfficiency
    if (kv.first.BeginsWith("denom_")) {
      hname = kv.first;
      denom = kv.second;
      hname.ReplaceAll("denom_", "num_");
      num = histograms_.at(hname);
      hname = kv.first;
      hname.ReplaceAll("denom_", "eff_");
      eff = new TEfficiency(*num, *denom);
      eff->SetName(hname);
      eff->SetStatisticOption(TEfficiency::kFCP);
      eff->SetDirectory(gDirectory);
    }
  }

  f->Write();
  f->Close();
}

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