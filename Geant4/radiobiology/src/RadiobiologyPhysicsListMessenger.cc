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
// FIXME test for the physicslist of the radiobiological
// extended example in Geant4. Taken starting from the Hadrontherapy
// advanced example.
// AUTHOR: D. Chiappara
// Creation Date: 19/05/13
// Last modified: 19/05/13
//

#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
:pPhysicsList(pPhys), physDir(0), pListCmd(0)
{
  // Directory for Physics commands
  physDir = new G4UIdirectory("/Physics/");
  physDir->SetGuidance("Commands to activate physics models and set cuts");
  
  // Add a Physics List
  pListCmd = new G4UIcmdWithAString("/Physics/addPhysics",this);  
  pListCmd->SetGuidance("Add physics list.");
  pListCmd->SetGuidance("The available choices are: ");
  pListCmd->SetGuidance("standard_opt4: Only E.M. Physics");
  pListCmd->SetGuidance("HADRONTHERAPY_1: E.M. Physics (standard_opt4) + Hadron Physics HP");
  pListCmd->SetGuidance("HADRONTHERAPY_2: E.M. Physics (standard_opt4) + Hadron Physics");
  pListCmd->SetCandidates("standard_opt4 HADRONTHERAPY_1 HADRONTHERAPY_2");
  pListCmd->SetDefaultValue("HADRONTHERAPY_1");
  pListCmd->SetParameterName("PList",false);
  pListCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PhysicsListMessenger::~PhysicsListMessenger()
{
   delete physDir;
   delete pListCmd;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{
       if( command == pListCmd )
   { pPhysicsList->AddPhysicsList(newValue);}
}

