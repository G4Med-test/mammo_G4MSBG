// PrimaryGeneratorAction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "CLHEP/Random/RandGeneral.h"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"


//enum fieldType {flat, iso, heel, straight};
enum materialType {polycarbonate, air};

class DetectorConstruction;
class EventAction;
class PrimaryGeneratorMessenger;
class G4ParticleGun;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
   ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
		void PositionAndAim();

		G4ParticleGun*	GetParticleGun()			{return particleGun;}

		G4double	 			GetuyMin()						{return uyMin;}
		G4double 				GetuyMax()						{return uyMax;}

    G4double				GetReferenceAirKerma(G4double, G4bool, G4bool);
    G4double			  PhotonFluenceAtReferencePoint(G4double, G4bool, G4bool);

		// Experiment Dimensions:
		void  					SetGunNumber(G4int val)					{gunNumber = val;} 				// change number of particles to shoot per event
		void  					SetSID(G4double);
		void  					SetDCOR(G4double);
		void  					SetDistFromCW(G4double);
		//void 						SetField(fieldType val) {field = val;}

		// Experiment Dimensions:
    G4int					GetGunNumber()					{return gunNumber;}
    G4double			GetSID()								{return SID;}
    G4double			GetDCOR()								{return DCOR;}
    G4double			GetDistFromCW()					{return distFromCW;}
		//G4int					GetField()							{return field;}
  

  private:
 
		PrimaryGeneratorMessenger* 						generatorMessenger;
    G4ParticleGun* 												particleGun;
    DetectorConstruction* 								detectorConst;
		EventAction* 													eventAction;

    G4double															PolycarbonateLinearAttCoeff(G4double);
		G4double															AirLinearAttCoeff(G4double);
		G4double															AirMassEnergyAbsCoeff(G4double);
		
		G4double 															EffectiveAttenuation(materialType, G4double);
		G4double 															EffectiveAttenuation(materialType, G4double, G4double);

		G4double 	totalExposure;
		G4double 	totalAirKerma;

		G4double*															xpdf;
		G4double*															ypdf;
		G4double 															cosMinTheta;
		G4double 															deltaCosTheta;

    CLHEP::RandGeneral* 									xrandGeneral;
		CLHEP::RandGeneral* 									yrandGeneral;

		G4double															uyMin;
		G4double															uyMax;
		G4double															deltauy;

		// Experiment Dimensions:
		G4int																	gunNumber;
		G4double															SID;
		G4double															DCOR;										// distance from detector to center of

		G4double															dx, dy, dz;
		G4double															distFromCW;
		//fieldType															field;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
