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


#ifndef RadiobiologyPhysicsList_h
#define RadiobiologyPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
class RadiobiologyPhysicsListMessenger;

class RadiobiologyPhysicsList: public G4VModularPhysicsList
{
public:
    
    RadiobiologyPhysicsList();
    virtual ~RadiobiologyPhysicsList();
    
    void ConstructParticle();
    void SetCutForGamma(G4double);
    void SetCutForElectron(G4double);
    void SetCutForPositron(G4double);
    void SetDetectorCut(G4double cut);
    void AddPhysicsList(const G4String& name);
    void ConstructProcess();
    void AddPackage(const G4String& name);
    
private:
    
    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;
    G4String fEmName;
    G4VPhysicsConstructor* fEmPhysicsList;
    G4VPhysicsConstructor* fDecayPhysicsList;

    std::vector<G4VPhysicsConstructor*>  hadronPhys;
        
    RadiobiologyPhysicsListMessenger* pMessenger;
};

#endif //PhysicsList_h
