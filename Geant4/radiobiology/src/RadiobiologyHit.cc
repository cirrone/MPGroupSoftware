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
/// \file RadioBioHit.cc
/// \brief Implementation of the RadioBioHit class

#include "RadiobiologyHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

//for touchable handle
#include "G4TouchableHandle.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<RadiobiologyHit>* RadioBioHitAllocator=0;


RadiobiologyHit::RadiobiologyHit()
 : G4VHit(),
   fTrackID(-1),
   fParticleType(0),
   fEkinMean(0.),
   fDeltaE(0.),
   fEinit(0.),
   fTrackLength(0.),
   fPhysicalVolume(0),
   xIndex(-1), yIndex(-1), zIndex(-1)
{}

RadiobiologyHit::~RadiobiologyHit() {}

RadiobiologyHit::RadiobiologyHit(const RadiobiologyHit& right)
  : G4VHit()
{
  fTrackID        = right.fTrackID;
  fParticleType   = right.fParticleType;
  fEkinMean       = right.fEkinMean;
  fDeltaE         = right.fDeltaE;
  fEinit          = right.fEinit;
  fTrackLength    = right.fTrackLength;
  fPhysicalVolume = right.fPhysicalVolume;
  xIndex          = right.xIndex;
  yIndex          = right.yIndex;
  zIndex          = right.zIndex;
}

const RadiobiologyHit& RadiobiologyHit::operator=(const RadiobiologyHit& right)
{
    fTrackID        = right.fTrackID;
    fParticleType   = right.fParticleType;
    fEkinMean       = right.fEkinMean;
    fDeltaE         = right.fDeltaE;
    fEinit          = right.fEinit;
    fTrackLength    = right.fTrackLength;
    fPhysicalVolume = right.fPhysicalVolume;
    xIndex          = right.xIndex;
    yIndex          = right.yIndex;
    zIndex          = right.zIndex;

  return *this;
}

G4int RadiobiologyHit::operator==(const RadiobiologyHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RadiobiologyHit::Draw()
{
  // Not implemented
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RadiobiologyHit::Print()
{
  // Not implemented
}


void RadiobiologyHit::SetVoxelIndexes(const G4TouchableHandle& TH)
{
    // Calculation of voxel number
    xIndex = TH->GetReplicaNumber(2);
    yIndex = TH->GetReplicaNumber(1);
    zIndex = TH->GetReplicaNumber( );
}
