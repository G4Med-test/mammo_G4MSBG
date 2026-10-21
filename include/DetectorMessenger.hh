// DetectorMessenger.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction*);
   ~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    G4String GetCurrentValue(G4UIcommand*);
    
  private:
    DetectorConstruction* 		 	detectorConst;
    
    G4UIdirectory*             	appDir;
    G4UIdirectory*             	detDir;
    G4UIdirectory*             	breastDir;
    G4UIdirectory*             	detector_deviceDir;

    G4UIcmdWithADoubleAndUnit*	airGapCmd;
    G4UIcmdWithADoubleAndUnit*	paddleThicknessCmd;
    G4UIcmdWithADoubleAndUnit*	breastPhantom_InferiorCmd;
    G4UIcmdWithADoubleAndUnit*	breastPhantom_SuperiorCmd;
    G4UIcmdWithADoubleAndUnit*	imagerXCmd;
    G4UIcmdWithADoubleAndUnit*	imagerYCmd;

		G4UIcmdWithABool*						supportCmd;
		G4UIcmdWithABool*						compressionCmd;
		G4UIcmdWithABool*						coverCmd;
 		G4UIcmdWithABool*						airCmd;
		G4UIcmdWithABool*						imagerKillCmd;

		G4UIcmdWithABool*						detector_deviceCmd;
    G4UIcmdWithoutParameter*		updateCmd;
		G4UIcmdWithoutParameter*		NOTypeCmd;
    G4UIcmdWithoutParameter*		TLDTypeCmd;
		G4UIcmdWithABool*						visualizeCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

