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

#include "DetectorConstruction.hh"
#include "LET.hh"
#include "RadioBioHit.hh"

#include "VoxelizedSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>
#include "G4EmCalculator.hh"
#include "LETAccumulable.hh"
#include "LETMessenger.hh"


LET::LET()
: VRadiobiologicalQuantity()
{
    fPath = "LET.out"; // Default output filename

    fMessenger = new LETMessenger(this);

    Initialize();
}

LET::~LET()
{
  delete fMessenger;
}

void LET::Initialize()
{
    G4int VoxelNumber = VoxelizedSensitiveDetector::GetInstance()->GetTotalVoxelNumber();

    // Instances for Total LET
    fNTotalLETT.resize(VoxelNumber);
    fNTotalLETD.resize(VoxelNumber);
    fDTotalLETT.resize(VoxelNumber);
    fDTotalLETD.resize(VoxelNumber);

    fTotalLETD.resize(VoxelNumber);
    fTotalLETT.resize(VoxelNumber);

    fInitialized = true;
}


void LET::Compute()
{
    if (fVerboseLevel > 0)
    G4cout << "LET::Compute()" << G4endl;

    if(VoxelizedSensitiveDetector::GetInstance() == nullptr)
        G4Exception("LET::ComputeLET", "PointerNotAvailable", FatalException, "Computing LET without voxelized geometry pointer!");

    G4int VoxelNumber = VoxelizedSensitiveDetector::GetInstance()->GetTotalVoxelNumber();
    for(G4int v=0; v < VoxelNumber; v++)
    {
        if(fVerboseLevel > 1)
          G4cout << "COMPUTING LET of voxel " << v << G4endl;

        // Compute total LET
        if (fDTotalLETD[v]>0.) fTotalLETD[v] = fNTotalLETD[v]/fDTotalLETD[v];
        //G4cout << "ERASE ME. DEBUG: voxel = " << v << " fNTotalLETD[v] = " << fNTotalLETD[v]
        //       << " fDTotalLETD[v] = " << fDTotalLETD[v] << G4endl;
        if (fDTotalLETT[v]>0.) fTotalLETT[v] = fNTotalLETT[v]/fDTotalLETT[v];
    }

    // Sort ions by A and then by Z ...
    std::sort(IonLetStore.begin(), IonLetStore.end());
    
    
    // Compute LET Track and LET Dose for ions
    for (size_t ion=0; ion < IonLetStore.size(); ion++)
    {
        IonLetStore[ion].calculate();
    }

    fCalculated = true;
}


// save LET
void LET::Store()
{
    if (fSaved == true)
        G4Exception("LET::StoreLET", "NtuplesAlreadySaved", FatalException,
                    "Saving LET for a second run without accumulation. Use \"/LET/accumulate true\"");

    Compute();
#define width 15L
    if( fVerboseLevel > 0)
      G4cout << "LET::StoreLET" << G4endl;

    // If there is at least one ion
    if(IonLetStore.size())
    {
        std::ofstream ofs(fPath);
        //ofs.open(fPath, std::ios::out);
        if (ofs.is_open())
        {
            
            // Write the voxels index and total LETs and the list of particles/ions
            ofs << std::setprecision(6) << std::left <<
            "i\tj\tk\t";
            ofs <<  std::setw(width) << "LDT";
            ofs <<  std::setw(width) << "LTT";
            
            for (size_t l=0; l < IonLetStore.size(); l++) // Write ions name
            {
                G4String a = (IonLetStore[l].isPrimary()) ? "_1_D":"_D";
                ofs << std::setw(width) << IonLetStore[l].GetName()  + a ;
                G4String b = (IonLetStore[l].isPrimary()) ? "_1_T":"_T";
                ofs << std::setw(width) << IonLetStore[l].GetName()  + b ;
            }
            ofs << G4endl;
            
            // Write data
            G4AnalysisManager*  LETFragmentTuple = G4AnalysisManager::Instance();
            
            LETFragmentTuple->SetVerboseLevel(1);
            LETFragmentTuple->SetFirstHistoId(1);
            LETFragmentTuple->SetFirstNtupleId(1);
            LETFragmentTuple ->OpenFile("LET");
            
            LETFragmentTuple ->CreateNtuple("coordinate", "LET");
            
            LETFragmentTuple ->CreateNtupleIColumn("i");         //0
            LETFragmentTuple ->CreateNtupleIColumn("j");         //1
            LETFragmentTuple ->CreateNtupleIColumn("k");         //2
            LETFragmentTuple ->CreateNtupleDColumn("TotalLETD"); //3
            LETFragmentTuple ->CreateNtupleDColumn("TotalLETT"); //4
            LETFragmentTuple ->CreateNtupleIColumn("A");         //5
            LETFragmentTuple ->CreateNtupleIColumn("Z");         //6
            LETFragmentTuple ->CreateNtupleDColumn("IonLetD");   //7
            LETFragmentTuple ->CreateNtupleDColumn("IonLetT");   //8
            LETFragmentTuple ->FinishNtuple();
            
            
            for(G4int i = 0; i < VoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongX(); i++)
                for(G4int j = 0; j < VoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongY(); j++)
                    for(G4int k = 0; k < VoxelizedSensitiveDetector::GetInstance()->GetVoxelNumberAlongZ(); k++)
                    {
                        LETFragmentTuple->FillNtupleIColumn(1,0, i);
                        LETFragmentTuple->FillNtupleIColumn(1,1, j);
                        LETFragmentTuple->FillNtupleIColumn(1,2, k);
                        
                        G4int v = VoxelizedSensitiveDetector::GetInstance()->GetThisVoxelNumber(i, j, k);
                        
                        // Write total LETs and voxels index
                        ofs << G4endl;
                        ofs << i << '\t' << j << '\t' << k << '\t';
                        ofs << std::setw(width) << fTotalLETD[v]/(keV/um);
                        ofs << std::setw(width) << fTotalLETT[v]/(keV/um);
                        
            
                        // Write ions LETs
                        for (size_t l=0; l < IonLetStore.size(); l++)
                        {
                                // Write ions LETs
                                ofs << std::setw(width) << IonLetStore[l].GetLETD()[v]/(keV/um) ;
                                ofs << std::setw(width) << IonLetStore[l].GetLETT()[v]/(keV/um);   
                        }
                        
                        LETFragmentTuple->FillNtupleDColumn(1,3, fTotalLETD[v]/(keV/um));
                        LETFragmentTuple->FillNtupleDColumn(1,4, fTotalLETT[v]/(keV/um));
                        
                        
                        for (size_t ll=0; ll < IonLetStore.size(); ll++)
                        {
                            
                            
                            LETFragmentTuple->FillNtupleIColumn(1,5, IonLetStore[ll].GetA());
                            LETFragmentTuple->FillNtupleIColumn(1,6, IonLetStore[ll].GetZ());
                            
                            
                            LETFragmentTuple->FillNtupleDColumn(1,7, IonLetStore[ll].GetLETD()[v]/(keV/um));
                            LETFragmentTuple->FillNtupleDColumn(1,8, IonLetStore[ll].GetLETT()[v]/(keV/um));
                            LETFragmentTuple->AddNtupleRow(1);
                        }
                    }
            ofs.close();
            
            //LETFragmentTuple->Write();
            LETFragmentTuple->CloseFile();
        }
        
    }

    fSaved = true;

}

void LET::Reset()
{
    if (fVerboseLevel > 1)
    {
        G4cout << "LET::Reset(): ";
    }
    fNTotalLETT = 0.0;
    fNTotalLETD = 0.0;
    fDTotalLETT = 0.0;
    fDTotalLETD = 0.0;

    fTotalLETD = 0.0;
    fTotalLETT = 0.0;

    IonLetStore.clear();
    fCalculated = false;
}

// Add data taken from accumulables
void LET::AddFromAccumulable(G4VAccumulable* GenAcc)
{
    LETAccumulable* acc = (LETAccumulable*) GenAcc;

    AddNTotalLETT(acc->GetTotalLETT());
    AddDTotalLETT(acc->GetDTotalLETT());
    AddNTotalLETD(acc->GetTotalLETD());
    AddDTotalLETD(acc->GetDTotalLETD());

    // Merges ion counters
    // Loop over rhs ions
    for (unsigned int l=0; l < acc->GetIonLetStore().size(); l++)
    {
        G4int PDGencoding = acc->GetIonLetStore()[l].GetPDGencoding();
        size_t q;
        // Loop over lhs ions to find the one
        for (q=0; q < IonLetStore.size(); q++)
        {

            // If he found it, sums values
            if (IonLetStore[q].GetPDGencoding() == PDGencoding)
            {
                if ( acc->GetIonLetStore()[l].isPrimary() == IonLetStore[q].isPrimary() )
                    break;
            }
        }
        // If ion is missing, copy it
        if (q == IonLetStore.size())
            IonLetStore.push_back(acc->GetIonLetStore()[l]);
        else    // Merge rhs data with lhs ones
            IonLetStore[q].merge(&(acc->GetIonLetStore()[l]));
    }
    fCalculated = false;
}



void LET::SetFromAccumulable(G4VAccumulable* GenAcc)
{
    LETAccumulable* acc = (LETAccumulable*) GenAcc;

    SetNTotalLETT(acc->GetTotalLETT());
    SetDTotalLETT(acc->GetDTotalLETT());
    SetNTotalLETD(acc->GetTotalLETD());
    SetDTotalLETD(acc->GetDTotalLETD());

    // Merges ion counters
    // Loop over rhs ions
    for (unsigned int l=0; l < acc->GetIonLetStore().size(); l++)
    {
        IonLetStore.push_back(acc->GetIonLetStore()[l]);
    }
    fCalculated = false;

}

void LET::PrintParameters()
{
    G4cout << "*******************************************" << G4endl
           << "******* Parameters of the class LET *******" << G4endl
           << "*******************************************" << G4endl;
           PrintVirtualParameters();

}
