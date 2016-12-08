#include <memory>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "TFile.h"
#include "TH1F.h"
#include "TString.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/EMTFTrackExtra.h"

#include "DataFormatsSep2016/L1TMuon/interface/EMTFHit.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrack.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrackExtra.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


#define SEP2016_VERSION

#ifdef SEP2016_VERSION
namespace l1t_std = l1t;
namespace l1t_sep = L1TMuonEndCap;
#else
namespace l1t_std = l1t;
namespace l1t_sep = l1t;
#endif

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

  // Member data
  //const edm::InputTag unpHitTag_;
  //const edm::InputTag emuHitTag_;
  //const edm::InputTag unpTrackTag_;
  //const edm::InputTag emuTrackTag_;

  const edm::InputTag emuHitTag2_;
  const edm::InputTag emuTrackTag2_;

  const edm::InputTag genPartTag_;

  const std::string outFileName_;

  int verbose_;

  //edm::EDGetTokenT<l1t_std::EMTFHitCollection>   unpHitToken_;
  //edm::EDGetTokenT<l1t_sep::EMTFHitCollection>   emuHitToken_;
  //edm::EDGetTokenT<l1t_std::EMTFTrackCollection> unpTrackToken_;
  //edm::EDGetTokenT<l1t_sep::EMTFTrackCollection> emuTrackToken_;

  edm::EDGetTokenT<l1t_sep::EMTFHitExtraCollection>   emuHitToken2_;
  edm::EDGetTokenT<l1t_sep::EMTFTrackExtraCollection> emuTrackToken2_;

  edm::EDGetTokenT<reco::GenParticleCollection> genPartToken_;

  //l1t_std::EMTFHitCollection    unpHits_;
  //l1t_sep::EMTFHitCollection    emuHits_;
  //l1t_std::EMTFTrackCollection  unpTracks_;
  //l1t_sep::EMTFTrackCollection  emuTracks_;

  l1t_sep::EMTFHitExtraCollection    emuHits2_;
  l1t_sep::EMTFTrackExtraCollection  emuTracks2_;

  reco::GenParticleCollection genParts_;

  std::map<TString, TH1F*> histograms_;
};

// _____________________________________________________________________________
RPCIntegration::RPCIntegration(const edm::ParameterSet& iConfig) :
    //unpHitTag_   (iConfig.getParameter<edm::InputTag>("unpHitTag")),
    //emuHitTag_   (iConfig.getParameter<edm::InputTag>("emuHitTag")),
    //unpTrackTag_ (iConfig.getParameter<edm::InputTag>("unpTrackTag")),
    //emuTrackTag_ (iConfig.getParameter<edm::InputTag>("emuTrackTag")),

    emuHitTag2_  (iConfig.getParameter<edm::InputTag>("emuHitTag2")),
    emuTrackTag2_(iConfig.getParameter<edm::InputTag>("emuTrackTag2")),

    genPartTag_  (iConfig.getParameter<edm::InputTag>("genPartTag")),
    outFileName_ (iConfig.getParameter<std::string>  ("outFileName")),
    verbose_     (iConfig.getUntrackedParameter<int> ("verbosity"))
{
    //unpHitToken_   = consumes<l1t_std::EMTFHitCollection>  (unpHitTag_);
    //emuHitToken_   = consumes<l1t_sep::EMTFHitCollection>  (emuHitTag_);
    //unpTrackToken_ = consumes<l1t_std::EMTFTrackCollection>(unpTrackTag_);
    //emuTrackToken_ = consumes<l1t_sep::EMTFTrackCollection>(emuTrackTag_);

    emuHitToken2_    = consumes<l1t_sep::EMTFHitExtraCollection>  (emuHitTag2_);
    emuTrackToken2_  = consumes<l1t_sep::EMTFTrackExtraCollection>(emuTrackTag2_);

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

  //edm::Handle<decltype(unpHits_)>    unpHits_handle;
  //edm::Handle<decltype(emuHits_)>    emuHits_handle;
  //edm::Handle<decltype(unpTracks_)>  unpTracks_handle;
  //edm::Handle<decltype(emuTracks_)>  emuTracks_handle;

  edm::Handle<decltype(emuHits2_)>   emuHits2_handle;
  edm::Handle<decltype(emuTracks2_)> emuTracks2_handle;

  edm::Handle<decltype(genParts_)> genParts_handle;

  if (iEvent.isRealData()) {
    //if (!unpHitToken_.isUninitialized()) {
    //  iEvent.getByToken(unpHitToken_, unpHits_handle);
    //}
    //if (!unpHits_handle.isValid()) {
    //  edm::LogError("RPCIntegration") << "Cannot get the product: " << unpHitTag_;
    //  return;
    //}
    //
    //if (!unpTrackToken_.isUninitialized()) {
    //  iEvent.getByToken(unpTrackToken_, unpTracks_handle);
    //}
    //if (!unpTracks_handle.isValid()) {
    //  edm::LogError("RPCIntegration") << "Cannot get the product: " << unpTrackTag_;
    //  return;
    //}
  }

  //if (!emuHitToken_.isUninitialized()) {
  //  iEvent.getByToken(emuHitToken_, emuHits_handle);
  //}
  //if (!emuHits_handle.isValid()) {
  //  edm::LogError("RPCIntegration") << "Cannot get the product: " << emuHitTag_;
  //  return;
  //}
  //
  //if (!emuTrackToken_.isUninitialized()) {
  //  iEvent.getByToken(emuTrackToken_, emuTracks_handle);
  //}
  //if (!emuTracks_handle.isValid()) {
  //  edm::LogError("RPCIntegration") << "Cannot get the product: " << emuTrackTag_;
  //  return;
  //}

  if (!emuHitToken2_.isUninitialized()) {
    iEvent.getByToken(emuHitToken2_, emuHits2_handle);
  }
  if (!emuHits2_handle.isValid()) {
    edm::LogError("RPCIntegration") << "Cannot get the product: " << emuHitTag2_;
    return;
  }

  if (!emuTrackToken2_.isUninitialized()) {
    iEvent.getByToken(emuTrackToken2_, emuTracks2_handle);
  }
  if (!emuTracks2_handle.isValid()) {
    edm::LogError("RPCIntegration") << "Cannot get the product: " << emuTrackTag2_;
    return;
  }

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

  //// Exclude out-of-emu BX
  //auto out_of_emu_bx = [](const auto& trk) {
  //  bool out = (trk.BX() < -1) || (+1 < trk.BX());
  //  //bool out = (trk.BX() != 0);
  //  return out;
  //};
  //
  //// Exclude sector -6
  //auto out_of_emu_sector = [](const auto& trk) {
  //  bool out = (trk.Endcap() == -1 && trk.Sector() == 6);
  //  return out;
  //};
  //
  //unpHits_.clear();
  //for (const auto& hit : (*unpHits_handle)) {
  //  unpHits_.push_back(hit);
  //}
  //
  //emuHits_.clear();
  //for (const auto& hit : (*emuHits_handle)) {
  //  emuHits_.push_back(hit);
  //}
  //
  //unpTracks_.clear();
  //for (const auto& trk : reversed(*unpTracks_handle)) {
  //  if (out_of_emu_bx(trk))      continue;
  //  if (out_of_emu_sector(trk))  continue;
  //  unpTracks_.push_back(trk);
  //}
  //
  //emuTracks_.clear();
  //for (const auto& trk : (*emuTracks_handle)) {
  //  if (out_of_emu_bx(trk))      continue;
  //  if (out_of_emu_sector(trk))  continue;
  //  emuTracks_.push_back(trk);
  //}

  emuHits2_.clear();
  for (const auto& hit : (*emuHits2_handle)) {
    emuHits2_.push_back(hit);
  }
  //assert(emuHits_.size() == emuHits2_.size());

  emuTracks2_.clear();
  for (const auto& trk : (*emuTracks2_handle)) {
#ifdef SEP2016_VERSION
    if (trk.bx < -1 || +1 < trk.bx)      continue;  //if (out_of_emu_bx(trk))      continue;
    if (trk.endcap==2 && trk.sector==6)  continue;  //if (out_of_emu_sector(trk))  continue;

    if (trk.endcap==2)                   continue;  // only positive endcap
#else
    if (out_of_emu_bx(trk))      continue;
    if (out_of_emu_sector(trk))  continue;
#endif
    emuTracks2_.push_back(trk);
  }
  //assert(emuTracks_.size() == emuTracks2_.size());

  auto not_muon = [](const auto& part) {
    int absPdgId  = std::abs(part.pdgId());
    double pt     = part.pt();
    double eta    = part.eta();                     // only positive endcap
    bool b = !((absPdgId == 13) && (pt >= 2.) && (1.24 <= eta && eta <= 2.4));
    return b;
  };

  genParts_.clear();
  for (const auto& part : (*genParts_handle)) {
    if (not_muon(part))  continue;
    genParts_.push_back(part);
  }
}

// _____________________________________________________________________________
void RPCIntegration::makeEfficiency() {
  TString hname;
  TH1F* h;

  auto get_l1t_mode_bin = [](const auto& trk) {
    int mode      = trk.mode;
    if (mode == 15)                              return 3;
    if (mode == 11 || mode == 13 || mode == 14)  return 2;
    if (mode ==  7 || mode == 10 || mode == 12)  return 1;
    if (mode ==  3 || mode ==  5 || mode ==  6 || mode == 9)  return 0;
    return -1;
  };

  auto get_l1t_pt_bin = [](const auto& trk) {
    float pt      = trk.pt;
    if (pt >= 100.)  return 3;
    if (pt >=  22.)  return 2;
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
    for (const auto& trk : emuTracks2_) {
      std::cout << "trk " << itrack++ << " " << trk.pt << " " << trk.gmt_eta << " " << trk.gmt_phi << " " << trk.mode << std::endl;
    }
  }

  if (genParts_.empty())
    keep_event = false;

  if (keep_event) {
    const auto& part = genParts_.front();

    bool trigger = !emuTracks2_.empty();

    if (genParts_.size() != 1) {
      std::cout << "[WARNING] perche non uno? num of genParts: " << genParts_.size() << std::endl;
    }

    if (trigger && emuTracks2_.size() != 1) {
      std::cout << "[WARNING] perche non uno? num of emuTracks2: " << emuTracks2_.size() << std::endl;
    }

    int mode_bin    = trigger ? get_l1t_mode_bin(emuTracks2_.front()) : -1;
    int pt_bin      = trigger ? get_l1t_pt_bin(emuTracks2_.front()) : -1;
    int eta_bin     = get_gen_eta_bin(part);
    int eta_any_bin = get_gen_eta_any_bin(part);

    double pt     = part.pt();
    double absEta = std::abs(part.eta());

    // Efficiency vs pT
    for (int i=0; i<4; ++i) {  // mode
      for (int j=0; j<4; ++j) {  // eta
        if (j != eta_bin || j != eta_any_bin) continue;

        hname = Form("denom_vs_pt_mode%i_eta%i", i, j);
        h = histograms_.at(hname);
        h->Fill(pt);

        if (trigger) {
          if (i < mode_bin) continue;

          hname = Form("num_vs_pt_mode%i_eta%i", i, j);
          h = histograms_.at(hname);
          h->Fill(pt);
        }
      }
    }

    // Efficiency vs eta
    for (int i=0; i<4; ++i) {  // mode
      for (int j=0; j<4; ++j) {  // pt
        hname = Form("denom_vs_eta_mode%i_pt%i", i, j);
        h = histograms_.at(hname);
        h->Fill(absEta);

        if (trigger) {
          if (i < mode_bin) continue;
          if (j < pt_bin) continue;

          hname = Form("num_vs_eta_mode%i_pt%i", i, j);
          h = histograms_.at(hname);
          h->Fill(absEta);
        }
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

  const Double_t pt_bins[41] = {0.0, 0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0, 10.0, 12.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0, 70.0, 80.0, 100.0, 125.0, 150.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 800.0, 1000.0, 2000.0, 7000.0};

  for (int i=0; i<4; ++i) {  // mode
    for (int j=0; j<4; ++j) {  // eta
      for (int k=0; k<2; ++k) {
        if (k == 0)
          hname = Form("denom_vs_pt_mode%i_eta%i", i, j);
        else
          hname = Form("num_vs_pt_mode%i_eta%i", i, j);
        h = new TH1F(hname, "; p_{T} [GeV]; entries", 41-1, pt_bins);
        histograms_[hname] = h;
      }
    }
  }

  // Efficiency vs eta
  // Make [mode] x [pT] where
  //   mode 0,1,2,3 = MuOpen, DoubleMu, SingleMu, Mode15
  //   pt   0,1,2,3 = >=0, >=12, >=22, >=100
  for (int i=0; i<4; ++i) {  // mode
    for (int j=0; j<4; ++j) {  // pt
      for (int k=0; k<2; ++k) {
        if (k == 0)
          hname = Form("denom_vs_eta_mode%i_pt%i", i, j);
        else
          hname = Form("num_vs_eta_mode%i_pt%i", i, j);
        h = new TH1F(hname, "; |#eta|; entries", 48, 1.2, 2.4);
        histograms_[hname] = h;
      }
    }
  }
}

void RPCIntegration::writeHistograms() {
  TFile* f = TFile::Open(outFileName_.c_str(), "RECREATE");
  for (const auto& kv : histograms_) {
    kv.second->SetDirectory(gDirectory);
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
