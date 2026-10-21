// PrimaryGeneratorAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Random/RandGeneral.h"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

extern G4double detector_deviceHalfSize;

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* myDC)
:detectorConst(myDC)
{
  generatorMessenger = new PrimaryGeneratorMessenger(this);

  // default parameter values of the geometry
	DCOR       = 0.0*cm;
	SetSID(10.0*cm);
	distFromCW = 0.0;
	gunNumber  = 1;

	// Default x-ray field is isotropic source WITHOUT heel effect

  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

	// Initialize with a default energy, position, and momentum
	particleGun->SetParticleEnergy(20*keV);
	particleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle = particleTable->FindParticle("gamma");
  particleGun->SetParticleDefinition(particle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
	delete generatorMessenger;
	delete[] xpdf;
	delete[] ypdf;
	delete xrandGeneral;
	delete yrandGeneral;
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::PositionAndAim()
{
	// Define the correct particle
	//G4RunManager* runManager = G4RunManager::GetRunManager();
	//RunAction* runAction = (RunAction*) runManager->GetUserRunAction();

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* particle;

		particle = particleTable->FindParticle("gamma");
   //particle = particleTable->FindParticle("geantino");
    particleGun->SetParticleDefinition(particle);
		particleGun->SetParticlePosition(G4ThreeVector(dx + detectorConst->GetImagerSizeX()/2.0, -detectorConst->GetImagerSizeY()/2.0 + dy, dz));


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
		// Actual simulations, not raytracing
		for(G4int index = 0; index < gunNumber; index++){

			    G4double ux;
	    		G4double uy;
	    		G4double uz;

				ux= G4UniformRand()* detectorConst->GetImagerSizeX(); 
				uy= -(detectorConst->GetImagerSizeY()/2.0+detectorConst->GetImagerSizeY()/2.0)+G4UniformRand()*detectorConst->GetImagerSizeY();
				uz= SID;

			G4ThreeVector ShootDir = G4ThreeVector(ux,uy,uz);
      particleGun->SetParticlePosition(ShootDir);
	    particleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,-1));
			particleGun->GeneratePrimaryVertex(anEvent);
	}

	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::EffectiveAttenuation(materialType material, G4double thickness, G4double currentEnergy)
{
	// for monoenergetic
	G4double linearAttCoeff;

	if(material == polycarbonate)
		linearAttCoeff = PolycarbonateLinearAttCoeff(currentEnergy);
	else
		linearAttCoeff = AirLinearAttCoeff(currentEnergy);

	return exp(- linearAttCoeff * thickness);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::GetReferenceAirKerma(G4double numPhotons, G4bool referenceESE, G4bool referenceCompression)
{
	G4double meanAirKermaPerPhoton;
	
		meanAirKermaPerPhoton = particleGun->GetParticleEnergy() * AirMassEnergyAbsCoeff(particleGun->GetParticleEnergy());

	return meanAirKermaPerPhoton * PhotonFluenceAtReferencePoint(numPhotons, referenceESE, referenceCompression);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4double PrimaryGeneratorAction::PhotonFluenceAtReferencePoint(G4double numPhotons, G4bool referenceESE, G4bool referenceCompression)
{
	// photons per mm2 at the support plate under the focal spot (centerline at chest wall) 
	// use number of photons simulated divided by weighted detector area times exp(-miu*t) for air
	// and polycarbonate (depending on settings)
	G4double distanceOfAir;
  
   // as default, reference point is set at the suport plate
   distanceOfAir = SID - detectorConst->GetAirGap();
   
   // adjustment if reference point is to be at the entrance of the breast (ESE)
		if(referenceESE){
		distanceOfAir -= (detectorConst->GetBreastPhantom_Inferior()+detectorConst->GetBreastPhantom_Superior()+detector_deviceHalfSize*2.0);}

		// adjustment if compression plate is present in computation of reference exposure
		// compression plate IS NOT PRESENT IN THE COMPUTATION by default
		if(referenceCompression) {
		
				numPhotons *= EffectiveAttenuation(polycarbonate, detectorConst->GetPaddleThickness(), particleGun->GetParticleEnergy());
			
		}
			

 G4double photonFluence = numPhotons / (detectorConst->GetImagerSizeX() * detectorConst->GetImagerSizeY());

	return photonFluence;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void PrimaryGeneratorAction::SetSID(G4double val)
{
  // update SID and reset dx, dy and dz
	SID = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetDCOR(G4double val)
{
  // update DCOR and reset dx, dy and dz
	DCOR = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::SetDistFromCW(G4double val)
{
  // update DCOR and reset dx, dy and dz
	distFromCW = val;
	PositionAndAim();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::AirLinearAttCoeff(G4double energy)
{
	// Calculation of Air's linear attenuation coefficient for calculation of 
	// exposure at support plate using TableCurve 2D interpolation
	// ONLY VALID FROM 1 to 200 keV

	G4double x = energy/keV;
	G4double y;

	if (x > 200.0 || x < 1.0) G4cout << "WARNING: X-ray Energy is lower/higher than Air linear attenuation interpolation !!" << G4endl;

  x=logl(x);
  y=(3618.678161647859712+x*(-3865.488818351917586+
    x*(1685.960043687799660+x*(-373.5969138331777310+
    x*(42.14135517447581247+x*-1.921817306940189337)))))/
    (1.0+x*(1.786849871039888767+x*(1.186144482728460472+
    x*(0.6726680039492787047+x*(0.1463008375393740102+
    x*0.10325884386565669382)))));

  G4double airMassAttCoeff = y*(cm2/g);
	G4double airDensity = 1.205E-03*(g/cm3);

  return(airMassAttCoeff * airDensity);
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::PolycarbonateLinearAttCoeff(G4double energy)
{
	// Calculation of Compression Plate (polycarbonate) linear attenuation coefficient for calculation of 
	// exposure at support plate using TableCurve 2D interpolation
	// ONLY VALID FROM 1 to 200 keV

	G4double x = energy/keV;
	G4double y;

	if (x > 200.0 || x < 1.0) G4cout << "WARNING: X-ray Energy is lower/higher than Polycarbonate linear attenuation interpolation !!" << G4endl;

  x=logl(x);
  y=(3048.000000917903592+x*(-1772.393018934020325+
    x*(277.4739417004223587+x*(4.440711438519138541+
    x*(-0.5127391717220360950+x*-0.10092343602094603411)))))/
    (1.0+x*(2.786711727005362017+x*(-1.255696493536884278+
    x*(8.967618822334968426+x*(-5.263739352550730797+
    x*2.648265497771060221)))));

  return(y*(1.0/cm));


}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::AirMassEnergyAbsCoeff(G4double energy)
{
	// Calculation of Air's miu energy divided by rho using TableCurve 2D interpolations
	// ONLY VALID FROM 1 to 200 keV
	G4double x = energy/keV;
	G4double y;

	if (x > 200.0 || x < 1.0) G4cout << "WARNING: X-ray Energy is lower/higher than Air miu energy / rho interpolation !!" << G4endl;

	if (x <= 50.0) {
	  y=(650.4261611965774822+x*(1190.023150591885247+
	    x*(-976.1395941443393144+x*(184.3342609676975966+
	    x*(-4.803852541412326728+x*(0.02522871550584928163+
	    x*(0.0007381882561278156938+x*-1.266038559772674808E-05)))))))/
	    (1.0+x*(-2.834437284531587452+x*(3.157035296150413945+
	    x*(-1.444776429237642725+x*(0.6044254079848197341+
	    x*(-0.2270461013894292562+x*(0.03546042588056884208+
	    x*-0.0006178584484229187045)))))));
	} else {
	  G4double x1,x2,x3,x4;
	  x1=x;
	  x2=logl(x)*logl(x);
	  x3=1.0/logl(x);
	  x4=logl(x)/x;
	  y=-16.67385724790798204-0.001320780678056679182*x1
	    +0.1501918664643708148*x2+76.76977342378309974*x3
	    -65.71674422191662520*x4;
	}

  return y*(cm2/g);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
