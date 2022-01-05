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
// See more at: https://twiki.cern.ch/twiki/bin/view/Geant4/AdvancedExamplesHadrontherapy

#ifndef HadrontherapyDetectorSD_h
#define HadrontherapyDetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "HadrontherapyDetectorHit.hh"
#include "globals.hh"

#include <iostream> // SR
#include <fstream>// SR

#include "HadrontherapyDetectorSDMessenger.hh"//SR
class HadrontherapyDetectorSDMessenger; //SR

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class HadrontherapyRBEAccumulable;
class HadrontherapyDetectorSD : public G4VSensitiveDetector
{
public:
    HadrontherapyDetectorSD(G4String name);
    ~HadrontherapyDetectorSD();
    
    
    std::ofstream ofs;
    void Initialize(G4HCofThisEvent*);
    
    G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    
    void EndOfEvent(G4HCofThisEvent*HCE);
    
   // void SetSpectraFileName(G4String name){spectrafilename = name;} //SR
    void SetSpectraFileName(G4String name){
        
        filenameout_1 = name+"_1mm.out";
        filenameout_2 = name+"_2mm.out";
        filenameout_3 = name+"_3mm.out";
        filenameout_4 = name+"_4mm.out";
        filenameout_5 = name+"_5mm.out";
        filenameout_6 = name+"_6mm.out";
        filenameout_7 = name+"_7mm.out";
        filenameout_8 = name+"_8mm.out";
        filenameout_9 = name+"_9mm.out";
        filenameout_10 = name+"_10mm.out";
        filenameout_11 = name+"_11mm.out";
        filenameout_12 = name+"_12mm.out";
        filenameout_13 = name+"_13mm.out";
        filenameout_14 = name+"_14mm.out";
        filenameout_15 = name+"_15mm.out";
        filenameout_16 = name+"_16mm.out";
        filenameout_17 = name+"_17mm.out";
        filenameout_18 = name+"_18mm.out";
        filenameout_19 = name+"_19mm.out";
        filenameout_20 = name+"_20mm.out";
        filenameout_21 = name+"_21mm.out";
        filenameout_22 = name+"_22mm.out";
        filenameout_23 = name+"_23mm.out";
        filenameout_24 = name+"_24mm.out";
        filenameout_25 = name+"_25mm.out";
        filenameout_26 = name+"_26mm.out";
        filenameout_27 = name+"_27mm.out";
        filenameout_28 = name+"_28mm.out";
        filenameout_29 = name+"_29mm.out";
        filenameout_30 = name+"_30mm.out";
        
       
        filenameout_0_50 = name+"_0.50mm.out";
        filenameout_0_70 = name+"_0.70mm.out";
        filenameout_1_20 = name+"_1.20mm.out";
        filenameout_1_50 = name+"_1.50mm.out";
        
        filenameout_23_50 = name+"_23.50mm.out";
        filenameout_23_70 = name+"_23.70mm.out";
        filenameout_24_20 = name+"_24.20mm.out";
        filenameout_24_50 = name+"_24.50mm.out";
        
        filenameout_27_90 = name+"_27.90mm.out";
        filenameout_28_10 = name+"_28.10mm.out";
        filenameout_28_40 = name+"_28.40mm.out";
        filenameout_28_60 = name+"_28.60mm.out";
        filenameout_28_90 = name+"_28.90mm.out";
        
  /*
        std::ofstream WriteDataIn(filenameout1, std::ios::app); //SR
           WriteDataIn
            <<   "i"              <<'\t' //  1
            <<   "runID"          <<'\t' //  2
            <<   "eventID"        <<'\t' //  3
            <<   "trackID"        <<'\t' //  4
            <<   "parentID"       <<'\t' //  5
            <<   "pdg"            <<'\t' //  6
            <<   "Z"              <<'\t' //  7
            <<   "A"             <<'\t' //  8
            <<   "kineticEnergy"  <<'\t' //  9
            <<   "init_pos[0]"    <<'\t' //  10
            <<   "init_pos[1]"    <<'\t' //  11
            <<   "init_pos[2]"    <<'\t' //  12
            <<   "process_name"          //  13
            <<   G4endl;
        
*/
        
        

        
        } //SR
        
   

    
private:
    HadrontherapyDetectorHitsCollection *HitsCollection;
    G4String sensitiveDetectorName;
    HadrontherapyRBEAccumulable* fRBEAccumulable{ nullptr };
    
    HadrontherapyDetectorSDMessenger* fMess; //SR
    G4String filenameout_1; //SR
    G4String filenameout_2; //SR
    G4String filenameout_3; //SR
    G4String filenameout_4; //SR
    G4String filenameout_5; //SR
    G4String filenameout_6; //SR
    G4String filenameout_7; //SR
    G4String filenameout_8; //SR
    G4String filenameout_9; //SR
    G4String filenameout_10; //SR
    G4String filenameout_11; //SR
    G4String filenameout_12; //SR
    G4String filenameout_13; //SR
    G4String filenameout_14; //SR
    G4String filenameout_15; //SR
    G4String filenameout_16; //SR
    G4String filenameout_17; //SR
    G4String filenameout_18; //SR
    G4String filenameout_19; //SR
    G4String filenameout_20; //SR
    G4String filenameout_21; //SR
    G4String filenameout_22; //SR
    G4String filenameout_23; //SR
    G4String filenameout_24; //SR
    G4String filenameout_25; //SR
    G4String filenameout_26; //SR
    G4String filenameout_27; //SR
    G4String filenameout_28; //SR
    G4String filenameout_29; //SR
    G4String filenameout_30; //SR
    
    
    G4String filenameout_0_50; //SR
    G4String filenameout_0_70; //SR
    G4String filenameout_1_20; //SR
    G4String filenameout_1_50; //SR
    
    G4String filenameout_23_50; //SR
    G4String filenameout_23_70; //SR
    G4String filenameout_24_20; //SR
    G4String filenameout_24_50; //SR
    
    G4String filenameout_27_90; //SR
    G4String filenameout_28_10; //SR
    G4String filenameout_28_40; //SR
    G4String filenameout_28_60; //SR
    G4String filenameout_28_90; //SR


};
#endif


