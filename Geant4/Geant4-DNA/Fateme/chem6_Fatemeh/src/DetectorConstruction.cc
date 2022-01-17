#include "DetectorConstruction.hh"
#include "G4Orb.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

#include "ScoreSpecies.hh"
#include "PrimaryKiller.hh"
#include "ScoreLET.hh"

DetectorConstruction::DetectorConstruction():
fpDefaultMaterial(0),fpWaterMaterial(0),fptube_mat(0)
{
}



DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    

  DefineMaterials();
  return ConstructVolumes();
}

void DetectorConstruction::DefineMaterials()
{ 
 
  G4NistManager* nistManager = G4NistManager::Instance();

  nistManager->FindOrBuildMaterial("G4_WATER", false);
  fpWaterMaterial = G4Material::GetMaterial("G4_WATER");

  nistManager->FindOrBuildMaterial("G4_Galactic", false);
  fpDefaultMaterial = G4Material::GetMaterial("G4_Galactic");
  
  G4double atomicNumber = 8.;
    G4double massOfMole = 32*g/mole;
    G4double density = 0.284*g/cm3;
    G4double temperature = 293*kelvin;
    G4double pressure = 21331*pascal;
    fptube_mat= new G4Material("O2", atomicNumber,
    massOfMole, density, kStateGas,
    temperature, pressure); 

 }



G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // World
  G4double worldSize  = 1 * km;

  G4Box* solidWorld = new G4Box("World",
                                worldSize/2.,
                                worldSize/2.,
                                worldSize/2.);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,
                                                    fpDefaultMaterial,
                                                    "World");
  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0,
                                                    G4ThreeVector(),
                                                    "World",
                                                    logicWorld,
                                                    0,
                                                    false,
                                                    0);
  
  // Box1
  
  G4Box* solidTarget1 = new G4Box("Box1", 5.2*um, 5.2*um, 5.2*um);
  logicTarget1 = new G4LogicalVolume(solidTarget1,  fptube_mat, "Box1");
  pTarget1 =new G4PVPlacement(0,
							G4ThreeVector(),
							"Box1",
							logicTarget1,
							physiWorld,
							false,
							0);

//  Box2
  
  G4Box* solidTarget2 = new G4Box("Box", 5*um, 5*um, 5*um);
  logicTarget2 = new G4LogicalVolume(solidTarget2, fpWaterMaterial, "Box");
  pTarget2 =new G4PVPlacement(0,
							G4ThreeVector(),
							"Box",
							logicTarget2,
							physiWorld,
							false,
							0);
                    
                    
                    
   //**************************************************************************
    //                 Subcomponent 1: Base pair
    //**************************************************************************
   /* G4int fNumberOfBasePairs = 200; 
    G4double HL = (0.34 * nm * fNumberOfBasePairs)/2;
    
    //Base pair - a cylinder of radius 0.5 nm and length 0.34 nm.
   
    G4Tubs* gBp = new G4Tubs("BasePair", 0, 0.5*nm, 0.17*nm, 0.0*deg, 360.0*deg);
    lBp = new G4LogicalVolume(gBp, fpWaterMaterial, "LBasePair");*/
    
    //**************************************************************************
    //                 Subcomponent 2: Sugar phosphate backbone
    //**************************************************************************
    
    //Phosphodiester group - two sugars each consisting of quarter cylinders
    //The sugars are wrapped around the base pair

   /* G4Tubs* gSugarPhosphate1 = new G4Tubs("Sugar1", 0.5*nm, 1.185*nm, 0.17*nm, 0*deg, 45*deg);
    lSugarPhosphate1 = new G4LogicalVolume(gSugarPhosphate1, fpWaterMaterial, "LSugar1");
    
    G4Tubs* gSugarPhosphate2 = new G4Tubs("Sugar2", 0.5*nm, 1.185*nm, 0.17*nm, 180*deg, 45*deg);
    lSugarPhosphate2 = new G4LogicalVolume(gSugarPhosphate2, fpWaterMaterial,"LSugar2");
    
    // Rotation of strands around the base pair
    
    G4double x = 0.0;
    G4double y = 0.0;
    G4double z0 = -HL + 0.17*nm; 
    
    for (G4int j = 0; j < fNumberOfBasePairs ; j++)
    {
        
        G4double theta = 36*deg*j;
        G4double z = z0 + j*0.34*nm;
        
        G4RotationMatrix* rot = new G4RotationMatrix();
        rot ->rotateZ(theta);
        
        pBP = new G4PVPlacement(rot, G4ThreeVector(x, y, z), lBp, "pBasePair", logicTarget2,false,j,false);
        pSugar1 = new G4PVPlacement(rot, G4ThreeVector(x, y, z), lSugarPhosphate1, "pSugar1",logicTarget2,false,j,false);
        pSugar2 = new G4PVPlacement(rot, G4ThreeVector(x, y, z), lSugarPhosphate2, "pSugar2",logicTarget2,false,j,false);
        
    }*/
    
    


  // Visualization attributes
  G4VisAttributes* worldVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  worldVisAtt->SetVisibility(true);
  logicWorld->SetVisAttributes(worldVisAtt);

  G4VisAttributes* target1VisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  target1VisAtt->SetVisibility(true);
  target1VisAtt->SetForceAuxEdgeVisible(true);
  logicTarget1->SetVisAttributes(target1VisAtt);

G4VisAttributes* target2VisAtt = new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  target2VisAtt->SetVisibility(true);
  target2VisAtt->SetForceAuxEdgeVisible(true);
  logicTarget2->SetVisAttributes(target2VisAtt);

  /*G4VisAttributes* VisAtt1 = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  VisAtt1->SetVisibility(true);
  VisAtt1->SetForceSolid(true);
  lBp->SetVisAttributes(VisAtt1);
  
  G4VisAttributes* VisAtt2 = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  VisAtt2->SetVisibility(true);
  VisAtt2->SetForceSolid(true);
  lSugarPhosphate1->SetVisAttributes(VisAtt2); 
  
  G4VisAttributes* VisAtt3 = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  VisAtt3->SetVisibility(true);
  VisAtt3->SetForceSolid(true);
  lSugarPhosphate2->SetVisAttributes(VisAtt3);*/
    
  //always return the world volume
  return physiWorld;
}
  ///////////////////////////////////SD/////////////////////////////////////////

void DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

 G4MultiFunctionalDetector* mfDetector =
  new G4MultiFunctionalDetector("mfDetector");

// Kill primary track after a chosen energy loss OR under a chosen
  // kinetic energy

  PrimaryKiller* primaryKiller = new PrimaryKiller("PrimaryKiller");
  primaryKiller->SetMinLossEnergyLimit(500*eV); // default value
  primaryKiller->SetMaxLossEnergyLimit(1*eV); // default value
  mfDetector->RegisterPrimitive(primaryKiller);

// LET scorer
  //  - scores restricted or unrestricted LET

  ScoreLET* LET = new ScoreLET("LET");
  mfDetector->RegisterPrimitive(LET);
  
  
// Record Species scorer:
  //  - scores number of species over time
  //  - score the total energy deposition
  //  - compute the radiochemical yields (G values)

  G4VPrimitiveScorer* primitivSpecies = new ScoreSpecies("Species");
  mfDetector->RegisterPrimitive(primitivSpecies);

  G4SDManager::GetSDMpointer()->AddNewDetector(mfDetector); 
  SetSensitiveDetector("Box", mfDetector);
}

