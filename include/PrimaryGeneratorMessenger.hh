// PrimaryGeneratorMessenger.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
   ~PrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    G4String GetCurrentValue(G4UIcommand*);
    
  private:
    PrimaryGeneratorAction*		myGenerator;
    
    G4UIdirectory*							tubeDir;

	
		G4UIcmdWithAnInteger*			  gunNumCmd;
    G4UIcmdWithADoubleAndUnit*	SIDCmd;
    G4UIcmdWithADoubleAndUnit*	DCORCmd;
    G4UIcmdWithADoubleAndUnit*	distFromCWCmd;
 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

