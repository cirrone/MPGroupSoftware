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
#include "RadiobiologyRBEAccumulable.hh"
#include "RadiobiologyRBE.hh"
#include "RadiobiologyRadioBioHit.hh"
#include "G4ParticleDefinition.hh"
#include "RadiobiologyVoxelizedSensitiveDetector.hh"
#include "RadiobiologyManager.hh"


#include <tuple>
#include <G4SystemOfUnits.hh>


RadiobiologyRBEAccumulable::RadiobiologyRBEAccumulable()
    : RadiobiologyVRadiobiologicalAccumulable("RBE")
{

}

void RadiobiologyRBEAccumulable::Merge(const G4VAccumulable& rhs)
{
    if (GetVerboseLevel() > 1)
    {
        G4cout << "RBEAccumulable::Merge()" << G4endl;
    }
    const RadiobiologyRBEAccumulable& other = dynamic_cast<const RadiobiologyRBEAccumulable&>(rhs);
    fAlphaNumerator += other.fAlphaNumerator;
    fDenominator += other.fDenominator;
    fBetaNumerator += other.fBetaNumerator;
}

void RadiobiologyRBEAccumulable::Reset()
{
    if (GetVerboseLevel() > 0)
    {
        G4cout << "RBEAccumulable::Reset()" << G4endl;
    }
    if (fInitialized)
    {
        fAlphaNumerator = 0.0;
        fBetaNumerator = 0.0;
        fDenominator = 0.0;
    }
    else
    {
        Initialize();
    }
}

//to accumulate given the hit
void RadiobiologyRBEAccumulable::Accumulate(RadiobiologyRadioBioHit* hit)
{
  G4double kineticEnergy = hit->GetEkinMean();
  G4int A = hit->GetPartType()->GetAtomicMass();
  G4double energyDeposit = hit->GetDeltaE();
  G4double DX = hit->GetTrackLength();
  G4int Z = hit->GetPartType()->GetAtomicNumber();
  G4int i = hit->GetXindex();
  G4int j = hit->GetYindex();
  G4int k = hit->GetZindex();

  Accumulate(kineticEnergy / A, energyDeposit, DX, Z, i, j, k);

}

void RadiobiologyRBEAccumulable::Accumulate(G4double E, G4double energyDeposit, G4double dX, G4int Z, G4int i, G4int j, G4int k)
{
    if (!fInitialized)
    {
        G4Exception("RBEAccumulable::Accumulate", "NotInitialized", FatalException, "Accumulable not initialized. Must be a programming error.");
    }
    if (GetVerboseLevel() > 2)
    {
        G4cout << "RBEAccumulable::Accumulate() in " << i << ", " << j << ", " << k << G4endl;
    }
    if (energyDeposit <= 0)
    {
        return;
    }
    //TODO no la getindex, ma quella del voxelized FIXME
    size_t n = VoxelizedSensitiveDetector::GetInstance()->GetThisVoxelNumber(i, j, k);

    if ((Z >= 1) && (dX > 0) && (E > 0)) // TODO: Verify this condition
    {
        RBE* rbe = dynamic_cast<RBE*> (RadioBioManager::GetInstance()->GetQuantity("RBE"));
        std::tuple<G4double, G4double> alpha_beta =
                rbe->GetHitAlphaAndBeta(E, Z);
        fDenominator[n] += energyDeposit;
        fAlphaNumerator[n] += std::get<0>(alpha_beta) * energyDeposit;
        fBetaNumerator[n] += sqrt(std::get<1>(alpha_beta)) * energyDeposit;
    }
}


G4int RadiobiologyRBEAccumulable::GetVerboseLevel() const
{
    // Return same verbosity of RBE class
    return RadioBioManager::GetInstance()->GetQuantity("RBE")->GetVerboseLevel();
}

void RadiobiologyRBEAccumulable::Initialize()
{
    if (GetVerboseLevel() > 0)
    {
        G4cout << "RBEAccumulable::Initialize(): "  << G4endl;
    }

    fVoxelsAlongX = VoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongX();
    fVoxelsAlongY = VoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongY();
    fVoxelsAlongZ = VoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongZ();
    fVoxels = fVoxelsAlongX * fVoxelsAlongY * fVoxelsAlongZ;

    if (GetVerboseLevel() > 1)
    {
        G4cout << fVoxels << " voxels." << G4endl;
    }

    fAlphaNumerator = array_type(0.0, fVoxels);
    fBetaNumerator = array_type(0.0, fVoxels);
    fDenominator = array_type(0.0, fVoxels);
    fInitialized = true;
}
