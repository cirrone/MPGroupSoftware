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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RadiobiologyVoxelizedSensitiveDetectorMessenger_h
#define RadiobiologyVoxelizedSensitiveDetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class RadiobiologyVoxelizedSensitiveDetector;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithoutParameter;


class RadiobiologyVoxelizedSensitiveDetectorMessenger: public G4UImessenger
{
  public:
  
    RadiobiologyVoxelizedSensitiveDetectorMessenger(RadiobiologyVoxelizedSensitiveDetector* );
   ~RadiobiologyVoxelizedSensitiveDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    RadiobiologyVoxelizedSensitiveDetector* fVoxelizedDetector;
    
    G4UIdirectory*              fVoxelsDir;
    G4UIcmdWith3VectorAndUnit*  fVoxelSizeCmd;
    G4UIcmdWithADoubleAndUnit*  fVoxelSizeXCmd;
    G4UIcmdWithADoubleAndUnit*  fVoxelSizeYCmd;
    G4UIcmdWithADoubleAndUnit*  fVoxelSizeZCmd;
    G4UIcmdWithoutParameter*    fUpdateVoxCmd;
};

#endif //VoxelizedSensitiveDetectorMessenger_h

