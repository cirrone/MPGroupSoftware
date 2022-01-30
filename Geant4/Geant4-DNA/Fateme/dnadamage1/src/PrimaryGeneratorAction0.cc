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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class


#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction0.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction0::PrimaryGeneratorAction0(G4ParticleGun* gun)
: fParticleGun(gun)

{
    
  
// default gun parameters
    fParticleGun->SetParticleEnergy(10.*CLHEP::keV);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-20.*CLHEP::nm));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction0::~PrimaryGeneratorAction0()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction0::GeneratePrimaries(G4Event* anEvent)
{
	G4ParticleDefinition* particle
  = G4ParticleTable::GetParticleTable()->FindParticle("proton");
    G4double randomx = -1 + 2*G4UniformRand();
    G4double randomy = -1 + 2*G4UniformRand();
    fParticleGun->SetParticlePosition(G4ThreeVector(10.*randomx*CLHEP::nm,
                                                     10.*randomy*CLHEP::nm,
                                                     -20.*CLHEP::nm));
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
