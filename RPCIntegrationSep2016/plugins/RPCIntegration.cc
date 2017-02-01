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
  void makeExtrapolation();

  void bookHistograms();
  void writeHistograms();

  double angle_func(double part_pt, double part_eta, int hit_subsystem, int hit_station, int hit_ring, bool hit_fr, float hit_eta);

  // Options
  const edm::InputTag emuHitTag_;
  const edm::InputTag emuTrackTag_;

  const edm::InputTag genPartTag_;

  const std::string outFileName_;

  const std::string angleFileName_;

  int verbose_;

  // Member data
  edm::EDGetTokenT<l1t::EMTFHitExtraCollection>   emuHitToken_;
  edm::EDGetTokenT<l1t::EMTFTrackExtraCollection> emuTrackToken_;

  edm::EDGetTokenT<reco::GenParticleCollection> genPartToken_;

  l1t::EMTFHitExtraCollection    emuHits_;
  l1t::EMTFTrackExtraCollection  emuTracks_;

  reco::GenParticleCollection genParts_;

  std::map<TString, TH1F*> histograms_;
  std::map<TString, TH2F*> histogram2Ds_;
};


static const float table_common_dphi[9][4][12] = {
  {
    {1,1,1,1,0.84666,0.853464,0.867617,0.873055,0.891733,0.895144,0.899437,0.901102},
    {1,1,1,1,0.824506,0.830128,0.827465,0.833594,0.8342,0.837148,0.835547,0.833259},
    {1,1,1,1,1.02119,1.02803,1.03408,1.04254,1.05516,1.06089,1.06734,1.072},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,0.815518,0.794248,0.811911,0.834043,0.846266,0.861189,0.870185,0.875999},
    {1,1,1,1,0.797369,0.835246,0.845331,0.849306,0.858728,0.859638,0.860124,0.864665},
    {1,1,1,1,0.924963,0.953833,0.975241,0.986756,0.990523,1.00145,1.00868,1.01503},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,0.818443,0.834547,0.803042,0.825854,0.841015,0.851987,0.86675,0.873194},
    {1,1,1,1,0.722867,0.773742,0.836682,0.841727,0.858967,0.857886,0.866257,0.868019},
    {1,1,1,1,0.850137,0.889076,0.971381,0.981557,0.987687,0.990899,1.00386,1.00315},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.63038,0.667363,0.69958,0.725422,0.771336,1,1,1,1,1,1,1},
    {0.703956,0.715577,0.698035,0.681031,0.675162,1,1,1,1,1,1,1},
    {1.15616,1.14609,1.14795,1.12045,0.577519,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.939943,0.876313,0.873834,0.866203,0.920202,1,1,1,1,1,1,1},
    {0.855573,0.715455,0.729931,0.745724,0.741166,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {0.296704,1.17925,1.24827,1.19046,1.21726,1,1,1,1,1,1,1}
  }, {
    {1.30612,1.0013,0.933209,0.932063,0.860496,1,1,1,1,1,1,1},
    {0.818715,0.733709,0.713075,0.739424,0.708431,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1.3812,1.26763,1.29899,1.24171,1.33557,1,1,1,1,1,1,1}
  }, {
    {0.710205,1.23066,1.07329,0.937422,0.944576,1.23562,0.995471,0.73616,0.736419,0.846077,0.837642,0.850825},
    {0.617322,0.624397,0.607213,0.657476,0.596371,0.623193,0.6065,0.631478,0.638557,0.642369,0.638006,0.63419},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {0.326154,0.865316,0.731991,0.648373,0.741352,0.893415,0.964539,1.04719,1.08863,1.12308,1.12227,1.10385}
  }, {
    {1.12786,1.25566,1.19662,1.04014,1.04503,1.23896,1.42962,1.17385,0.794457,0.790477,1.00387,1.0101},
    {0.965046,0.876151,0.846881,0.830578,0.894659,0.868373,0.81295,0.762267,0.693277,0.7008,0.710171,0.720975},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1.26593,1.05604,0.965916,0.830185,0.873131,1.12788,1.24738,1.29535,1.18649,1.1627,1.20282,1.21041}
  }, {
    {0.983456,0.995348,0.996956,0.956463,0.96291,1.00854,1.03451,1.04552,1.05104,1.05786,0.802445,0.836741},
    {0.784907,0.865083,0.897923,0.848196,0.840051,0.845405,0.910135,1.01395,1.04911,1.3011,0.33401,0.694493},
    {1.30986,1.61264,1.6152,1.50259,1.30524,1.50893,1.76375,1.80443,1.94766,1.76301,1.16413,1.00962},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }
};

// _____________________________________________________________________________
RPCIntegration::RPCIntegration(const edm::ParameterSet& iConfig) :
    emuHitTag_    (iConfig.getParameter<edm::InputTag>("emuHitTag")),
    emuTrackTag_  (iConfig.getParameter<edm::InputTag>("emuTrackTag")),
    genPartTag_   (iConfig.getParameter<edm::InputTag>("genPartTag")),
    outFileName_  (iConfig.getParameter<std::string>  ("outFileName")),
    angleFileName_(iConfig.getParameter<std::string>  ("angleFileName")),
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

  makeEfficiency();
  makeExtrapolation();
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
    //if (!(part.pt() >= 2.))     continue;  // only pT > 2
    if (!(1.24 <= part.eta() && part.eta() <= 2.4))  continue;  // only positive endcap
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
      if (hit.subsystem == TriggerPrimitive::kCSC) {
        std::cout << "CSC hit " << ihit++ << " " << hit.sector << " " << hit.station << " " << hit.ring << " " << hit.chamber << " " << hit.bx << std::endl;
      }
      if (hit.subsystem == TriggerPrimitive::kRPC) {
        std::cout << "RPC hit " << ihit++ << " " << hit.sector << " " << hit.station << " " << hit.ring << " " << hit.chamber << " " << hit.bx << std::endl;
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
      for (int sector = 1; sector <= 6; ++sector) {
        int mode = 0;
        for (const auto& hit : emuHits_) {
          assert(1 <= hit.sector && hit.sector <= 6);
          assert(1 <= hit.station && hit.station <= 4);
          if (hit.sector == sector && hit.subsystem == TriggerPrimitive::kCSC) {
            mode |= (1<<(4-hit.station));
          }
          //if (hit.sector == sector && hit.subsystem == TriggerPrimitive::kRPC) {
          //  mode |= (1<<(4-hit.station));
          //}
        }
        if (tmp_track.mode < mode) {
          tmp_track.mode = mode;
        }
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
          if (hit.subsystem == TriggerPrimitive::kCSC) {
            csc_phis.push_back(hit.phi_fp);
          }
          if (hit.subsystem == TriggerPrimitive::kRPC) {
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

  }  // end if keep_event
}

// _____________________________________________________________________________
void RPCIntegration::makeExtrapolation() {
  TString hname;
  TH2F* h2;

  std::random_device rd;
  std::mt19937 genrd(rd());

  // ___________________________________________________________________________
  bool keep_event = true;
  std::vector<l1t::EMTFHitExtra> myhits_csc;
  std::vector<l1t::EMTFHitExtra> myhits_rpc;

  // from RecoMuon/DetLayers/src/MuonCSCDetLayerGeometryBuilder.cc
  // from RecoMuon/DetLayers/src/MuonRPCDetLayerGeometryBuilder.cc
  auto isFront = [](int subsystem, int station, int ring, int chamber, int subsector) {
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

  if (genParts_.empty()) {
    keep_event = false;
  }

  if (keep_event) {
    const auto& part = genParts_.front();

    const int    charge = part.charge();
    const double pt     = part.pt();
    const double absEta = std::abs(part.eta());

    for (int sector = 1; sector <= 6; ++sector) {
      int mode = 0;
      myhits_csc.clear();
      myhits_rpc.clear();
      for (const auto& hit : emuHits_) {
        assert(1 <= hit.sector && hit.sector <= 6);
        assert(1 <= hit.station && hit.station <= 4);
        if (hit.sector == sector && hit.subsystem == TriggerPrimitive::kCSC) {
          mode |= (1<<(4-hit.station));
          myhits_csc.push_back(hit);
        } else if (hit.sector == sector && hit.subsystem == TriggerPrimitive::kRPC) {
          mode |= (1<<(4-hit.station));
          myhits_rpc.push_back(hit);
        }
      }

      //if (mode == 15 && myhits_csc.size() == 4) {
      //  for (const auto& hit : myhits_csc) {
      //    int hit_fr = isFront(hit.subsystem, hit.station, hit.ring, hit.chamber, hit.subsector);
      //    float hit_phi = hit.phi_glob_deg;
      //    float hit_eta = hit.eta;
      //    double dphi_tmp_2 = angle_func(part.pt(), part.eta(), hit.subsystem, hit.station, hit.ring, hit_fr, hit_eta);
      //    double dphi_tmp_1 = dphi_tmp_2 * (static_cast<double>(part.charge())/part.pz());
      //    double dphi_tmp   = l1t::rad_to_deg(dphi_tmp_1);
      //    double new_hit_phi = hit_phi - dphi_tmp;
      //
      //    int phi_loc_int     = l1t::calc_phi_loc_int(hit_phi, sector);
      //    int new_phi_loc_int = l1t::calc_phi_loc_int(new_hit_phi, sector);
      //
      //    std::string sr = "";
      //    if (hit.subsystem == TriggerPrimitive::kCSC)
      //      sr += "ME";
      //    else if (hit.subsystem == TriggerPrimitive::kRPC)
      //      sr += "RE";
      //    sr += std::to_string(hit.station);
      //    sr += "/";
      //    sr += std::to_string(hit.ring);
      //    if (hit_fr)
      //      sr += "f";
      //    else
      //      sr += "r";
      //
      //    if (verbose_)  std::cout << sr << ": " << phi_loc_int << " --> " << new_phi_loc_int << " phi: " << hit_phi << " --> " << new_hit_phi << " dphi_tmp: " << dphi_tmp_2 << "," << dphi_tmp_1 << "," << dphi_tmp << std::endl;
      //  }
      //  break;  // break from loop over sector
      //}  // end if


      // Make deflection angles
      // ipair 0: dphi(1/1,2)
      // ipair 1: dphi(1/1,3)
      // ipair 2: dphi(1/1,4)
      // ipair 3: dphi(1/2,2)
      // ipair 4: dphi(1/2,3)
      // ipair 5: dphi(1/2,4)
      // ipair 6: dphi(2,3)
      // ipair 7: dphi(2,4)
      // ipair 8: dphi(3,4)
      for (int ipair = 0; ipair < 9; ++ipair) {
        std::vector<l1t::EMTFHitExtra> myhits1;
        std::vector<l1t::EMTFHitExtra> myhits2;
        if (ipair == 0) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 1) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 2) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 3) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 4) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 5) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 6) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 7) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 8) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else {
          assert(false && "invalid ipair");
        }

        if (myhits1.size() > 0 && myhits2.size() > 0) {
          std::uniform_int_distribution<> index1(0, myhits1.size()-1);
          std::uniform_int_distribution<> index2(0, myhits2.size()-1);
          l1t::EMTFHitExtra myhit1 = myhits1.at(index1(genrd));
          l1t::EMTFHitExtra myhit2 = myhits2.at(index2(genrd));

          // Histogramming
          int ifr = (int(isFront(myhit1.subsystem, myhit1.station, myhit1.ring, myhit1.chamber, myhit1.subsector)) << 1) | int(isFront(myhit2.subsystem, myhit2.station, myhit2.ring, myhit2.chamber, myhit2.subsector));
          int ieta = int((absEta - 1.2) / (2.4 - 1.2) * 12);

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

          // dphi
          //int dphi = myhit1.phi_fp - myhit2.phi_fp;
          int dphi = l1t::calc_phi_loc_int(myhit1.phi_glob_deg, sector) - l1t::calc_phi_loc_int(myhit2.phi_glob_deg, sector);
          if (charge > 0)  dphi = -dphi;

          const float common_dphi_ratio = table_common_dphi[ipair][ifr][ieta];
          const int common_dphi = static_cast<int>(std::round(1.0 / common_dphi_ratio * dphi));

          // dphi vs 1/pT
          hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_stp%i_frp%i_eta%i", ipair, 4, ieta);  // inclusive
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);

          hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, 4, ieta);  // inclusive
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);

          // dphi vs |eta|
          if (ipt != -1) {
            hname = Form("deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, dphi);
            hname = Form("deflection_stp%i_frp%i_pt%i", ipair, 4, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, dphi);

            hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, common_dphi);
            hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, 4, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, common_dphi);
          }
        }
      }  // end make deflection angles

      // Make deflection angles (with RPC)
      // ipair  9: dphi(RE1/2,ME2)
      // ipair 10: dphi(RE1/2,ME3)
      // ipair 11: dphi(RE1/2,ME4)
      // ipair 12: dphi(ME1/1,RE2)
      // ipair 13: dphi(ME1/2,RE2)
      // ipair 14: dphi(RE2,ME3)
      // ipair 15: dphi(RE2,ME4)
      // ipair 16: dphi(ME1/1,RE3)
      // ipair 17: dphi(ME1/2,RE3)
      // ipair 18: dphi(ME2,RE3)
      // ipair 19: dphi(RE3,ME4)
      // ipair 20: dphi(ME1/1,RE4)
      // ipair 21: dphi(ME1/2,RE4)
      // ipair 22: dphi(ME2,RE4)
      // ipair 23: dphi(ME3,RE4)
      for (int ipair = 9; ipair < (9+15); ++ipair) {
        std::vector<l1t::EMTFHitExtra> myhits1;
        std::vector<l1t::EMTFHitExtra> myhits2;
        if (ipair == 9) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 10) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 11) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 12) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 13) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 14) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 15) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 16) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 17) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 18) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 19) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 20) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 21) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 22) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 23) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else {
          assert(false && "invalid ipair");
        }

        if (myhits1.size() > 0 && myhits2.size() > 0) {
          std::uniform_int_distribution<> index1(0, myhits1.size()-1);
          std::uniform_int_distribution<> index2(0, myhits2.size()-1);
          l1t::EMTFHitExtra myhit1 = myhits1.at(index1(genrd));
          l1t::EMTFHitExtra myhit2 = myhits2.at(index2(genrd));

          // Histogramming
          int ifr = (int(isFront(myhit1.subsystem, myhit1.station, myhit1.ring, myhit1.chamber, myhit1.subsector)) << 1) | int(isFront(myhit2.subsystem, myhit2.station, myhit2.ring, myhit2.chamber, myhit2.subsector));
          int ieta = int((absEta - 1.2) / (2.4 - 1.2) * 12);

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

          // dphi
          //int dphi = myhit1.phi_fp - myhit2.phi_fp;
          int dphi = l1t::calc_phi_loc_int(myhit1.phi_glob_deg, sector) - l1t::calc_phi_loc_int(myhit2.phi_glob_deg, sector);
          if (charge > 0)  dphi = -dphi;

          const float common_dphi_ratio = 1.0;  //FIXME
          //const float common_dphi_ratio = table_common_dphi[ipair][ifr][ieta];
          const int common_dphi = static_cast<int>(std::round(1.0 / common_dphi_ratio * dphi));

          // dphi vs 1/pT
          hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_stp%i_frp%i_eta%i", ipair, 4, ieta);  // inclusive
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);

          hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, 4, ieta);  // inclusive
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);

          // dphi vs |eta|
          if (ipt != -1) {
            hname = Form("deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, dphi);
            hname = Form("deflection_stp%i_frp%i_pt%i", ipair, 4, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, dphi);

            hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, common_dphi);
            hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, 4, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(absEta, common_dphi);
          }
        }
      }  // end make deflection angles (with RPC)

    }  // end loop over sector


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
  TH1F* h;
  TH2F* h2;

  // Efficiency vs pT
  // Make [mode] x [eta] where
  //   mode 0,1,2,3 = MuOpen, DoubleMu, SingleMu, Mode15
  //   eta  0,1,2,3 = 1.2-1.6, 1.6-2.0, 2.0-2.4, inclusive

  const Double_t pt_bins[47] = {1., 1.5, 2., 2.5, 3., 4., 5., 6., 7., 8., 9., 10., 12., 14., 16., 18., 20., 22., 24., 26., 28., 30., 35., 40., 45., 50., 55., 60., 70., 80., 100., 120., 140., 160., 180., 200., 250., 300., 350., 400., 450., 500., 550., 600., 700., 800., 1000.};

  for (int i=0; i<4; ++i) {  // mode
    for (int j=0; j<4; ++j) {  // eta
      for (int k=0; k<2; ++k) {
        if (k == 0)
          hname = Form("denom_vs_pt_mode%i_eta%i", i, j);
        else
          hname = Form("num_vs_pt_mode%i_eta%i", i, j);
        h = new TH1F(hname, "; gen p_{T} [GeV]; entries", 47-1, pt_bins);
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

  // RPC-CSC st 0-3
  for (int i=0; i<4; ++i) {
    hname = Form("deflection_rpc_csc_st%i", i+1);
    h = new TH1F(hname, "; RPC #phi - CSC #phi [integer unit]", 121, -60.5, 60.5);
    histograms_[hname] = h;
  }

  // Deflection angles
  // station pair 0-8: (1/1,2), (1/1,3), (1/1,4), (1/2,2), (1/2,3), (1/2,4), (2,3), (2,4), (3,4)
  // station pair 9-24: (RE1/2,ME2), (RE1/2,ME3), (RE1/2,ME4),
  //                    (ME1/1,RE2), (ME1/2,RE2), (RE2,ME3), (RE2,ME4),
  //                    (ME1/1,RE3), (ME1/2,RE3), (ME2,RE3), (RE3,ME4),
  //                    (ME1/1,RE4), (ME1/2,RE4), (ME2,RE4), (ME3,RE4)
  // f/r pair 0-3: (R,R), (R,F), (F,R), (F,F)
  // eta 0-11: (1.2,1.3), (1.3,1.4), ... , (2.3, 2.4)
  // pt 0-7: 2,3,5,10,20,50,100,200
  TString deflection_labels1[] = {
      "#phi(ME1/1) - #phi(ME2/n)",
      "#phi(ME1/1) - #phi(ME3/n)",
      "#phi(ME1/1) - #phi(ME4/n)",
      "#phi(ME1/2) - #phi(ME2/n)",
      "#phi(ME1/2) - #phi(ME3/n)",
      "#phi(ME1/2) - #phi(ME4/n)",
      "#phi(ME2/n) - #phi(ME3/n)",
      "#phi(ME2/n) - #phi(ME4/n)",
      "#phi(ME3/n) - #phi(ME4/n)",
      "#phi(RE1/2) - #phi(ME2/n)",
      "#phi(RE1/2) - #phi(ME3/n)",
      "#phi(RE1/2) - #phi(ME4/n)",
      "#phi(ME1/1) - #phi(RE2/n)",
      "#phi(ME1/2) - #phi(RE2/n)",
      "#phi(RE2/n) - #phi(ME3/n)",
      "#phi(RE2/n) - #phi(ME4/n)",
      "#phi(ME1/1) - #phi(RE3/n)",
      "#phi(ME1/2) - #phi(RE3/n)",
      "#phi(ME2/n) - #phi(RE3/n)",
      "#phi(RE3/n) - #phi(ME4/n)",
      "#phi(ME1/1) - #phi(RE4/n)",
      "#phi(ME1/2) - #phi(RE4/n)",
      "#phi(ME2/n) - #phi(RE4/n)",
      "#phi(ME3/n) - #phi(RE4/n)"
  };
  TString deflection_labels2[] = {
      "R+R",
      "R+F",
      "F+R",
      "F+F",
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
      "2#leqp_{T}<2.04",
      "3#leqp_{T}<3.09",
      "5#leqp_{T}<5.26",
      "10#leqp_{T}<11.1",
      "20#leqp_{T}<25.0",
      "50#leqp_{T}<66.7",
      "100#leqp_{T}<125",
      "200#leqp_{T}<250"
  };

  for (int ipair=0; ipair<(9+15); ++ipair) {
    for (int ifr=0; ifr<5; ++ifr) {
      for (int ieta=0; ieta<12; ++ieta) {
        // dphi vs 1/pT
        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = new TH2F(hname, TString("; 1/p_{T} [1/GeV]; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels3[ieta]+"}", 50, 0., 0.5, 801, -801, 801);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }
      for (int ipt=0; ipt<8; ++ipt) {
        // dphi vs |eta|
        hname = Form("deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
        h2 = new TH2F(hname, TString("; |#eta|; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels4[ipt]+"}", 48, 1.2, 2.4, 801, -801, 801);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }
    }
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

  for (const auto& kv : histogram2Ds_) {
    kv.second->SetDirectory(gDirectory);
  }

  f->Write();
  f->Close();
}

// _____________________________________________________________________________
double RPCIntegration::angle_func(double part_pt, double part_eta, int hit_subsystem, int hit_station, int hit_ring, bool hit_fr, float hit_eta) {
  static std::vector<double> coefficients;

  int num_z_bins = 20;
  int num_eta_bins = 2048;

  if (coefficients.size() == 0) {
    TFile* tfile = TFile::Open(angleFileName_.c_str(), "READ");
    assert(tfile != nullptr);
    TTree* ttree = (TTree*) tfile->Get("tree");
    assert(ttree != nullptr);
    std::vector<double> * tmp_coefficients = 0;
    ttree->SetBranchAddress("coefficients", &tmp_coefficients);
    ttree->GetEntry(0);
    coefficients = (*tmp_coefficients);  // copy
    assert(coefficients.size() == (unsigned) (num_z_bins * num_eta_bins));
  }

  auto get_eta_bin = [](float x) {
    // nbinsx, xlow, xup = 2048, 1.1, 2.5
    int b = static_cast<int>(std::round((x - 1.1) * 2048./(2.5 - 1.1)));
    return b;
  };

  auto get_z_bin = [](int subsystem, int station, int ring, bool fr) {
    int b = -1;
    if (subsystem == TriggerPrimitive::kCSC) {
      if (station == 1 && (ring == 1 || ring == 4)) {
        b = fr ? 0 : 1;
      } else if (station == 1 && ring == 2) {
        b = fr ? 2 : 3;
      } else if (station == 1 && ring == 3) {
        b = fr ? 4 : 5;
      } else if (station == 2) {
        b = fr ? 6 : 7;
      } else if (station == 3) {
        b = fr ? 8 : 9;
      } else if (station == 4) {
        b = fr ? 10 : 11;
      }
    } else if (subsystem == TriggerPrimitive::kRPC) {
      if (station == 1) {
        b = fr ? 12 : 13;
      } else if (station == 2) {
        b = fr ? 14 : 15;
      } else if (station == 3) {
        b = fr ? 16 : 17;
      } else if (station == 4) {
        b = fr ? 18 : 19;
      }
    }
    assert(b != -1);
    return b;
  };

  const int b = get_z_bin(hit_subsystem, hit_station, hit_ring, hit_fr) * num_eta_bins + get_eta_bin(part_eta);
  assert(0 <= b && b < (num_z_bins * num_eta_bins));
  return coefficients.at(b);
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
