
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger
                                                  (PrimaryGeneratorAction* Gun)
:G4UImessenger(),
 fAction(Gun),
 fPrimeDir(0), 
 fSelectActionCmd(0)
{
  fPrimeDir = new G4UIdirectory("/gunExample/");
  fPrimeDir->SetGuidance("this example");

fSelectActionCmd = new G4UIcmdWithAnInteger("/gunExample/selectGunAction",this);
  fSelectActionCmd->SetGuidance("Select primary generator action");
  fSelectActionCmd->SetParameterName("id",false);
  fSelectActionCmd->SetRange("id>=0 && id<2");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete fSelectActionCmd;
  delete fPrimeDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{ 
  if (command == fSelectActionCmd)
    fAction->SelectAction(fSelectActionCmd->GetNewIntValue(newValue));      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

