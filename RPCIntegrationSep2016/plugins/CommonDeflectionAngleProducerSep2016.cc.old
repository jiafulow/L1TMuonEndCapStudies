//
// This is an example CMSSW code that applies the deflection angle corrections
// that corrects the angles to the common reference planes (same z-positions).
//
// This is the very first version. It will be going through a few iterations
// before the final version, so please bear in mind that everything in this
// piece of code is very unstable. Notable to-do items include:
// - Switch from eta to theta
// - Reduce eta or theta binning
// - Use integers instead of floats
// - Implement in a firmware-friendly format, most likely in a big LUT
//
// That being said, my goal is to keep the interface of the function
// get_common_dphi() the same, and simply modify the internals of the function
// as needed. So, in principle, users should only have to worry about how to
// interface with the function, and not to mess with the internals.
//

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "DataFormatsSep2016/L1TMuon/interface/EMTFHit.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFHitExtra.h"
//#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrack.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrackExtra.h"

//#include "L1TriggerSep2016/L1TMuonEndCap/interface/EMTFTrackTools.hh"

namespace l1t = L1TMuonEndCap;


// _____________________________________________________________________________
// Class declaration
class CommonDeflectionAngleProducer : public edm::EDAnalyzer {
public:
  explicit CommonDeflectionAngleProducer(const edm::ParameterSet& iConfig);
  ~CommonDeflectionAngleProducer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  // Core functions
  bool get_common_dphi(const l1t::EMTFHitExtra& hit1, const l1t::EMTFHitExtra& hit2, int& dphi, int& common_dphi);
  float get_common_dphi_correction(const l1t::EMTFHitExtra& hit1, const l1t::EMTFHitExtra& hit2);


  //virtual void beginJob() override;
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
  //virtual void endJob() override;

  // Options
  const edm::InputTag emuHitTag_;
  const edm::InputTag emuTrackTag_;

  int verbose_;

  // Member data
  edm::EDGetTokenT<l1t::EMTFHitExtraCollection>   emuHitToken_;
  edm::EDGetTokenT<l1t::EMTFTrackExtraCollection> emuTrackToken_;
};


// _____________________________________________________________________________
// Class implementation

// from DataFormats/MuonDetId/interface/CSCDetId.h
#define MIN_ENDCAP 1
#define MAX_ENDCAP 2

// from DataFormats/MuonDetId/interface/CSCTriggerNumbering.h
#define MIN_TRIGSECTOR 1
#define MAX_TRIGSECTOR 6
#define NUM_SECTORS 12

// from L1TriggerSep2016/L1TMuonEndCap/interface/MuonTriggerPrimitive.h
class TriggerPrimitive {
public:
  enum subsystem_type{kDT,kCSC,kRPC,kNSubsystems};
};

//using namespace l1t;


// _____________________________________________________________________________
// Core functions

// The current common deflection angle correction scale factors parametrized by [ipair][ifr][ieta]
// (subject to change). See the function for more documentation.
static const float table_common_dphi_2[9+16][4][12] = {
  {
    {1,1,1,0.901339,0.870075,0.870788,0.883171,0.886469,0.897551,0.894626,0.901733,0.902195},
    {1,1,1,0.84606,0.830355,0.831717,0.837355,0.836053,0.833618,0.837332,0.837711,0.82927},
    {1,1,1,1.10841,1.03858,1.03741,1.0454,1.04837,1.06102,1.06297,1.06873,1.07355},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.944555,0.850682,0.835825,0.850504,0.859744,0.865969,0.873528,0.881321,0.883556},
    {1,1,1,0.858886,0.79945,0.849889,0.856355,0.857238,0.861842,0.860041,0.863574,0.86479},
    {1,1,1,0.934547,0.94012,0.985171,0.997611,1.0046,1.00692,1.01332,1.01841,1.01823},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.958252,0.840684,0.866236,0.842094,0.85789,0.861114,0.867679,0.88235,0.879184},
    {1,1,1,0.835045,0.766892,0.781422,0.856054,0.856707,0.864661,0.861331,0.872279,0.873919},
    {1,1,1,0.915482,0.902039,0.903682,0.987304,1.00024,0.998884,1.00378,1.00952,1.00527},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.683276,0.723353,0.738926,0.746247,0.677852,1,1,1,1,1,1,1},
    {0.683276,0.723353,0.738926,0.746247,0.677852,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.700668,0.728328,0.733951,0.749791,0.75726,1,1,1,1,1,1,1},
    {0.700668,0.728328,0.733951,0.749791,0.75726,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.693876,0.715328,0.723527,0.749835,0.74931,1,1,1,1,1,1,1},
    {0.693876,0.715328,0.723527,0.749835,0.74931,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.460461,0.536219,0.554377,0.557975,0.485687,0.575901,0.597978,0.612054,0.624708,0.63209,0.625154,0.627883},
    {0.460461,0.536219,0.554377,0.557975,0.485687,0.575901,0.597978,0.612054,0.624708,0.63209,0.625154,0.627883},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.557996,0.567325,0.582097,0.587976,0.493803,0.551503,0.599103,0.630596,0.670763,0.709831,0.720161,0.734637},
    {0.557996,0.567325,0.582097,0.587976,0.493803,0.551503,0.599103,0.630596,0.670763,0.709831,0.720161,0.734637},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1.03666,1.10164,1.01898,1.03797,0.960394,0.938931,1.07472,1.08245,0.757731,0.615042,0.738697,0.832576},
    {1.03666,1.10164,1.01898,1.03797,0.960394,0.938931,1.07472,1.08245,0.757731,0.615042,0.738697,0.832576},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {0.516001,0.568506,0.58813,0.601648,0.546567,1,1,1,1,1,1,1},
    {0.516001,0.568506,0.58813,0.601648,0.546567,1,1,1,1,1,1,1},
    {0.863201,0.875923,0.869558,0.893347,0.826568,1,1,1,1,1,1,1},
    {0.863201,0.875923,0.869558,0.893347,0.826568,1,1,1,1,1,1,1}
  }, {
    {0.536671,0.578175,0.602261,0.624528,0.633493,1,1,1,1,1,1,1},
    {0.536671,0.578175,0.602261,0.624528,0.633493,1,1,1,1,1,1,1},
    {0.868749,0.889811,0.888446,0.90208,0.903381,1,1,1,1,1,1,1},
    {0.868749,0.889811,0.888446,0.90208,0.903381,1,1,1,1,1,1,1}
  }, {
    {0.522028,0.549081,0.588694,0.614224,0.607993,1,1,1,1,1,1,1},
    {0.522028,0.549081,0.588694,0.614224,0.607993,1,1,1,1,1,1,1},
    {0.860811,0.882427,0.886533,0.901895,0.895666,1,1,1,1,1,1,1},
    {0.860811,0.882427,0.886533,0.901895,0.895666,1,1,1,1,1,1,1}
  }, {
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.7551,0.824211,1,1,1,1,1,1,1},
    {1,1,1,0.7551,0.824211,1,1,1,1,1,1,1},
    {1,1,1,0.875935,0.957085,1,1,1,1,1,1,1},
    {1,1,1,0.875935,0.957085,1,1,1,1,1,1,1}
  }, {
    {0.602536,0.605146,0.598492,0.606095,0.447725,1,1,1,1,1,1,1},
    {0.602536,0.605146,0.598492,0.606095,0.447725,1,1,1,1,1,1,1},
    {0.940833,0.920285,0.906823,0.890907,0.820343,1,1,1,1,1,1,1},
    {0.940833,0.920285,0.906823,0.890907,0.820343,1,1,1,1,1,1,1}
  }, {
    {1.87256,1.68258,1.55727,1.53033,1.32426,1,1,1,1,1,1,1},
    {1.87256,1.68258,1.55727,1.53033,1.32426,1,1,1,1,1,1,1},
    {1.8404,1.76043,1.62035,1.58545,1.75869,1,1,1,1,1,1,1},
    {1.8404,1.76043,1.62035,1.58545,1.75869,1,1,1,1,1,1,1}
  }, {
    {1.83958,1.67113,1.55494,1.54285,1.30351,1,1,1,1,1,1,1},
    {1.83958,1.67113,1.55494,1.54285,1.30351,1,1,1,1,1,1,1},
    {0.826058,1.71905,1.61425,1.58968,1.83355,1,1,1,1,1,1,1},
    {0.826058,1.71905,1.61425,1.58968,1.83355,1,1,1,1,1,1,1}
  }, {
    {1,1,1,0.928889,0.841116,0.841906,1,1,1,1,1,1},
    {1,1,1,0.928889,0.841116,0.841906,1,1,1,1,1,1},
    {1,1,1,0.990306,0.992257,0.996812,1,1,1,1,1,1},
    {1,1,1,0.990306,0.992257,0.996812,1,1,1,1,1,1}
  }, {
    {0.629995,0.663196,0.704983,0.729569,0.745191,1,1,1,1,1,1,1},
    {0.629995,0.663196,0.704983,0.729569,0.745191,1,1,1,1,1,1,1},
    {0.965827,0.971495,0.990922,0.980452,0.995502,1,1,1,1,1,1,1},
    {0.965827,0.971495,0.990922,0.980452,0.995502,1,1,1,1,1,1,1}
  }, {
    {0.447678,0.531585,0.5355,0.56978,0.533491,0.581317,1,1,1,1,1,1},
    {0.447678,0.531585,0.5355,0.56978,0.533491,0.581317,1,1,1,1,1,1},
    {0.987546,0.990094,0.982461,0.932184,0.898263,0.90147,1,1,1,1,1,1},
    {0.987546,0.990094,0.982461,0.932184,0.898263,0.90147,1,1,1,1,1,1}
  }, {
    {0.503039,0.506491,0.468453,0.473231,0.473288,0.49471,1,1,1,1,1,1},
    {0.503039,0.506491,0.468453,0.473231,0.473288,0.49471,1,1,1,1,1,1},
    {0.884342,0.805864,0.76769,0.78073,0.816751,0.640234,1,1,1,1,1,1},
    {0.884342,0.805864,0.76769,0.78073,0.816751,0.640234,1,1,1,1,1,1}
  }, {
    {1,1,1,0.92841,0.837014,0.83726,1,1,1,1,1,1},
    {1,1,1,0.92841,0.837014,0.83726,1,1,1,1,1,1},
    {1,1,1,0.983022,0.986102,0.983079,1,1,1,1,1,1},
    {1,1,1,0.983022,0.986102,0.983079,1,1,1,1,1,1}
  }, {
    {0.609617,0.638116,0.68799,0.719679,0.718577,1,1,1,1,1,1,1},
    {0.609617,0.638116,0.68799,0.719679,0.718577,1,1,1,1,1,1,1},
    {0.965127,0.969365,0.978861,0.982648,0.990773,1,1,1,1,1,1,1},
    {0.965127,0.969365,0.978861,0.982648,0.990773,1,1,1,1,1,1,1}
  }, {
    {0.960407,0.567199,0.794125,0.464691,0.4464,0.536691,1,1,1,1,1,1},
    {0.960407,0.567199,0.794125,0.464691,0.4464,0.536691,1,1,1,1,1,1},
    {1.10899,1.02518,1.03927,0.994688,0.962703,1.07077,1,1,1,1,1,1},
    {1.10899,1.02518,1.03927,0.994688,0.962703,1.07077,1,1,1,1,1,1}
  }, {
    {1.2504,1.27451,1.20952,1.26434,1.23319,1.12332,1,1,1,1,1,1},
    {1.2504,1.27451,1.20952,1.26434,1.23319,1.12332,1,1,1,1,1,1},
    {1.54702,1.58774,1.55368,1.57053,1.66656,1.32671,1,1,1,1,1,1},
    {1.54702,1.58774,1.55368,1.57053,1.66656,1.32671,1,1,1,1,1,1}
  }
};

// This function checks if the pairing makes sense; if yes, it returns dphi and common_dphi
// A pairing makes sense if the two hits are in different stations, and if there is at least one CSC hit
bool CommonDeflectionAngleProducer::get_common_dphi(const l1t::EMTFHitExtra& hit1, const l1t::EMTFHitExtra& hit2, int& dphi, int& common_dphi) {
  if (hit1.subsystem == TriggerPrimitive::kRPC && hit2.subsystem == TriggerPrimitive::kRPC)
    return false;

  if (hit1.station == hit2.station)
    return false;

  // Initialize return values
  dphi = 0;
  common_dphi = 0;
  float corr = 1.0;

  // The phi difference is calculated as phi_1 - phi_2, where phi_1 is from the
  // hit closer to the interaction point ('front' station) and phi_2 is from the
  // 'rear' station
  if (hit1.station < hit2.station) {
    corr = get_common_dphi_correction(hit1, hit2);
    dphi = hit1.phi_fp - hit2.phi_fp;
    common_dphi = static_cast<int>(std::round(1.0 / corr * dphi));
  } else {
    corr = get_common_dphi_correction(hit2, hit1);
    dphi = hit2.phi_fp - hit1.phi_fp;
    common_dphi = static_cast<int>(std::round(1.0 / corr * dphi));
  }
  return true;
}

// This function returns the correction scale factor. The correction is a
// constant parametrized by [ipair][ifr][ieta],
// where ipair (0-24) is enumerated as (subject to change):
//   ipair  0: dphi(1/1,2)
//   ipair  1: dphi(1/1,3)
//   ipair  2: dphi(1/1,4)
//   ipair  3: dphi(1/2,2)
//   ipair  4: dphi(1/2,3)
//   ipair  5: dphi(1/2,4)
//   ipair  6: dphi(2,3)
//   ipair  7: dphi(2,4)
//   ipair  8: dphi(3,4)
//   ipair  9: dphi(RE1/2,ME2)
//   ipair 10: dphi(RE1/2,ME3)
//   ipair 11: dphi(RE1/2,ME4)
//   ipair 12: null
//   ipair 13: dphi(ME1/1,RE2)
//   ipair 14: dphi(ME1/2,RE2)
//   ipair 15: dphi(RE2,ME3)
//   ipair 16: dphi(RE2,ME4)
//   ipair 17: dphi(ME1/1,RE3)
//   ipair 18: dphi(ME1/2,RE3)
//   ipair 19: dphi(ME2,RE3)
//   ipair 20: dphi(RE3,ME4)
//   ipair 21: dphi(ME1/1,RE4)
//   ipair 22: dphi(ME1/2,RE4)
//   ipair 23: dphi(ME2,RE4)
//   ipair 24: dphi(ME3,RE4)
//
// and ifr (0-3) is one of the F/R combinations (subject to change):
//   ifr 0: R/R
//   ifr 1: R/F
//   ifr 2: F/R
//   ifr 3: F/F
//
// and ieta (0-11) is in 0.1 eta binning (subject to change):
//   ieta  0: 1.2 <= |eta| < 1.3
//   ieta  1: 1.3 <= |eta| < 1.4
//   ...
//   ieta 11: 2.3 <= |eta| < 2.4
//
enum EndcapPlane {ME11, ME12, ME2, ME3, ME4, RE12, RE2, RE3, RE4, UNDEFINED};

auto get_endcap_plane = [](const l1t::EMTFHitExtra& hit) {
  if (hit.subsystem == TriggerPrimitive::kCSC) {
    if (hit.station == 1 && (hit.ring == 1 || hit.ring == 4)) {
      return ME11;
    } else if (hit.station == 1 && (hit.ring == 2 || hit.ring == 3)) {
      return ME12;
    } else if (hit.station == 2) {
      return ME2;
    } else if (hit.station == 3) {
      return ME3;
    } else if (hit.station == 4) {
      return ME4;
    } else {
      return UNDEFINED;
    }
  } else if (hit.subsystem == TriggerPrimitive::kRPC) {
    if (hit.station == 1 && (hit.ring == 2 || hit.ring == 3)) {
      return RE12;
    } else if (hit.station == 2) {
      return RE2;
    } else if (hit.station == 3) {
      return RE3;
    } else if (hit.station == 4) {
      return RE4;
    } else {
      return UNDEFINED;
    }
  }
  return UNDEFINED;
};

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

float CommonDeflectionAngleProducer::get_common_dphi_correction(const l1t::EMTFHitExtra& hit1, const l1t::EMTFHitExtra& hit2) {
  // Find the detector planes
  const EndcapPlane plane1 = get_endcap_plane(hit1);
  const EndcapPlane plane2 = get_endcap_plane(hit2);
  assert(plane1 != UNDEFINED && plane2 != UNDEFINED && "invalid detector planes");

  // Find the ipair, ifr, ieta indices
  int ipair = 0;

  // 9 CSC-only angles
  if        (plane1 == ME11 && plane2 == ME2) {
    ipair = 0;
  } else if (plane1 == ME11 && plane2 == ME3) {
    ipair = 1;
  } else if (plane1 == ME11 && plane2 == ME4) {
    ipair = 2;
  } else if (plane1 == ME12 && plane2 == ME2) {
    ipair = 3;
  } else if (plane1 == ME12 && plane2 == ME3) {
    ipair = 4;
  } else if (plane1 == ME12 && plane2 == ME4) {
    ipair = 5;
  } else if (plane1 == ME2  && plane2 == ME3) {
    ipair = 6;
  } else if (plane1 == ME2  && plane2 == ME4) {
    ipair = 7;
  } else if (plane1 == ME3  && plane2 == ME4) {
    ipair = 8;
  // 16 CSC+RPC angles
  } else if (plane1 == RE12 && plane2 == ME2) {
    ipair = 9;
  } else if (plane1 == RE12 && plane2 == ME3) {
    ipair = 10;
  } else if (plane1 == RE12 && plane2 == ME4) {
    ipair = 11;
  //} else if (plane1 == RE12 && plane2 == ME4) {  // null
  //  ipair = 12;
  } else if (plane1 == ME11 && plane2 == RE2) {
    ipair = 13;
  } else if (plane1 == ME12 && plane2 == RE2) {
    ipair = 14;
  } else if (plane1 == RE2  && plane2 == ME3) {
    ipair = 15;
  } else if (plane1 == RE2  && plane2 == ME4) {
    ipair = 16;
  } else if (plane1 == ME11 && plane2 == RE3) {
    ipair = 17;
  } else if (plane1 == ME12 && plane2 == RE3) {
    ipair = 18;
  } else if (plane1 == ME2  && plane2 == RE3) {
    ipair = 19;
  } else if (plane1 == RE3  && plane2 == ME4) {
    ipair = 20;
  } else if (plane1 == ME11 && plane2 == RE4) {
    ipair = 21;
  } else if (plane1 == ME12 && plane2 == RE4) {
    ipair = 22;
  } else if (plane1 == ME2  && plane2 == RE4) {
    ipair = 23;
  } else if (plane1 == ME3  && plane2 == RE4) {
    ipair = 24;
  } else {
    assert(false && "invalid ipair");
  }

  int ifr = (int(isFront(hit1)) << 1) | int(isFront(hit2));

  double absEta = std::abs(hit1.eta);
  absEta = std::min(std::max(1.2, absEta), (2.4 - 1e-6));
  int ieta = int((absEta - 1.2) / (2.4 - 1.2) * 12);
  assert(ipair < 9+16 && ifr < 4 && ieta < 12 && "invalid [ipair][ifr][ieta] index");

  const float common_dphi_ratio = table_common_dphi_2[ipair][ifr][ieta];

  // Debug
  //std::cout << ".. ipair " << ipair << " ifr " << ifr << " ieta " << ieta << " ratio " << common_dphi_ratio << std::endl;

  return common_dphi_ratio;
}


// _____________________________________________________________________________
CommonDeflectionAngleProducer::CommonDeflectionAngleProducer(const edm::ParameterSet& iConfig) :
    emuHitTag_    (iConfig.getParameter<edm::InputTag>("emuHitTag")),
    emuTrackTag_  (iConfig.getParameter<edm::InputTag>("emuTrackTag")),
    verbose_      (iConfig.getUntrackedParameter<int> ("verbosity"))
{
  emuHitToken_   = consumes<l1t::EMTFHitExtraCollection>  (emuHitTag_);
  emuTrackToken_ = consumes<l1t::EMTFTrackExtraCollection>(emuTrackTag_);
}

CommonDeflectionAngleProducer::~CommonDeflectionAngleProducer() {}

void CommonDeflectionAngleProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // ___________________________________________________________________________
  // Get handles
  edm::Handle<l1t::EMTFHitExtraCollection>   emuHits_handle;
  edm::Handle<l1t::EMTFTrackExtraCollection> emuTracks_handle;

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

  // ___________________________________________________________________________
  // Make hit pairs

  for (int endcap = MIN_ENDCAP; endcap <= MAX_ENDCAP; ++endcap) {
    for (int sector = MIN_TRIGSECTOR; sector <= MAX_TRIGSECTOR; ++sector) {
      const int es = (endcap - MIN_ENDCAP) * (MAX_TRIGSECTOR - MIN_TRIGSECTOR + 1) + (sector - MIN_TRIGSECTOR);

      // Collect hits in a sector processor
      std::vector<l1t::EMTFHitExtra> myhits;

      for (const auto& hit : (*emuHits_handle)) {
        if (hit.endcap == endcap && hit.sector == sector && hit.subsystem == TriggerPrimitive::kCSC) {  // CSC
          myhits.push_back(hit);
        } else if (hit.endcap == endcap && hit.sector == sector && hit.subsystem == TriggerPrimitive::kRPC) {  // RPC
          if (!(hit.station <= 2 && hit.ring == 3)) {  // do not include RE1/3, RE2/3
            myhits.push_back(hit);
          }
        }
      }  // end loop over hits

      // Verbose
      for (const auto& hit : myhits) {
        std::cout << "HIT es: " << es << " st: " << hit.station << " ri: " << hit.ring << " is_csc: " << (hit.subsystem == TriggerPrimitive::kCSC) << " fr: " << isFront(hit) << " phi: " << hit.phi_fp << " eta: " << hit.eta << std::endl;
      }

      // Pairing
      for (unsigned int i = 0; i+1 < myhits.size(); ++i) {
        for (unsigned int j = i+1; j < myhits.size(); ++j) {
          int dphi = 0.;         // is integer!
          int common_dphi = 0.;  // is integer!

          // The function checks if the pairing makes sense; if yes, it returns dphi and common_dphi
          // See the function for more documentation.
          bool valid = get_common_dphi(myhits.at(i), myhits.at(j), dphi, common_dphi);

          // Verbose
          if (valid) {
            std::cout << "PAIR es: " << es << " st1: " << myhits.at(i).station << " st2: " << myhits.at(j).station << " dphi: " << dphi << " corr dphi: " << common_dphi << std::endl;
          } else {
            std::cout << "PAIR es: " << es << " st1: " << myhits.at(i).station << " st2: " << myhits.at(j).station << " INVALID" << std::endl;
          }
        }  // end j loop over myhits
      }  // end i loop over myhits

    }  // end loop over sector
  }  // end loop over endcap

}

// _____________________________________________________________________________
void CommonDeflectionAngleProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

// _____________________________________________________________________________
// Define this as a plug-in
typedef CommonDeflectionAngleProducer CommonDeflectionAngleProducerSep2016;
DEFINE_FWK_MODULE(CommonDeflectionAngleProducerSep2016);
