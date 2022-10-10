/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void placeDetector(G4double angle, G4double largeDetectorFoilThickness, G4double largeDetectorLength, G4bool checkOverlaps,
  G4LogicalVolume* logicLargeDetector, G4LogicalVolume* logicLargeDetectorFoil, G4LogicalVolume* logicLargeDetectorFrontFoil, G4LogicalVolume* logicWorld)
{
  G4double distanceToLargeDetector = 17.62*cm;;
  G4RotationMatrix rotm = G4RotationMatrix();
  rotm.rotateY(90*deg);
  rotm.rotateZ(angle);
  G4ThreeVector uz = G4ThreeVector(std::cos(angle),  std::sin(angle), 0.);
  G4ThreeVector position = (distanceToLargeDetector + largeDetectorFoilThickness + largeDetectorLength / 2)*uz;
  G4Transform3D largeDetectorTransform = G4Transform3D(rotm, position);

  G4int copyNumber = std::round(angle / deg);
  new G4PVPlacement(largeDetectorTransform,                       //rotation
                    logicLargeDetector,             //its logical volume
                    "LargeDetector",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    copyNumber,                       //copy number
                    checkOverlaps);          //overlaps checking
  
  new G4PVPlacement(largeDetectorTransform,                       //rotation
                    logicLargeDetectorFoil,             //its logical volume
                    "LargeDetectorFoil",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    copyNumber,                       //copy number
                    checkOverlaps);          //overlaps checking

  G4ThreeVector frontFoilPosition = (distanceToLargeDetector + largeDetectorFoilThickness / 2)*uz;
  G4Transform3D frontFoilTransform = G4Transform3D(rotm, frontFoilPosition);
  new G4PVPlacement(frontFoilTransform,                       //rotation
                    logicLargeDetectorFrontFoil,             //its logical volume
                    "LargeDetectorFrontFoil",                //its name
                    logicWorld,                //its mother volume
                    false,                   //no boolean operation
                    copyNumber,                       //copy number
                    checkOverlaps);          //overlaps checking
  
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 60*cm, world_sizeZ = 10*cm;
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
                    G4ThreeVector(0., 0., 0.),                    //at position
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

  logicLargeDetector =
    new G4LogicalVolume(largeDetector,         //its solid
                        largeDetectorMaterial,          //its material
                        "LargeDetector");           //its name

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

  // Place detectors at every angle that has been measured in experiment.
  for (G4double angle = 50.*deg; angle < 131.*deg; angle += 20.*deg) {
      placeDetector(angle, largeDetectorFoilThickness, largeDetectorLength, checkOverlaps, 
        logicLargeDetector, logicLargeDetectorFoil, logicLargeDetectorFrontFoil, logicWorld); 
  }
  for (G4double angle = -40.*deg; angle > -141.*deg; angle -= 20.*deg) {
      placeDetector(angle, largeDetectorFoilThickness, largeDetectorLength, checkOverlaps, 
        logicLargeDetector, logicLargeDetectorFoil, logicLargeDetectorFrontFoil, logicWorld); 
  }

  //
  // Lead cylinder
  //
  G4Material* leadMaterial = nist->FindOrBuildMaterial("G4_Pb");
  G4double leadThickness = 4.55*cm;
  G4Tubs* lead = new G4Tubs("lead",  // Name
                  0.5*cm, // inner radius
                  10 / 2 *cm,  // Outer radius
                  leadThickness / 2,  // Height in z direction / 2
                  0.*deg,   // Start angle
                  360.*deg);  // Spanning angle

  G4LogicalVolume* logicLead =
    new G4LogicalVolume(lead,         //its solid
                        leadMaterial,          //its material
                        "lead");           //its name

  G4RotationMatrix rotm = G4RotationMatrix();
  rotm.rotateY(90*deg);
  G4Transform3D leadTransform = G4Transform3D(rotm, G4ThreeVector(24.4*cm - leadThickness / 2, 0., 0.));
  
  new G4PVPlacement(leadTransform,
                    logicLead,             //its logical volume
                    "lead",                //its name
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

void DetectorConstruction::ConstructSDandField() {
  SensitiveDetector* largeDetectorSD = new SensitiveDetector("SensitiveDetector");
  logicLargeDetector->SetSensitiveDetector(largeDetectorSD);
}
}
