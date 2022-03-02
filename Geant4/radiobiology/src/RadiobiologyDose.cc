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
// Hadrontherapy advanced example for Geant4
// See more at: https://twiki.cern.ch/twiki/bin/view/Geant4/AdvancedExamplesHadrontherapy

#include "RadiobiologyDose.hh"
#include <fstream>

#include "RadiobiologyDetectorConstruction.hh"
#include "RadiobiologyVoxelizedSensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "RadiobiologyDoseAccumulable.hh"
#include "RadiobiologyDoseMessenger.hh"


#define width 15L

RadiobiologyDose::RadiobiologyDose()
: RadiobiologyVRadiobiologicalQuantity(),
  fEnDep(),
  fDose()
{
    // Default output filename
    fPath = "Dose.out";

    //CreateMessenger
    fMessenger = new RadiobiologyDoseMessenger(this);

    Initialize();
}

RadiobiologyDose::~RadiobiologyDose()
{
  delete fMessenger;
}

void RadiobiologyDose::Initialize()
{
    if(fVerboseLevel > 0)
        G4cout << "Dose::Initialize() called" << G4endl;

    G4int VoxelNumber = RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetTotalVoxelNumber();

    fEnDep.resize(VoxelNumber);
    fDose.resize(VoxelNumber);
    fInitialized = true;
}


void RadiobiologyDose::Compute()
{
    // Skip Dose computation if calculation not enabled.
    if(!fCalculationEnabled)
    {
      if(fVerboseLevel > 0)
      {
        G4cout << "Dose::Compute() called but skipped as calculation not enabled"
               << G4endl;
      }
      return;
    }

    if(fCalculated)
        return;

    if(fVerboseLevel > 0)
        G4cout << "Dose::Compute() called" << G4endl;

    if(RadiobiologyVoxelizedSensitiveDetector::GetInstance() == nullptr)
        G4Exception("Dose::Compute", "VoxNotInit", FatalException, "Calculating dose without voxelized geometry pointer!");

    G4double voxelMass = RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelMass();

    for(G4int v=0; v < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetTotalVoxelNumber(); v++)
    {
        if(fVerboseLevel > 1)
          G4cout << "Calculating Dose for voxel number " << v << G4endl;

        // compute total Dose
        fDose[v] = fEnDep[v] /voxelMass;
    }

    fCalculated = true;
}



void RadiobiologyDose::Store()
{
    // Skip Dose store if calculation not enabled.
    if(!fCalculationEnabled)
    {
      if(fVerboseLevel > 0)
      {
        G4cout << "Dose::Store() called but skipped as calculation not enabled"
               << G4endl;
      }
      return;
    }

    if(fSaved == true)
        G4Exception("Dose::Store", "DoseOverwrite", JustWarning,
                    "Overwriting Dose file. To accumulate along runs, use Use \"/Dose/accumulate true\"");

    Compute();

    std::ofstream ofs(fPath);

    if (ofs.is_open())
    {
        ofs << "x_index" << std::setw(width) << "y_index" << std::setw(width) << "z_index" << std::setw(width) << "Dose (Gy)" << G4endl;

        for(G4int i = 0; i < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongX(); i++)
            for(G4int j = 0; j < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongY(); j++)
                for(G4int k = 0; k < RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongZ(); k++)
                {
                    G4int v = RadiobiologyVoxelizedSensitiveDetector::GetInstance()->GetThisVoxelNumber(i, j, k);
                    ofs << i << std::setw(width) << j << std::setw(width) << k
                        << std::setw(width) << fDose[v]/gray << G4endl;
                }
    }
    if (fVerboseLevel > 0)
    {
        G4cout << "Dose: Dose written to " << fPath << G4endl;
    }
    fSaved = true;

}


void RadiobiologyDose::AddFromAccumulable(G4VAccumulable* GenAcc)
{
    RadiobiologyDoseAccumulable* acc = (RadiobiologyDoseAccumulable*) GenAcc;
    AddEnergyDeposit(acc->GetEnDeposit());
    fCalculated = false;
}

void RadiobiologyDose::SetFromAccumulable(G4VAccumulable* GenAcc)
{
    RadiobiologyDoseAccumulable* acc = (RadiobiologyDoseAccumulable*) GenAcc;
    SetEnergyDeposit(acc->GetEnDeposit());
    fCalculated = false;
}

void RadiobiologyDose::Reset()
{
    if (fVerboseLevel > 1)
    {
        G4cout << "Dose::Reset(): ";
    }
    fEnDep = 0.0;
    fDose = 0.0;
    fCalculated = false;
}

void RadiobiologyDose::PrintParameters()
{
    G4cout << "*******************************************" << G4endl
           << "****** Parameters of the class Dose *******" << G4endl
           << "*******************************************" << G4endl;
           PrintVirtualParameters();

}
