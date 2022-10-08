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
  G4double world_sizeXY = 60*cm, world_sizeZ = 20*cm;
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
  // TODO introduce doping of TL into the material
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
  G4Material* largeDetectorMaterial = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");
  G4ThreeVector largeDetectorPosition = G4ThreeVector(0., 0., 0.);
  G4double largeDetectorFoilThickness = 0.4*mm;
  G4double largeDetectorLength = 7.5*cm - largeDetectorFoilThickness;
  G4double largeDetectorRadius = 5.1/2*cm;
  
  G4Tubs* largeDetector
    = new G4Tubs("LargeDetector",  // Name
                  0.*cm, // Inner radius
                  largeDetectorRadius,  // Outer radius
                  largeDetectorLength / 2,  // Height in z direction
                  0.*deg,   // Rotation
                  360.*deg);  // Span

  // TODO Simulation should be run for different values of this angle
  G4double angle = 90*deg;
  
  G4double distanceToLargeDetector = 17.62*cm;;
  G4RotationMatrix rotm = G4RotationMatrix();
  rotm.rotateY(90*deg);
  rotm.rotateZ(angle);
  G4ThreeVector uz = G4ThreeVector(std::cos(angle),  std::sin(angle), 0.);
  G4ThreeVector position = (distanceToLargeDetector + largeDetectorFoilThickness + largeDetectorLength / 2)*uz;
  G4Transform3D largeDetectorTransform = G4Transform3D(rotm, position);
  
  G4LogicalVolume* logicLargeDetector =
    new G4LogicalVolume(largeDetector,         //its solid
                        largeDetectorMaterial,          //its material
                        "LargeDetector");           //its name

  new G4PVPlacement(largeDetectorTransform,                       //rotation
                    logicLargeDetector,             //its logical volume
                    "LargeDetector",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  
  //
  // Large detector outer foil
  //
  G4Material* largeDetectorFoilMaterial = nist->FindOrBuildMaterial("G4_Al");
  G4Tubs* largeDetectorFoil
    = new G4Tubs("LargeDetectorFoil",  // Name
                  largeDetectorRadius, // Inner radius
                  largeDetectorRadius + largeDetectorFoilThickness,  // Outer radius
                  largeDetectorLength / 2,  // Height in z direction
                  0.*deg,   // Rotation
                  360.*deg);  // Span
  
  G4LogicalVolume* logicLargeDetectorFoil =
    new G4LogicalVolume(largeDetectorFoil,         //its solid
                        largeDetectorFoilMaterial,          //its material
                        "LargeDetectorFoil");           //its name

  new G4PVPlacement(largeDetectorTransform,                       //rotation
                    logicLargeDetectorFoil,             //its logical volume
                    "LargeDetectorFoil",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  // Large detector front foil
  //
  G4Material* largeDetectorFrontFoilMaterial = nist->FindOrBuildMaterial("G4_Al");
  G4Tubs* largeDetectorFrontFoil
    = new G4Tubs("LargeDetectorFrontFoil",  // Name
                  0, // Inner radius
                  largeDetectorRadius + largeDetectorFoilThickness,  // Outer radius
                  largeDetectorFoilThickness / 2,  // Height in z direction
                  0.*deg,   // Rotation
                  360.*deg);  // Span
  
  G4LogicalVolume* logicLargeDetectorFrontFoil =
    new G4LogicalVolume(largeDetectorFrontFoil,         //its solid
                        largeDetectorFrontFoilMaterial,          //its material
                        "LargeDetectorFrontFoil");           //its name


  G4ThreeVector frontFoilPosition = (distanceToLargeDetector + largeDetectorFoilThickness / 2)*uz;
  G4Transform3D frontFoilTransform = G4Transform3D(rotm, frontFoilPosition);
  new G4PVPlacement(frontFoilTransform,                       //rotation
                    logicLargeDetectorFrontFoil,             //its logical volume
                    "LargeDetectorFrontFoil",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
