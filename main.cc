// mammo_G4MSBG.cc
// Simulation of breast imaging, planar, tomosynthesis and ct
// using a voxelized breast volume
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {

  // Run manager
  G4RunManager * runManager = new G4RunManager;

	G4String materialFilename;
	G4String volumeFilename;
	
  // UserInitialization classes (mandatory)
  DetectorConstruction* detectorConst = new DetectorConstruction();
  runManager->SetUserInitialization(detectorConst);
  runManager->SetUserInitialization(new PhysicsList);
  
#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
   
  // UserAction classes
  PrimaryGeneratorAction* generatorAction = new PrimaryGeneratorAction(detectorConst);
	RunAction* runAction = new RunAction(detectorConst, generatorAction);
	EventAction* eventAction = new EventAction(runAction);
  runManager->SetUserAction(generatorAction);
  runManager->SetUserAction(runAction);  
  runManager->SetUserAction(eventAction);

  //Initialize G4 kernel
//  runManager->Initialize();
      
  //get the pointer to the User Interface manager 
  G4UImanager * UI = G4UImanager::GetUIpointer();  

  if(argc==1)
  // Define (G)UI terminal for interactive mode  
  { 
    // G4UIterminal is a (dumb) terminal.
    G4UIsession * session = 0;
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif    

    session->SessionStart();
    delete session;
  }
  else
  // Batch mode
  { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

