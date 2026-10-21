// RunAction.hh
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1


#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class Run;
class DetectorConstruction;
class PrimaryGeneratorAction;
class RunActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*);
   ~RunAction();

    void			BeginOfRunAction(const G4Run*);
    void			EndOfRunAction(const G4Run*);
    void			SaveResults(G4bool, const G4Run*);
    G4String		        GetFileNameNumbers()						{return fileNameNumbers;}

		void 										ResumeRun(G4double);

		void 										SetRandomSeed(G4long seedIndex);
		//void 										SetSaveBinary()													{saveFormat = BIN;}
		//void 										SetSaveText()														{saveFormat = TXT;}
		//void 										SetSaveTiff()														{saveFormat = TIF;}
		void 		                SetKeepPartial(G4bool val)           {keepPartial = val;}
    void 				SetReferenceESE(G4bool val)	         {referenceESE = val;}
		void 		    SetReferenceCompression(G4bool val)	 {referenceCompression = val;}

		G4bool 									GetRayTracing()													{return rayTracing;}
		G4bool 									GetKeepPartial()												{return keepPartial;}
		G4bool 		GetReferenceESE()					{return referenceESE;}
		G4bool 		GetReferenceCompression()	{return referenceCompression;}


  private:
    G4Run* 									GenerateRun();

		void										AnalyzeResults(Run*);
		void										FileNameNumbers();

    G4double								PhiOverX(G4double);
		G4double								AirLinearAttCoeff(G4double);
		G4double								PolycarbonateLinearAttCoeff(G4double);
		
		G4String								fileNameNumbers;
		G4String								results;
		G4String								summary;

		//fileType								saveFormat;
		
		G4long									resumedFrom;					//number of photons in previous partial run, if any
		G4long									taskID;

		G4bool									keepPartial;
		G4bool								  referenceESE;		
		G4bool						      referenceCompression;
		G4bool									rayTracing;
		

    RunActionMessenger* 		runMessenger;
		DetectorConstruction* 	detectorConst; 
		PrimaryGeneratorAction* generatorAction;

    G4int time_start;
		G4int time_end;

		G4double Kerma_Evaluation_total;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
