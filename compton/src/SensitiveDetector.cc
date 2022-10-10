#include "SensitiveDetector.hh"

namespace B1
{

SensitiveDetector::SensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
    aStep->GetTrack()->SetTrackStatus(fStopAndKill);
    // Continue if the particle is not a photon
    if (aStep->GetTrack()->GetDefinition()->GetParticleName() != "gamma") return true;
    // Get energy of particle and write to file
    const G4VTouchable *detector = aStep->GetPreStepPoint()->GetTouchable();
    // Write energy and angle to file
    #ifdef G4MULTITHREADED
    static G4Mutex mutex = G4MUTEX_INITIALIZER;
    G4AutoLock al(&mutex);
    #endif
    static std::ofstream outputFile("data/energy.csv");
    static bool first = true;
    if (first) {
        first = false;
        outputFile << "angle,Energy/keV" << std::endl;
    };
    outputFile << detector->GetCopyNumber() << "," << aStep->GetPreStepPoint()->GetKineticEnergy() / keV << "\n";
    
    return true;
}
}