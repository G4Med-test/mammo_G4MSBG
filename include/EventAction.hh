// EventAction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

// Added for validation purposes only ******************************************
//#include "Image.hh"
// Here end addition for validation purposes only ******************************

class G4RunManager;
class EventActionMessenger;
class RunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction*);
   ~EventAction();

  public:
    void 			BeginOfEventAction(const G4Event*);
    void 			EndOfEventAction(const G4Event*);

		G4double 	GetGammaEnergy()									{return gammaEnergy;}
		void 			SetGammaEnergy(G4double energy)		{gammaEnergy = energy;}
    void 			SetPrintModulo(G4int val) 				{printModulo = val;}
    void 			SetSaveModulo(G4int val) 					{saveModulo = val;}

void			AddDoseDep_Tld_1(G4double val)	 			{doseDep_Tld_1 += val;}
void			AddDoseDep_Tld_2(G4double val)	 			{doseDep_Tld_2 += val;}
void			AddDoseDep_Tld_3(G4double val)	 			{doseDep_Tld_3 += val;}
void			AddDoseDep_Tld_4(G4double val)	 			{doseDep_Tld_4 += val;}
void			AddDoseDep_Tld_5(G4double val)	 			{doseDep_Tld_5 += val;}
void			AddDoseDep_Tld_6(G4double val)	 			{doseDep_Tld_6 += val;}
void			AddDoseDep_Tld_7(G4double val)	 			{doseDep_Tld_7 += val;}
void			AddDoseDep_Tld_8(G4double val)	 			{doseDep_Tld_8 += val;}
void			AddDoseDep_Tld_9(G4double val)	 			{doseDep_Tld_9 += val;}
void			AddDoseDep_Tld_10(G4double val)	 			{doseDep_Tld_10 += val;}
void			AddDoseDep_Tld_11(G4double val)	 			{doseDep_Tld_11 += val;}
void			AddDoseDep_Tld_12(G4double val)	 			{doseDep_Tld_12 += val;}
void			AddDoseDep_Tld_13(G4double val)	 			{doseDep_Tld_13 += val;}
void			AddDoseDep_Tld_14(G4double val)	 			{doseDep_Tld_14 += val;}
void			AddDoseDep_Tld_15(G4double val)	 			{doseDep_Tld_15 += val;}
void			AddDoseDep_Tld_16(G4double val)	 			{doseDep_Tld_16 += val;}
void			AddDoseDep_Tld_17(G4double val)	 			{doseDep_Tld_17 += val;}
void			AddDoseDep_Tld_18(G4double val)	 			{doseDep_Tld_18 += val;}
void			AddDoseDep_Tld_19(G4double val)	 			{doseDep_Tld_19 += val;}
void			AddDoseDep_Tld_20(G4double val)	 			{doseDep_Tld_20 += val;}
void			AddDoseDep_Tld_21(G4double val)	 			{doseDep_Tld_21 += val;}
void			AddDoseDep_Tld_22(G4double val)	 			{doseDep_Tld_22 += val;}
void			AddDoseDep_Tld_23(G4double val)	 			{doseDep_Tld_23 += val;}
void			AddDoseDep_Tld_24(G4double val)	 			{doseDep_Tld_24 += val;}
void			AddDoseDep_Tld_25(G4double val)	 			{doseDep_Tld_25 += val;}
void			AddDoseDep_Tld_26(G4double val)	 			{doseDep_Tld_26 += val;}
void			AddDoseDep_Tld_27(G4double val)	 			{doseDep_Tld_27 += val;}
void			AddDoseDep_Tld_28(G4double val)	 			{doseDep_Tld_28 += val;}
void			AddDoseDep_Tld_29(G4double val)	 			{doseDep_Tld_29 += val;}
void			AddDoseDep_Tld_30(G4double val)	 			{doseDep_Tld_30 += val;}




	private:
		G4double 	gammaEnergy;
    
    G4double	doseDep_Tld_1;
		G4double	doseDep_Tld_2;
		G4double	doseDep_Tld_3;
		G4double	doseDep_Tld_4;
		G4double	doseDep_Tld_5;
		G4double	doseDep_Tld_6;
		G4double	doseDep_Tld_7;
		G4double	doseDep_Tld_8;
		G4double	doseDep_Tld_9;
		G4double	doseDep_Tld_10;
		G4double	doseDep_Tld_11;
		G4double	doseDep_Tld_12;
		G4double	doseDep_Tld_13;
		G4double	doseDep_Tld_14;
		G4double	doseDep_Tld_15;
		G4double	doseDep_Tld_16;
		G4double	doseDep_Tld_17;
		G4double	doseDep_Tld_18;
    G4double	doseDep_Tld_19;
    G4double	doseDep_Tld_20;
    G4double	doseDep_Tld_21;
		G4double	doseDep_Tld_22;
		G4double	doseDep_Tld_23;
		G4double	doseDep_Tld_24;
		G4double	doseDep_Tld_25;
		G4double	doseDep_Tld_26;
		G4double	doseDep_Tld_27;
		G4double	doseDep_Tld_28;
		G4double	doseDep_Tld_29;
		G4double	doseDep_Tld_30;
		

    G4int			printModulo;
    G4int			saveModulo;

    EventActionMessenger* eventMessenger;
		RunAction*						runAction;
		G4RunManager* 				runManager;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
