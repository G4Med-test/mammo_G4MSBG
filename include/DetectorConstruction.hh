// DetectorConstruction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include <vector>
#include <map>

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"


enum detector_deviceType {NO,TLD};

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4PVParameterised;
class G4Material;
class DetectorMessenger;
class Voxelizer;
class G4PVReplica;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
		DetectorConstruction();
		~DetectorConstruction();
		void 								PrintGeometryParameters();
	
	// Geometry Parameters Set by User:

		G4VPhysicalVolume*	Construct();
		void 								UpdateGeometry();
    void								SetDetectorDevice(detector_deviceType val)	{detector_device = val;}
		void 								SetVisualization(G4bool);

		// Target Dimensions:

		void 								SetAirGap(G4double val)											{airGap = val;} 	
		void 								SetPaddleThickness(G4double val)            {paddleThickness = val;}
		void 								SetBreastPhantom_Inferior(G4double val)     {breastPhantom_Inferior = val;}
		void 								SetBreastPhantom_Superior(G4double val)     {breastPhantom_Superior = val;}
		
		// Imager Dimensions:
	
		void 								SetImagerSizeX(G4double);
		void 								SetImagerSizeY(G4double);
		void 								SetSupportPlate(G4bool val)									{supportPlate = val;}
		void 								SetCompressionPlate(G4bool val)							{compressionPlate = val;}
		void 								SetCoverPlate(G4bool val)										{coverPlate = val;}
		void								SetAirInWorld(G4bool val)										{airInWorld = val;}
		// Sensitivity:
	
		
		void 								SetDetectorDeviceSensitivity(G4bool val)		{DetectorDeviceSense = val;}					// change Detector Device Sensitivity
		void 								SetKillerImager(G4bool val)									{killerImager = val;}					// change Imager Killing attribute
	
		detector_deviceType					GetDetectorDevice()								  {return detector_device;}

		G4double						GetAirGap()      														{return airGap;}
		G4double						GetPaddleThickness()      								  {return paddleThickness;}	
    G4double						GetBreastPhantom_Inferior()      						{return breastPhantom_Inferior;}
    G4double						GetBreastPhantom_Superior()      						{return breastPhantom_Superior;}	
		G4double						GetpcGland()																{return pcGland;}
		G4bool							GetAirInWorld()															{return airInWorld;}

		// Imager Dimensions:
		G4double						GetImagerSizeX()      											{return imagerSizeX;}
		G4double						GetImagerSizeY()      											{return imagerSizeY;}
		G4bool							GetSupportPlate()														{return supportPlate;}
		G4bool							GetCompressionPlate()												{return compressionPlate;}
		G4bool							GetCoverPlate()															{return coverPlate;}
	
		// Sensitivity:
		G4bool							GetDetectorDeviceSensitivity()							{return DetectorDeviceSense;}
		G4bool							GetKillerImager()														{return killerImager;}
		G4bool							GetVisualization()													{return visualization;}

		G4ThreeVector breastPhantomSize_Superior;
		G4ThreeVector breastPhantomSize_Inferior;
    G4double z_half_dimension_AK;

	private:

		void 								CleanGeometry();
		void 								ConstructWorld();
    void 								ConstructBreastPhantom();
		void								ConstructDetector();
  	void								ConstructDetectorDevice();
    void                ConstructSurfaceAirKerma();
	  void 								ConstructImager();


		void 								DefineMaterials();
		G4Material*					DefineBreastMaterial(G4double);
		void 								DefineDetectors();
		void 								AssignDetectors();
		G4VPhysicalVolume*	ConstructGeometry();     

		void 								DumpGeometricalTree(G4VPhysicalVolume*, G4int depth=0);

		void 								ReadMaterialFile();

  private:

		DetectorMessenger*	detectorMessenger;
		detector_deviceType	detector_device;

 		G4bool							airInWorld;
		G4bool							overlapCheck;
		G4bool							visualization;

		// Target Dimensions:
		G4double						airGap;
		G4double						pcGland;
	
		
		// Imager Dimensions:

	  G4double						paddleThickness;	
    G4double						breastPhantom_Inferior;
    G4double						breastPhantom_Superior;			
	  G4double						imagerSizeX;
		G4double						imagerSizeY;
		G4bool							supportPlate;
		G4bool							compressionPlate;
		G4bool							coverPlate;
		// Sensitivity:;
 		G4bool							DetectorDeviceSense;
		G4bool							killerImager;

 		G4ThreeVector breastHalfSize_Image1;
 		G4ThreeVector breastHalfSize_Image2;

		G4Box*             	solidWorld;
		G4LogicalVolume*		logicWorld;
		G4VPhysicalVolume*	physiWorld;
    
    G4VSolid*           solidBreastPhantom_Superior;
		G4LogicalVolume*		logicBreastPhantom_Superior;
		G4VPhysicalVolume*	physiBreastPhantom_Superior;

    G4Box*             	solidBoxSubraction_Superior;
    G4Box*             	solidBoxSubraction_Inferior;
	

    G4VSolid*           solidBreastPhantom_Inferior;
		G4LogicalVolume*		logicBreastPhantom_Inferior;
		G4VPhysicalVolume*	physiBreastPhantom_Inferior;


    G4Box*             	solidAirKerma;
		G4LogicalVolume*	  logicAirKerma;
		G4VPhysicalVolume*	physiAirKerma;
		
		G4VSolid*						solidImager;
		G4VSolid*						solidCompression;
		G4VSolid*						solidSupport;
		G4VSolid*						solidCoverAl;
		G4VSolid*						solidCoverC;
		G4VSolid*						solidImagerHolder;

		G4LogicalVolume*		logicImager;
		G4LogicalVolume*		logicCompression;
		G4LogicalVolume*		logicSupport;  
		G4LogicalVolume*		logicCoverAl;  
		G4LogicalVolume*		logicCoverC;  
		G4LogicalVolume* 		logicImagerHolder;

		G4VPhysicalVolume*	physiImager;
		G4VPhysicalVolume*	physiCompression;
		G4VPhysicalVolume*	physiSupport;
		G4VPhysicalVolume*	physiCoverAlUnder;
		G4VPhysicalVolume*	physiCoverC;
		G4VPhysicalVolume*	physiCoverAlOver;
		G4VPhysicalVolume*	physiImagerHolder;

G4Box*             	solidTld_1;
		G4LogicalVolume*	logicTld_1;
		G4VPhysicalVolume*	physiTld_1;

G4Box*             	solidTld_2;
		G4LogicalVolume*	logicTld_2;
		G4VPhysicalVolume*	physiTld_2;

G4Box*             	solidTld_3;
		G4LogicalVolume*	logicTld_3;
		G4VPhysicalVolume*	physiTld_3;

G4Box*             	solidTld_4;
		G4LogicalVolume*	logicTld_4;
		G4VPhysicalVolume*	physiTld_4;

G4Box*             	solidTld_5;
		G4LogicalVolume*	logicTld_5;
		G4VPhysicalVolume*	physiTld_5;

G4Box*             	solidTld_6;
		G4LogicalVolume*	logicTld_6;
		G4VPhysicalVolume*	physiTld_6;

G4Box*             	solidTld_7;
		G4LogicalVolume*	logicTld_7;
		G4VPhysicalVolume*	physiTld_7;

G4Box*             	solidTld_8;
		G4LogicalVolume*	logicTld_8;
		G4VPhysicalVolume*	physiTld_8;

G4Box*             	solidTld_9;
		G4LogicalVolume*	logicTld_9;
		G4VPhysicalVolume*	physiTld_9;

G4Box*             	solidTld_10;
		G4LogicalVolume*	logicTld_10;
		G4VPhysicalVolume*	physiTld_10;

G4Box*             	solidTld_11;
		G4LogicalVolume*	logicTld_11;
		G4VPhysicalVolume*	physiTld_11;

G4Box*             	solidTld_12;
		G4LogicalVolume*	logicTld_12;
		G4VPhysicalVolume*	physiTld_12;

G4Box*             	solidTld_13;
		G4LogicalVolume*	logicTld_13;
		G4VPhysicalVolume*	physiTld_13;

G4Box*             	solidTld_14;
		G4LogicalVolume*	logicTld_14;
		G4VPhysicalVolume*	physiTld_14;

G4Box*             	solidTld_15;
		G4LogicalVolume*	logicTld_15;
		G4VPhysicalVolume*	physiTld_15;

G4Box*             	solidTld_16;
		G4LogicalVolume*	logicTld_16;
		G4VPhysicalVolume*	physiTld_16;

G4Box*             	solidTld_17;
		G4LogicalVolume*	logicTld_17;
		G4VPhysicalVolume*	physiTld_17;

G4Box*             	solidTld_18;
		G4LogicalVolume*	logicTld_18;
		G4VPhysicalVolume*	physiTld_18;

G4Box*             	solidTld_19;
		G4LogicalVolume*	logicTld_19;
		G4VPhysicalVolume*	physiTld_19;

G4Box*             	solidTld_20;
		G4LogicalVolume*	logicTld_20;
		G4VPhysicalVolume*	physiTld_20;

G4Box*             	solidTld_21;
		G4LogicalVolume*	logicTld_21;
		G4VPhysicalVolume*	physiTld_21;

G4Box*             	solidTld_22;
		G4LogicalVolume*	logicTld_22;
		G4VPhysicalVolume*	physiTld_22;

G4Box*             	solidTld_23;
		G4LogicalVolume*	logicTld_23;
		G4VPhysicalVolume*	physiTld_23;

G4Box*             	solidTld_24;
		G4LogicalVolume*	logicTld_24;
		G4VPhysicalVolume*	physiTld_24;

G4Box*             	solidTld_25;
		G4LogicalVolume*	logicTld_25;
		G4VPhysicalVolume*	physiTld_25;

G4Box*             	solidTld_26;
		G4LogicalVolume*	logicTld_26;
		G4VPhysicalVolume*	physiTld_26;

G4Box*             	solidTld_27;
		G4LogicalVolume*	logicTld_27;
		G4VPhysicalVolume*	physiTld_27;

G4Box*             	solidTld_28;
		G4LogicalVolume*	logicTld_28;
		G4VPhysicalVolume*	physiTld_28;

G4Box*             	solidTld_29;
		G4LogicalVolume*	logicTld_29;
		G4VPhysicalVolume*	physiTld_29;

G4Box*             	solidTld_30;
		G4LogicalVolume*	logicTld_30;
		G4VPhysicalVolume*	physiTld_30;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
