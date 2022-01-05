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
/// \file HadrontherapyStackingAction.cc
/// \brief Implementation of the HadrontherapyStackingAction class

#include "HadrontherapyStackingAction.hh"

#include "G4Track.hh"
//#include "G4NeutrinoE.hh"

// SERENA:
#include "G4Step.hh"
#include "G4Run.hh" // SERENA
#include "G4RunManager.hh" // SERENA
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4HadronicProcess.hh"  // SERENA
#include "G4Isotope.hh" // SERENA

#include "G4ios.hh"
#include <iostream> // SERENA
#include <fstream>// SERENA


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadrontherapyStackingAction::HadrontherapyStackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadrontherapyStackingAction::~HadrontherapyStackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



G4ClassificationOfNewTrack
HadrontherapyStackingAction::ClassifyNewTrack(const G4Track* theTrack)
{
    
    // IDs
    G4int parentID = theTrack -> GetParentID();
    G4int eventID = G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID();
    G4int trackID = theTrack -> GetTrackID();
    
    // Particle
    G4ParticleDefinition *particleDef = theTrack -> GetDefinition();
    G4String particleName =  particleDef -> GetParticleName();
    G4double particle_Energy = theTrack->GetKineticEnergy();
    
    
    // Process
    if (theTrack->GetCreatorProcess() == nullptr)
        return G4UserStackingAction::ClassifyNewTrack(theTrack);
    
    G4String process_name = theTrack->GetCreatorProcess()->GetProcessName();
    G4int process_type = theTrack->GetCreatorProcess()->GetProcessType();
    G4int process_subtype = theTrack->GetCreatorProcess()->GetProcessSubType();
    G4int model_id = theTrack->GetCreatorModelID();
    auto proc = const_cast<G4VProcess*>(theTrack->GetCreatorProcess());
    auto hproc = const_cast<G4HadronicProcess*>(static_cast<const G4HadronicProcess*>(proc));
    

    // Target
    const G4Isotope* target = NULL;
    const G4Nucleus* target_nucleus = NULL;
    G4String target_name = "XYZ";
    
    
    // Volume
    G4VPhysicalVolume *PhysVolume = theTrack->GetVolume();
    G4String PhysicalVolume = PhysVolume->GetName();
    
   // G4String VolumeAtVertex = theTrack->GetLogicalVolumeAtVertex()->GetName();

    
    //if (particleName == "alpha" && theTrack->GetCreatorProcess()){
    if (theTrack->GetCreatorProcess()){
        
       // G4cout << G4endl << "******* ALPHA PARTICLE CREATED *******" << G4endl;
        
        if(proc->GetProcessType()==4){
               if(hproc){
                   target = hproc->GetTargetIsotope();
                   if (target){
                   target_name = target->GetName();
                      // if (target_name=="B10" || target_name=="B11"){
                           target_nucleus = hproc->GetTargetNucleus();
                           //const G4int target_A = target_nucleus->GetA_asInt();
                           //const G4int target_N = target_nucleus->GetN_asInt();
                           //const G4int target_Z = target_nucleus->GetZ_asInt();
                   
                           // Salva su file i secondari generati da processi adronici su B11
                          /* std::ofstream WriteDataIn("Hadronic_Stack.out", std::ios::app);
                           WriteDataIn
                                    << particleName << " "
                                    << parentID << " "
                                    << eventID << " "
                                    << trackID << " "
                                    << target_name << " "
                                 //   << VolumeAtVertex << " "
                                    << PhysicalVolume << " "
                                    << particle_Energy << " "
                                    << process_name << " "
                                    << process_type << " "
                                    << process_subtype << " "
                                    << model_id << " "
                                    << G4endl;
                           */
                           
                           
                           // Stampa a schermo i secondari generati da processi adronici su B11
                          // if (particleName == "alpha") G4cout << "********************************************" << G4endl;
                          // G4cout << "Particle name:" << particleName  <<"  parentID:" << parentID << "    eventID:" << eventID << "    trackID:" << trackID<< "    particle energy:" << particle_Energy  << G4endl;
                           
                         //  G4cout << "  PhysicalVolume:" << PhysicalVolume << "  process_name: " << process_name << "  process type: " << process_type << "  process subtype:" << process_subtype << "  model id:" << model_id << "    target_name: " << target_name << G4endl << G4endl << G4endl;
                               
                           //}
                   }
                 
                   
                   
                 // target = NULL;
               }
            
            
        }
        else if(proc->GetProcessType()==6) {
            
            // Salva su file i secondari generati da decadimento
           /* std::ofstream WriteDataIn("Decay_Stack.out", std::ios::app);
            WriteDataIn
                    << particleName << " "
                    << parentID << " "
                    << eventID << " "
                    << trackID << " "
                  //  << VolumeAtVertex << " "
                    << PhysicalVolume << " "
                    << particle_Energy << " "
                    << process_name << " "
                    << process_type << " "
                    << process_subtype << " "
                    << model_id << " "
                    << G4endl;
            */
            // Stampa a schermo i secondari generati da decadimento
           // G4cout  << "--------- DECADIMENTO ---------" << G4endl;
           // G4cout  << "Particle name:" << particleName  <<"  parentID:" << parentID << "    eventID:" << eventID << "    trackID:" << trackID <<"    particle energy:" << particle_Energy  << G4endl;
            
           // G4cout << "   PhysicalVolume:" << PhysicalVolume << "  process_name: " << process_name << " process type: " << process_type << "  process subtype:" << process_subtype << " model id:" << model_id << G4endl << G4endl << G4endl;
            
        }
    }
    
    return G4UserStackingAction::ClassifyNewTrack(theTrack);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




