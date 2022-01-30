 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction0.hh"
#include "PrimaryGeneratorAction1.hh"

#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0),
   fAction0(0),
   fAction1(0),
   fSelectedAction(0),
   fGunMessenger(0)
{
  // default particle kinematic
  //
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
    
  G4ParticleDefinition* particle
           = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
  fParticleGun->SetParticleDefinition(particle);
        
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
  
  fAction0 = new PrimaryGeneratorAction0(fParticleGun);
  fAction1 = new PrimaryGeneratorAction1(fParticleGun);

  
  //create a messenger for this class
  fGunMessenger = new PrimaryGeneratorMessenger(this);    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fAction0;
  delete fAction1;
  delete fParticleGun;  
  delete fGunMessenger;      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  switch(fSelectedAction)
  {
   case 0:
    fAction0->GeneratePrimaries(anEvent);
    break;
   case 1:
    fAction1->GeneratePrimaries(anEvent);
    break;    
   default:
    G4cerr << "Invalid generator fAction" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
