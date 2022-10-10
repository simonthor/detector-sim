/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the B1::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"


namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  fParticleGun = new G4ParticleGun(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event 
  G4ParticleDefinition* particle = G4IonTable::GetIonTable()->GetIon(55,137,0);
  // Move the radioactiva sample randomly 0-5 mm in x direction and in a circle of 5 mm radius in yz plane
  G4double phi = 2*pi*G4UniformRand();
  G4double yz_r = 5*mm*G4UniformRand();
  G4double y = yz_r*sin(phi);
  G4double z = yz_r*cos(phi);
  G4double x = G4UniformRand()*5*mm;
  G4ThreeVector position = G4ThreeVector(24.4*cm + x, y, z);  
  G4ThreeVector momentumDirection = G4ThreeVector(0., 0., 0.);
  fParticleGun->SetParticleCharge(0);
  fParticleGun->SetParticlePosition(position);
  fParticleGun->SetParticleMomentumDirection(momentumDirection);
  fParticleGun->SetParticleMomentum(0.);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

}


