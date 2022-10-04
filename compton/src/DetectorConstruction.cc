//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SystemOfUnits.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 50*cm, world_sizeZ = 20*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Small detector
  //
  G4Material* smallDetectorMaterial = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4ThreeVector smallDetectorPosition = G4ThreeVector(0., 0., 0.);

   G4Tubs* smallDetector
    = new G4Tubs("SmallDetector",  // Name
                  0.*cm, // inner radius
                  1.5 / 2 *cm,  // Outer radius
                  2.36 / 2 *cm,  // Height in z direction / 2
                  0.*deg,   // Start angle
                  360.*deg);  // Spanning angle

  G4LogicalVolume* logicSmallDetector =
    new G4LogicalVolume(smallDetector,         //its solid
                        smallDetectorMaterial,          //its material
                        "SmallDetector");           //its name

  new G4PVPlacement(0,                       //no rotation
                    smallDetectorPosition,                    //at position
                    logicSmallDetector,             //its logical volume
                    "SmallDetector",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  // Large detector
  //
  G4Material* largeDetectorMaterial = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE"); // TODO check material
  G4ThreeVector largeDetectorPosition = G4ThreeVector(0., 0., 0.);

  G4double largeDetectorLength = 7.5*cm;
  G4Tubs* largeDetector
    = new G4Tubs("LargeDetector",  // Name
                  0.*cm, // Inner radius
                  5.8 / 2 *cm,  // Outer radius
                  largeDetectorLength / 2,  // Height in z direction
                  0.*deg,   // Rotation
                  360.*deg);  // Span

  // TODO Simulation should be run for different values of this angle
  G4double angle = 90*deg;
  
  G4RotationMatrix rotm = G4RotationMatrix();
  rotm.rotateY(90*deg);
  rotm.rotateZ(angle);
  G4ThreeVector uz = G4ThreeVector(std::cos(angle),  std::sin(angle), 0.);
  G4ThreeVector position = (17.62*cm + 5.81/2*cm + largeDetectorLength / 2)*uz;
  G4Transform3D transform = G4Transform3D(rotm, position);
  
  G4LogicalVolume* logicLargeDetector =
    new G4LogicalVolume(largeDetector,         //its solid
                        largeDetectorMaterial,          //its material
                        "LargeDetector");           //its name

  new G4PVPlacement(transform,                       //rotation
                    logicLargeDetector,             //its logical volume
                    "LargeDetector",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  // TODO
  // Lead block (alternatively only model that particles move in a certain cone in a random direction)
  //
  // G4Box* leadBlock1 =
  //   new G4Box("leadBlock",                       //its name
  //      0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  // G4LogicalVolume* logicWorld =
  //   new G4LogicalVolume(solidWorld,          //its solid
  //                       world_mat,           //its material
  //                       "World");            //its name

  // G4VPhysicalVolume* physWorld =
  //   new G4PVPlacement(0,                     //no rotation
  //                     G4ThreeVector(),       //at (0,0,0)
  //                     logicWorld,            //its logical volume
  //                     "World",               //its name
  //                     0,                     //its mother  volume
  //                     false,                 //no boolean operation
  //                     0,                     //copy number
  //                     checkOverlaps);        //overlaps checking
  // TODO change code below. Maybe/probably remove
  // Set large detector as scoring volume
  //
  fScoringVolume = logicLargeDetector;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
