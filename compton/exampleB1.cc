/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

using namespace B1;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {
  G4UIExecutive* ui = new G4UIExecutive(argc, argv);
  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  runManager->SetUserInitialization(new PhysicsList());

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Run simulation (I think)  
  runManager->Initialize();

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  UImanager->ApplyCommand("/vis/open HepRepFile");
  UImanager->ApplyCommand("/vis/scene/create");
  UImanager->ApplyCommand("/vis/scene/add/volume");
  UImanager->ApplyCommand("/vis/sceneHandler/attach");
  UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate 1000");
  UImanager->ApplyCommand("/vis/scene/add/trajectories");
  UImanager->ApplyCommand("/vis/scene/add/hits");
  UImanager->ApplyCommand("/tracking/storeTrajectory 1");

  UImanager->ApplyCommand("/run/initialize");
  UImanager->ApplyCommand("/run/beamOn 100");
  // ui->SessionStart();
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !
  delete ui;
  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
