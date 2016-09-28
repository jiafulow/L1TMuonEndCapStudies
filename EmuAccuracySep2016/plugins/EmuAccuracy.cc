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

#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/EMTFTrackExtra.h"

#include "DataFormatsSep2016/L1TMuon/interface/EMTFHit.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFHitExtra.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrack.h"
#include "DataFormatsSep2016/L1TMuon/interface/EMTFTrackExtra.h"

#include "Helper.hh"

namespace l1t_std = l1t;
namespace l1t_sep = L1TMuonEndCap;


// _____________________________________________________________________________
class EmuAccuracy : public edm::EDAnalyzer {
public:
  explicit EmuAccuracy(const edm::ParameterSet& iConfig);
  ~EmuAccuracy();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
  virtual void endJob() override;

  // Member functions
  void findMatches();

  void printEventId();
  void printTracks();

  // Member data
  const edm::InputTag unpHitTag_;
  const edm::InputTag emuHitTag_;
  const edm::InputTag unpTrackTag_;
  const edm::InputTag emuTrackTag_;

  int verbose_;

  edm::EDGetTokenT<l1t_std::EMTFHitCollection>   unpHitToken_;
  edm::EDGetTokenT<l1t_sep::EMTFHitCollection>   emuHitToken_;
  edm::EDGetTokenT<l1t_std::EMTFTrackCollection> unpTrackToken_;
  edm::EDGetTokenT<l1t_sep::EMTFTrackCollection> emuTrackToken_;

  unsigned nBadEvents_;
  unsigned nEvents_;

  edm::EventID eid_;

  edm::Handle<l1t_std::EMTFHitCollection>    unpHits_;
  edm::Handle<l1t_sep::EMTFHitCollection>    emuHits_;
  edm::Handle<l1t_std::EMTFTrackCollection>  unpTracks_;
  edm::Handle<l1t_sep::EMTFTrackCollection>  emuTracks_;
};

// _____________________________________________________________________________
EmuAccuracy::EmuAccuracy(const edm::ParameterSet& iConfig) :
    unpHitTag_  (iConfig.getParameter<edm::InputTag>("unpHitTag")),
    emuHitTag_  (iConfig.getParameter<edm::InputTag>("emuHitTag")),
    unpTrackTag_(iConfig.getParameter<edm::InputTag>("unpTrackTag")),
    emuTrackTag_(iConfig.getParameter<edm::InputTag>("emuTrackTag")),
    verbose_    (iConfig.getUntrackedParameter<int> ("verbosity"))
{
    unpHitToken_   = consumes<l1t_std::EMTFHitCollection>  (unpHitTag_);
    emuHitToken_   = consumes<l1t_sep::EMTFHitCollection>  (emuHitTag_);
    unpTrackToken_ = consumes<l1t_std::EMTFTrackCollection>(unpTrackTag_);
    emuTrackToken_ = consumes<l1t_sep::EMTFTrackCollection>(emuTrackTag_);

    nBadEvents_ = 0;
    nEvents_    = 0;
}

EmuAccuracy::~EmuAccuracy() {}

// _____________________________________________________________________________
void EmuAccuracy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  eid_ = iEvent.id();

  if (iEvent.isRealData()) {
    // Get products
    if (!unpHitToken_.isUninitialized()) {
        iEvent.getByToken(unpHitToken_, unpHits_);
    }
    if (!unpHits_.isValid()) {
        edm::LogError("EmuAccuracy") << "Cannot get the product: " << unpHitTag_;
        return;
    }

    if (!emuHitToken_.isUninitialized()) {
        iEvent.getByToken(emuHitToken_, emuHits_);
    }
    if (!emuHits_.isValid()) {
        edm::LogError("EmuAccuracy") << "Cannot get the product: " << emuHitTag_;
        return;
    }

    if (!unpTrackToken_.isUninitialized()) {
        iEvent.getByToken(unpTrackToken_, unpTracks_);
    }
    if (!unpTracks_.isValid()) {
        edm::LogError("EmuAccuracy") << "Cannot get the product: " << unpTrackTag_;
        return;
    }

    if (!emuTrackToken_.isUninitialized()) {
        iEvent.getByToken(emuTrackToken_, emuTracks_);
    }
    if (!emuTracks_.isValid()) {
        edm::LogError("EmuAccuracy") << "Cannot get the product: " << emuTrackTag_;
        return;
    }
  }  // end if iEvent.isRealData()

  // Find matches
  findMatches();
}

// _____________________________________________________________________________
void EmuAccuracy::findMatches() {
  TracksMatch tracksMatch;

  // Check that unpacker tracks have matching emulator tracks
  std::vector<int> unp_matches(unpTracks_->size(), -99);
  bool unp_has_match = true;

  if (unpTracks_->empty()) {
    unp_has_match = true;

  } else {
    // Unpacker tracks
    int i = 0;
    for (const auto& trk1 : (*unpTracks_)) {
      // Emulator tracks
      int j = 0;
      for (const auto& trk2 : (*emuTracks_)) {
        bool m = tracksMatch(trk1, trk2);
        if (m) {
          unp_matches.at(i) = j;
        }
        j++;
      }

      if (unp_matches.at(i) == -99) {
        unp_has_match = false;
      }
      i++;
    }
  }

  // Check that emulator tracks have matching unpacker tracks
  std::vector<int> emu_matches(emuTracks_->size(), -99);
  bool emu_has_match = true;

  if (emuTracks_->empty()) {
    emu_has_match = true;

  } else {
    // Emulator tracks
    int i = 0;
    for (const auto& trk1 : (*emuTracks_)) {
      // Unpacker tracks
      int j = 0;
      for (const auto& trk2 : (*unpTracks_)) {
        bool m = tracksMatch(trk1, trk2);
        if (m) {
          emu_matches.at(i) = j;
        }
        j++;
      }

      if (emu_matches.at(i) == -99) {
        emu_has_match = false;
      }
      i++;
    }
  }

  bool no_match = (!unp_has_match || !emu_has_match);
  if (verbose_ || no_match) {  // if verbose, always print
    printEventId();

    if (!unp_has_match) {
      std::cout << "ERROR: Unpacker tracks have no matching emulator tracks." << std::endl;
      for (unsigned i = 0; i < unp_matches.size(); ++i) {
        int j = unp_matches.at(i);
        std::cout << "  unp " << i << " --> emu " << j << std::endl;
      }
    }

    if (!emu_has_match) {
      std::cout << "ERROR: Emulator tracks have no matching unpacker tracks." << std::endl;
      for (unsigned i = 0; i < emu_matches.size(); ++i) {
        int j = emu_matches.at(i);
        std::cout << "  emu " << i << " --> unp " << j << std::endl;
      }
    }

    printTracks();
  }

  if (no_match) {
    nBadEvents_ += 1;
  }
  nEvents_ += 1;
}

// _____________________________________________________________________________
void EmuAccuracy::printEventId() {
  std::cout << "******** Run " << eid_.run() << ", Lumi " << eid_.luminosityBlock() << ", Event " << eid_.event() << " ********" << std::endl;
}

void EmuAccuracy::printTracks() {
  TrackPrint trackPrint;
  int i = 0;

  // Unpacker tracks
  std::cout << "Num of unpacker tracks = " << unpTracks_->size() << std::endl;
  i = 0;
  for (const auto& trk : (*unpTracks_)) {
    std::cout << i++ << " ";
    trackPrint(trk);
  }

  // Emulator tracks
  std::cout << "Num of emulator tracks = " << emuTracks_->size() << std::endl;
  i = 0;
  for (const auto& trk : (*emuTracks_)) {
    std::cout << i++ << " ";
    trackPrint(trk);
  }
}

// _____________________________________________________________________________
void EmuAccuracy::beginJob() {}

void EmuAccuracy::endJob() {
  std::cout << "Num of bad events: " << nBadEvents_ << "/" << nEvents_ << std::endl;
}

void EmuAccuracy::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

// _____________________________________________________________________________
// Define this as a plug-in
typedef EmuAccuracy EmuAccuracySep2016;
DEFINE_FWK_MODULE(EmuAccuracySep2016);
