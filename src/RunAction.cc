// RunAction.cc
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "Run.hh"
#include "EventAction.hh"
#include "PhysicsList.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"
#include "RunActionMessenger.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <time.h>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
using namespace std;
G4String dirName;
extern G4String PhysicsListName;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(DetectorConstruction* det, PrimaryGeneratorAction* gen)
:resumedFrom(0), taskID(0), keepPartial(false),referenceESE(true),referenceCompression(false),detectorConst(det), generatorAction(gen)
{
  runMessenger = new RunActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{
	return new Run(detectorConst);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{

  // To store the time
	time_t t_start; 
	t_start = time(NULL);
  time_start = t_start;

	// Set position and aim source
	generatorAction->PositionAndAim();

	FileNameNumbers();

  //create the output files directory
  dirName = "output_MC_G4";
  mkdir(dirName, 0777);


	if (resumedFrom) {

		// Resetting random engine status
		G4RunManager* runManager = G4RunManager::GetRunManager();
		runManager->RestoreRandomNumberStatus((G4String) ("Random" + fileNameNumbers + ".dat"));
		G4cout << "### Random engine reset:" << G4endl;
		CLHEP::HepRandom::showEngineStatus();

		// Loading partial results
	  ifstream fileSummary;
    ifstream fileDataOutput;

	 	fileSummary.open((G4String) (dirName+"/Summary_Partial" + fileNameNumbers + ".txt"));
	  fileDataOutput.open((G4String) (dirName+"/Data_output_Partial" + fileNameNumbers + ".txt"));

		if (fileSummary.is_open()){

			G4int runNum;
			fileSummary >> resumedFrom;
			fileSummary >> runNum;
			runManager->SetRunIDCounter(runNum);
			//Run* theRun = (Run*) aRun;

			fileSummary.close();
			G4cout << "### Partial results loaded." << G4endl;
		} else G4Exception("RunAction::BeginOfRunAction", "Error", FatalException, "Error loading partial results!");

	if (fileDataOutput.is_open()){

			G4int runNum;
			fileDataOutput >> resumedFrom;
			fileDataOutput >> runNum;
			runManager->SetRunIDCounter(runNum);
			//Run* theRun = (Run*) aRun;

			fileDataOutput.close();
			G4cout << "### Partial results loaded." << G4endl;
		} else G4Exception("RunAction::BeginOfRunAction", "Error", FatalException, "Error loading partial results!");

	}

	// Output start of run
	detectorConst->PrintGeometryParameters();
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{

  //To retrieve the end time
  time_t t_end; 
	t_end = time(NULL);
  time_end = t_end;

	SaveResults(true, aRun);
  G4cout << summary;
	resumedFrom = 0;
	remove((G4String) ("Random" + fileNameNumbers + ".dat"));

 const Run* theRun = (Run*) aRun;
 Kerma_Evaluation_total = theRun->GetTotKerma_Evaluation();
 Kerma_Evaluation_total=0.0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SaveResults(G4bool endOfRun, const G4Run* aRun)
{
  Run* theRun = (Run*) aRun;

		AnalyzeResults(theRun);

		G4String stageString = (endOfRun ? "Final":"Partial");

		ofstream fileSummary;
    ofstream fileDataOutput;

	 	fileSummary.open((G4String) (dirName+"/Summary_" + stageString + fileNameNumbers + ".txt"));
    fileDataOutput.open((G4String) (dirName+"/Data_output_" + stageString + fileNameNumbers + ".txt"));

		if (fileSummary.is_open()){
			fileSummary << summary;
		 	fileSummary.close();
		}

		if(!keepPartial && endOfRun) remove((G4String) (dirName+"/Summary_Partial" + fileNameNumbers + ".txt"));
	
		if (fileDataOutput.is_open()){
			fileDataOutput << results;
		 	fileDataOutput.close();
		}

		if(!keepPartial && endOfRun) remove((G4String) (dirName+"/Data_output_Partial" + fileNameNumbers + ".txt"));
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AnalyzeResults(Run* theRun)
{
	G4RunManager* runManager = G4RunManager::GetRunManager();

	ostringstream resultsIntro;
	ostringstream resultsDetectorDevice;
	
  ostringstream summaryIntro;
	ostringstream summaryDetectorDevice;
	ostringstream summaryEnd;

	G4double numPhotons = runManager->GetCurrentRun()->GetNumberOfEvent() + resumedFrom;
	G4double inEnergy = generatorAction->GetParticleGun()->GetParticleEnergy();

// Results and Summary Output

// adjustment for the reference point 
  if(referenceESE){
	summaryIntro << "Using Entrance Skin Exposure (ESE) as exposure reference point." << G4endl;
	} else {
	summaryIntro << "Using top of support plate as exposure reference point" << G4endl;
}

		// adjustment if compression plate is present in computation of reference exposure
		// compression plate IS NOT PRESENT IN THE COMPUTATION by default
		if(referenceCompression)
	summaryIntro << "Including breast compression plate for exposure reference." << G4endl;
			
	// adjustment if simulation does not include air in the world				
	// air is included in world by default
	if(detectorConst->GetAirInWorld())
		summaryIntro << "Including air for exposure reference." << G4endl;

  std::ostringstream detector_deviceName;
	switch(detectorConst->GetDetectorDevice() ){
		case 0:
			detector_deviceName << "NO"; break;
    case 1:
			detector_deviceName << "TLD";	break;
	}
 	
   std::ostringstream spectrum_useName;
	 spectrum_useName << inEnergy/keV << " keV";
		   

	// Results and Summary Output INTRO
		//G4double air_kerma =  generatorAction->GetReferenceAirKerma(numPhotons, referenceESE, referenceCompression);

  resultsIntro  << theRun->GetTotKerma_Evaluation()/(CLHEP::gray) << " 0 " << "0" << '\n';

	summaryIntro	<< "\n====================================================================" 
  					  	<< "\n### Run # " << runManager->GetCurrentRun()->GetRunID() 
								<< "\n### X-rays processed: " << numPhotons
                << "\n### Simulation Time: " << time_end - time_start << " sec"
								//<< "\n### Reference Air-Kerma (analytic): " << G4BestUnit(air_kerma,"Dose")  
								<< "\n### Reference Air-Kerma: " << G4BestUnit(theRun->GetTotKerma_Evaluation(),"Dose")                
								<< "\n### X-ray Energy: " << spectrum_useName.str()  
								<< "\n### X-ray Source Distance from Chestwall: " << generatorAction->GetDistFromCW()/cm << " cm" 
                << "\n### Detector Device: " << detector_deviceName.str();

//Detector Device Output
if (detectorConst->GetDetectorDeviceSensitivity()) {

if (detectorConst->GetDetectorDevice() == 1) {

G4double doseDepUncertainPercent_Tld_1 = 100.0 * theRun->GetDoseDepUncertainty_Tld_1()/ (theRun->GetDoseDep_Tld_1() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_2 = 100.0 * theRun->GetDoseDepUncertainty_Tld_2()/ (theRun->GetDoseDep_Tld_2() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_3 = 100.0 * theRun->GetDoseDepUncertainty_Tld_3()/ (theRun->GetDoseDep_Tld_3() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_4 = 100.0 * theRun->GetDoseDepUncertainty_Tld_4()/ (theRun->GetDoseDep_Tld_4() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_5 = 100.0 * theRun->GetDoseDepUncertainty_Tld_5()/ (theRun->GetDoseDep_Tld_5() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_6 = 100.0 * theRun->GetDoseDepUncertainty_Tld_6()/ (theRun->GetDoseDep_Tld_6() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_7 = 100.0 * theRun->GetDoseDepUncertainty_Tld_7()/ (theRun->GetDoseDep_Tld_7() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_8 = 100.0 * theRun->GetDoseDepUncertainty_Tld_8()/ (theRun->GetDoseDep_Tld_8() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_9 = 100.0 * theRun->GetDoseDepUncertainty_Tld_9()/ (theRun->GetDoseDep_Tld_9() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_10 = 100.0 * theRun->GetDoseDepUncertainty_Tld_10()/ (theRun->GetDoseDep_Tld_10() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_11 = 100.0 * theRun->GetDoseDepUncertainty_Tld_11()/ (theRun->GetDoseDep_Tld_11() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_12 = 100.0 * theRun->GetDoseDepUncertainty_Tld_12()/ (theRun->GetDoseDep_Tld_12() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_13 = 100.0 * theRun->GetDoseDepUncertainty_Tld_13()/ (theRun->GetDoseDep_Tld_13() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_14 = 100.0 * theRun->GetDoseDepUncertainty_Tld_14()/ (theRun->GetDoseDep_Tld_14() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_15 = 100.0 * theRun->GetDoseDepUncertainty_Tld_15()/ (theRun->GetDoseDep_Tld_15() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_16 = 100.0 * theRun->GetDoseDepUncertainty_Tld_16()/ (theRun->GetDoseDep_Tld_16() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_17 = 100.0 * theRun->GetDoseDepUncertainty_Tld_17()/ (theRun->GetDoseDep_Tld_17() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_18 = 100.0 * theRun->GetDoseDepUncertainty_Tld_18()/ (theRun->GetDoseDep_Tld_18() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_19 = 100.0 * theRun->GetDoseDepUncertainty_Tld_19()/ (theRun->GetDoseDep_Tld_19() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_20 = 100.0 * theRun->GetDoseDepUncertainty_Tld_20()/ (theRun->GetDoseDep_Tld_20() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_21 = 100.0 * theRun->GetDoseDepUncertainty_Tld_21()/ (theRun->GetDoseDep_Tld_21() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_22 = 100.0 * theRun->GetDoseDepUncertainty_Tld_22()/ (theRun->GetDoseDep_Tld_22() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_23 = 100.0 * theRun->GetDoseDepUncertainty_Tld_23()/ (theRun->GetDoseDep_Tld_23() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_24 = 100.0 * theRun->GetDoseDepUncertainty_Tld_24()/ (theRun->GetDoseDep_Tld_24() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_25 = 100.0 * theRun->GetDoseDepUncertainty_Tld_25()/ (theRun->GetDoseDep_Tld_25() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_26 = 100.0 * theRun->GetDoseDepUncertainty_Tld_26()/ (theRun->GetDoseDep_Tld_26() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_27 = 100.0 * theRun->GetDoseDepUncertainty_Tld_27()/ (theRun->GetDoseDep_Tld_27() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_28 = 100.0 * theRun->GetDoseDepUncertainty_Tld_28()/ (theRun->GetDoseDep_Tld_28() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_29 = 100.0 * theRun->GetDoseDepUncertainty_Tld_29()/ (theRun->GetDoseDep_Tld_29() / (G4double) numPhotons);
G4double doseDepUncertainPercent_Tld_30 = 100.0 * theRun->GetDoseDepUncertainty_Tld_30()/ (theRun->GetDoseDep_Tld_30() / (G4double) numPhotons);

resultsDetectorDevice << theRun->GetDoseDep_Tld_1()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_1()/gray << ' ' << doseDepUncertainPercent_Tld_1 << '\n'<< theRun->GetDoseDep_Tld_2()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_2()/gray << ' ' << doseDepUncertainPercent_Tld_2 << '\n'<< theRun->GetDoseDep_Tld_3()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_3()/gray << ' ' << doseDepUncertainPercent_Tld_3 << '\n'<< theRun->GetDoseDep_Tld_4()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_4()/gray << ' ' << doseDepUncertainPercent_Tld_4 << '\n'<< theRun->GetDoseDep_Tld_5()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_5()/gray << ' ' << doseDepUncertainPercent_Tld_5 << '\n'<< theRun->GetDoseDep_Tld_6()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_6()/gray << ' ' << doseDepUncertainPercent_Tld_6 << '\n'<< theRun->GetDoseDep_Tld_7()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_7()/gray << ' ' << doseDepUncertainPercent_Tld_7 << '\n'<< theRun->GetDoseDep_Tld_8()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_8()/gray << ' ' << doseDepUncertainPercent_Tld_8 << '\n'<< theRun->GetDoseDep_Tld_9()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_9()/gray << ' ' << doseDepUncertainPercent_Tld_9 << '\n'<< theRun->GetDoseDep_Tld_10()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_10()/gray << ' ' << doseDepUncertainPercent_Tld_10 << '\n'<< theRun->GetDoseDep_Tld_11()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_11()/gray << ' ' << doseDepUncertainPercent_Tld_11 << '\n'<< theRun->GetDoseDep_Tld_12()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_12()/gray << ' ' << doseDepUncertainPercent_Tld_12 << '\n' << theRun->GetDoseDep_Tld_13()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_13()/gray << ' ' << doseDepUncertainPercent_Tld_13 << '\n'<< theRun->GetDoseDep_Tld_14()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_14()/gray << ' ' << doseDepUncertainPercent_Tld_14 << '\n'<< theRun->GetDoseDep_Tld_15()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_15()/gray << ' ' << doseDepUncertainPercent_Tld_15 << '\n'<< theRun->GetDoseDep_Tld_16()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_16()/gray << ' ' << doseDepUncertainPercent_Tld_16 << '\n'<< theRun->GetDoseDep_Tld_17()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_17()/gray << ' ' << doseDepUncertainPercent_Tld_17 << '\n'<< theRun->GetDoseDep_Tld_18()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_18()/gray << ' ' << doseDepUncertainPercent_Tld_18 << '\n'<< theRun->GetDoseDep_Tld_19()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_19()/gray << ' ' << doseDepUncertainPercent_Tld_19 << '\n'<< theRun->GetDoseDep_Tld_20()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_20()/gray << ' ' << doseDepUncertainPercent_Tld_20 << '\n'<< theRun->GetDoseDep_Tld_21()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_21()/gray << ' ' << doseDepUncertainPercent_Tld_21 << '\n'<< theRun->GetDoseDep_Tld_22()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_22()/gray << ' ' << doseDepUncertainPercent_Tld_22 << '\n' << theRun->GetDoseDep_Tld_23()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_23()/gray << ' ' << doseDepUncertainPercent_Tld_23 << '\n'<< theRun->GetDoseDep_Tld_24()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_24()/gray << ' ' << doseDepUncertainPercent_Tld_24 << '\n'<< theRun->GetDoseDep_Tld_25()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_25()/gray << ' ' << doseDepUncertainPercent_Tld_25 << '\n'<< theRun->GetDoseDep_Tld_26()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_26()/gray << ' ' << doseDepUncertainPercent_Tld_26 << '\n'<< theRun->GetDoseDep_Tld_27()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_27()/gray << ' ' << doseDepUncertainPercent_Tld_27 << '\n'<< theRun->GetDoseDep_Tld_28()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_28()/gray << ' ' << doseDepUncertainPercent_Tld_28 << '\n'<< theRun->GetDoseDep_Tld_29()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_29()/gray << ' ' << doseDepUncertainPercent_Tld_29 << '\n'<< theRun->GetDoseDep_Tld_30()/gray  << ' '<< theRun->GetDoseDepUncertainty_Tld_30()/gray << ' ' << doseDepUncertainPercent_Tld_30 << '\n';

  summaryDetectorDevice << "\n----------------------- DETECTOR DEVICE ----------------------" 											<< "\n1) Dose Deposited in TLD 1: " << G4BestUnit(theRun->GetDoseDep_Tld_1(),"Dose") 
									<< "\n1) Uncertainty Dose Deposited in TLD 1: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_1(),"Dose") 									<< "\n1) Uncertainty Dose Deposited in TLD 1: " << doseDepUncertainPercent_Tld_1 << " %" << "\n2) Dose Deposited in TLD 2: " << G4BestUnit(theRun->GetDoseDep_Tld_2(),"Dose") 
									<< "\n2) Uncertainty Dose Deposited in TLD 2: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_2(),"Dose") 									<< "\n2) Uncertainty Dose Deposited in TLD 2: " << doseDepUncertainPercent_Tld_2 << " %" << "\n3) Dose Deposited in TLD 3: " << G4BestUnit(theRun->GetDoseDep_Tld_3(),"Dose") 
									<< "\n3) Uncertainty Dose Deposited in TLD 3: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_3(),"Dose") 									<< "\n3) Uncertainty Dose Deposited in TLD 3: " << doseDepUncertainPercent_Tld_3 << " %" << "\n4) Dose Deposited in TLD 4: " << G4BestUnit(theRun->GetDoseDep_Tld_4(),"Dose") 
									<< "\n4) Uncertainty Dose Deposited in TLD 4: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_4(),"Dose") 									<< "\n4) Uncertainty Dose Deposited in TLD 4: " << doseDepUncertainPercent_Tld_4 << " %" << "\n5) Dose Deposited in TLD 5: " << G4BestUnit(theRun->GetDoseDep_Tld_5(),"Dose") 
									<< "\n5) Uncertainty Dose Deposited in TLD 5: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_5(),"Dose") 									<< "\n5) Uncertainty Dose Deposited in TLD 5: " << doseDepUncertainPercent_Tld_5 << " %" << "\n6) Dose Deposited in TLD 6: " << G4BestUnit(theRun->GetDoseDep_Tld_6(),"Dose") 
									<< "\n6) Uncertainty Dose Deposited in TLD 6: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_6(),"Dose") 									<< "\n6) Uncertainty Dose Deposited in TLD 6: " << doseDepUncertainPercent_Tld_6 << " %" << "\n7) Dose Deposited in TLD 7: " << G4BestUnit(theRun->GetDoseDep_Tld_7(),"Dose") 
									<< "\n7) Uncertainty Dose Deposited in TLD 7: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_7(),"Dose") 									<< "\n7) Uncertainty Dose Deposited in TLD 7: " << doseDepUncertainPercent_Tld_7 << " %" << "\n8) Dose Deposited in TLD 8: " << G4BestUnit(theRun->GetDoseDep_Tld_8(),"Dose") 
									<< "\n8) Uncertainty Dose Deposited in TLD 8: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_8(),"Dose") 									<< "\n8) Uncertainty Dose Deposited in TLD 8: " << doseDepUncertainPercent_Tld_8 << " %" << "\n9) Dose Deposited in TLD 9: " << G4BestUnit(theRun->GetDoseDep_Tld_9(),"Dose") 
									<< "\n9) Uncertainty Dose Deposited in TLD 9: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_9(),"Dose") 									<< "\n9) Uncertainty Dose Deposited in TLD 9: " << doseDepUncertainPercent_Tld_9 << " %" << "\n10) Dose Deposited in TLD 10: " << G4BestUnit(theRun->GetDoseDep_Tld_10(),"Dose") 
									<< "\n10) Uncertainty Dose Deposited in TLD 10: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_10(),"Dose") 									<< "\n10) Uncertainty Dose Deposited in TLD 10: " << doseDepUncertainPercent_Tld_10 << " %" << "\n11) Dose Deposited in TLD 11: " << G4BestUnit(theRun->GetDoseDep_Tld_11(),"Dose") 
									<< "\n11) Uncertainty Dose Deposited in TLD 11: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_11(),"Dose") 									<< "\n11) Uncertainty Dose Deposited in TLD 11: " << doseDepUncertainPercent_Tld_11 << " %" << "\n12) Dose Deposited in TLD 12: " << G4BestUnit(theRun->GetDoseDep_Tld_12(),"Dose") 
									<< "\n12) Uncertainty Dose Deposited in TLD 12: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_12(),"Dose") 									<< "\n12) Uncertainty Dose Deposited in TLD 12: " << doseDepUncertainPercent_Tld_12 << " %"<< "\n13) Dose Deposited in TLD 13: " << G4BestUnit(theRun->GetDoseDep_Tld_13(),"Dose") 
									<< "\n13) Uncertainty Dose Deposited in TLD 13: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_13(),"Dose") 									<< "\n13) Uncertainty Dose Deposited in TLD 13: " << doseDepUncertainPercent_Tld_13 << " %"<< "\n14) Dose Deposited in TLD 14: " << G4BestUnit(theRun->GetDoseDep_Tld_14(),"Dose") 
									<< "\n14) Uncertainty Dose Deposited in TLD 14: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_14(),"Dose") 									<< "\n14) Uncertainty Dose Deposited in TLD 14: " << doseDepUncertainPercent_Tld_14 << " %"<< "\n15) Dose Deposited in TLD 15: " << G4BestUnit(theRun->GetDoseDep_Tld_15(),"Dose") 
									<< "\n15) Uncertainty Dose Deposited in TLD 15: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_15(),"Dose") 									<< "\n15) Uncertainty Dose Deposited in TLD 15: " << doseDepUncertainPercent_Tld_15 << " %"<< "\n16) Dose Deposited in TLD 16: " << G4BestUnit(theRun->GetDoseDep_Tld_16(),"Dose") 
									<< "\n16) Uncertainty Dose Deposited in TLD 16: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_16(),"Dose") 									<< "\n16) Uncertainty Dose Deposited in TLD 16: " << doseDepUncertainPercent_Tld_16 << " %"<< "\n17) Dose Deposited in TLD 17: " << G4BestUnit(theRun->GetDoseDep_Tld_17(),"Dose") 
									<< "\n17) Uncertainty Dose Deposited in TLD 17: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_17(),"Dose") 									<< "\n17) Uncertainty Dose Deposited in TLD 17: " << doseDepUncertainPercent_Tld_17 << " %"<< "\n18) Dose Deposited in TLD 18: " << G4BestUnit(theRun->GetDoseDep_Tld_18(),"Dose") 
									<< "\n18) Uncertainty Dose Deposited in TLD 18: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_18(),"Dose") 									<< "\n18) Uncertainty Dose Deposited in TLD 18: " << doseDepUncertainPercent_Tld_18 << " %"<< "\n19) Dose Deposited in TLD 19: " << G4BestUnit(theRun->GetDoseDep_Tld_19(),"Dose") 
									<< "\n19) Uncertainty Dose Deposited in TLD 19: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_19(),"Dose") 									<< "\n19) Uncertainty Dose Deposited in TLD 19: " << doseDepUncertainPercent_Tld_19 << " %"<< "\n20) Dose Deposited in TLD 20: " << G4BestUnit(theRun->GetDoseDep_Tld_20(),"Dose") 
									<< "\n20) Uncertainty Dose Deposited in TLD 20: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_20(),"Dose") 									<< "\n20) Uncertainty Dose Deposited in TLD 20: " << doseDepUncertainPercent_Tld_20 << " %"<< "\n21) Dose Deposited in TLD 21: " << G4BestUnit(theRun->GetDoseDep_Tld_21(),"Dose") 
									<< "\n21) Uncertainty Dose Deposited in TLD 21: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_21(),"Dose") 									<< "\n21) Uncertainty Dose Deposited in TLD 21: " << doseDepUncertainPercent_Tld_21 << " %" << "\n22) Dose Deposited in TLD 22: " << G4BestUnit(theRun->GetDoseDep_Tld_22(),"Dose") 
									<< "\n22) Uncertainty Dose Deposited in TLD 22: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_22(),"Dose") 									<< "\n22) Uncertainty Dose Deposited in TLD 22: " << doseDepUncertainPercent_Tld_22 << " %"<< "\n23) Dose Deposited in TLD 23: " << G4BestUnit(theRun->GetDoseDep_Tld_23(),"Dose") 
									<< "\n23) Uncertainty Dose Deposited in TLD 23: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_23(),"Dose") 									<< "\n23) Uncertainty Dose Deposited in TLD 23: " << doseDepUncertainPercent_Tld_23 << " %"<< "\n24) Dose Deposited in TLD 24: " << G4BestUnit(theRun->GetDoseDep_Tld_24(),"Dose") 
									<< "\n24) Uncertainty Dose Deposited in TLD 24: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_24(),"Dose") 									<< "\n24) Uncertainty Dose Deposited in TLD 24: " << doseDepUncertainPercent_Tld_24 << " %"<< "\n25) Dose Deposited in TLD 25: " << G4BestUnit(theRun->GetDoseDep_Tld_25(),"Dose") 
									<< "\n25) Uncertainty Dose Deposited in TLD 25: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_25(),"Dose") 									<< "\n25) Uncertainty Dose Deposited in TLD 25: " << doseDepUncertainPercent_Tld_25 << " %"<< "\n26) Dose Deposited in TLD 26: " << G4BestUnit(theRun->GetDoseDep_Tld_26(),"Dose") 
									<< "\n26) Uncertainty Dose Deposited in TLD 26: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_26(),"Dose") 									<< "\n26) Uncertainty Dose Deposited in TLD 26: " << doseDepUncertainPercent_Tld_26 << " %"<< "\n27) Dose Deposited in TLD 27: " << G4BestUnit(theRun->GetDoseDep_Tld_27(),"Dose") 
									<< "\n27) Uncertainty Dose Deposited in TLD 27: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_27(),"Dose") 									<< "\n27) Uncertainty Dose Deposited in TLD 27: " << doseDepUncertainPercent_Tld_27 << " %"<< "\n28) Dose Deposited in TLD 28: " << G4BestUnit(theRun->GetDoseDep_Tld_28(),"Dose") 
									<< "\n28) Uncertainty Dose Deposited in TLD 28: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_28(),"Dose") 									<< "\n28) Uncertainty Dose Deposited in TLD 28: " << doseDepUncertainPercent_Tld_18 << " %"<< "\n29) Dose Deposited in TLD 29: " << G4BestUnit(theRun->GetDoseDep_Tld_29(),"Dose") 
									<< "\n29) Uncertainty Dose Deposited in TLD 29: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_29(),"Dose") 									<< "\n29) Uncertainty Dose Deposited in TLD 29: " << doseDepUncertainPercent_Tld_29 << " %"<< "\n30) Dose Deposited in TLD 30: " << G4BestUnit(theRun->GetDoseDep_Tld_30(),"Dose") 
									<< "\n30) Uncertainty Dose Deposited in TLD 30: " << G4BestUnit(theRun->GetDoseDepUncertainty_Tld_30(),"Dose") 									<< "\n30) Uncertainty Dose Deposited in TLD 30: " << doseDepUncertainPercent_Tld_30 << " %"; 
}


}

  summaryEnd	<< "\n====================================================================\n";

	results = resultsDetectorDevice.str() + resultsIntro.str();
	summary = summaryIntro.str() + summaryDetectorDevice.str() + summaryEnd.str();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::ResumeRun(G4double totalEventsInRun)
{
	FileNameNumbers();

  ifstream fileRandom;
  ifstream fileSummary;
  ifstream fileDataOutput;

	// First check if the run had finalized. If so, then do not run

	G4String command = "/run/beamOn ";
	ostringstream num;
	num << ((long) totalEventsInRun - resumedFrom);
	G4String numberOfEvents = num.str();

  G4UImanager *UI = G4UImanager::GetUIpointer();  
	UI->ApplyCommand(command+numberOfEvents);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::SetRandomSeed(G4long seedIndex)
{
	// Sets the random seeds
	// Uses seedIndex (usually from SGE_TASK_ID) to look up two random numbers in
	// the seeds_55k.dat file of random numbers obtained from random.org
	// if seedIndex is 0 then uses system time
		
	G4cout << "Setting random seeds from seeds.dat file" << G4endl;

	if (seedIndex == 0)
		seedIndex = time(NULL);

  taskID = seedIndex;
	
  seedIndex = seedIndex%55000;
  ifstream fileSeed;
 	fileSeed.open("seeds_55k.dat");
	G4long seeds[2];

	if (fileSeed.is_open()){
		for (G4int i=0; i<seedIndex; i++){
			fileSeed >> seeds[0];
			fileSeed >> seeds[1];
		}
 		fileSeed.close();
	} else G4Exception("RunAction::SetRandomSeed", "Error", FatalException, "File seeds_55k.dat not found!");
			
	CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
	CLHEP::HepRandom::setTheSeeds(seeds);
	CLHEP::HepRandom::showEngineStatus();

	G4cout << "Random seeds set, results will include the SGE Task ID." << G4endl;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::FileNameNumbers()
{
	// Create string for file names
	ostringstream fileNameCode;
	ostringstream taskIDString;
	ostringstream energyString;

	if (taskID)
			taskIDString << "_" << taskID;	

	energyString << generatorAction->GetParticleGun()->GetParticleEnergy()/keV;

	if (detectorConst->GetBreastPhantom_Superior()/cm != 0.0000001){

  fileNameCode	<< taskIDString.str()
								<< "_DepthOf_" << detectorConst->GetBreastPhantom_Superior()/cm 
								<< "cm_" << energyString.str()
								<< "keV_" << PhysicsListName;
	} else {

	fileNameCode	<< taskIDString.str()
								<< "_DepthOf_0cm_" << energyString.str()
								<< "keV_" << PhysicsListName;
  }

  fileNameNumbers = fileNameCode.str();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
