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
// Radiobiology advanced example for Geant4


#include "RadiobiologyManager.hh"

#include "G4AccumulableManager.hh"


#include <map>


#define width 15L

// Singleton pointer first instatiated to NULL
RadiobiologyManager* RadiobiologyManager::instance = nullptr;

  // To create the instance the DetectorConstruction must
  // be passed (to account for correct voxelization)
RadiobiologyManager* RadiobiologyManager::CreateInstance()
{
    if (instance)
    {
        G4Exception("RadioBioManager::createInstance", "RecreatingRadioBioManager",
                    FatalException, "Creating another, new, instance of RadioBioManager");
        delete instance;
    }

    instance = new RadiobiologyManager();
    return instance;
}

RadiobiologyManager* RadiobiologyManager::GetInstance()
{
    return instance;
}

RadiobiologyManager::RadiobiologyManager():
    fQuantities()
{}

// Destructor deletes all the quantities
RadiobiologyManager::~RadiobiologyManager()
{
    for(auto q : fQuantities)
        delete q.second;
}

void RadiobiologyManager::InitializeAll()
{
    for(auto const& q : fQuantities)
        (q.second)->Initialize();
}

void RadiobiologyManager::ComputeAll()
{
    for(auto const& q : fQuantities)
        (q.second)->Compute();
}

void RadiobiologyManager::ResetAll()
{
    for(auto const& q : fQuantities)
        (q.second)->Reset();
}

void RadiobiologyManager::StoreAll()
{
    for(auto const& q : fQuantities)
        (q.second)->Store();
}

RadiobiologyVRadiobiologicalQuantity* RadiobiologyManager::GetQuantity(G4String str)
{
    return fQuantities.find(str)->second;
}

void RadiobiologyManager::PrintParameters()
{
    G4cout << "*******************************************" << G4endl
           << "*** right now registered quantities are ***" << G4endl;
    for(auto const& q : fQuantities)
        G4cout << "*** " << q.first;
    G4cout << "*** but their calculation might be not ****" << G4endl
           << "*** active. Ask for parameters of each ****" << G4endl
           << "*******************************************" << G4endl;
}

void RadiobiologyManager::DigestAccumulables()
{
    for(auto q : fQuantities)
    {
        // Hook in the accumulable manager the one named as the quantity (eg "Dose")
        G4VAccumulable* GenAcc = G4AccumulableManager::Instance()->GetAccumulable(q.first);

        if (!GenAcc)
        {
                G4Exception("RadioBioManager::AddFromAccumulable", "NoAccumulable",
                            FatalException, q.first);
        }

        // If calculation is not set enabled, exit
        if(!q.second->IsCalculationEnabled())
            continue;

        // Use the right function according to accumulation value
        q.second->IsAccumulationEnabled() ?
                    q.second->AddFromAccumulable(GenAcc) :
                    q.second->SetFromAccumulable(GenAcc) ;

    }
}

bool RadiobiologyManager::Register(RadiobiologyVRadiobiologicalQuantity* q, G4String name)
{
    if(q == nullptr)
    {
        G4Exception("RadioBioManager::Register", "RegisteringNullptr", JustWarning,
                    "Asking to register a quantity with null pointer!");
        return false;
    }

    if(fQuantities.find(name) != fQuantities.end())
    {
        G4Exception("RadioBioManager::Register", "RegisteringSameQuantity", FatalException,
                    "Registering two radiobiological quantities with the same name!");
        return false;
    }
    fQuantities[name] = q;
    return true;
}
