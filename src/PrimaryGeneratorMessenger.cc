// PrimaryGeneratorMessenger.cc
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* myPGA)
:myGenerator(myPGA)
{ 
  tubeDir = new G4UIdirectory("/app/tube/");
  tubeDir->SetGuidance("x-ray tube control.");
  
  gunNumCmd = new G4UIcmdWithAnInteger("/app/tube/setGunNumber",this);
  gunNumCmd->SetGuidance("Set number of particles to shoot per event.");
  gunNumCmd->SetParameterName("Number",false);
  gunNumCmd->SetRange("Number>0");
  gunNumCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SIDCmd = new G4UIcmdWithADoubleAndUnit("/app/tube/setSID",this);
  SIDCmd->SetGuidance("Set SID.");
  SIDCmd->SetParameterName("Size",false);
  SIDCmd->SetRange("Size>=0.");
  SIDCmd->SetUnitCategory("Length");
  SIDCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  DCORCmd = new G4UIcmdWithADoubleAndUnit("/app/tube/setDetToCOR",this);
  DCORCmd->SetGuidance("Set detector to center of rotation distance.");
  DCORCmd->SetParameterName("Size",false);
  DCORCmd->SetRange("Size>=0.");
  DCORCmd->SetUnitCategory("Length");
  DCORCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  distFromCWCmd = new G4UIcmdWithADoubleAndUnit("/app/tube/setDistFromCW",this);
  distFromCWCmd->SetGuidance("Set distance from chest wall to x-ray source.");
  distFromCWCmd->SetParameterName("Size",false);
  distFromCWCmd->SetRange("Size>=0.");
  distFromCWCmd->SetUnitCategory("Length");
  distFromCWCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{

	delete distFromCWCmd;
	delete SIDCmd;
	delete DCORCmd;
	delete gunNumCmd;
  delete tubeDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == gunNumCmd )
   { myGenerator->SetGunNumber(gunNumCmd->GetNewIntValue(newValue));}

  if( command == SIDCmd )
   { myGenerator->SetSID(SIDCmd->GetNewDoubleValue(newValue));}

  if( command == DCORCmd )
   { myGenerator->SetDCOR(DCORCmd->GetNewDoubleValue(newValue));}

	if( command == distFromCWCmd )
   { myGenerator->SetDistFromCW(distFromCWCmd->GetNewDoubleValue(newValue));}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand* command)
{
	G4String cv;

  if( command == gunNumCmd )
   { cv = gunNumCmd->ConvertToString(myGenerator->GetGunNumber());}

  if( command == SIDCmd )
   { cv = SIDCmd->ConvertToString(myGenerator->GetSID());}

   if( command == DCORCmd )
   { cv = DCORCmd->ConvertToString(myGenerator->GetDCOR());}

	if( command == distFromCWCmd )
   { cv = distFromCWCmd->ConvertToString(myGenerator->GetDistFromCW());}

  return cv; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
