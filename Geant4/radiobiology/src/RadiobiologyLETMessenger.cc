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
// FIXME test for the detectorconstruction of the radiobiological
// extended example in Geant4. Taken starting from the HADR03
// extended example.
// AUTHOR: D. Chiappara
// Creation Date: 19/05/13
// Last modified: 19/05/13
//


#include "RadiobiologyLETMessenger.hh"

#include "RadiobiologyLET.hh"
#include <G4UIdirectory.hh>
#include <G4UIcmdWithoutParameter.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <G4UIcmdWithABool.hh>
#include <G4UIcmdWithAString.hh>



RadiobiologyLETMessenger::RadiobiologyLETMessenger(
                                                   RadiobiologyLET * LET)
:G4UImessenger(), 
 fLET(LET),
 fLETDir(0),
 fCalculationCmd(0),
 fVerbosityCmd(0),
 fAccumulateCmd(0),
 fResetCmd(0),
 fLETPathCmd(0),
 fPrintCmd(0)
{ 
  // Directory for LET commands
  fLETDir = new G4UIdirectory("/LET/");
  fLETDir->SetGuidance("commands to setup LET calculation");

  // Activate LET calculation
  fCalculationCmd = new G4UIcmdWithABool("/LET/calculate", this);
  fCalculationCmd->SetGuidance("Whether to enable LET calculation");
  fCalculationCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fCalculationCmd->SetToBeBroadcasted(false);

  // Set LET verbosity
  fVerbosityCmd = new G4UIcmdWithAnInteger("/LET/verbose", this);
  fVerbosityCmd->SetGuidance("Set verbosity level of LET");
  fVerbosityCmd->SetGuidance("0 = quiet");
  fVerbosityCmd->SetGuidance("1 = important messages (~10 per run)");
  fVerbosityCmd->SetGuidance("2 = debug");
  fVerbosityCmd->SetToBeBroadcasted(false);

  // Enable LET accumulation
  fAccumulateCmd = new G4UIcmdWithABool("/LET/accumulate", this);
  fAccumulateCmd->SetGuidance("If false, reset the values at the beginning of each run.");
  fAccumulateCmd->SetGuidance("If true, all runs are summed together");
  fAccumulateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fAccumulateCmd->SetToBeBroadcasted(false);

  // Reset LET data
  fResetCmd = new G4UIcmdWithoutParameter("/LET/reset", this);
  fResetCmd->SetGuidance("Reset accumulated data (relevant only if accumulate mode is on)");
  fResetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fResetCmd->SetToBeBroadcasted(false);

  // Set LET filename
  fLETPathCmd = new G4UIcmdWithAString("/LET/fileName", this);
  fLETPathCmd->SetGuidance("Set the filename for the LET file");
  fLETPathCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fLETPathCmd->SetToBeBroadcasted(false);

  // Print Parameters
  fPrintCmd = new G4UIcmdWithoutParameter("/LET/print", this);
  fPrintCmd->SetGuidance("Print LET parameters");
  fPrintCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
  fPrintCmd->SetToBeBroadcasted(false);

}


RadiobiologyLETMessenger::~RadiobiologyLETMessenger()
{
    delete fLETDir;
    delete fCalculationCmd;
    delete fVerbosityCmd;
    delete fAccumulateCmd;
    delete fResetCmd;
    delete fLETPathCmd;
    delete fPrintCmd;
}

void RadiobiologyLETMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
    if( command == fCalculationCmd )
    { fLET->SetCalculationEnabled(fCalculationCmd->GetNewBoolValue(newValue));}

    if( command == fVerbosityCmd )
    { fLET->SetVerboseLevel(fVerbosityCmd->GetNewIntValue(newValue));}

    if( command == fAccumulateCmd )
    { fLET->SetAccumulationEnabled(fAccumulateCmd->GetNewBoolValue(newValue));}

    if( command == fResetCmd )
    { fLET->Reset();}

    if( command == fLETPathCmd )
    { fLET->SetPath(newValue);}

    if( command == fPrintCmd )
    { fLET->PrintParameters();}
}
