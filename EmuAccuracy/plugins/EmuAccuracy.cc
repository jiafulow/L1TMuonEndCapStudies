#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <memory>


// ____________________________________________________________________________
class EmuAccuracy : public edm::EDAnalyzer {
public:
    explicit EmuAccuracy(const edm::ParameterSet& iConfig);
    ~EmuAccuracy();

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
    virtual void beginJob() override;
    virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) override;
    virtual void endJob() override;
};

// ____________________________________________________________________________
EmuAccuracy::EmuAccuracy(const edm::ParameterSet& iConfig) {}

EmuAccuracy::~EmuAccuracy() {}

void EmuAccuracy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm;

}


void EmuAccuracy::beginJob() {}

void EmuAccuracy::endJob() {}

void EmuAccuracy::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

// ____________________________________________________________________________
// Define this as a plug-in
DEFINE_FWK_MODULE(EmuAccuracy);

