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

#include <memory>


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
    void find_matches(const edm::Handle<l1t::EMTFTrackCollection>& unpTracks,
            const edm::Handle<l1t::EMTFTrackExtraCollection>& emuTracks);

    // Member data
    const edm::InputTag unpTrackTag_, emuTrackTag_;

    edm::EDGetTokenT<l1t::EMTFTrackCollection> unpTrackToken_;
    edm::EDGetTokenT<l1t::EMTFTrackExtraCollection> emuTrackToken_;
};

// _____________________________________________________________________________
EmuAccuracy::EmuAccuracy(const edm::ParameterSet& iConfig) :
    unpTrackTag_(iConfig.getParameter<edm::InputTag>("unpTrackTag")),
    emuTrackTag_(iConfig.getParameter<edm::InputTag>("emuTrackTag"))
{
    unpTrackToken_ = consumes<l1t::EMTFTrackCollection>(unpTrackTag_);
    emuTrackToken_ = consumes<l1t::EMTFTrackExtraCollection>(emuTrackTag_);
}

EmuAccuracy::~EmuAccuracy() {}

// _____________________________________________________________________________
void EmuAccuracy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

    if (iEvent.isRealData()) {
        // Get products
        edm::Handle<l1t::EMTFTrackCollection> unpTracks;
        if (!unpTrackToken_.isUninitialized()) {
            iEvent.getByToken(unpTrackToken_, unpTracks);
        }
        if (!unpTracks.isValid()) {
            edm::LogError("EmuAccuracy") << "Cannot get the product: " << unpTrackTag_;
            return;
        }

        edm::Handle<l1t::EMTFTrackExtraCollection> emuTracks;
        if (!emuTrackToken_.isUninitialized()) {
            iEvent.getByToken(emuTrackToken_, emuTracks);
        }
        if (!emuTracks.isValid()) {
            edm::LogError("EmuAccuracy") << "Cannot get the product: " << emuTrackTag_;
            return;
        }

        // Find matches
        find_matches(unpTracks, emuTracks);

    }  // end iEvent.isRealData()
}

// _____________________________________________________________________________
void EmuAccuracy::find_matches(const edm::Handle<l1t::EMTFTrackCollection>& unpTracks,
        const edm::Handle<l1t::EMTFTrackExtraCollection>& emuTracks)
{
    // Unpacker tracks
    for (l1t::EMTFTrackCollection::const_iterator it = unpTracks->begin(); it != unpTracks->end(); ++it) {
        const l1t::EMTFTrack& t = *it;

        // From Andrew
        edm::LogVerbatim("EmuAccuracy")
            << "BX = "            << t.BX()
            << " sector = "       << t.Sector()
            << " mode = "         << t.Mode()
            << " quality = "      << t.Quality()
            << " phi_loc_int = "  << t.Phi_loc_int()
            << " phi_GMT = "      << t.Phi_GMT()
            << " eta_GMT = "      << t.Eta_GMT()
            << " pT_GMT = "       << t.Pt_GMT()
            << " phi_glob_deg = " << t.Phi_glob_deg()
            << " eta = "          << t.Eta()
            << " pT = "           << t.Pt()
            << " has some (all) neighbor hits = " << t.Has_neighbor() << " (" << t.All_neighbor() << ")"
            ;
        edm::LogVerbatim("EmuAccuracy")
            << "dPhi_12 = "       << t.DPhi_12()
            << " dPhi_24 = "      << t.DPhi_24()
            << " dTheta_14 = "    << t.DTheta_14()
            << " clct_1 = "       << t.CLCT_1()
            << " fr_1 = "         << t.FR_1()
            << " address = "      << t.Pt_LUT_addr()
            ;
    }

    // Emulator tracks
    for (l1t::EMTFTrackExtraCollection::const_iterator it = emuTracks->begin(); it != emuTracks->end(); ++it) {
        // pass
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

