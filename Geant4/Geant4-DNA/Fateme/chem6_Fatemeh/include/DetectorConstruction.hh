#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction();

  G4VPhysicalVolume* Construct();
  void ConstructSDandField() override;
private:

  void               DefineMaterials();
  G4VPhysicalVolume* ConstructVolumes();
  
  G4LogicalVolume* logicTarget1;
   G4LogicalVolume* logicTarget2;
  G4VPhysicalVolume* pTarget1;
  G4VPhysicalVolume* pTarget2;
    
  G4LogicalVolume* lBp;
  G4VPhysicalVolume* pBP;
  
  G4LogicalVolume* lSugarPhosphate1;
  G4VPhysicalVolume* pSugar1;
  
  G4LogicalVolume* lSugarPhosphate2;
  G4VPhysicalVolume* pSugar2;
  

  G4Material*       fpDefaultMaterial;
  G4Material*       fpWaterMaterial;
 G4Material*        fptube_mat;
};
#endif

