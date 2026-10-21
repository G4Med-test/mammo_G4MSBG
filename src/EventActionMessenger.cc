// EventActionMessenger.cc
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventActionMessenger.hh"
#include "EventAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventActionMessenger::EventActionMessenger(EventAction* EvAct)
:eventAction(EvAct)
{ 
  eventDir = new G4UIdirectory("/app/event/");
  eventDir ->SetGuidance("Event control.");
      
  printCmd = new G4UIcmdWithAnInteger("/app/event/printModulo",this);
  printCmd->SetGuidance("Print events modulo n.");
  printCmd->SetParameterName("EventNb",false);
  printCmd->SetRange("EventNb>0");
  printCmd->AvailableForStates(G4State_Idle);      

  saveCmd = new G4UIcmdWithAnInteger("/app/event/saveModulo",this);
  saveCmd->SetGuidance("Save events modulo n.");
  saveCmd->SetParameterName("EventNb",false);
  saveCmd->SetRange("EventNb>0");
  saveCmd->AvailableForStates(G4State_Idle);      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventActionMessenger::~EventActionMessenger()
{
  delete printCmd;
	delete saveCmd;
  delete eventDir;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if (command == printCmd)
    {eventAction->SetPrintModulo(printCmd->GetNewIntValue(newValue));}

  if (command == saveCmd)
    {eventAction->SetSaveModulo(saveCmd->GetNewIntValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
