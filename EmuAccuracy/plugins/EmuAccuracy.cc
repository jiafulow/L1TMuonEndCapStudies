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

#include "Helper.hh"

#include <memory>
#include <iostream>


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

    void printTracks();

    // Member data
    const edm::InputTag unpHitTag_, emuHitTag_, unpTrackTag_, emuTrackTag_;
    edm::EDGetTokenT<l1t::EMTFHitCollection>        unpHitToken_;
    edm::EDGetTokenT<l1t::EMTFHitExtraCollection>   emuHitToken_;
    edm::EDGetTokenT<l1t::EMTFTrackCollection>      unpTrackToken_;
    edm::EDGetTokenT<l1t::EMTFTrackExtraCollection> emuTrackToken_;
    int verbose_;

    // Global objects
    edm::EventID eid_;
    edm::Handle<l1t::EMTFHitCollection>         unpHits_;
    edm::Handle<l1t::EMTFHitExtraCollection>    emuHits_;
    edm::Handle<l1t::EMTFTrackCollection>       unpTracks_;
    edm::Handle<l1t::EMTFTrackExtraCollection>  emuTracks_;
};

// _____________________________________________________________________________
EmuAccuracy::EmuAccuracy(const edm::ParameterSet& iConfig) :
    unpHitTag_(iConfig.getParameter<edm::InputTag>("unpHitTag")),
    emuHitTag_(iConfig.getParameter<edm::InputTag>("emuHitTag")),
    unpTrackTag_(iConfig.getParameter<edm::InputTag>("unpTrackTag")),
    emuTrackTag_(iConfig.getParameter<edm::InputTag>("emuTrackTag")),
    verbose_(iConfig.getUntrackedParameter<int>("verbose"))
{
    unpHitToken_ = consumes<l1t::EMTFHitCollection>(unpHitTag_);
    emuHitToken_ = consumes<l1t::EMTFHitExtraCollection>(emuHitTag_);
    unpTrackToken_ = consumes<l1t::EMTFTrackCollection>(unpTrackTag_);
    emuTrackToken_ = consumes<l1t::EMTFTrackExtraCollection>(emuTrackTag_);
}

EmuAccuracy::~EmuAccuracy() {}

// _____________________________________________________________________________
void EmuAccuracy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;
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

        // Find matches
        findMatches();

    }  // end iEvent.isRealData()
}

// _____________________________________________________________________________
void EmuAccuracy::findMatches()
{
    // Check that unpacker tracks have match in emulator
    bool unp_has_match = false;
    std::vector<int> unp_matches(unpTracks_->size(), -999);

    // Unpacker tracks
    if (unpTracks_->empty()) {
        unp_has_match = true;
    }

    for (l1t::EMTFTrackCollection::const_iterator it1 = unpTracks_->begin(); it1 != unpTracks_->end(); ++it1) {
        // Emulator tracks
        for (l1t::EMTFTrackExtraCollection::const_iterator it2 = emuTracks_->begin(); it2 != emuTracks_->end(); ++it2) {
            bool m = tracksMatch(*it1, *it2);
            if (m) {
                unp_has_match = true;
                unp_matches.at(it1 - unpTracks_->begin()) = (it2 - emuTracks_->begin());
            }
        }
    }

    // Check that emulator tracks have match in unpacker
    bool emu_has_match = false;
    std::vector<int> emu_matches(emuTracks_->size(), -999);

    // Emulator tracks
    if (emuTracks_->empty()) {
        emu_has_match = true;
    }

    for (l1t::EMTFTrackExtraCollection::const_iterator it2 = emuTracks_->begin(); it2 != emuTracks_->end(); ++it2) {
        // Unpacker tracks
        for (l1t::EMTFTrackCollection::const_iterator it1 = unpTracks_->begin(); it1 != unpTracks_->end(); ++it1) {
            bool m = tracksMatch(*it2, *it1);
            if (m) {
                emu_has_match = true;
                emu_matches.at(it2 - emuTracks_->begin()) = (it1 - unpTracks_->begin());
            }
        }
    }

    bool no_match = (!unp_has_match || !emu_has_match);
    if (verbose_ || no_match) {  // if verbose, always print
        printEvent(eid_);

        if (!unp_has_match) {
            std::cout << "ERROR: Unpacker tracks have no match in emulator!" << std::endl;
            for (unsigned i=0; i<unp_matches.size(); ++i) {
                int j = unp_matches.at(i);
                std::cout << "  unp " << i << " --> emu " << j << std::endl;
            }
        }

        if (!emu_has_match) {
            std::cout << "ERROR: Emulator tracks have no match in unpacker!" << std::endl;
            for (unsigned i=0; i<emu_matches.size(); ++i) {
                int j = emu_matches.at(i);
                std::cout << "  emu " << i << " --> unp " << j << std::endl;
            }
        }

        printTracks();
    }
}

// _____________________________________________________________________________
void EmuAccuracy::printTracks()
{
    // Unpacker tracks
    std::cout << "Number of unpacker tracks = " << unpTracks_->size() << std::endl;
    for (l1t::EMTFTrackCollection::const_iterator it1 = unpTracks_->begin(); it1 != unpTracks_->end(); ++it1) {
        //std::cout << "Unpacker track " << it1 - unpTracks_->begin() << std::endl;
        printEMTFTrack(*it1);
        printPtLUT(*it1);
    }

    // Emulator tracks
    std::cout << "Number of emulator tracks = " << emuTracks_->size() << std::endl;
    for (l1t::EMTFTrackExtraCollection::const_iterator it2 = emuTracks_->begin(); it2 != emuTracks_->end(); ++it2) {
        //std::cout << "Emulator track " << it2 - emuTracks_->begin() << std::endl;
        printEMTFTrack(*it2);
        printPtLUT(*it2);
    }

    // Unpacker hits
    //std::cout << "Number of unpacker hits = " << unpHits_->size() << std::endl;
    //for (l1t::EMTFHitCollection::const_iterator it1 = unpHits_->begin(); it1 != unpHits_->end(); ++it1) {
    //    //std::cout << "Unpacker hit " << it1 - unpHits_->begin() << std::endl;
    //    printEMTFHit(*it1);
    //}

    // Emulator hits
    std::cout << "Number of emulator hits = " << emuHits_->size() << std::endl;
    for (l1t::EMTFHitExtraCollection::const_iterator it2 = emuHits_->begin(); it2 != emuHits_->end(); ++it2) {
        //std::cout << "Emulator hit " << it2 - emuHits_->begin() << std::endl;
        printEMTFHitExtra(*it2);
    }

    printSimulatorHitHeader();
    for (l1t::EMTFHitExtraCollection::const_iterator it2 = emuHits_->begin(); it2 != emuHits_->end(); ++it2) {
        //std::cout << "Emulator hit " << it2 - emuHits_->begin() << std::endl;
        printSimulatorHit(*it2);
    }
}

// _____________________________________________________________________________
void EmuAccuracy::beginJob() {}

void EmuAccuracy::endJob() {}

void EmuAccuracy::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

// _____________________________________________________________________________
// Define this as a plug-in
DEFINE_FWK_MODULE(EmuAccuracy);

