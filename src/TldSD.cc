// TldSD.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TldSD.hh"

#include "Run.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeometryManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VSolid.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TldSD::TldSD(G4String name, DetectorConstruction* det, EventAction* evt)
:G4VSensitiveDetector(name), detectorConst(det), eventAction(evt)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TldSD::~TldSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TldSD::Initialize(G4HCofThisEvent*)
{
	//EvtMap = new G4THitsMap<G4Double>(GetSensitiveDetector()->GetName(),GetName());
	runManager = G4RunManager::GetRunManager();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TldSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return true;

	G4String vol_pre = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
	G4double density = aStep->GetTrack()->GetStep()->GetPreStepPoint()->GetMaterial()->GetDensity();
	G4double volume = ComputeVolume (aStep);
        G4double dose = edep / (density*volume);


	G4int cfr_1 = strcmp(vol_pre, "physiTld_1");
	G4int cfr_2 = strcmp(vol_pre, "physiTld_2");
	G4int cfr_3 = strcmp(vol_pre, "physiTld_3");
	G4int cfr_4 = strcmp(vol_pre, "physiTld_4");
	G4int cfr_5 = strcmp(vol_pre, "physiTld_5");
	G4int cfr_6 = strcmp(vol_pre, "physiTld_6");
	G4int cfr_7 = strcmp(vol_pre, "physiTld_7");
	G4int cfr_8 = strcmp(vol_pre, "physiTld_8");
	G4int cfr_9 = strcmp(vol_pre, "physiTld_9");
	G4int cfr_10 = strcmp(vol_pre, "physiTld_10");
	G4int cfr_11 = strcmp(vol_pre, "physiTld_11");
	G4int cfr_12 = strcmp(vol_pre, "physiTld_12");
	G4int cfr_13 = strcmp(vol_pre, "physiTld_13");
	G4int cfr_14 = strcmp(vol_pre, "physiTld_14");
	G4int cfr_15 = strcmp(vol_pre, "physiTld_15");
	G4int cfr_16 = strcmp(vol_pre, "physiTld_16");
	G4int cfr_17 = strcmp(vol_pre, "physiTld_17");
        G4int cfr_18 = strcmp(vol_pre, "physiTld_18");
        G4int cfr_19 = strcmp(vol_pre, "physiTld_19");
        G4int cfr_20 = strcmp(vol_pre, "physiTld_20");
        G4int cfr_21 = strcmp(vol_pre, "physiTld_21");
	G4int cfr_22 = strcmp(vol_pre, "physiTld_22");
	G4int cfr_23 = strcmp(vol_pre, "physiTld_23");
	G4int cfr_24 = strcmp(vol_pre, "physiTld_24");
	G4int cfr_25 = strcmp(vol_pre, "physiTld_25");
	G4int cfr_26 = strcmp(vol_pre, "physiTld_26");
	G4int cfr_27 = strcmp(vol_pre, "physiTld_27");
	G4int cfr_28 = strcmp(vol_pre, "physiTld_28");
	G4int cfr_29 = strcmp(vol_pre, "physiTld_29");
	

  if ( cfr_1==0){eventAction->AddDoseDep_Tld_1(dose);
	}else if (cfr_2==0) {eventAction->AddDoseDep_Tld_2(dose);
	}else if (cfr_3==0) {eventAction->AddDoseDep_Tld_3(dose);
	}else if (cfr_4==0) {eventAction->AddDoseDep_Tld_4(dose);
	}else if (cfr_5==0) {eventAction->AddDoseDep_Tld_5(dose);
	}else if (cfr_6==0) {eventAction->AddDoseDep_Tld_6(dose);
	}else if (cfr_7==0) {eventAction->AddDoseDep_Tld_7(dose);
	}else if (cfr_8==0) {eventAction->AddDoseDep_Tld_8(dose);
	}else if (cfr_9==0) {eventAction->AddDoseDep_Tld_9(dose);
	}else if (cfr_10==0) {eventAction->AddDoseDep_Tld_10(dose);
	}else if (cfr_11==0) {eventAction->AddDoseDep_Tld_11(dose);
	}else if (cfr_12==0) {eventAction->AddDoseDep_Tld_12(dose);
	}else if (cfr_13==0) {eventAction->AddDoseDep_Tld_13(dose);
	}else if (cfr_14==0) {eventAction->AddDoseDep_Tld_14(dose);
	}else if (cfr_15==0) {eventAction->AddDoseDep_Tld_15(dose);
	}else if (cfr_16==0) {eventAction->AddDoseDep_Tld_16(dose);
	}else if (cfr_17==0) {eventAction->AddDoseDep_Tld_17(dose);
	}else if (cfr_18==0) {eventAction->AddDoseDep_Tld_18(dose);
        }else if (cfr_19==0) {eventAction->AddDoseDep_Tld_19(dose);
        }else if (cfr_20==0) {eventAction->AddDoseDep_Tld_20(dose);
	}else if (cfr_21==0) {eventAction->AddDoseDep_Tld_21(dose);
        }else if (cfr_22==0) {eventAction->AddDoseDep_Tld_22(dose);
	}else if (cfr_23==0) {eventAction->AddDoseDep_Tld_23(dose);
	}else if (cfr_24==0) {eventAction->AddDoseDep_Tld_24(dose);
	}else if (cfr_25==0) {eventAction->AddDoseDep_Tld_25(dose);
	}else if (cfr_26==0) {eventAction->AddDoseDep_Tld_26(dose);
	}else if (cfr_27==0) {eventAction->AddDoseDep_Tld_27(dose);
	}else if (cfr_28==0) {eventAction->AddDoseDep_Tld_28(dose);
	}else if (cfr_29==0) {eventAction->AddDoseDep_Tld_29(dose);
	}else  {eventAction->AddDoseDep_Tld_30(dose);}

	
	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TldSD::EndOfEvent(G4HCofThisEvent*)
{}

G4double TldSD::ComputeVolume(G4Step* aStep){

G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
G4VSolid* solid = 0;
solid = physVol->GetLogicalVolume()->GetSolid();

return solid->GetCubicVolume();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

