// RunActionMessenger.hh
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger: public G4UImessenger
{
  public:
    RunActionMessenger(RunAction*);
   ~RunActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    G4String GetCurrentValue(G4UIcommand*);
    
  private:
    RunAction* 								myRunAction;
    
    G4UIdirectory*        		runDir;   

    G4UIcmdWithABool* 				partialCmd;
    G4UIcmdWithABool* 				refESECmd;
    G4UIcmdWithABool* 				refCompressionCmd;
    
    G4UIcmdWithADouble* 			seedCmd;
    G4UIcmdWithADouble* 			resumeCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
