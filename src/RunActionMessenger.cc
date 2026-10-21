// RunActionMessenger.cc
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunActionMessenger.hh"

#include "RunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger(RunAction* aRunAction)
:myRunAction(aRunAction)
{ 
  runDir = new G4UIdirectory("/app/run/");
  runDir->SetGuidance("run control");

  partialCmd = new G4UIcmdWithABool("/app/run/keepPartial",this);
  partialCmd->SetGuidance("Flag to set whether to keep the partial results files or to delete them at the end of run");
  partialCmd->SetDefaultValue(false);
  partialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  refESECmd = new G4UIcmdWithABool("/app/run/useESEreference",this);
  refESECmd->SetGuidance("Flag to set whether to use Entrance Skin Exposure as exposure reference (point at top of breast)");
  refESECmd->SetDefaultValue(true);
  refESECmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  refCompressionCmd = new G4UIcmdWithABool("/app/run/useCompressionPlateReference",this);
  refCompressionCmd->SetGuidance("Flag to set whether to include the compression plate in the calculation of the exposure reference point");
  refCompressionCmd->SetDefaultValue(true);
  refCompressionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  seedCmd = new G4UIcmdWithADouble("/app/run/setSeed",this);
  seedCmd->SetGuidance("Set the random number seed from seeds.dat according to the task ID (passed as argument).");
  seedCmd->SetGuidance("If time is to be used as seed, use 0 as argument.");
  seedCmd->SetParameterName("Number",false);
  seedCmd->SetRange("Number>=0.");
  seedCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  resumeCmd = new G4UIcmdWithADouble("/app/run/resume",this);
  resumeCmd->SetGuidance("Resume interrupted run, specify total number of events (including those already run)");
  resumeCmd->SetParameterName("Number",false);
  resumeCmd->SetRange("Number>0.");
  resumeCmd->AvailableForStates(G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
	delete partialCmd;
	delete seedCmd;
	delete resumeCmd;
  delete runDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 

  if (command == partialCmd)
    {myRunAction->SetKeepPartial(partialCmd->GetNewBoolValue(newValue));}

 if (command == refESECmd)
    {myRunAction->SetReferenceESE(refESECmd->GetNewBoolValue(newValue));}

  if (command == refCompressionCmd)
    {myRunAction->SetReferenceCompression(refCompressionCmd->GetNewBoolValue(newValue));}

  if (command == seedCmd)
    {myRunAction->SetRandomSeed((G4long) seedCmd->GetNewDoubleValue(newValue));}

  if (command == resumeCmd)
    {myRunAction->ResumeRun(resumeCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4String RunActionMessenger::GetCurrentValue(G4UIcommand* command)
{ 
	G4String cv;

  if(command == partialCmd)
   {cv = partialCmd->ConvertToString(myRunAction->GetKeepPartial());}

  if(command == refESECmd)
   {cv = refESECmd->ConvertToString(myRunAction->GetReferenceESE());}

  if(command == refCompressionCmd)
   {cv = refCompressionCmd->ConvertToString(myRunAction->GetReferenceCompression());}

  return cv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
