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
// FIXME test for the physicslist of the radiobiological
// extended example in Geant4. Taken starting from the Hadrontherapy
// advanced example.
// AUTHOR: D. Chiappara
// Creation Date: 19/05/13
// Last modified: 19/05/13
//
//
// 'HADRONTHERAPY_1' and 'HADRONTHERAPY_2' are both suggested;
// It can be activated inside any macro file using the command:
// /Physics/addPhysics HADRONTHERAPY_1 (HADRONTHERAPY_2)

#include "RadiobiologyPhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "RadiobiologyPhysicsListMessenger.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"

RadiobiologyPhysicsList::RadiobiologyPhysicsList() : G4VModularPhysicsList(),
    fEmName("")
{
    // Set default cut values
    G4LossTableManager::Instance();
    defaultCutValue = 1.*mm;
    fCutForGamma     = defaultCutValue;
    fCutForElectron  = defaultCutValue;
    fCutForPositron  = defaultCutValue;
    
    pMessenger = new RadiobiologyPhysicsListMessenger(this);
    SetVerboseLevel(1);

    // Create default decay physics
    fDecayPhysicsList = new G4DecayPhysics();

    // Create default electromagnetic physics
    fEmPhysicsList = new G4EmStandardPhysics_option4();
    
}

RadiobiologyPhysicsList::~RadiobiologyPhysicsList()
{
    delete pMessenger;
    delete fEmPhysicsList;
    delete fDecayPhysicsList;

    // Destroy hadronic physics
    for(size_t i=0; i<hadronPhys.size(); i++)
    {
        delete hadronPhys[i];
    }

    // Clear pointers
    hadronPhys.clear();
}

void RadiobiologyPhysicsList::ConstructParticle()
{
    fDecayPhysicsList -> ConstructParticle();
}

void RadiobiologyPhysicsList::ConstructProcess()
{
    // Transportation
    AddTransportation();
    
    // Construct default decay adn EM processes
    fDecayPhysicsList -> ConstructProcess();
    fEmPhysicsList -> ConstructProcess();
    
    
    // Construct hadronic processes
    for(size_t i=0; i < hadronPhys.size(); i++)
    {
        hadronPhys[i] -> ConstructProcess();
    }
}

void RadiobiologyPhysicsList::AddPhysicsList(const G4String& name)
{
    if (verboseLevel>1) {
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    if (name == fEmName) return;
    
    ///////////////////////////////////
    //   ELECTROMAGNETIC MODELS
    ///////////////////////////////////
    if (name == "standard_opt4") {
        fEmName = name;
        delete fEmPhysicsList;
        hadronPhys.clear();
        fEmPhysicsList = new G4EmStandardPhysics_option4();
        G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
        if(verboseLevel>1)
        {
          G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: "
                 << "G4EmStandardPhysics_option4" << G4endl;
        }

        ////////////////////////////////////////
        //   ELECTROMAGNETIC + HADRONIC MODELS
        ////////////////////////////////////////
        
    }  else if (name == "HADRONTHERAPY_1") {
        
        AddPhysicsList("standard_opt4");
        hadronPhys.push_back( new G4DecayPhysics());
        hadronPhys.push_back( new G4RadioactiveDecayPhysics());
        hadronPhys.push_back( new G4IonBinaryCascadePhysics());
        hadronPhys.push_back( new G4EmExtraPhysics());
        hadronPhys.push_back( new G4HadronElasticPhysicsHP());
        hadronPhys.push_back( new G4StoppingPhysics());
        hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC_HP());
        hadronPhys.push_back( new G4NeutronTrackingCut());
        
        G4cout << "HADRONTHERAPY_1 PHYSICS LIST has been activated" << G4endl;
    }
    
    else if (name == "HADRONTHERAPY_2") {
        // HP models are switched off
        AddPhysicsList("standard_opt4");
        hadronPhys.push_back( new G4DecayPhysics());
        hadronPhys.push_back( new G4RadioactiveDecayPhysics());
        hadronPhys.push_back( new G4IonBinaryCascadePhysics());
        hadronPhys.push_back( new G4EmExtraPhysics());
        hadronPhys.push_back( new G4HadronElasticPhysics());
        hadronPhys.push_back( new G4StoppingPhysics());
        hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());
        hadronPhys.push_back( new G4NeutronTrackingCut());
        
        G4cout << "HADRONTHERAPY_2 PHYSICS LIST has been activated" << G4endl;    }
    else {
        G4Exception("PhysicsList::AddPhysicsList",
                    "NoPhysicsList", JustWarning, (name + " is not a defined physics list").c_str());
    }
    
}
