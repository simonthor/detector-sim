/// \file RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
// #include "Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager * analysisManager = G4AnalysisManager::Instance();
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // // Merge accumulables
  // G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  // accumulableManager->Merge();

  // // Compute dose = total energy deposit in a run and its variance
  // //

  // G4double rms = edep2 - edep*edep/nofEvents;
  // if (rms > 0.) rms = std::sqrt(rms); else rms = 0.;

  // const DetectorConstruction* detConstruction
  //  = static_cast<const DetectorConstruction*>
  //    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // G4double mass = detConstruction->GetScoringVolume()->GetMass();
  // G4double dose = edep/mass;
  // G4double rmsDose = rms/mass;

  // // Run conditions
  // //  note: There is no primary generator action object for "master"
  // //        run manager for multi-threaded mode.
  // const PrimaryGeneratorAction* generatorAction
  //  = static_cast<const PrimaryGeneratorAction*>
  //    (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  // G4String runCondition;
  // if (generatorAction)
  // {
  //   const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
  //   runCondition += particleGun->GetParticleDefinition()->GetParticleName();
  //   runCondition += " of ";
  //   G4double particleEnergy = particleGun->GetParticleEnergy();
  //   runCondition += G4BestUnit(particleEnergy,"Energy");
  // }

  // Save energy deposit to ROOT file
  G4AnalysisManager * analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0, edep);
  analysisManager->Write();
  analysisManager->CloseFile();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
