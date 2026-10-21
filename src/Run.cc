// Run.cc
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Run.hh"
#include "DetectorConstruction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"

#include <math.h>

extern G4String dirName;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::Run(DetectorConstruction* det)
: doseDep_Tld_1(0.0),doseDepSquared_Tld_1(0.0),doseDep_Tld_2(0.0),doseDepSquared_Tld_2(0.0),doseDep_Tld_3(0.0),doseDepSquared_Tld_3(0.0),doseDep_Tld_4(0.0),doseDepSquared_Tld_4(0.0),doseDep_Tld_5(0.0),doseDepSquared_Tld_5(0.0),doseDep_Tld_6(0.0),doseDepSquared_Tld_6(0.0),doseDep_Tld_7(0.0),doseDepSquared_Tld_7(0.0),doseDep_Tld_8(0.0),doseDepSquared_Tld_8(0.0),doseDep_Tld_9(0.0),doseDepSquared_Tld_9(0.0),doseDep_Tld_10(0.0),doseDepSquared_Tld_10(0.0),doseDep_Tld_11(0.0),doseDepSquared_Tld_11(0.0),doseDep_Tld_12(0.0),doseDepSquared_Tld_12(0.0),doseDep_Tld_13(0.0),doseDepSquared_Tld_13(0.0),doseDep_Tld_14(0.0),doseDepSquared_Tld_14(0.0),doseDep_Tld_15(0.0),doseDepSquared_Tld_15(0.0),doseDep_Tld_16(0.0),doseDepSquared_Tld_16(0.0),doseDep_Tld_17(0.0),doseDepSquared_Tld_17(0.0),doseDep_Tld_18(0.0),doseDepSquared_Tld_18(0.0),doseDep_Tld_19(0.0),doseDepSquared_Tld_19(0.0),doseDep_Tld_20(0.0),doseDepSquared_Tld_20(0.0),doseDep_Tld_21(0.0),doseDepSquared_Tld_21(0.0),doseDep_Tld_22(0.0),doseDepSquared_Tld_22(0.0),doseDep_Tld_23(0.0),doseDepSquared_Tld_23(0.0),doseDep_Tld_24(0.0),doseDepSquared_Tld_24(0.0),doseDep_Tld_25(0.0),doseDepSquared_Tld_25(0.0),doseDep_Tld_26(0.0),doseDepSquared_Tld_26(0.0),doseDep_Tld_27(0.0),doseDepSquared_Tld_27(0.0),doseDep_Tld_28(0.0),doseDepSquared_Tld_28(0.0),doseDep_Tld_29(0.0),doseDepSquared_Tld_29(0.0),doseDep_Tld_30(0.0),doseDepSquared_Tld_30(0.0),detectorConst(det)
{
	 
G4SDManager* SDMan= G4SDManager::GetSDMpointer();
Kerma_Evaluation_ID= SDMan->GetCollectionID("SurfaceFluxSD/theSurfaceFluxSD");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::~Run()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::AddDoseDep_Tld_1(G4double val)
{
	doseDep_Tld_1 += val;
	doseDepSquared_Tld_1 += val*val;
}

void Run::AddDoseDep_Tld_2(G4double val)
{
	doseDep_Tld_2 += val;
	doseDepSquared_Tld_2 += val*val;
}

void Run::AddDoseDep_Tld_3(G4double val)
{
	doseDep_Tld_3 += val;
	doseDepSquared_Tld_3 += val*val;
}

void Run::AddDoseDep_Tld_4(G4double val)
{
	doseDep_Tld_4 += val;
	doseDepSquared_Tld_4 += val*val;
}

void Run::AddDoseDep_Tld_5(G4double val)
{
	doseDep_Tld_5 += val;
	doseDepSquared_Tld_5 += val*val;
}

void Run::AddDoseDep_Tld_6(G4double val)
{
	doseDep_Tld_6 += val;
	doseDepSquared_Tld_6 += val*val;
}

void Run::AddDoseDep_Tld_7(G4double val)
{
	doseDep_Tld_7 += val;
	doseDepSquared_Tld_7 += val*val;
}

void Run::AddDoseDep_Tld_8(G4double val)
{
	doseDep_Tld_8 += val;
	doseDepSquared_Tld_8 += val*val;
}

void Run::AddDoseDep_Tld_9(G4double val)
{
	doseDep_Tld_9 += val;
	doseDepSquared_Tld_9 += val*val;
}

void Run::AddDoseDep_Tld_10(G4double val)
{
	doseDep_Tld_10 += val;
	doseDepSquared_Tld_10 += val*val;
}

void Run::AddDoseDep_Tld_11(G4double val)
{
	doseDep_Tld_11 += val;
	doseDepSquared_Tld_11 += val*val;
}

void Run::AddDoseDep_Tld_12(G4double val)
{
	doseDep_Tld_12 += val;
	doseDepSquared_Tld_12 += val*val;
}

void Run::AddDoseDep_Tld_13(G4double val)
{
	doseDep_Tld_13 += val;
	doseDepSquared_Tld_13 += val*val;
}

void Run::AddDoseDep_Tld_14(G4double val)
{
	doseDep_Tld_14 += val;
	doseDepSquared_Tld_14 += val*val;
}

void Run::AddDoseDep_Tld_15(G4double val)
{
	doseDep_Tld_15 += val;
	doseDepSquared_Tld_15 += val*val;
}

void Run::AddDoseDep_Tld_16(G4double val)
{
	doseDep_Tld_16 += val;
	doseDepSquared_Tld_16 += val*val;
}

void Run::AddDoseDep_Tld_17(G4double val)
{
	doseDep_Tld_17 += val;
	doseDepSquared_Tld_17 += val*val;
}

void Run::AddDoseDep_Tld_18(G4double val)
{
	doseDep_Tld_18 += val;
	doseDepSquared_Tld_18 += val*val;
}

void Run::AddDoseDep_Tld_19(G4double val)
{
	doseDep_Tld_19 += val;
	doseDepSquared_Tld_19 += val*val;
}

void Run::AddDoseDep_Tld_20(G4double val)
{
	doseDep_Tld_20 += val;
	doseDepSquared_Tld_20 += val*val;
}

void Run::AddDoseDep_Tld_21(G4double val)
{
	doseDep_Tld_21 += val;
	doseDepSquared_Tld_21 += val*val;
}

void Run::AddDoseDep_Tld_22(G4double val)
{
	doseDep_Tld_22 += val;
	doseDepSquared_Tld_22 += val*val;
}

void Run::AddDoseDep_Tld_23(G4double val)
{
	doseDep_Tld_23 += val;
	doseDepSquared_Tld_23 += val*val;
}

void Run::AddDoseDep_Tld_24(G4double val)
{
	doseDep_Tld_24 += val;
	doseDepSquared_Tld_24 += val*val;
}

void Run::AddDoseDep_Tld_25(G4double val)
{
	doseDep_Tld_25 += val;
	doseDepSquared_Tld_25 += val*val;
}

void Run::AddDoseDep_Tld_26(G4double val)
{
	doseDep_Tld_26 += val;
	doseDepSquared_Tld_26 += val*val;
}

void Run::AddDoseDep_Tld_27(G4double val)
{
	doseDep_Tld_27 += val;
	doseDepSquared_Tld_27 += val*val;
}

void Run::AddDoseDep_Tld_28(G4double val)
{
	doseDep_Tld_28 += val;
	doseDepSquared_Tld_28 += val*val;
}

void Run::AddDoseDep_Tld_29(G4double val)
{
	doseDep_Tld_29 += val;
	doseDepSquared_Tld_29 += val*val;
}

void Run::AddDoseDep_Tld_30(G4double val)
{
	doseDep_Tld_30 += val;
	doseDepSquared_Tld_30 += val*val;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::RecordEvent(const G4Event* evt)
{
	numberOfEvent++;
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
	if (!HCE) return;

  Kerma_Evaluation_map    =(G4THitsMap<G4double>*) (HCE->GetHC(Kerma_Evaluation_ID));
  Kerma_Evaluation_value += *Kerma_Evaluation_map;

}


G4double Run::GetKerma_Evaluation(const G4THitsMap<G4double> &map) const
{

	G4double tot =0.;
  std::map<G4int,G4double*>::iterator itr=map.GetMap()->begin();
		for(; itr !=map.GetMap()->end(); itr ++)
			{
	    tot +=*(itr->second);
			}

return tot;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double Run::GetDoseDepUncertainty_Tld_1()
{
	// Uncertainty analysis
	G4double numEvents_Tld_1 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_1 = sqrt( (1.0 / (numEvents_Tld_1 - 1.0) ) * 
																				( doseDepSquared_Tld_1 / numEvents_Tld_1 -
																					(doseDep_Tld_1 * doseDep_Tld_1 / (numEvents_Tld_1 * numEvents_Tld_1) ) ) );

	return doseDepUncertainty_Tld_1;
}

G4double Run::GetDoseDepUncertainty_Tld_2()
{
	// Uncertainty analysis
	G4double numEvents_Tld_2 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_2 = sqrt( (1.0 / (numEvents_Tld_2 - 1.0) ) * 
																				( doseDepSquared_Tld_2 / numEvents_Tld_2 -
																					(doseDep_Tld_2 * doseDep_Tld_2 / (numEvents_Tld_2 * numEvents_Tld_2) ) ) );

	return doseDepUncertainty_Tld_2;
}

G4double Run::GetDoseDepUncertainty_Tld_3()
{
	// Uncertainty analysis
	G4double numEvents_Tld_3 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_3 = sqrt( (1.0 / (numEvents_Tld_3 - 1.0) ) * 
																				( doseDepSquared_Tld_3 / numEvents_Tld_3 -
																					(doseDep_Tld_3 * doseDep_Tld_3 / (numEvents_Tld_3 * numEvents_Tld_3) ) ) );

	return doseDepUncertainty_Tld_3;
}

G4double Run::GetDoseDepUncertainty_Tld_4()
{
	// Uncertainty analysis
	G4double numEvents_Tld_4 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_4 = sqrt( (1.0 / (numEvents_Tld_4 - 1.0) ) * 
																				( doseDepSquared_Tld_4 / numEvents_Tld_4 -
																					(doseDep_Tld_4 * doseDep_Tld_4 / (numEvents_Tld_4 * numEvents_Tld_4) ) ) );

	return doseDepUncertainty_Tld_4;
}

G4double Run::GetDoseDepUncertainty_Tld_5()
{
	// Uncertainty analysis
	G4double numEvents_Tld_5 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_5 = sqrt( (1.0 / (numEvents_Tld_5 - 1.0) ) * 
																				( doseDepSquared_Tld_5 / numEvents_Tld_5 -
																					(doseDep_Tld_5 * doseDep_Tld_5 / (numEvents_Tld_5 * numEvents_Tld_5) ) ) );

	return doseDepUncertainty_Tld_5;
}

G4double Run::GetDoseDepUncertainty_Tld_6()
{
	// Uncertainty analysis
	G4double numEvents_Tld_6 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_6 = sqrt( (1.0 / (numEvents_Tld_6 - 1.0) ) * 
																				( doseDepSquared_Tld_6 / numEvents_Tld_6 -
																					(doseDep_Tld_6 * doseDep_Tld_6 / (numEvents_Tld_6 * numEvents_Tld_6) ) ) );

	return doseDepUncertainty_Tld_6;
}

G4double Run::GetDoseDepUncertainty_Tld_7()
{
	// Uncertainty analysis
	G4double numEvents_Tld_7 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_7 = sqrt( (1.0 / (numEvents_Tld_7 - 1.0) ) * 
																				( doseDepSquared_Tld_7 / numEvents_Tld_7 -
																					(doseDep_Tld_7 * doseDep_Tld_7 / (numEvents_Tld_7 * numEvents_Tld_7) ) ) );

	return doseDepUncertainty_Tld_7;
}

G4double Run::GetDoseDepUncertainty_Tld_8()
{
	// Uncertainty analysis
	G4double numEvents_Tld_8 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_8 = sqrt( (1.0 / (numEvents_Tld_8 - 1.0) ) * 
																				( doseDepSquared_Tld_8 / numEvents_Tld_8 -
																					(doseDep_Tld_8 * doseDep_Tld_8 / (numEvents_Tld_8 * numEvents_Tld_8) ) ) );

	return doseDepUncertainty_Tld_8;
}

G4double Run::GetDoseDepUncertainty_Tld_9()
{
	// Uncertainty analysis
	G4double numEvents_Tld_9 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_9 = sqrt( (1.0 / (numEvents_Tld_9 - 1.0) ) * 
																				( doseDepSquared_Tld_9 / numEvents_Tld_9 -
																					(doseDep_Tld_9 * doseDep_Tld_9 / (numEvents_Tld_9 * numEvents_Tld_9) ) ) );

	return doseDepUncertainty_Tld_9;
}

G4double Run::GetDoseDepUncertainty_Tld_10()
{
	// Uncertainty analysis
	G4double numEvents_Tld_10 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_10 = sqrt( (1.0 / (numEvents_Tld_10 - 1.0) ) * 
																				( doseDepSquared_Tld_10 / numEvents_Tld_10 -
																					(doseDep_Tld_10 * doseDep_Tld_10 / (numEvents_Tld_10 * numEvents_Tld_10) ) ) );

	return doseDepUncertainty_Tld_10;
}

G4double Run::GetDoseDepUncertainty_Tld_11()
{
	// Uncertainty analysis
	G4double numEvents_Tld_11 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_11 = sqrt( (1.0 / (numEvents_Tld_11 - 1.0) ) * 
																				( doseDepSquared_Tld_11 / numEvents_Tld_11 -
																					(doseDep_Tld_11 * doseDep_Tld_11 / (numEvents_Tld_11 * numEvents_Tld_11) ) ) );

	return doseDepUncertainty_Tld_11;
}

G4double Run::GetDoseDepUncertainty_Tld_12()
{
	// Uncertainty analysis
	G4double numEvents_Tld_12 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_12 = sqrt( (1.0 / (numEvents_Tld_12 - 1.0) ) * 
																				( doseDepSquared_Tld_12 / numEvents_Tld_12 -
																					(doseDep_Tld_12 * doseDep_Tld_12 / (numEvents_Tld_12 * numEvents_Tld_12) ) ) );

	return doseDepUncertainty_Tld_12;
}

G4double Run::GetDoseDepUncertainty_Tld_13()
{
	// Uncertainty analysis
	G4double numEvents_Tld_13 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_13 = sqrt( (1.0 / (numEvents_Tld_13 - 1.0) ) * 
																				( doseDepSquared_Tld_13 / numEvents_Tld_13 -
																					(doseDep_Tld_13 * doseDep_Tld_13 / (numEvents_Tld_13 * numEvents_Tld_13) ) ) );

	return doseDepUncertainty_Tld_13;
}


G4double Run::GetDoseDepUncertainty_Tld_14()
{
	// Uncertainty analysis
	G4double numEvents_Tld_14 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_14 = sqrt( (1.0 / (numEvents_Tld_14 - 1.0) ) * 
																				( doseDepSquared_Tld_14 / numEvents_Tld_14 -
																					(doseDep_Tld_14 * doseDep_Tld_14 / (numEvents_Tld_14 * numEvents_Tld_14) ) ) );

	return doseDepUncertainty_Tld_14;
}


G4double Run::GetDoseDepUncertainty_Tld_15()
{
	// Uncertainty analysis
	G4double numEvents_Tld_15 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_15 = sqrt( (1.0 / (numEvents_Tld_15 - 1.0) ) * 
																				( doseDepSquared_Tld_15 / numEvents_Tld_15 -
																					(doseDep_Tld_15 * doseDep_Tld_15 / (numEvents_Tld_15 * numEvents_Tld_15) ) ) );

	return doseDepUncertainty_Tld_15;
}

G4double Run::GetDoseDepUncertainty_Tld_16()
{
	// Uncertainty analysis
	G4double numEvents_Tld_16 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_16 = sqrt( (1.0 / (numEvents_Tld_16 - 1.0) ) * 
																				( doseDepSquared_Tld_16 / numEvents_Tld_16 -
																					(doseDep_Tld_16 * doseDep_Tld_16 / (numEvents_Tld_16 * numEvents_Tld_16) ) ) );

	return doseDepUncertainty_Tld_16;
}

G4double Run::GetDoseDepUncertainty_Tld_17()
{
	// Uncertainty analysis
	G4double numEvents_Tld_17 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_17 = sqrt( (1.0 / (numEvents_Tld_17 - 1.0) ) * 
																				( doseDepSquared_Tld_17 / numEvents_Tld_17 -
																					(doseDep_Tld_17 * doseDep_Tld_17 / (numEvents_Tld_17 * numEvents_Tld_17) ) ) );

	return doseDepUncertainty_Tld_17;
}

G4double Run::GetDoseDepUncertainty_Tld_18()
{
	// Uncertainty analysis
	G4double numEvents_Tld_18 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_18 = sqrt( (1.0 / (numEvents_Tld_18 - 1.0) ) * 
																				( doseDepSquared_Tld_18 / numEvents_Tld_18 -
																					(doseDep_Tld_18 * doseDep_Tld_18 / (numEvents_Tld_18 * numEvents_Tld_18) ) ) );

	return doseDepUncertainty_Tld_18;
}

G4double Run::GetDoseDepUncertainty_Tld_19()
{
	// Uncertainty analysis
	G4double numEvents_Tld_19 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_19 = sqrt( (1.0 / (numEvents_Tld_19 - 1.0) ) * 
																				( doseDepSquared_Tld_19 / numEvents_Tld_19 -
																					(doseDep_Tld_19 * doseDep_Tld_19 / (numEvents_Tld_19 * numEvents_Tld_19) ) ) );

	return doseDepUncertainty_Tld_19;
}

G4double Run::GetDoseDepUncertainty_Tld_20()
{
	// Uncertainty analysis
	G4double numEvents_Tld_20 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_20 = sqrt( (1.0 / (numEvents_Tld_20 - 1.0) ) * 
																				( doseDepSquared_Tld_20 / numEvents_Tld_20 -
																					(doseDep_Tld_20 * doseDep_Tld_20 / (numEvents_Tld_20 * numEvents_Tld_20) ) ) );

	return doseDepUncertainty_Tld_20;
}

G4double Run::GetDoseDepUncertainty_Tld_21()
{
	// Uncertainty analysis
	G4double numEvents_Tld_21 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_21 = sqrt( (1.0 / (numEvents_Tld_21 - 1.0) ) * 
																				( doseDepSquared_Tld_21 / numEvents_Tld_21 -
																					(doseDep_Tld_21 * doseDep_Tld_21 / (numEvents_Tld_21 * numEvents_Tld_21) ) ) );

	return doseDepUncertainty_Tld_21;
}

G4double Run::GetDoseDepUncertainty_Tld_22()
{
	// Uncertainty analysis
	G4double numEvents_Tld_22 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_22 = sqrt( (1.0 / (numEvents_Tld_22 - 1.0) ) * 
																				( doseDepSquared_Tld_22 / numEvents_Tld_22 -
																					(doseDep_Tld_22 * doseDep_Tld_22 / (numEvents_Tld_22 * numEvents_Tld_22) ) ) );

	return doseDepUncertainty_Tld_22;
}

G4double Run::GetDoseDepUncertainty_Tld_23()
{
	// Uncertainty analysis
	G4double numEvents_Tld_23 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_23 = sqrt( (1.0 / (numEvents_Tld_23 - 1.0) ) * 
																				( doseDepSquared_Tld_23 / numEvents_Tld_23 -
																					(doseDep_Tld_23 * doseDep_Tld_23 / (numEvents_Tld_23 * numEvents_Tld_23) ) ) );

	return doseDepUncertainty_Tld_23;
}

G4double Run::GetDoseDepUncertainty_Tld_24()
{
	// Uncertainty analysis
	G4double numEvents_Tld_24 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_24 = sqrt( (1.0 / (numEvents_Tld_24 - 1.0) ) * 
																				( doseDepSquared_Tld_24 / numEvents_Tld_24 -
																					(doseDep_Tld_24 * doseDep_Tld_24 / (numEvents_Tld_24 * numEvents_Tld_24) ) ) );

	return doseDepUncertainty_Tld_24;
}

G4double Run::GetDoseDepUncertainty_Tld_25()
{
	// Uncertainty analysis
	G4double numEvents_Tld_25 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_25 = sqrt( (1.0 / (numEvents_Tld_25 - 1.0) ) * 
																				( doseDepSquared_Tld_25 / numEvents_Tld_25 -
																					(doseDep_Tld_25 * doseDep_Tld_25 / (numEvents_Tld_25 * numEvents_Tld_25) ) ) );

	return doseDepUncertainty_Tld_25;
}

G4double Run::GetDoseDepUncertainty_Tld_26()
{
	// Uncertainty analysis
	G4double numEvents_Tld_26 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_26 = sqrt( (1.0 / (numEvents_Tld_26 - 1.0) ) * 
																				( doseDepSquared_Tld_26 / numEvents_Tld_26 -
																					(doseDep_Tld_26 * doseDep_Tld_26 / (numEvents_Tld_26 * numEvents_Tld_26) ) ) );

	return doseDepUncertainty_Tld_26;
}

G4double Run::GetDoseDepUncertainty_Tld_27()
{
	// Uncertainty analysis
	G4double numEvents_Tld_27 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_27 = sqrt( (1.0 / (numEvents_Tld_27 - 1.0) ) * 
																				( doseDepSquared_Tld_27 / numEvents_Tld_27 -
																					(doseDep_Tld_27 * doseDep_Tld_27 / (numEvents_Tld_27 * numEvents_Tld_27) ) ) );

	return doseDepUncertainty_Tld_27;
}

G4double Run::GetDoseDepUncertainty_Tld_28()
{
	// Uncertainty analysis
	G4double numEvents_Tld_28 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_28 = sqrt( (1.0 / (numEvents_Tld_28 - 1.0) ) * 
																				( doseDepSquared_Tld_28 / numEvents_Tld_28 -
																					(doseDep_Tld_28 * doseDep_Tld_28 / (numEvents_Tld_28 * numEvents_Tld_28) ) ) );

	return doseDepUncertainty_Tld_28;
}

G4double Run::GetDoseDepUncertainty_Tld_29()
{
	// Uncertainty analysis
	G4double numEvents_Tld_29 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_29 = sqrt( (1.0 / (numEvents_Tld_29 - 1.0) ) * 
																				( doseDepSquared_Tld_29 / numEvents_Tld_29 -
																					(doseDep_Tld_29 * doseDep_Tld_29 / (numEvents_Tld_29 * numEvents_Tld_29) ) ) );

	return doseDepUncertainty_Tld_29;
}

G4double Run::GetDoseDepUncertainty_Tld_30()
{
	// Uncertainty analysis
	G4double numEvents_Tld_30 = (G4double) GetNumberOfEvent();

	G4double doseDepUncertainty_Tld_30 = sqrt( (1.0 / (numEvents_Tld_30 - 1.0) ) * 
																				( doseDepSquared_Tld_30 / numEvents_Tld_30 -
																					(doseDep_Tld_30 * doseDep_Tld_30 / (numEvents_Tld_30 * numEvents_Tld_30) ) ) );

	return doseDepUncertainty_Tld_30;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

