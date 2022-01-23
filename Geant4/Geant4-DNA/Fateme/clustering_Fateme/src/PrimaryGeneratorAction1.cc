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
/// \file eventgenerator/particleGun/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction1.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction1::PrimaryGeneratorAction1(G4ParticleGun* gun)
: fParticleGun(gun)

{

  InitFunction();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction1::~PrimaryGeneratorAction1()
{ }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction1::GeneratePrimaries(G4Event* anEvent)
{
 G4ParticleDefinition* particle
  = G4ParticleTable::GetParticleTable()->FindParticle("proton");
 fParticleGun->SetParticleDefinition(particle);
 fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
 fParticleGun->SetParticlePosition(
      G4ThreeVector(0.*micrometer,0.*micrometer,-0.25*micrometer));
  
  //set energy from a tabulated distribution
  //
 G4double energy = RejectAccept();

  fParticleGun->SetParticleEnergy(energy);    

  //create vertex
  //   
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction1::InitFunction()
{
  // tabulated function 
    // Y is assumed positive, linear per segment, continuous

          // file loading as an array
          ifp.open("60MeV-Proton-at24mm.txt");
          
          if (!ifp)      
            G4Exception("G4PrimaryGenaratorAction","ifp",FatalException, "error opening File data file");

          while(!ifp.eof())
          {
                  ifp >> Energy >> Weight ;

                  fX.push_back(Energy*MeV);
                  fY.push_back(Weight);

                  length_file ++;
          }
          
          ifp.close();

    fNPoints = length_file;
  
 //compute fMax
  //
  fX.resize(fNPoints); fY.resize(fNPoints);
  fYmax = 0.;
  for (G4int j=0; j<fNPoints; j++) {

    if (fYmax < fY[j]) fYmax = fY[j];
  };
     
  //compute slopes
  //
  fSlp.resize(fNPoints);
  for (G4int j=0; j<fNPoints-1; j++) { 
    fSlp[j] = (fY[j+1] - fY[j])/(fX[j+1] - fX[j]);
  };
  
  //compute cumulative function
  //
  fYC.resize(fNPoints);  
  fYC[0] = 0.;
  for (G4int j=1; j<fNPoints; j++) {
    fYC[j] = fYC[j-1] + 0.5*(fY[j] + fY[j-1])*(fX[j] - fX[j-1]);
  };     
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction1::RejectAccept()
{
   // tabulated function
    // Y is assumed positive, linear per segment, continuous
    //
    G4double Xrndm = 0., Yrndm = 0., Yinter = -1.;

    while (Yrndm > Yinter) {
      //choose a point randomly
      Xrndm = fX[0] + G4UniformRand()*(fX[fNPoints-1] - fX[0]);
      Yrndm = G4UniformRand()*fYmax;
      //find bin
      G4int j = fNPoints-2;
      while ((fX[j] > Xrndm) && (j > 0)) j--;
      //compute Y(x_rndm) by linear interpolation
      Yinter = fY[j] + fSlp[j]*(Xrndm - fX[j]);
    };
    return Xrndm;
  }



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
