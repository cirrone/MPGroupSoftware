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
// Hadrontherapy advanced example for Geant4
// See more at: https://twiki.cern.ch/twiki/bin/view/Geant4/AdvancedExamplesHadrontherapy

#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"
#include "HadrontherapySteppingAction.hh"
#include "G4ios.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4TrackVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4UserEventAction.hh"
#include "G4TransportationManager.hh"
#include "G4VSensitiveDetector.hh"
#include "HadrontherapyRunAction.hh"
#include "HadrontherapyMatrix.hh"
#include "G4SystemOfUnits.hh"

#include <iostream> // SERENA
#include <fstream>// SERENA
#include "HadrontherapyRunAction.hh" // SERENA
#include "G4Run.hh" // SERENA
#include "HadrontherapyDetectorSDMessenger.hh"  //SERENA
#include "G4HadronicProcess.hh"  // SERENA


/////////////////////////////////////////////////////////////////////////////
HadrontherapySteppingAction::HadrontherapySteppingAction( HadrontherapyRunAction *run)
{
    runAction = run;
}

/////////////////////////////////////////////////////////////////////////////
HadrontherapySteppingAction::~HadrontherapySteppingAction()
{
}

/////////////////////////////////////////////////////////////////////////////
void HadrontherapySteppingAction::UserSteppingAction(const G4Step* aStep)
{
    
    // The followings are calls to usefuls information retrieved at the step level
    // Please, comment out them if want to use
    
   // G4Track* theTrack = aStep->GetTrack();
    
    G4StepPoint* PreStep = aStep->GetPreStepPoint();
    G4StepPoint* PostStep = aStep->GetPostStepPoint();
    
    G4TouchableHandle touchPreStep = PreStep->GetTouchableHandle();
    G4TouchableHandle touchPostStep = PostStep->GetTouchableHandle();
    
    //G4double PreStepX =PreStep->GetPosition().x();
    //G4double PreStepY =PreStep->GetPosition().y();
    //G4double PreStepZ =PreStep->GetPosition().z();
    
    //G4double PostStepX =PostStep->GetPosition().x();
    //G4double PostStepY =PostStep->GetPosition().y();
    //G4double PostStepZ  =PostStep->GetPosition().z();
    
    //To get the current volume:
    G4VPhysicalVolume* volumePre = touchPreStep->GetVolume();
    //G4VPhysicalVolume* volumePost =touchPostStep->GetVolume();
    
    //To get its name:
    G4String namePre = volumePre->GetName();

    
    // positions in the global coordinate system:
    //G4ThreeVector posPreStep  = PreStep->GetPosition();
    //G4ThreeVector posPostStep = PostStep->GetPosition();
    
    //G4int eventNum = G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID();
    
    //G4double parentID =aStep->GetTrack()->GetParentID();
    //G4double trackID =aStep->GetTrack()->GetTrackID();
    
    G4double eKin = aStep -> GetPreStepPoint() -> GetKineticEnergy();
    
    G4double PosX = aStep->GetTrack()->GetPosition().x();
    G4double PosY = aStep->GetTrack()->GetPosition().y();
    G4double PosZ = aStep->GetTrack()->GetPosition().z();
    
    G4String volume=  aStep->GetTrack()->GetVolume()->GetName();
    G4Track* theTrack = aStep->GetTrack();
    
    //G4String material= aStep -> GetTrack() -> GetMaterial() -> GetName();
    //G4cout << "material   " << material << G4endl;
    //G4String volume=  aStep->GetTrack()->GetVolume()->GetName();
    //G4String pvname= pv-> GetName();
    


    G4double momentumX =  aStep->GetTrack()->GetMomentumDirection().x();
    G4double momentumY =  aStep->GetTrack()->GetMomentumDirection().y();
    G4double momentumZ =  aStep->GetTrack()->GetMomentumDirection().z();
    
    // Particle name/pdg, A, Z
    G4ParticleDefinition *particleDef = theTrack -> GetDefinition();
    G4String particleName = particleDef->GetParticleName();
    G4int pdg = particleDef ->GetPDGEncoding();
    G4int Z = particleDef-> GetAtomicNumber();
   // G4int A = particleDef-> GetAtomicMass();
    
    // ADDED BY SERENA 29-NOV-2021 -----------------------------------------------------------------------------------------------------------
    G4int parentID = theTrack -> GetParentID();
    G4int eventID = G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID();
    G4int trackID = theTrack -> GetTrackID(); // Get unique track_id (in an event)
    
    G4VPhysicalVolume *PhysVolume = theTrack->GetVolume();
    G4String PhysicalVolume_Track = PhysVolume->GetName();
    G4String PhysicalVolume_PreStep = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    G4String PhysicalVolume_PostStep;
    if(aStep->GetPostStepPoint()->GetPhysicalVolume()) PhysicalVolume_PostStep = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    else PhysicalVolume_PostStep = "OutOfWorld";
    
    if (Z > 1){
  //  if (particleName == "alpha"){
        if(aStep->GetTrack()->GetCreatorProcess()){
            G4int process_type = aStep->GetTrack()->GetCreatorProcess()->GetProcessType();
            G4int process_subtype = aStep->GetTrack()->GetCreatorProcess()->GetProcessSubType();
            G4int model_id = aStep->GetTrack()->GetCreatorModelID();
            //G4cout << G4endl << "******* ALPHA PARTICLE CREATED *******" << G4endl;
            //G4cout << "process type: " << process_type <<"  process subtype:"<<process_subtype<<" model id"<<model_id<< "    Energy:" << aStep->GetTrack()->GetVertexKineticEnergy()  << G4endl;
            //auto proc = const_cast<G4VProcess*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
            auto proc = const_cast<G4VProcess*>(aStep->GetTrack()->GetCreatorProcess());
            G4String VolumeAtVertex = aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
            //if(proc->GetProcessType()==4 || proc->GetProcessType()==6)
            if(proc->GetProcessType()==4)
            {
                auto hproc = const_cast<G4HadronicProcess*>(static_cast<const G4HadronicProcess*>(proc));
                const G4Isotope* target = NULL;
                if(hproc){
                    target = hproc->GetTargetIsotope();
                    G4String target_name = target->GetName();
                    //G4cout <<"  The alpha particle has:      parentID:" << parentID << "    eventID:" <<eventID << "    trackID:"<< trackID << "    production target:"  << target_name << "   Volume@Vertex:"  << VolumeAtVertex << "  PhysicalVolume_Track: " << PhysicalVolume_Track << "  PhysicalVolume_PreStep: " << PhysicalVolume_PreStep << "  PhysicalVolume_PostStep: " << PhysicalVolume_PostStep <<G4endl;
                   
                    // Salva su file
                 /*   std::ofstream WriteDataIn("Hadronic_Step.out", std::ios::app);
                    WriteDataIn
                             << particleName << " "
                             << parentID << " "
                             << eventID << " "
                             << trackID << " "
                             << target_name << " "
                             << VolumeAtVertex << " "
                             << PhysicalVolume_Track << " "
                             << PhysicalVolume_PreStep << " "
                             << PhysicalVolume_PostStep << " "
                             << process_type << " "      // ADDED 13-12-2021
                             << process_subtype << " "   // ADDED 13-12-2021
                             << model_id << " "          // ADDED 13-12-2021
                             << G4endl;
                */
                }
            }
            else if(proc->GetProcessType()==6){
                //G4cout <<"  The alpha particle DA DECADIMENTO has:      parentID:" << parentID << "    eventID:" <<eventID << "    trackID:"<< trackID <<  "   Volume@Vertex:"  << VolumeAtVertex << "  PhysicalVolume_Track: " << PhysicalVolume_Track << "  PhysicalVolume_PreStep: " << PhysicalVolume_PreStep << "  PhysicalVolume_PostStep: " << PhysicalVolume_PostStep  <<G4endl;
            
                // Salva su file
              /*  std::ofstream WriteDataIn("Decay_Step.out", std::ios::app);
                WriteDataIn
                         << particleName << " "
                         << parentID << " "
                         << eventID << " "
                         << trackID << " "
                         << VolumeAtVertex << " "
                         << PhysicalVolume_Track << " "
                         << PhysicalVolume_PreStep << " "
                         << PhysicalVolume_PostStep << " "
                         << process_type << " "      // ADDED 13-12-2021
                         << process_subtype << " "   // ADDED 13-12-2021
                         << model_id << " "          // ADDED 13-12-2021
                         << G4endl;
               */
            }
        }
    }
    //  ---------------------------------------------------------------------------------------------------------------------------------------------
    
    
    
    if(namePre == "VirtualLayer")
    {
        std::ofstream WriteDataIn("Virtual_Layer.txt", std::ios::app);
        WriteDataIn
        
        <<   eKin             <<" " //  1
        <<   PosX             <<" " //  2
        <<   PosY             <<" " //  3
        <<   PosZ             <<" " //  4
        <<   momentumX        <<" " //  5
        <<   momentumY        <<" " //  6
        <<   momentumZ        <<" " //  7
        <<   pdg
        //<<   theTrack         << '\t' << "   "
        
        <<   G4endl;
        
        theTrack -> SetTrackStatus(fKillTrackAndSecondaries);
        
        
    }
    
    
    
  
}



