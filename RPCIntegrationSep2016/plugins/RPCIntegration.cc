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
static const float table_common_dphi_ieta[9][16][12] = {
  {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,0.826329,0.951533,1,1,1,1,1,1,1},
    {1,1,1,0.826329,0.951533,1,1,1,1,1,1,1},
    {1,1,1,4.18494,0.831719,0.835802,0.838787,0.840769,0.839386,0.840755,0.839914,0.838162},
    {1,1,1,4.18494,0.831719,0.835802,0.838787,0.840769,0.839386,0.840755,0.839914,0.838162},
    {1,1,1,0.82631,0.955249,1,1,1,1,1,1,1},
    {1,1,1,0.82631,0.955249,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,-0.184858,0.838774,0.857649,1,1,1,1,1,1},
    {1,1,1,-0.184858,0.838774,0.857649,1,1,1,1,1,1},
    {1,1,1,1.15616,0.860875,0.846226,0.852129,0.857306,0.860823,0.859827,0.862646,0.867052},
    {1,1,1,1.15616,0.860875,0.846226,0.852129,0.857306,0.860823,0.859827,0.862646,0.867052},
    {1,1,1,0.995954,0.993803,1.00848,1,1,1,1,1,1},
    {1,1,1,0.995954,0.993803,1.00848,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1.91611,0.843419,0.826703,1,1,1,1,1,1},
    {1,1,1,1.91611,0.843419,0.826703,1,1,1,1,1,1},
    {1,1,1,4.73918,0.845645,0.870426,0.850829,0.857627,0.863674,0.861718,0.867913,0.874991},
    {1,1,1,4.73918,0.845645,0.870426,0.850829,0.857627,0.863674,0.861718,0.867913,0.874991},
    {1,1,1,0.980512,0.986643,0.979748,1,1,1,1,1,1},
    {1,1,1,0.980512,0.986643,0.979748,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.461093,0.470769,1,0.438645,1,1,1,1,1,1,1,1},
    {0.461093,0.470769,1,0.438645,1,1,1,1,1,1,1,1},
    {0.550523,0.611699,1,0.573474,1,1,1,1,1,1,1,1},
    {0.550523,0.611699,1,0.573474,1,1,1,1,1,1,1,1},
    {0.852788,0.808882,1,0.74258,1,1,1,1,1,1,1,1},
    {0.852788,0.808882,1,0.74258,1,1,1,1,1,1,1,1},
    {0.871892,0.878511,1,0.855374,1,1,1,1,1,1,1,1},
    {0.871892,0.878511,1,0.855374,1,1,1,1,1,1,1,1},
    {0.625469,0.595089,0.603792,0.613069,0.540642,1,1,1,1,1,1,1},
    {0.625469,0.595089,0.603792,0.613069,0.540642,1,1,1,1,1,1,1},
    {0.668854,0.696153,0.735713,0.744354,0.689826,1,1,1,1,1,1,1},
    {0.668854,0.696153,0.735713,0.744354,0.689826,1,1,1,1,1,1,1},
    {0.98117,0.927771,0.914328,0.89406,0.890856,1,1,1,1,1,1,1},
    {0.98117,0.927771,0.914328,0.89406,0.890856,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.493093,0.584856,1,0.575428,1,1,1,1,1,1,1,1},
    {0.493093,0.584856,1,0.575428,1,1,1,1,1,1,1,1},
    {0.606009,0.677803,1,0.601322,1,1,1,1,1,1,1,1},
    {0.606009,0.677803,1,0.601322,1,1,1,1,1,1,1,1},
    {0.809538,0.852994,1,0.838924,1,1,1,1,1,1,1,1},
    {0.809538,0.852994,1,0.838924,1,1,1,1,1,1,1,1},
    {0.871868,0.928714,1,0.876958,1,1,1,1,1,1,1,1},
    {0.871868,0.928714,1,0.876958,1,1,1,1,1,1,1,1},
    {0.574237,0.637473,0.702528,0.734866,0.744734,1,1,1,1,1,1,1},
    {0.574237,0.637473,0.702528,0.734866,0.744734,1,1,1,1,1,1,1},
    {0.712057,0.714718,0.733573,0.745892,0.756625,1,1,1,1,1,1,1},
    {0.712057,0.714718,0.733573,0.745892,0.756625,1,1,1,1,1,1,1},
    {0.964301,0.960763,0.997004,0.978608,0.993706,1,1,1,1,1,1,1},
    {0.964301,0.960763,0.997004,0.978608,0.993706,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.527599,0.595174,1,0.559138,1,1,1,1,1,1,1,1},
    {0.527599,0.595174,1,0.559138,1,1,1,1,1,1,1,1},
    {0.609337,0.66386,1,0.58314,1,1,1,1,1,1,1,1},
    {0.609337,0.66386,1,0.58314,1,1,1,1,1,1,1,1},
    {0.846807,0.858257,1,0.829497,1,1,1,1,1,1,1,1},
    {0.846807,0.858257,1,0.829497,1,1,1,1,1,1,1,1},
    {0.877568,0.929633,1,0.873891,1,1,1,1,1,1,1,1},
    {0.877568,0.929633,1,0.873891,1,1,1,1,1,1,1,1},
    {0.572111,0.639567,0.672575,0.725796,0.723734,1,1,1,1,1,1,1},
    {0.572111,0.639567,0.672575,0.725796,0.723734,1,1,1,1,1,1,1},
    {0.702682,0.707656,0.72427,0.747779,0.754796,1,1,1,1,1,1,1},
    {0.702682,0.707656,0.72427,0.747779,0.754796,1,1,1,1,1,1,1},
    {0.980709,0.95144,0.977001,0.989567,0.984446,1,1,1,1,1,1,1},
    {0.980709,0.95144,0.977001,0.989567,0.984446,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1.93922,1.20575,1.37434,1.4417,1,1,1,1,1,1,1,1},
    {1.93922,1.20575,1.37434,1.4417,1,1,1,1,1,1,1,1},
    {2.33721,1.26093,1.5699,1.54553,1,1,1,1,1,1,1,1},
    {2.33721,1.26093,1.5699,1.54553,1,1,1,1,1,1,1,1},
    {2.1858,1,1.38776,1.49085,1,1,1,1,1,1,1,1},
    {2.1858,1,1.38776,1.49085,1,1,1,1,1,1,1,1},
    {2.47334,1,1.38751,1.68873,1,1,1,1,1,1,1,1},
    {2.47334,1,1.38751,1.68873,1,1,1,1,1,1,1,1},
    {0.556038,0.471826,0.534429,0.507992,0.378368,0.593094,1,1,1,1,1,1},
    {0.556038,0.471826,0.534429,0.507992,0.378368,0.593094,1,1,1,1,1,1},
    {0.44502,0.501535,0.563239,0.588559,0.480622,0.580772,0.590541,0.617739,0.610488,0.639079,0.625721,0.628907},
    {0.44502,0.501535,0.563239,0.588559,0.480622,0.580772,0.590541,0.617739,0.610488,0.639079,0.625721,0.628907},
    {0.791157,1.22119,1.16088,0.938632,0.877565,0.966703,1,1,1,1,1,1},
    {0.791157,1.22119,1.16088,0.938632,0.877565,0.966703,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.340075,1.38479,1.41902,1.56352,1,1,1,1,1,1,1,1},
    {0.340075,1.38479,1.41902,1.56352,1,1,1,1,1,1,1,1},
    {1.92384,1.28143,1.13018,1.60926,1,1,1,1,1,1,1,1},
    {1.92384,1.28143,1.13018,1.60926,1,1,1,1,1,1,1,1},
    {0.566284,1,1.38579,1.55373,1,1,1,1,1,1,1,1},
    {0.566284,1,1.38579,1.55373,1,1,1,1,1,1,1,1},
    {2.04622,1,1.23656,1.67177,1,1,1,1,1,1,1,1},
    {2.04622,1,1.23656,1.67177,1,1,1,1,1,1,1,1},
    {1.5213,0.994794,1.23628,0.580533,0.418491,0.668831,1,1,1,1,1,1},
    {1.5213,0.994794,1.23628,0.580533,0.418491,0.668831,1,1,1,1,1,1},
    {0.829185,0.639666,0.750328,0.619916,0.509476,0.542842,0.588804,0.626851,0.651338,0.71124,0.720077,0.729862},
    {0.829185,0.639666,0.750328,0.619916,0.509476,0.542842,0.588804,0.626851,0.651338,0.71124,0.720077,0.729862},
    {1.09053,1.10787,1.12543,1.09338,0.991479,1.05019,1,1,1,1,1,1},
    {1.09053,1.10787,1.12543,1.09338,0.991479,1.05019,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1.08113,0.863649,0.795501,1,1.10989,0.845028,1,1,1,1,1,1},
    {1.08113,0.863649,0.795501,1,1.10989,0.845028,1,1,1,1,1,1},
    {0.990336,0.365578,0.35958,1,0.593591,0.414396,1,1,1,1,1,1},
    {0.990336,0.365578,0.35958,1,0.593591,0.414396,1,1,1,1,1,1},
    {1.13743,1.08006,0.786491,1.1168,1.13352,0.797259,1,1,1,1,1,1},
    {1.13743,1.08006,0.786491,1.1168,1.13352,0.797259,1,1,1,1,1,1},
    {0.674709,0.735516,0.59279,1.06562,0.601168,0.607802,1,1,1,1,1,1},
    {0.674709,0.735516,0.59279,1.06562,0.601168,0.607802,1,1,1,1,1,1},
    {1.41907,1.35137,1.19947,1.26818,1.32891,1.01325,1,1,1,1,1,1},
    {1.41907,1.35137,1.19947,1.26818,1.32891,1.01325,1,1,1,1,1,1},
    {1.01876,1.05772,0.97773,1.01656,1.09945,0.797823,1.04252,1.102,1.09416,0.623252,0.702057,0.857394},
    {1.01876,1.05772,0.97773,1.01656,1.09945,0.797823,1.04252,1.102,1.09416,0.623252,0.702057,0.857394},
    {1.47176,1.93444,1.63112,1.67047,1.69935,1.15919,1,1,1,1,1,1},
    {1.47176,1.93444,1.63112,1.67047,1.69935,1.15919,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }
};



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
          //eta = (in_track.endcap == 2) ? -eta : eta;

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

          const float common_dphi_ratio = table_common_dphi_ieta[ipair][ifr][ieta];
          //const float common_dphi_ratio = table_common_dphi_itheta[ipair][ifr][itheta];
          const int common_dphi = static_cast<int>(std::round(1.0 / common_dphi_ratio * dphi));

          // dphi vs particle 1/pT
          hname = Form("deflection_pair%i_fr%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_pair%i_fr%i_eta%i", ipair, 16, ieta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_pair%i_fr%i_theta%i", ipair, ifr, itheta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);
          hname = Form("deflection_pair%i_fr%i_theta%i", ipair, 16, itheta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, dphi);

          // corrected dphi vs particle 1/pT
          hname = Form("common_deflection_pair%i_fr%i_eta%i", ipair, ifr, ieta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_pair%i_fr%i_eta%i", ipair, 16, ieta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_pair%i_fr%i_theta%i", ipair, ifr, itheta);
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);
          hname = Form("common_deflection_pair%i_fr%i_theta%i", ipair, 16, itheta);  // inclusive ifr
          h2 = histogram2Ds_.at(hname);
          h2->Fill(1.0/pt, common_dphi);

          if (ipt != -1) {
            // dphi vs particle |eta|
            hname = Form("deflection_pair%i_fr%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(part.eta()), dphi);
            hname = Form("deflection_pair%i_fr%i_pt%i", ipair, 16, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(part.eta()), dphi);

            // corrected dphi vs particle |eta|
            hname = Form("common_deflection_pair%i_fr%i_pt%i", ipair, ifr, ipt);
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(part.eta()), common_dphi);
            hname = Form("common_deflection_pair%i_fr%i_pt%i", ipair, 16, ipt);  // inclusive
            h2 = histogram2Ds_.at(hname);
            h2->Fill(std::abs(part.eta()), common_dphi);
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
      "RR/RR",
      "RR/RF",
      "RR/MR",
      "RR/MF",
      "RF/RR",
      "RF/RF",
      "RF/MR",
      "RF/MF",
      "MR/RR",
      "MR/RF",
      "MR/MR",
      "MR/MF",
      "MF/RR",
      "MF/RF",
      "MF/MR",
      "MF/MF",
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
        hname = Form("deflection_pair%i_fr%i_eta%i", ipair, ifr, ieta);
        h2 = new TH2F(hname, TString("; 1/p_{T} [1/GeV]; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels3[ieta]+"}", 50, 0., 0.5, 808, -808, 808);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_pair%i_fr%i_eta%i", ipair, ifr, ieta);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }

      for (int itheta=0; itheta<16; ++itheta) {
        hname = Form("deflection_pair%i_fr%i_theta%i", ipair, ifr, itheta);
        h2 = new TH2F(hname, TString("; 1/p_{T} [1/GeV]; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels4[itheta]+"}", 50, 0., 0.5, 808, -808, 808);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_pair%i_fr%i_theta%i", ipair, ifr, itheta);
        histogram2Ds_[hname] = (TH2F*) h2->Clone(hname);
      }

      // dphi vs |eta|
      for (int ipt=0; ipt<8; ++ipt) {
        hname = Form("deflection_pair%i_fr%i_pt%i", ipair, ifr, ipt);
        h2 = new TH2F(hname, TString("; |#eta|; ")+deflection_labels1[ipair]+" ("+deflection_labels2[ifr]+") {"+deflection_labels5[ipt]+"}", 48, 1.2, 2.4, 808, -808, 808);
        histogram2Ds_[hname] = h2;

        hname = Form("common_deflection_pair%i_fr%i_pt%i", ipair, ifr, ipt);
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
