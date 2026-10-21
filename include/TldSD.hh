// TldSD.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef TldSD_h
#define TldSD_h 1

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"

class G4Step;
class G4HCofThisEvent;
class G4RunManager;
class G4Material;
class DetectorConstruction;
class EventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TldSD : public G4VSensitiveDetector
{
  public:
      TldSD(G4String, DetectorConstruction*, EventAction*);
     ~TldSD();

      G4bool 									ProcessHits(G4Step*, G4TouchableHistory*);
			void 										Initialize(G4HCofThisEvent*);
			void 										EndOfEvent(G4HCofThisEvent*);

	virtual G4double ComputeVolume(G4Step*);

	public:
			DetectorConstruction* 	detectorConst; 
			EventAction*						eventAction;
			G4RunManager* 					runManager;
		
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

