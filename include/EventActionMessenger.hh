// EventActionMessenger.hh
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventActionMessenger_h
#define EventActionMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class EventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventActionMessenger: public G4UImessenger
{
  public:
    EventActionMessenger(EventAction*);
   ~EventActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    EventAction* 					eventAction;
    
    G4UIdirectory*        eventDir;   
    G4UIcmdWithAnInteger* printCmd;    
    G4UIcmdWithAnInteger* saveCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
