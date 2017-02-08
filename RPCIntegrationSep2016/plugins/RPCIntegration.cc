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


#if 0
static const float table_common_dphi[9][4][12] = {
  {
    {1,1,1,1,0.846323,0.852672,0.867885,0.875119,0.892154,0.893014,0.897351,0.900686},
    {1,1,1,1,0.824808,0.829518,0.829463,0.833866,0.833403,0.836744,0.834175,0.831884},
    {1,1,1,1,1.02036,1.02665,1.03454,1.04139,1.05281,1.06052,1.06375,1.07253},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,0.811959,0.794714,0.813745,0.835994,0.847929,0.861594,0.87255,0.875903},
    {1,1,1,1,0.793768,0.835972,0.845788,0.850213,0.862102,0.859298,0.862829,0.864425},
    {1,1,1,1,0.920532,0.954955,0.975204,0.987141,0.993699,1.00395,1.01339,1.01479},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,0.817627,0.836039,0.804238,0.827455,0.841192,0.851749,0.867615,0.874569},
    {1,1,1,1,0.721606,0.772805,0.836832,0.842935,0.858716,0.856335,0.868038,0.86981},
    {1,1,1,1,0.85171,0.887388,0.970499,0.983848,0.988485,0.990061,1.00743,1.00489},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.621467,0.65959,0.69841,0.72374,1,1,1,1,1,1,1,1},
    {0.715207,0.722377,0.694042,0.679957,1,1,1,1,1,1,1,1},
    {1.16572,1.15656,1.14647,1.12091,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.980397,0.857843,0.871264,0.866576,1,1,1,1,1,1,1,1},
    {0.876505,0.708824,0.727835,0.743702,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {0.885017,1.19218,1.25679,1.1821,1,1,1,1,1,1,1,1}
  }, {
    {1.29828,1.00485,0.942082,0.924435,1,1,1,1,1,1,1,1},
    {0.773278,0.731348,0.706409,0.736268,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1.41102,1.25659,1.2919,1.24193,1,1,1,1,1,1,1,1}
  }, {
    {0.724366,1.21378,1.06392,0.923372,0.952012,1.24746,0.976344,0.73218,0.634689,0.858273,0.820337,0.868109},
    {0.666422,0.62379,0.603248,0.652976,0.600217,0.616858,0.596928,0.610453,0.62334,0.630044,0.628413,0.625042},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {0.590317,0.865265,0.724441,0.62982,0.729087,0.881415,0.976484,1.05575,1.11075,1.06125,1.09558,1.0914}
  }, {
    {1.12475,1.22047,1.18725,1.04438,1.03964,1.23569,1.46359,1.12307,0.822121,0.735986,0.984462,1.03996},
    {0.901263,0.88121,0.846819,0.834914,0.900472,0.867615,0.813411,0.750137,0.67895,0.687514,0.699622,0.716093},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1.27643,1.0456,0.953896,0.826806,0.87114,1.12494,1.26233,1.31073,1.15908,1.16063,1.21068,1.28293}
  }, {
    {0.972322,0.993049,0.999165,0.954587,0.971144,1.00816,1.03605,1.04082,1.05927,1.07764,0.753684,0.791347},
    {0.769051,0.864449,0.899895,0.857426,0.853095,0.862031,0.918589,1.01252,1.10444,1.28828,0.601397,0.449829},
    {1.26031,1.59854,1.61259,1.54695,1.31621,1.49841,1.73039,1.8196,1.94749,1.94946,1.05409,0.958127},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }
};
#endif

static const float table_common_dphi_2[9+16][4][12] = {
  {
    {1,1,1,0.820043,0.82708,0.831583,0.834213,0.837367,0.84029,0.840705,0.839633,0.838238},
    {1,1,1,0.820043,0.82708,0.831583,0.834213,0.837367,0.84029,0.840705,0.839633,0.838238},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.874003,0.846341,0.835298,0.840936,0.849317,0.856242,0.859463,0.863371,0.86599},
    {1,1,1,0.874003,0.846341,0.835298,0.840936,0.849317,0.856242,0.859463,0.863371,0.86599},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.863733,0.838654,0.860418,0.834359,0.843563,0.855307,0.858881,0.865472,0.871958},
    {1,1,1,0.863733,0.838654,0.860418,0.834359,0.843563,0.855307,0.858881,0.865472,0.871958},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.595706,0.646637,0.670203,0.683458,0.654962,1,1,1,1,1,1,1},
    {0.595706,0.646637,0.670203,0.683458,0.654962,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.739077,0.689079,0.704829,0.726914,0.722125,1,1,1,1,1,1,1},
    {0.739077,0.689079,0.704829,0.726914,0.722125,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1.05396,0.751002,0.696393,0.718516,0.663915,1,1,1,1,1,1,1},
    {1.05396,0.751002,0.696393,0.718516,0.663915,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.714687,0.734524,0.7089,0.773934,0.672868,0.701079,0.649278,0.621475,0.629695,0.636909,0.630848,0.636178},
    {0.714687,0.734524,0.7089,0.773934,0.672868,0.701079,0.649278,0.621475,0.629695,0.636909,0.630848,0.636178},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1.00863,1.03259,0.978771,1.02234,0.995201,0.939361,0.897054,0.815615,0.68744,0.688995,0.700481,0.730682},
    {1.00863,1.03259,0.978771,1.02234,0.995201,0.939361,0.897054,0.815615,0.68744,0.688995,0.700481,0.730682},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.863733,0.864397,0.903047,0.867765,0.841059,0.947532,0.960251,0.996716,1.02304,1.05384,0.78236,0.759511},
    {0.863733,0.864397,0.903047,0.867765,0.841059,0.947532,0.960251,0.996716,1.02304,1.05384,0.78236,0.759511},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.40396,0.460256,0.503327,0.522028,0.467695,1,1,1,1,1,1,1},
    {0.40396,0.460256,0.503327,0.522028,0.467695,1,1,1,1,1,1,1},
    {0.848942,0.883508,0.885513,0.90765,0.790192,1,1,1,1,1,1,1},
    {0.848942,0.883508,0.885513,0.90765,0.790192,1,1,1,1,1,1,1}
  }, {
    {0.59125,0.504447,0.538756,0.566312,0.551119,1,1,1,1,1,1,1},
    {0.59125,0.504447,0.538756,0.566312,0.551119,1,1,1,1,1,1,1},
    {0.916251,0.869807,0.871675,0.892012,0.900137,1,1,1,1,1,1,1},
    {0.916251,0.869807,0.871675,0.892012,0.900137,1,1,1,1,1,1,1}
  }, {
    {0.910361,0.603623,0.535183,0.562191,0.467702,1,1,1,1,1,1,1},
    {0.910361,0.603623,0.535183,0.562191,0.467702,1,1,1,1,1,1,1},
    {1.30895,0.934051,0.874372,0.891425,0.841739,1,1,1,1,1,1,1},
    {1.30895,0.934051,0.874372,0.891425,0.841739,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.417195,0.66493,1,1,1,1,1,1,1},
    {1,1,1,0.623872,0.379797,1,1,1,1,1,1,1},
    {1,1,1,0.833044,0.860509,1,1,1,1,1,1,1},
    {1,1,1,0.821939,0.952113,1,1,1,1,1,1,1}
  }, {
    {0.608677,0.606185,0.607688,0.58792,0.575736,1,1,1,1,1,1,1},
    {0.608677,0.606185,0.607688,0.58792,0.575736,1,1,1,1,1,1,1},
    {0.993544,0.967241,0.942351,0.912429,0.785368,1,1,1,1,1,1,1},
    {0.993544,0.967241,0.942351,0.912429,0.785368,1,1,1,1,1,1,1}
  }, {
    {0.629345,0.541604,0.273468,0.298631,1,1,1,1,1,1,1,1},
    {0.629345,0.541604,0.273468,0.298631,1,1,1,1,1,1,1,1},
    {0.69861,0.701956,0.434163,0.358692,1,1,1,1,1,1,1,1},
    {0.69861,0.701956,0.434163,0.358692,1,1,1,1,1,1,1,1}
  }, {
    {1.01321,0.913336,0.67359,0.534053,1,1,1,1,1,1,1,1},
    {1.01321,0.913336,0.67359,0.534053,1,1,1,1,1,1,1,1},
    {0.961345,0.964359,0.743597,0.59056,1,1,1,1,1,1,1,1},
    {0.961345,0.964359,0.743597,0.59056,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.80093,0.79493,0.736691,1,1,1,1,1,1},
    {1,1,1,0.80093,0.79493,0.736691,1,1,1,1,1,1},
    {1,1,1,0.941917,0.954922,0.896406,1,1,1,1,1,1},
    {1,1,1,0.941917,0.954922,0.896406,1,1,1,1,1,1}
  }, {
    {0.673073,0.539597,0.570564,0.591685,0.588142,1,1,1,1,1,1,1},
    {0.673073,0.539597,0.570564,0.591685,0.588142,1,1,1,1,1,1,1},
    {1.03355,0.900007,0.9011,0.909265,0.934533,1,1,1,1,1,1,1},
    {1.03355,0.900007,0.9011,0.909265,0.934533,1,1,1,1,1,1,1}
  }, {
    {1.52333,1.6653,1.45929,1.29139,1.11284,1.41568,1,1,1,1,1,1},
    {1.52333,1.6653,1.45929,1.29139,1.11284,1.41568,1,1,1,1,1,1},
    {1.54378,1.61821,1.50943,1.31989,1.1357,1.39463,1,1,1,1,1,1},
    {1.54378,1.61821,1.50943,1.31989,1.1357,1.39463,1,1,1,1,1,1}
  }, {
    {0.454183,0.43908,0.461864,0.404545,0.293196,0.407099,1,1,1,1,1,1},
    {0.454183,0.43908,0.461864,0.404545,0.293196,0.407099,1,1,1,1,1,1},
    {0.631374,0.642729,0.701027,0.631162,0.447971,0.488387,1,1,1,1,1,1},
    {0.631374,0.642729,0.701027,0.631162,0.447971,0.488387,1,1,1,1,1,1}
  }, {
    {1,1,1,0.799914,0.79283,0.793143,0.768125,1,1,1,1,1},
    {1,1,1,0.799914,0.79283,0.793143,0.768125,1,1,1,1,1},
    {1,1,1,0.927992,0.951392,0.954368,0.917889,1,1,1,1,1},
    {1,1,1,0.927992,0.951392,0.954368,0.917889,1,1,1,1,1}
  }, {
    {1.04096,0.736841,0.628281,0.647118,0.569925,1,1,1,1,1,1,1},
    {1.04096,0.736841,0.628281,0.647118,0.569925,1,1,1,1,1,1,1},
    {0.958433,1.04047,0.950703,0.960662,0.899172,1,1,1,1,1,1,1},
    {0.958433,1.04047,0.950703,0.960662,0.899172,1,1,1,1,1,1,1}
  }, {
    {1.31021,1.43808,1.47274,1.4972,1.26286,1.19162,1.24413,1,1,1,1,1},
    {1.31021,1.43808,1.47274,1.4972,1.26286,1.19162,1.24413,1,1,1,1,1},
    {1.21991,1.35603,1.44539,1.39621,1.25624,1.15597,1.2775,1,1,1,1,1},
    {1.21991,1.35603,1.44539,1.39621,1.25624,1.15597,1.2775,1,1,1,1,1}
  }, {
    {1.35556,1.28827,1.32689,1.32553,1.22103,1.20486,1.31835,1,1,1,1,1},
    {1.35556,1.28827,1.32689,1.32553,1.22103,1.20486,1.31835,1,1,1,1,1},
    {1.63258,1.61248,1.6682,1.72202,1.43366,1.4386,1.54419,1,1,1,1,1},
    {1.63258,1.61248,1.6682,1.72202,1.43366,1.4386,1.54419,1,1,1,1,1}
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

          //const float common_dphi_ratio = table_common_dphi[ipair][ifr][ieta];
          const float common_dphi_ratio = table_common_dphi_2[ipair][ifr][ieta];
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
      // ipair 12: null
      // ipair 13: dphi(ME1/1,RE2)
      // ipair 14: dphi(ME1/2,RE2)
      // ipair 15: dphi(RE2,ME3)
      // ipair 16: dphi(RE2,ME4)
      // ipair 17: dphi(ME1/1,RE3)
      // ipair 18: dphi(ME1/2,RE3)
      // ipair 19: dphi(ME2,RE3)
      // ipair 20: dphi(RE3,ME4)
      // ipair 21: dphi(ME1/1,RE4)
      // ipair 22: dphi(ME1/2,RE4)
      // ipair 23: dphi(ME2,RE4)
      // ipair 24: dphi(ME3,RE4)
      for (int ipair = 9; ipair < (9+16); ++ipair) {
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
          // pass
        } else if (ipair == 13) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 14) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
        } else if (ipair == 15) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 16) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 17) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 18) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 19) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
        } else if (ipair == 20) {
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 3); });
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 21) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 22) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 1 && hit.ring == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 23) {
          std::copy_if(myhits_csc.begin(), myhits_csc.end(), std::back_inserter(myhits1), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 2); });
          std::copy_if(myhits_rpc.begin(), myhits_rpc.end(), std::back_inserter(myhits2), [](const l1t::EMTFHitExtra& hit) { return (hit.station == 4); });
        } else if (ipair == 24) {
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

          //const float common_dphi_ratio = table_common_dphi[ipair][ifr][ieta];
          const float common_dphi_ratio = table_common_dphi_2[ipair][ifr][ieta];
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
  // station pair 9-25: (RE1/2,ME2), (RE1/2,ME3), (RE1/2,ME4), null
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
      //
      "#phi(RE1/2) - #phi(ME2/n)",
      "#phi(RE1/2) - #phi(ME3/n)",
      "#phi(RE1/2) - #phi(ME4/n)",
      "null",
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

  for (int ipair=0; ipair<(9+16); ++ipair) {
    for (int ifr=0; ifr<5; ++ifr) {
      for (int ieta=0; ieta<12; ++ieta) {
        // dphi vs 1/pT
        hname = Form("deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        h2 = new TH2F(hname, TString("; 1/p_{T} [1/GeV]; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels3[ieta]+"}", 50, 0., 0.5, 808, -808, 808);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_stp%i_frp%i_eta%i", ipair, ifr, ieta);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }
      for (int ipt=0; ipt<8; ++ipt) {
        // dphi vs |eta|
        hname = Form("deflection_stp%i_frp%i_pt%i", ipair, ifr, ipt);
        h2 = new TH2F(hname, TString("; |#eta|; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels4[ipt]+"}", 48, 1.2, 2.4, 808, -808, 808);
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
