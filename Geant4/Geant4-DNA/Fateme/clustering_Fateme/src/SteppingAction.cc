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
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
//
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"

#include "ClusteringAlgo.hh"
#include "G4EventManager.hh"
#include "EventAction.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>
#include "Analysis.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
:G4UserSteppingAction(),RunInitObserver(),fpEventAction(0)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::Initialize()
{
  fpEventAction = (EventAction*) G4EventManager::GetEventManager()->
      GetUserEventAction();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* pStep)
{
  G4double edepStep = pStep->GetTotalEnergyDeposit();
  if(edepStep>0)
  {
    fpEventAction->AddEdep(edepStep);
    fpEventAction->fpClustering->RegisterDamage(
        pStep->GetPreStepPoint()->GetPosition(),edepStep);
  }
  
   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   
   G4int trackID =pStep->GetTrack()-> GetTrackID(); 
   G4int stepNo =pStep->GetTrack()-> GetCurrentStepNumber();    
   
   if(trackID==1 && stepNo==1) {
	  
	 G4double eKin= pStep-> GetPreStepPoint()-> GetKineticEnergy();
	 
	 std::ofstream f2;
  f2.open("Spectrum.txt", std::ios::app);
	 
	 analysisManager-> FillH1(6, eKin);
	 f2 << ", Energy-Spectrum: "<< eKin << G4endl;
	 
	 f2.close();
	 
}

}
