//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *8
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

#include "HadrontherapyDetectorSD.hh"
#include "HadrontherapyDetectorHit.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "HadrontherapyMatrix.hh"
#include "HadrontherapyLet.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "HadrontherapyMatrix.hh"


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
#include "G4SystemOfUnits.hh"
#include "HadrontherapyRBE.hh"
#include <G4AccumulableManager.hh>

#include <iostream> // SERENA
#include <fstream>// SERENA
#include "HadrontherapyRunAction.hh" // SERENA
#include "G4Run.hh" // SERENA
#include "HadrontherapyDetectorSDMessenger.hh"  //SERENA
#include "G4HadronicProcess.hh"  // SERENA

// LOCK DECLARATION***********************************************
namespace {
 //Mutex to acquire access to singleton instance check/creation
 G4Mutex instanceMutex = G4MUTEX_INITIALIZER;
 //Mutex to acquire accss to histograms creation/access
 //It is also used to control all operations related to histos
 //File writing and check analysis
 G4Mutex dataManipulationMutex = G4MUTEX_INITIALIZER;
}
// ****************************************************************




/////////////////////////////////////////////////////////////////////////////
HadrontherapyDetectorSD::HadrontherapyDetectorSD(G4String name):
G4VSensitiveDetector(name)
{
    G4String HCname;
    collectionName.insert(HCname="HadrontherapyDetectorHitsCollection");
    HitsCollection = NULL;
    sensitiveDetectorName = name;

    fMess = new HadrontherapyDetectorSDMessenger(this); //SR

    G4cout << "i    PreStepEnergy     PostStepEnergy       TrackEnergy     PreStepProcessName       PrePostProcessName      PreStepStatus       PostStepStatus" << G4endl;

}



/////////////////////////////////////////////////////////////////////////////
HadrontherapyDetectorSD::~HadrontherapyDetectorSD()
{
    delete fMess; //SR

}

/////////////////////////////////////////////////////////////////////////////
void HadrontherapyDetectorSD::Initialize(G4HCofThisEvent*)
{

    HitsCollection = new HadrontherapyDetectorHitsCollection(sensitiveDetectorName,
                                                             collectionName[0]);


}

/////////////////////////////////////////////////////////////////////////////
G4bool HadrontherapyDetectorSD::ProcessHits(G4Step* aStep, G4TouchableHistory* )
{

    if (aStep -> GetPreStepPoint() -> GetPhysicalVolume() -> GetName() != "RODetectorZDivisionPhys") return false;
    
    // Traccia, IDs
    G4Track * theTrack = aStep -> GetTrack();
    G4int parentID = theTrack -> GetParentID();
    G4int eventID = G4RunManager::GetRunManager() -> GetCurrentEvent() -> GetEventID();
    G4int trackID = theTrack -> GetTrackID(); // Get unique track_id (in an event)

    // Particle name/pdg, A, Z
    G4ParticleDefinition *particleDef = theTrack -> GetDefinition();
    G4String particleName =  particleDef -> GetParticleName();
    G4int pdg = particleDef -> GetPDGEncoding();
    G4int Z = particleDef-> GetAtomicNumber();
    G4int A = particleDef-> GetAtomicMass();

    // Energie cinetiche e depositate
    G4double kineticEnergy = aStep -> GetPreStepPoint() -> GetKineticEnergy();
    G4double energyDeposit = aStep -> GetTotalEnergyDeposit(); // The energy lost to the medium (during a step?) is obtained with GetTotalEnergyDeposit
    G4double deltaEnergy = aStep -> GetDeltaEnergy(); // The energy lost by a particle (to the medium and to secondaries) during a step is obtained with GetDeltaEnergy
    G4double nonIonizingEnergyDeposit = aStep -> GetNonIonizingEnergyDeposit(); // The energy available for ionization is: GetTotalEnergyDeposit() - GetNonIonizingEnergyDeposit()

    // informazioni sul vertex
    G4double vertexKineticEnergy =  theTrack -> GetVertexKineticEnergy();
    G4ThreeVector vertexPosition = theTrack -> GetVertexPosition();
    G4String VolumeAtVertex = theTrack->GetLogicalVolumeAtVertex()->GetName();


    // lunghezza traccia (cumulativa?) e lunghezza step
    G4double trackLength = theTrack -> GetTrackLength(); // Accumulated track length ???
    G4double stepLength = theTrack -> GetStepLength(); // step length
    G4double DX = aStep -> GetStepLength(); // sarà lo stesso?

    // Processi
    const G4VProcess* creatorProcess;
    G4VProcess* proc;
    G4HadronicProcess* hproc;
    G4String process_name;
    G4int process_type;
    G4int process_subtype;
    G4int model_id;
    if(parentID>0){
        creatorProcess = theTrack -> GetCreatorProcess();
        proc = const_cast<G4VProcess*>(creatorProcess);
        process_name = creatorProcess -> GetProcessName();
        process_type = creatorProcess->GetProcessType();
        process_subtype = creatorProcess->GetProcessSubType();
        model_id = aStep->GetTrack()->GetCreatorModelID();
    }
    
    // TARGET
    const G4Isotope* target = NULL;
    G4String target_name;
    
    // Read voxel indexes: i is the x index, k is the z index
    const G4VTouchable* touchable = aStep->GetPreStepPoint()->GetTouchable();
    G4int k  = touchable->GetReplicaNumber(0);
    G4int i  = touchable->GetReplicaNumber(2);
    G4int j  = touchable->GetReplicaNumber(1);

    // coordinate dello step
    G4ThreeVector aPosition = aStep -> GetPreStepPoint() -> GetPosition();
    G4ThreeVector bPosition = aStep -> GetPostStepPoint() -> GetPosition();

    // Volumi Vari
   // G4StepPoint* PreStep = aStep->GetPreStepPoint();
   // G4TouchableHandle touchPreStep = PreStep->GetTouchableHandle();
   // G4VPhysicalVolume* volumePre = touchPreStep->GetVolume();
   // G4String namePre = volumePre->GetName();
    G4VPhysicalVolume *PhysVolume = theTrack->GetVolume();
    G4String PhysicalVolume_Track = PhysVolume->GetName();
    G4String PhysicalVolume_PreStep = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    G4String PhysicalVolume_PostStep;
    if(aStep->GetPostStepPoint()->GetPhysicalVolume()) PhysicalVolume_PostStep = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    else PhysicalVolume_PostStep = "OutOfWorld";
    

    HadrontherapyMatrix* matrix = HadrontherapyMatrix::GetInstance();
    HadrontherapyLet* let = HadrontherapyLet::GetInstance();

    G4int* hitTrack = matrix -> GetHitTrack(i,j,k);

    
    // NEW CODE TO DETECT WHAT SPECIFIC PROCESS PRODUCED ALPHAS (BORON OR NOT) ************************************************************
    /*
    G4String p2name;
    if (particleName == "neutron") {
       if(aStep -> GetPostStepPoint() -> GetProcessDefinedStep()) p2name = (aStep -> GetPostStepPoint() -> GetProcessDefinedStep() -> GetProcessName());
       // if (p2name != "CoupledTransportation" && p2name != "DetectorROGeometry") G4cout << "pippo: per   " << p2name << G4endl;
        if (p2name == "neutronInelastic") {
            G4cout << G4endl << "We had a neutronInelastic   " << G4endl;
            const std::vector<const G4Track*>* secondaries = aStep -> GetSecondaryInCurrentStep();
            G4cout << "secondaries size: " << secondaries->size() << G4endl ;
            for (auto it = secondaries->cbegin(); it != secondaries->cend();++it){
                G4cout << "secondary produced:   " << (*it)->GetParticleDefinition()->GetParticleName() << "   parentID:" << (*it)->GetParentID() << " EventID:"<< eventID << "   trackID:" << (*it)->GetTrackID() << "   Energy:" << (*it)->GetKineticEnergy() << "   Status:" << (*it)->GetTrackStatus()    << G4endl;
                
            }
        }
    }

    else if (particleName == "proton") {
       if(aStep -> GetPostStepPoint() -> GetProcessDefinedStep()) p2name = (aStep -> GetPostStepPoint() -> GetProcessDefinedStep() -> GetProcessName());
        //if (p2name != "CoupledTransportation" && p2name != "DetectorROGeometry") G4cout << "pippo: per   " << p2name << G4endl;
        if (p2name == "protonInelastic") {
            G4cout << G4endl << "We had a protonInelastic   " << G4endl;
            const std::vector<const G4Track*>* secondaries = aStep -> GetSecondaryInCurrentStep();
            G4cout << "secondaries size: " << secondaries->size() << G4endl ;
            for (auto it = secondaries->cbegin(); it != secondaries->cend();++it){
                G4cout << "secondary produced:   " << (*it)->GetParticleDefinition()->GetParticleName() << "   parentID:" << (*it)->GetParentID() << " EventID:"<< eventID << "   trackID:" << (*it)->GetTrackID() << "   Energy:" << (*it)->GetKineticEnergy() << "   Status:" << (*it)->GetTrackStatus()    << G4endl;
                
            }
        }
    }

*/


    // MODIFIED BY SERENA 29-NOV-2021 -----------------------------------------------------------------------------------------------------------
  /*  else if (Z > 1){
  // else if (particleName == "alpha"){
        if(aStep->GetTrack()->GetCreatorProcess()){
           // G4cout << G4endl << "******* FRAGMENT CREATED *******" << G4endl;
            //G4cout << "process type: " << process_type <<"  process subtype:"<<process_subtype<<" model id"<<model_id<< "    Energy:" << vertexKineticEnergy  << G4endl;
            if(proc->GetProcessType()==4)
            {
                hproc = const_cast<G4HadronicProcess*>(static_cast<const G4HadronicProcess*>(proc));
                
                if(hproc){
                    target = hproc->GetTargetIsotope();
                    target_name = target->GetName();
                  //  G4cout <<"  The fragment has:      parentID:" << parentID << "    eventID:" <<eventID << "    trackID:"<< trackID << "    production target:"  << target_name << "   Volume@Vertex:"  << VolumeAtVertex << "  PhysicalVolume_Track: " << PhysicalVolume_Track << "  PhysicalVolume_PreStep: " << PhysicalVolume_PreStep << "  PhysicalVolume_PostStep: " << PhysicalVolume_PostStep <<G4endl;
                   
                    // Salva su file
                    std::ofstream WriteDataIn("Hadronic_SD.out", std::ios::app);
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
                             << G4endl;
                
                }
            }
            else if(proc->GetProcessType()==6){
               // G4cout <<"  The fragment DA DECADIMENTO has:      parentID:" << parentID << "    eventID:" <<eventID << "    trackID:"<< trackID <<  "   Volume@Vertex:"  << VolumeAtVertex << "  PhysicalVolume_Track: " << PhysicalVolume_Track << "  PhysicalVolume_PreStep: " << PhysicalVolume_PreStep << "  PhysicalVolume_PostStep: " << PhysicalVolume_PostStep  <<G4endl;
            
                // Salva su file
                std::ofstream WriteDataIn("Decay_SD.out", std::ios::app);
                WriteDataIn
                         << particleName << " "
                         << parentID << " "
                         << eventID << " "
                         << trackID << " "
                         << VolumeAtVertex << " "
                         << PhysicalVolume_Track << " "
                         << PhysicalVolume_PreStep << " "
                         << PhysicalVolume_PostStep << " "
                         << G4endl;
            
            }
        }
    }
   
   */
    //  ---------------------------------------------------------------------------------------------------------------------------------------------
    
    
   /* else if (Z>1){
        G4cout << G4endl <<"    Secondary particle (not alpha) with Z>1:  " << particleName << "   parentID:" << parentID << "    eventID:" <<eventID << "     trackID:"<< trackID   << "   trackStatus:" << aStep->GetTrack()->GetTrackStatus()      << " Energy: " << aStep->GetTrack()->GetVertexKineticEnergy() << "    Creator process: "<< process_name
            << "    Post creation process: " << aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
    }
*/
    // *************************************************************************************************************************************
    
     
     
    // Salva tutti i frammenti
    
    if ( parentID >=1 && Z>=0 && A>=1) {
        std::ofstream WriteDataIn("DatiFrammenti.out", std::ios::app);
        // ADDED 30 NOVEMBRE 2021 ------------------------------------------------------------------
         if(creatorProcess->GetProcessType()==4) // 15-12-2021
         {
            // hproc = const_cast<G4HadronicProcess*>(static_cast<const G4HadronicProcess*>(proc)); // 15-12-2021
             hproc = (G4HadronicProcess*)creatorProcess; // 15-12-2021
             if(hproc){
                 target = hproc->GetTargetIsotope();
                 target_name = target->GetName();
             }
         }
         else target_name = "NO TARGET";
         // -----------------------------------------------------------------------------------------
        G4AutoLock l(&dataManipulationMutex); // LOCK START
        WriteDataIn
            << i << " "
            << j << " "
            << k << " "

        << trackID << " "
        << eventID << " "
        << parentID << " "
        << Z << " "
        << A << " "
        << particleName << " "

        << trackLength << " "
        << stepLength << " "

        << vertexKineticEnergy << " "
        << vertexPosition[0] << " "
        << vertexPosition[1] << " "
        << vertexPosition[2] << " "
        << VolumeAtVertex << " " // ADDED 30-11-2021
        

        << kineticEnergy << " "
        << deltaEnergy << " "
        << energyDeposit << " "
        << nonIonizingEnergyDeposit << " "

        << aPosition[0] << " "
        << aPosition[1] << " "
        << aPosition[2] << " "
        << bPosition[0] << " "
        << bPosition[1] << " "
        << bPosition[2] << " "
        << process_name << " "
        << process_type << " "      // ADDED 13-12-2021
        << process_subtype << " "   // ADDED 13-12-2021
        << model_id << " "          // ADDED 13-12-2021
        << target_name // ADDED 30-11-2021
        << G4endl;
        l.unlock(); // LOCK STOP
    }
     

    // Salva la ciccia per i primari (ogni tot)
    std::ofstream WriteDataBis("DatiPrimari.out", std::ios::app);
    if ( (eventID % 1000) == 0 && parentID == 0 ) { // salva primari ogni 1000 eventi (saltando quelli che non entrano nelle cellule)
        // if ( (eventID % 1) == 0 && parentID == 0 ) { // salva tutti i primari (per analisi g(r))
        G4AutoLock l(&dataManipulationMutex); // LOCK START
        WriteDataBis
            << i << " "
            << j << " "
            << k << " "

        << trackID << " "
        << eventID << " "
        << parentID << " "
        << Z << " "
        << A << " "
        << particleName << " "

        << trackLength << " "
        << stepLength << " "

        << vertexKineticEnergy << " "
        << vertexPosition[0] << " "
        << vertexPosition[1] << " "
        << vertexPosition[2] << " "

        << kineticEnergy << " "
        << deltaEnergy << " "
        << energyDeposit << " "
        << nonIonizingEnergyDeposit << " "

        << aPosition[0] << " "
        << aPosition[1] << " "
        << aPosition[2] << " "
        << bPosition[0] << " "
        << bPosition[1] << " "
        << bPosition[2] << " "
        << process_name
        << G4endl;
        l.unlock(); // LOCK STOP
    }


    //  ******************** let ***************************
    if (let)
    {
        if ( !(Z==0 && A==1) ) // All but not neutrons
        {
            if( energyDeposit>0. && DX >0. )// calculate only energy deposit
            {
                if (pdg !=22 && pdg !=11) // not gamma and electrons
                {

                    // Get the pre-step kinetic energy
                    G4double eKinPre = aStep -> GetPreStepPoint() -> GetKineticEnergy();
                    // Get the post-step kinetic energy
                    G4double eKinPost = aStep -> GetPostStepPoint() -> GetKineticEnergy();
                    // Get the step average kinetic energy
                    G4double eKinMean = (eKinPre + eKinPost) * 0.5;

                    // get the material
                    G4Material * materialStep = aStep -> GetPreStepPoint() -> GetMaterial();

                    // get the secondary paticles
                    G4Step fstep = *theTrack -> GetStep();
                    // store all the secondary partilce in current step
                    const std::vector<const G4Track*> * secondary = fstep.GetSecondaryInCurrentStep();

                    size_t SecondarySize = (*secondary).size();
                    G4double EnergySecondary = 0.;

                    // get secondary electrons energy deposited
                    if (SecondarySize) // calculate only secondary particles
                    {
                        for (size_t numsec = 0; numsec< SecondarySize ; numsec ++)
                        {
                            //Get the PDG code of every secondaty particles in current step
                            G4int PDGSecondary=(*secondary)[numsec]->GetDefinition()->GetPDGEncoding();

                            if(PDGSecondary == 11) // calculate only secondary electrons
                            {
                                // calculate the energy deposit of secondary electrons in current step
                                EnergySecondary += (*secondary)[numsec]->GetKineticEnergy();
                            }
                        }

                    }

                    // call the let filldatas function to calculate let
                    G4AutoLock l(&dataManipulationMutex); // LOCK START
                    let -> FillEnergySpectrum(trackID, particleDef, eKinMean, materialStep,
                                              energyDeposit,EnergySecondary,DX, i, j, k);
                    l.unlock(); // LOCK STOP
                }
            }
        }
    }


    if (matrix)
    {
        G4AutoLock l(&dataManipulationMutex);  // LOCK START

        // Increment Fluences & accumulate energy spectra
        // Hit voxels are marked with track_id throught hitTrack matrix
        //G4int* hitTrack = matrix -> GetHitTrack(i,j,k); // hitTrack MUST BE cleared at every eventAction!
        if ( *hitTrack != trackID )
        {
            *hitTrack = trackID;
            /*
             * Fill FLUENCE data for every single nuclide
             * Exclude e-, neutrons, gamma, ...
             */
            if ( Z >= 1) matrix -> Fill(trackID, particleDef, i, j, k, 0, true);

        }

        if(energyDeposit != 0)
        {
            /*
             *  This method will fill a dose matrix for every single nuclide.
             *  A method of the HadrontherapyMatrix class (StoreDoseFluenceAscii())
             *  is called automatically at the end of main (or via the macro command /analysis/writeDoseFile.
             *  It permits to store all dose/fluence data into a single plane ASCII file.
             */
            // if (A==1 && Z==1) // primary and sec. protons
            if ( Z>=1 )    //  exclude e-, neutrons, gamma, ...
                matrix -> Fill(trackID, particleDef, i, j, k, energyDeposit);
            /*
             * Create a hit with the information of position is in the detector
             */
            HadrontherapyDetectorHit* detectorHit = new HadrontherapyDetectorHit();
            detectorHit -> SetEdepAndPosition(i, j, k, energyDeposit);
            HitsCollection -> insert(detectorHit);
        }

        l.unlock(); // LOCK STOP
    }

    auto rbe = HadrontherapyRBE::GetInstance();
    if (rbe->IsCalculationEnabled())
    {
        if (!fRBEAccumulable)
        {
            fRBEAccumulable = dynamic_cast<HadrontherapyRBEAccumulable*>(G4AccumulableManager::Instance()->GetAccumulable("RBE"));
            if (!fRBEAccumulable)
            {
                G4Exception("HadrontherapyDetectorSD::ProcessHits", "NoAccumulable", FatalException, "Accumulable RBE not found.");
            }
        }
	if (A>0) //protect against gammas, e- , etc
	  {
	    fRBEAccumulable->Accumulate(kineticEnergy / A, energyDeposit, DX, Z, i, j, k);
	  }
    }


    return true;
}

/////////////////////////////////////////////////////////////////////////////
void HadrontherapyDetectorSD::EndOfEvent(G4HCofThisEvent* HCE)
{

    static G4int HCID = -1;
    if(HCID < 0)
    {
        HCID = GetCollectionID(0);
    }

    HCE -> AddHitsCollection(HCID,HitsCollection);
}

