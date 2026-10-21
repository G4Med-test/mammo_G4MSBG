// Run.hh
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Run_h
#define Run_h 1

#include "G4THitsMap.hh"

#include "G4Run.hh"

class DetectorConstruction;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Run : public G4Run
{
  public:
    Run(DetectorConstruction*);
   ~Run();
   virtual void RecordEvent(const G4Event*);

  public:
   
   G4double								GetDoseDep_Tld_1()										{return doseDep_Tld_1;}
   G4double								GetDoseDepUncertainty_Tld_1();
   G4double								GetDoseDep_Tld_2()										{return doseDep_Tld_2;}
   G4double								GetDoseDepUncertainty_Tld_2();
   G4double								GetDoseDep_Tld_3()										{return doseDep_Tld_3;}
   G4double								GetDoseDepUncertainty_Tld_3();
   G4double								GetDoseDep_Tld_4()										{return doseDep_Tld_4;}
   G4double								GetDoseDepUncertainty_Tld_4();
   G4double								GetDoseDep_Tld_5()										{return doseDep_Tld_5;}
   G4double								GetDoseDepUncertainty_Tld_5();
   G4double								GetDoseDep_Tld_6()										{return doseDep_Tld_6;}
   G4double								GetDoseDepUncertainty_Tld_6();
   G4double								GetDoseDep_Tld_7()										{return doseDep_Tld_7;}
   G4double								GetDoseDepUncertainty_Tld_7();
   G4double								GetDoseDep_Tld_8()										{return doseDep_Tld_8;}
   G4double								GetDoseDepUncertainty_Tld_8();
   G4double								GetDoseDep_Tld_9()										{return doseDep_Tld_9;}
   G4double								GetDoseDepUncertainty_Tld_9();
   G4double								GetDoseDep_Tld_10()										{return doseDep_Tld_10;}
   G4double								GetDoseDepUncertainty_Tld_10();
   G4double								GetDoseDep_Tld_11()										{return doseDep_Tld_11;}
   G4double								GetDoseDepUncertainty_Tld_11();
   G4double								GetDoseDep_Tld_12()										{return doseDep_Tld_12;}
   G4double								GetDoseDepUncertainty_Tld_12();
   G4double								GetDoseDep_Tld_13()										{return doseDep_Tld_13;}
   G4double								GetDoseDepUncertainty_Tld_13();
   G4double								GetDoseDep_Tld_14()										{return doseDep_Tld_14;}
   G4double								GetDoseDepUncertainty_Tld_14();
   G4double								GetDoseDep_Tld_15()										{return doseDep_Tld_15;}
   G4double								GetDoseDepUncertainty_Tld_15();
   G4double								GetDoseDep_Tld_16()										{return doseDep_Tld_16;}
   G4double								GetDoseDepUncertainty_Tld_16();
   G4double								GetDoseDep_Tld_17()										{return doseDep_Tld_17;}
   G4double								GetDoseDepUncertainty_Tld_17();
   G4double								GetDoseDep_Tld_18()										{return doseDep_Tld_18;}
   G4double								GetDoseDepUncertainty_Tld_18();
   G4double								GetDoseDep_Tld_19()										{return doseDep_Tld_19;}
   G4double								GetDoseDepUncertainty_Tld_19();
   G4double								GetDoseDep_Tld_20()										{return doseDep_Tld_20;}
   G4double								GetDoseDepUncertainty_Tld_20();
   G4double								GetDoseDep_Tld_21()										{return doseDep_Tld_21;}
   G4double								GetDoseDepUncertainty_Tld_21();
   G4double								GetDoseDep_Tld_22()										{return doseDep_Tld_22;}
   G4double								GetDoseDepUncertainty_Tld_22();
   G4double								GetDoseDep_Tld_23()										{return doseDep_Tld_23;}
   G4double								GetDoseDepUncertainty_Tld_23();
   G4double								GetDoseDep_Tld_24()										{return doseDep_Tld_24;}
   G4double								GetDoseDepUncertainty_Tld_24();
   G4double								GetDoseDep_Tld_25()										{return doseDep_Tld_25;}
   G4double								GetDoseDepUncertainty_Tld_25();
   G4double								GetDoseDep_Tld_26()										{return doseDep_Tld_26;}
   G4double								GetDoseDepUncertainty_Tld_26();
   G4double								GetDoseDep_Tld_27()										{return doseDep_Tld_27;}
   G4double								GetDoseDepUncertainty_Tld_27();
   G4double								GetDoseDep_Tld_28()										{return doseDep_Tld_28;}
   G4double								GetDoseDepUncertainty_Tld_28();
   G4double								GetDoseDep_Tld_29()										{return doseDep_Tld_29;}
   G4double								GetDoseDepUncertainty_Tld_29();
   G4double								GetDoseDep_Tld_30()										{return doseDep_Tld_30;}
   G4double								GetDoseDepUncertainty_Tld_30();

void 										AddDoseDep_Tld_1(G4double val);
void 										AddDoseDep_Tld_2(G4double val);
void 										AddDoseDep_Tld_3(G4double val);
void 										AddDoseDep_Tld_4(G4double val);
void 										AddDoseDep_Tld_5(G4double val);
void 										AddDoseDep_Tld_6(G4double val);
void 										AddDoseDep_Tld_7(G4double val);
void 										AddDoseDep_Tld_8(G4double val);
void 										AddDoseDep_Tld_9(G4double val);
void 										AddDoseDep_Tld_10(G4double val);
void 										AddDoseDep_Tld_11(G4double val);
void 										AddDoseDep_Tld_12(G4double val);
void 										AddDoseDep_Tld_13(G4double val);
void 										AddDoseDep_Tld_14(G4double val);
void 										AddDoseDep_Tld_15(G4double val);
void 										AddDoseDep_Tld_16(G4double val);
void 										AddDoseDep_Tld_17(G4double val);
void 										AddDoseDep_Tld_18(G4double val);
void 										AddDoseDep_Tld_19(G4double val);
void 										AddDoseDep_Tld_20(G4double val);
void 										AddDoseDep_Tld_21(G4double val);
void 										AddDoseDep_Tld_22(G4double val);
void 										AddDoseDep_Tld_23(G4double val);
void 										AddDoseDep_Tld_24(G4double val);
void 										AddDoseDep_Tld_25(G4double val);
void 										AddDoseDep_Tld_26(G4double val);
void 										AddDoseDep_Tld_27(G4double val);
void 										AddDoseDep_Tld_28(G4double val);
void 										AddDoseDep_Tld_29(G4double val);
void 										AddDoseDep_Tld_30(G4double val);

void										SetDoseDep_Tld_1(G4double val)				{doseDep_Tld_1 = val;}
void										SetDoseDep_Tld_2(G4double val)				{doseDep_Tld_2 = val;}
void										SetDoseDep_Tld_3(G4double val)				{doseDep_Tld_3 = val;}
void										SetDoseDep_Tld_4(G4double val)				{doseDep_Tld_4 = val;}
void										SetDoseDep_Tld_5(G4double val)				{doseDep_Tld_5 = val;}
void										SetDoseDep_Tld_6(G4double val)				{doseDep_Tld_6 = val;}
void										SetDoseDep_Tld_7(G4double val)				{doseDep_Tld_7 = val;}
void										SetDoseDep_Tld_8(G4double val)				{doseDep_Tld_8 = val;}
void										SetDoseDep_Tld_9(G4double val)				{doseDep_Tld_9 = val;}
void										SetDoseDep_Tld_10(G4double val)				{doseDep_Tld_10 = val;}
void										SetDoseDep_Tld_11(G4double val)				{doseDep_Tld_11 = val;}
void										SetDoseDep_Tld_12(G4double val)				{doseDep_Tld_12 = val;}
void										SetDoseDep_Tld_13(G4double val)				{doseDep_Tld_13 = val;}
void										SetDoseDep_Tld_14(G4double val)				{doseDep_Tld_14 = val;}
void										SetDoseDep_Tld_15(G4double val)				{doseDep_Tld_15 = val;}
void										SetDoseDep_Tld_16(G4double val)				{doseDep_Tld_16 = val;}
void										SetDoseDep_Tld_17(G4double val)				{doseDep_Tld_17 = val;}
void										SetDoseDep_Tld_18(G4double val)				{doseDep_Tld_18 = val;}
void										SetDoseDep_Tld_19(G4double val)				{doseDep_Tld_19 = val;}
void										SetDoseDep_Tld_20(G4double val)				{doseDep_Tld_20 = val;}
void										SetDoseDep_Tld_21(G4double val)				{doseDep_Tld_21 = val;}
void										SetDoseDep_Tld_22(G4double val)				{doseDep_Tld_22 = val;}
void										SetDoseDep_Tld_23(G4double val)				{doseDep_Tld_23 = val;}
void										SetDoseDep_Tld_24(G4double val)				{doseDep_Tld_24 = val;}
void										SetDoseDep_Tld_25(G4double val)				{doseDep_Tld_25 = val;}
void										SetDoseDep_Tld_26(G4double val)				{doseDep_Tld_26 = val;}
void										SetDoseDep_Tld_27(G4double val)				{doseDep_Tld_27 = val;}
void										SetDoseDep_Tld_28(G4double val)				{doseDep_Tld_28 = val;}
void										SetDoseDep_Tld_29(G4double val)				{doseDep_Tld_29 = val;}
void										SetDoseDep_Tld_30(G4double val)				{doseDep_Tld_30 = val;}


  public:


    G4double								doseDep_Tld_1;
    G4double								doseDepSquared_Tld_1;
    G4double								doseDep_Tld_2;
    G4double								doseDepSquared_Tld_2;
    G4double								doseDep_Tld_3;
    G4double								doseDepSquared_Tld_3;
    G4double								doseDep_Tld_4;
    G4double								doseDepSquared_Tld_4;
    G4double								doseDep_Tld_5;
    G4double								doseDepSquared_Tld_5;
    G4double								doseDep_Tld_6;
    G4double								doseDepSquared_Tld_6;
    G4double								doseDep_Tld_7;
    G4double								doseDepSquared_Tld_7;
    G4double								doseDep_Tld_8;
    G4double								doseDepSquared_Tld_8;
    G4double								doseDep_Tld_9;
    G4double								doseDepSquared_Tld_9;
    G4double								doseDep_Tld_10;
    G4double								doseDepSquared_Tld_10;
    G4double								doseDep_Tld_11;
    G4double								doseDepSquared_Tld_11;
    G4double								doseDep_Tld_12;
    G4double								doseDepSquared_Tld_12;
    G4double								doseDep_Tld_13;
    G4double								doseDepSquared_Tld_13;
    G4double								doseDep_Tld_14;
    G4double								doseDepSquared_Tld_14;
    G4double								doseDep_Tld_15;
    G4double								doseDepSquared_Tld_15;
    G4double								doseDep_Tld_16;
    G4double								doseDepSquared_Tld_16;
    G4double								doseDep_Tld_17;
    G4double								doseDepSquared_Tld_17;
    G4double								doseDep_Tld_18;
    G4double								doseDepSquared_Tld_18;
    G4double								doseDep_Tld_19;
    G4double								doseDepSquared_Tld_19;
    G4double								doseDep_Tld_20;
    G4double								doseDepSquared_Tld_20;
    G4double								doseDep_Tld_21;
    G4double								doseDepSquared_Tld_21;
    G4double								doseDep_Tld_22;
    G4double								doseDepSquared_Tld_22;
    G4double								doseDep_Tld_23;
    G4double								doseDepSquared_Tld_23;
    G4double								doseDep_Tld_24;
    G4double								doseDepSquared_Tld_24;
    G4double								doseDep_Tld_25;
    G4double								doseDepSquared_Tld_25;
    G4double								doseDep_Tld_26;
    G4double								doseDepSquared_Tld_26;
    G4double								doseDep_Tld_27;
    G4double								doseDepSquared_Tld_27;
    G4double								doseDep_Tld_28;
    G4double								doseDepSquared_Tld_28;
    G4double								doseDep_Tld_29;
    G4double								doseDepSquared_Tld_29;
    G4double								doseDep_Tld_30;
    G4double								doseDepSquared_Tld_30;


		DetectorConstruction* 	detectorConst; 

   G4double GetKerma_Evaluation(const G4THitsMap<G4double> &map) const;
	 G4int Kerma_Evaluation_ID;
   G4THitsMap<G4double>* Kerma_Evaluation_map;
   G4THitsMap<G4double> Kerma_Evaluation_value;


 public:

 inline G4double GetTotKerma_Evaluation() const {return GetKerma_Evaluation(Kerma_Evaluation_value);}
   

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
