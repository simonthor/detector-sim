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
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName = "gamma";
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
  G4ThreeVector position = G4ThreeVector(24.4*cm, 0., 0.);
  G4double max_angle = std::atan(0.5/4.55);
  G4double x_angle = G4UniformRand() * max_angle;
  G4double yz_angle = G4UniformRand() * twopi;
  
  G4ThreeVector momentumDirection = G4ThreeVector(-std::cos(x_angle), std::sin(x_angle)*std::cos(yz_angle), std::sin(x_angle)*std::sin(yz_angle));
  fParticleGun->SetParticlePosition(position);
  fParticleGun->SetParticleMomentumDirection(momentumDirection);
  // Set the particle energy to 661.657 keV with a 85.10% chance,
  // 31.817 keV with a 1.99% chance and 32.194 keV with a 3.64% chance
  G4double energy = 661.657*keV;
  G4double rand = G4UniformRand();
  if (rand < 0.8510) {
    energy = 661.657*keV;
  } else if (rand < 0.8510 + 0.0199) {
    energy = 31.817*keV;
  } else {
    energy = 32.194*keV;
  }
  fParticleGun->SetParticleMomentum(energy);
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

}


