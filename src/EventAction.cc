// EventAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "EventAction.hh"
#include "Run.hh"
#include "RunAction.hh"
#include "EventActionMessenger.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
EventAction::EventAction(RunAction *run)
:printModulo(100000000), saveModulo(100000000), runAction(run)
{
  eventMessenger = new EventActionMessenger(this);
	runManager = G4RunManager::GetRunManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
EventAction::~EventAction()
{

  delete eventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void EventAction::BeginOfEventAction(const G4Event*)
{
  doseDep_Tld_1  = 0.0;
	doseDep_Tld_2  = 0.0;
	doseDep_Tld_3  = 0.0;
	doseDep_Tld_4  = 0.0;
	doseDep_Tld_5  = 0.0;
	doseDep_Tld_6  = 0.0;
	doseDep_Tld_7  = 0.0;
	doseDep_Tld_8  = 0.0;
	doseDep_Tld_9  = 0.0;
	doseDep_Tld_10 = 0.0;
	doseDep_Tld_11 = 0.0;
	doseDep_Tld_12 = 0.0;
	doseDep_Tld_13 = 0.0;
	doseDep_Tld_14 = 0.0;
	doseDep_Tld_15 = 0.0;
	doseDep_Tld_16 = 0.0;
	doseDep_Tld_17 = 0.0;
	doseDep_Tld_18 = 0.0;
  doseDep_Tld_19 = 0.0;
  doseDep_Tld_20 = 0.0;
  doseDep_Tld_21 = 0.0;
	doseDep_Tld_22 = 0.0;
	doseDep_Tld_23 = 0.0;
	doseDep_Tld_24 = 0.0;
	doseDep_Tld_25 = 0.0;
	doseDep_Tld_26 = 0.0;
	doseDep_Tld_27 = 0.0;
	doseDep_Tld_28 = 0.0;
	doseDep_Tld_29 = 0.0;
	doseDep_Tld_30 = 0.0;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void EventAction::EndOfEventAction(const G4Event* evt)
{
  G4int event_id = evt->GetEventID();
	Run* theRun = (Run*)runManager->GetCurrentRun();

  theRun->AddDoseDep_Tld_1(doseDep_Tld_1);
	theRun->AddDoseDep_Tld_2(doseDep_Tld_2);
	theRun->AddDoseDep_Tld_3(doseDep_Tld_3);
	theRun->AddDoseDep_Tld_4(doseDep_Tld_4);
	theRun->AddDoseDep_Tld_5(doseDep_Tld_5);
	theRun->AddDoseDep_Tld_6(doseDep_Tld_6);
	theRun->AddDoseDep_Tld_7(doseDep_Tld_7);
	theRun->AddDoseDep_Tld_8(doseDep_Tld_8);
	theRun->AddDoseDep_Tld_9(doseDep_Tld_9);
	theRun->AddDoseDep_Tld_10(doseDep_Tld_10);	
	theRun->AddDoseDep_Tld_11(doseDep_Tld_11);
	theRun->AddDoseDep_Tld_12(doseDep_Tld_12);
	theRun->AddDoseDep_Tld_13(doseDep_Tld_13);
	theRun->AddDoseDep_Tld_14(doseDep_Tld_14);
	theRun->AddDoseDep_Tld_15(doseDep_Tld_15);
	theRun->AddDoseDep_Tld_16(doseDep_Tld_16);
	theRun->AddDoseDep_Tld_17(doseDep_Tld_17);
	theRun->AddDoseDep_Tld_18(doseDep_Tld_18);
  theRun->AddDoseDep_Tld_19(doseDep_Tld_19);
  theRun->AddDoseDep_Tld_20(doseDep_Tld_20);
  theRun->AddDoseDep_Tld_21(doseDep_Tld_21);
	theRun->AddDoseDep_Tld_22(doseDep_Tld_22);
	theRun->AddDoseDep_Tld_23(doseDep_Tld_23);
	theRun->AddDoseDep_Tld_24(doseDep_Tld_24);
	theRun->AddDoseDep_Tld_25(doseDep_Tld_25);
	theRun->AddDoseDep_Tld_26(doseDep_Tld_26);
	theRun->AddDoseDep_Tld_27(doseDep_Tld_27);
	theRun->AddDoseDep_Tld_28(doseDep_Tld_28);
	theRun->AddDoseDep_Tld_29(doseDep_Tld_29);
	theRun->AddDoseDep_Tld_30(doseDep_Tld_30);

  // periodic printing and saving
  //
  if (event_id%printModulo == 0 && event_id) {
    G4cout << ">>> Event " << event_id << G4endl;

		if (event_id%saveModulo == 0) {
			runAction->SaveResults(false, runManager->GetCurrentRun());
			CLHEP::HepRandom::saveEngineStatus((G4String) ("Random" + runAction->GetFileNameNumbers() + ".dat"));
		}
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
