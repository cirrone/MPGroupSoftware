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

#include "RadiobiologyVoxelizedSensitiveDetectorMessenger.hh"

#include "RadiobiologyVoxelizedSensitiveDetector.hh"
#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


RadiobiologyVoxelizedSensitiveDetectorMessenger::RadiobiologyVoxelizedSensitiveDetectorMessenger(
                                                                                                 RadiobiologyVoxelizedSensitiveDetector * VoxDet)
:G4UImessenger(), 
 fVoxelizedDetector(VoxDet), fVoxelsDir(0), fVoxelSizeCmd(0), fVoxelSizeXCmd(0),
  fVoxelSizeYCmd(0), fVoxelSizeZCmd(0)
{ 
  // Diretory for voxelization commands
  fVoxelsDir = new G4UIdirectory("/voxels/");
  fVoxelsDir->SetGuidance("commands to change voxels size");

  // Set voxel size with a ThreeVector
  fVoxelSizeCmd = new G4UIcmdWith3VectorAndUnit("/voxels/setVoxelSizes",this);
  fVoxelSizeCmd->SetGuidance("Insert voxel sizes X Y and Z");
  fVoxelSizeCmd->SetParameterName("SizeAlongX", "SizeAlongY", "SizeAlongZ", false);
  fVoxelSizeCmd->SetRange(
              "SizeAlongX>0. && SizeAlongY>0. && SizeAlongZ>0.");
  fVoxelSizeCmd->SetUnitCategory("Length");
  fVoxelSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fVoxelSizeCmd->SetToBeBroadcasted(false);

  // Set X voxel size
  fVoxelSizeXCmd = new G4UIcmdWithADoubleAndUnit("/voxels/setSizeX",this);
  fVoxelSizeXCmd->SetGuidance("Set X width of voxels");
  fVoxelSizeXCmd->SetParameterName("Size",false);
  fVoxelSizeXCmd->SetRange("Size>0.");
  fVoxelSizeXCmd->SetUnitCategory("Length");
  fVoxelSizeXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fVoxelSizeXCmd->SetToBeBroadcasted(false);

  // Set Y voxel size
  fVoxelSizeYCmd = new G4UIcmdWithADoubleAndUnit("/voxels/setSizeY",this);
  fVoxelSizeYCmd->SetGuidance("Set Y width of voxels");
  fVoxelSizeYCmd->SetParameterName("SfVoxelsDirize",false);
  fVoxelSizeYCmd->SetRange("Size>0.");
  fVoxelSizeYCmd->SetUnitCategory("Length");
  fVoxelSizeYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fVoxelSizeYCmd->SetToBeBroadcasted(false);

  // Set Z voxel size
  fVoxelSizeZCmd = new G4UIcmdWithADoubleAndUnit("/voxels/setSizeZ",this);
  fVoxelSizeZCmd->SetGuidance("Set Z width of voxels");
  fVoxelSizeZCmd->SetParameterName("Size",false);
  fVoxelSizeZCmd->SetRange("Size>0.");
  fVoxelSizeZCmd->SetUnitCategory("Length");
  fVoxelSizeZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fVoxelSizeZCmd->SetToBeBroadcasted(false);

  // Update voxelized geometry
  fUpdateVoxCmd = new G4UIcmdWithoutParameter("/voxels/update", this);
  fUpdateVoxCmd->SetGuidance("Update voxelized geometry");
  fUpdateVoxCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateVoxCmd->SetGuidance("if you changed voxelization");
  fUpdateVoxCmd->AvailableForStates(G4State_Idle);
}

RadiobiologyVoxelizedSensitiveDetectorMessenger::~RadiobiologyVoxelizedSensitiveDetectorMessenger()
{
  delete fVoxelsDir;
  delete fVoxelSizeCmd;
  delete fVoxelSizeXCmd;
  delete fVoxelSizeYCmd;
  delete fVoxelSizeZCmd;
  delete fUpdateVoxCmd;
}

void RadiobiologyVoxelizedSensitiveDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fVoxelSizeCmd )
    { fVoxelizedDetector->SetVoxelWidth(fVoxelSizeCmd->GetNew3VectorValue(newValue));}

  if( command == fVoxelSizeXCmd )
   { fVoxelizedDetector->SetVoxelWidthX(fVoxelSizeXCmd->GetNewDoubleValue(newValue));}

  if( command == fVoxelSizeYCmd )
   { fVoxelizedDetector->SetVoxelWidthY(fVoxelSizeYCmd->GetNewDoubleValue(newValue));}

  if( command == fVoxelSizeZCmd )
   { fVoxelizedDetector->SetVoxelWidthZ(fVoxelSizeZCmd->GetNewDoubleValue(newValue));}

  if( command == fUpdateVoxCmd )
  { fVoxelizedDetector->UpdateVoxelizedGeometry();}
}
