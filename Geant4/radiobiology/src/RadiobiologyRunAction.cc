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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RadiobiologyRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"

#include "RadiobiologyDoseAccumulable.hh"
#include "RadiobiologyLETAccumulable.hh"
#include "RadiobiologyRBEAccumulable.hh"

#include "RadiobiologyManager.hh"



RadiobiologyRunAction::RadiobiologyRunAction(RadiobiologyDetectorConstruction* det)
 : G4UserRunAction(), fDetector(det)
{
  // Set printing event number per each 10000 events
  G4RunManager::GetRunManager()->SetPrintProgress(10000);

  // Create rabiobiological accumulables
  fDoseAccumulable = new RadiobiologyDoseAccumulable();
  fLETAccumulable  = new RadiobiologyLETAccumulable();
  fRBEAccumulable  = new RadiobiologyRBEAccumulable();

  // Register accumulables to the manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fDoseAccumulable);
  accumulableManager->RegisterAccumulable(fLETAccumulable);
  accumulableManager->RegisterAccumulable(fRBEAccumulable);

}

RadiobiologyRunAction::~RadiobiologyRunAction()
{
}

void RadiobiologyRunAction::BeginOfRunAction(const G4Run*)
{
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();
    RadiobiologyManager* RBman = RadiobiologyManager::GetInstance();
    RBman->InitializeAll();

}

void RadiobiologyRunAction::EndOfRunAction(const G4Run* )
{
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Merge();


    // Tell the manager what we have accumulated if this is master thread
    if(IsMaster())
        RadiobiologyManager::GetInstance()->DigestAccumulables();
}
