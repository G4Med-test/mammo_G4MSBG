// DetectorConstruction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "TldSD.hh"
#include "PrimaryGeneratorAction.hh"

#include "Kerma_Evaluation.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

#include "G4ParticleGun.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Ellipsoid.hh"
#include "G4EllipticalTube.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SubtractionSolid.hh"

#include "G4RunManager.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include <fstream>

G4double detector_deviceHalfSize = 0.0*mm;          //Initialize the Devicehalfsize        NOT TO BE CHANGED

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:supportPlate(true), compressionPlate(true), coverPlate(true)
{
  // default parameter values of the geometry
	airGap    = 1.7*cm;

  paddleThickness = 2.0*mm;
  breastPhantom_Inferior = 1.0*cm;
  breastPhantom_Superior = 1.0*cm;

	imagerSizeX = 12.0*cm;
	imagerSizeY = 20.0*cm;

	airInWorld = true;
	visualization = false;
	killerImager = true;
	overlapCheck = true;

	DetectorDeviceSense = false; 
	detector_device     = TLD;
	
  // create commands for interactive definition of the breast phantom (target)
  detectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::~DetectorConstruction()
{
  delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterials();
	G4VPhysicalVolume* myWorld = ConstructGeometry();
	DefineDetectors();  
	AssignDetectors();

	return myWorld;
}
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructGeometry()
{
  //----------------------------------------------------------------------------
  //      Definitions of Solids, Logical Volumes, Physical Volumes 
  //----------------------------------------------------------------------------	

	CleanGeometry();

	ConstructWorld();
  ConstructBreastPhantom();
  ConstructDetectorDevice();
  ConstructSurfaceAirKerma();
	ConstructImager();      
	return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::CleanGeometry()
{
  // Clean old geometry, if any
  
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructWorld()
{
  //-------------
  // World Volume 
  //-------------

	// Calculation of Dimensions
  G4ThreeVector worldSize = G4ThreeVector(100.0*cm, 150.0*cm, 200.0*cm);

	// Creation of Solid
  solidWorld = new G4Box("world", worldSize.x()/2.0, worldSize.y()/2.0, worldSize.z()/2.0);

	// Logic and physical
	G4Material* worldMaterial;
	if(airInWorld)
	  worldMaterial = G4Material::GetMaterial("G4_AIR");
	else
	  worldMaterial = G4Material::GetMaterial("G4_Galactic");

  logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "World", 0, 0, 0);

  physiWorld = new G4PVPlacement(0,               // no rotation
			                           G4ThreeVector(), // at (0,0,0)
                            		logicWorld,       // its logical volume
                           			"World",          // its name
                           			0,                // its mother  volume
                           			false,            // no boolean operations
	                           		0,	              // copy number
																overlapCheck);	  // perform overlap check

	// Attributes
  G4VisAttributes* worldVisAtt= new G4VisAttributes();
	worldVisAtt->SetColor(255/255., 255/255., 255/255., 1.0);
	worldVisAtt->SetVisibility(true);
 	worldVisAtt->SetForceWireframe(true);
	logicWorld->SetVisAttributes(worldVisAtt);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::ConstructBreastPhantom()
{
//-------------
// Breast Phantom Volume
//-------------

	 // G4ThreeVector breastPhantomSize_Inferior;
		breastPhantomSize_Inferior = G4ThreeVector(10.0*cm, 9.0*cm, breastPhantom_Inferior);

		if (breastPhantom_Superior !=0.0){
	// G4ThreeVector breastPhantomSize_Superior;
		breastPhantomSize_Superior = G4ThreeVector(10.0*cm, 9.0*cm, breastPhantom_Superior);
		
	//---
		G4ThreeVector breastPhantomPosition_Inferior;
		breastPhantomPosition_Inferior = G4ThreeVector(0.0,
																 									 - imagerSizeY/2.0,
																									 breastPhantomSize_Inferior.z()/2.0 + airGap);     

		G4ThreeVector breastPhantomPosition_Superior;
		breastPhantomPosition_Superior = G4ThreeVector(0.0,
																 									 - imagerSizeY/2.0,
																									 breastPhantomSize_Superior.z()/2.0 + detector_deviceHalfSize*2.0+ breastPhantomSize_Inferior.z() + airGap); 

  // Creation of Solid

	solidBoxSubraction_Inferior = new G4Box("solidBoxSubraction_Inferior", breastPhantomSize_Inferior.x()/2.0+0.1*cm, breastPhantomSize_Inferior.y(), breastPhantomSize_Inferior.z());

	solidBoxSubraction_Superior = new G4Box("solidBoxSubraction_Superior", breastPhantomSize_Superior.x()/2.0+0.1*cm, breastPhantomSize_Superior.y(), breastPhantomSize_Superior.z());

	solidBreastPhantom_Inferior = new G4EllipticalTube("solidBreastPhantom_Inferior",   breastPhantomSize_Inferior.x(), breastPhantomSize_Inferior.y(), breastPhantomSize_Inferior.z()/2.0);

	solidBreastPhantom_Superior = new G4EllipticalTube("solidBreastPhantom_Superior",   breastPhantomSize_Superior.x(), breastPhantomSize_Superior.y(), breastPhantomSize_Superior.z()/2.0);

 // Boolean Operations on Cut the ellipticalTube

	G4VSolid* sub_solidBreastPhantom_Inferior = new G4SubtractionSolid("sub_solidBreastPhantom_Inferior", solidBreastPhantom_Inferior, solidBoxSubraction_Inferior, 0, G4ThreeVector(-(breastPhantomSize_Inferior.x()/2.0+0.1*cm), 0.0 , 0.0));

 G4VSolid* sub_solidBreastPhantom_Superior = new G4SubtractionSolid("sub_solidBreastPhantom_Superior", solidBreastPhantom_Superior, solidBoxSubraction_Superior, 0, G4ThreeVector(-(breastPhantomSize_Superior.x()/2.0+0.1*cm), 0.0 , 0.0));


	G4Material* breastPhantomMaterial = G4Material::GetMaterial("breastMaterialCIRS");

	logicBreastPhantom_Inferior = new G4LogicalVolume(sub_solidBreastPhantom_Inferior,
																										breastPhantomMaterial,
																										"logicBreastPhantom_Inferior",
																										0,0,0);

 	logicBreastPhantom_Superior = new G4LogicalVolume(sub_solidBreastPhantom_Superior,
																										breastPhantomMaterial,
																										"logicBreastPhantom_Superior",
																										0,0,0); 

	// Physical
	physiBreastPhantom_Inferior = new G4PVPlacement(0,
																									breastPhantomPosition_Inferior,
																									logicBreastPhantom_Inferior,
																									"physiBreastPhantom_Inferior",
																									logicWorld,
																									false,
                 																	0,	             
																									overlapCheck);

	physiBreastPhantom_Superior = new G4PVPlacement(0,
																									breastPhantomPosition_Superior,
																									logicBreastPhantom_Superior,
																									"physiBreastPhantom_Superior",
																									logicWorld,
																									false,
                 																	0,	             
																									overlapCheck);

// Visualization of BreastPhantom_Inferior
  G4VisAttributes* BreastPhantom_InferiorVisAtt= new G4VisAttributes(); 
	BreastPhantom_InferiorVisAtt->SetColor(0., 0., 1.); // Blue
	BreastPhantom_InferiorVisAtt->SetVisibility(true);
 //	BreastPhantom_InferiorVisAtt->SetForceWireframe(true);
	BreastPhantom_InferiorVisAtt->SetForceSolid(true);
	logicBreastPhantom_Inferior->SetVisAttributes(BreastPhantom_InferiorVisAtt);	


// Visualization of BreastPhantom_Superior
  G4VisAttributes* BreastPhantom_SuperiorVisAtt= new G4VisAttributes(); 
	BreastPhantom_SuperiorVisAtt->SetColor(1., 0., 0.); // Red
	BreastPhantom_SuperiorVisAtt->SetVisibility(true);
 //	BreastPhantom_SuperiorVisAtt->SetForceWireframe(true);
	BreastPhantom_SuperiorVisAtt->SetForceSolid(true);
	logicBreastPhantom_Superior->SetVisAttributes(BreastPhantom_SuperiorVisAtt);	 


	} else {

    breastPhantom_Superior = 0.0000001*cm; //1nm
    breastPhantomSize_Superior = G4ThreeVector(10.0*cm, 9.0*cm, breastPhantom_Superior);
    
	//---
		G4ThreeVector breastPhantomPosition_Inferior;
		breastPhantomPosition_Inferior = G4ThreeVector(0.0,
																 									 - imagerSizeY/2.0,
																									 breastPhantomSize_Inferior.z()/2.0 + airGap);     

		G4ThreeVector breastPhantomPosition_Superior;
		breastPhantomPosition_Superior = G4ThreeVector(0.0,
																 									 - imagerSizeY/2.0,
																									 breastPhantomSize_Superior.z()/2.0 + detector_deviceHalfSize*2.0+ breastPhantomSize_Inferior.z() + airGap); 

  // Creation of Solid

	solidBoxSubraction_Inferior = new G4Box("solidBoxSubraction_Inferior", breastPhantomSize_Inferior.x()/2.0+0.1*cm, breastPhantomSize_Inferior.y(), breastPhantomSize_Inferior.z());

	solidBoxSubraction_Superior = new G4Box("solidBoxSubraction_Superior", breastPhantomSize_Superior.x()/2.0+0.1*cm, breastPhantomSize_Superior.y(), breastPhantomSize_Superior.z());

	solidBreastPhantom_Inferior = new G4EllipticalTube("solidBreastPhantom_Inferior",   breastPhantomSize_Inferior.x(), breastPhantomSize_Inferior.y(), breastPhantomSize_Inferior.z()/2.0);

	solidBreastPhantom_Superior = new G4EllipticalTube("solidBreastPhantom_Superior",   breastPhantomSize_Superior.x(), breastPhantomSize_Superior.y(), breastPhantomSize_Superior.z()/2.0);

 // Boolean Operations on Cut the ellipticalTube

	G4VSolid* sub_solidBreastPhantom_Inferior = new G4SubtractionSolid("sub_solidBreastPhantom_Inferior", solidBreastPhantom_Inferior, solidBoxSubraction_Inferior, 0, G4ThreeVector(-(breastPhantomSize_Inferior.x()/2.0+0.1*cm), 0.0 , 0.0));

 G4VSolid* sub_solidBreastPhantom_Superior = new G4SubtractionSolid("sub_solidBreastPhantom_Superior", solidBreastPhantom_Superior, solidBoxSubraction_Superior, 0, G4ThreeVector(-(breastPhantomSize_Superior.x()/2.0+0.1*cm), 0.0 , 0.0));

	G4Material* breastPhantomMaterial     = G4Material::GetMaterial("breastMaterialCIRS");
  G4Material* breastPhantomMaterial_air = G4Material::GetMaterial("G4_AIR");

	logicBreastPhantom_Inferior = new G4LogicalVolume(sub_solidBreastPhantom_Inferior,
																										breastPhantomMaterial,
																										"logicBreastPhantom_Inferior",
																										0,0,0);

 	logicBreastPhantom_Superior = new G4LogicalVolume(sub_solidBreastPhantom_Superior,
																										breastPhantomMaterial_air,
																										"logicBreastPhantom_Superior",
																										0,0,0); 

	// Physical
	physiBreastPhantom_Inferior = new G4PVPlacement(0,
																									breastPhantomPosition_Inferior,
																									logicBreastPhantom_Inferior,
																									"physiBreastPhantom_Inferior",
																									logicWorld,
																									false,
                 																	0,	             
																									overlapCheck);

	physiBreastPhantom_Superior = new G4PVPlacement(0,
																									breastPhantomPosition_Superior,
																									logicBreastPhantom_Superior,
																									"physiBreastPhantom_Superior",
																									logicWorld,
																									false,
                 																	0,	             
																									overlapCheck);

// Visualization of BreastPhantom_Inferior
  G4VisAttributes* BreastPhantom_InferiorVisAtt= new G4VisAttributes(); 
	BreastPhantom_InferiorVisAtt->SetColor(0., 0., 1.); // Blue
	BreastPhantom_InferiorVisAtt->SetVisibility(true);
 //	BreastPhantom_InferiorVisAtt->SetForceWireframe(true);
	BreastPhantom_InferiorVisAtt->SetForceSolid(true);
	logicBreastPhantom_Inferior->SetVisAttributes(BreastPhantom_InferiorVisAtt);	


// Visualization of BreastPhantom_Superior
  G4VisAttributes* BreastPhantom_SuperiorVisAtt= new G4VisAttributes(); 
	BreastPhantom_SuperiorVisAtt->SetColor(1., 0., 0.); // Red
	BreastPhantom_SuperiorVisAtt->SetVisibility(true);
 //	BreastPhantom_SuperiorVisAtt->SetForceWireframe(true);
	BreastPhantom_SuperiorVisAtt->SetForceSolid(true);
	logicBreastPhantom_Superior->SetVisAttributes(BreastPhantom_SuperiorVisAtt);	 




	}		
}

void DetectorConstruction::ConstructDetectorDevice()
{
  //-------------
  // Dose Detector Device
  //-------------

	if (detector_device == NO) {

  detector_deviceHalfSize=0.0*mm;

	} else {

  G4Material* MaterialTld;
 	MaterialTld= G4Material::GetMaterial("tldMaterial");
 
	detector_deviceHalfSize   =0.19*mm;  //this is half height of TLD device
	G4ThreeVector tldHalfSize = G4ThreeVector(1.6*mm, 1.6*mm, detector_deviceHalfSize);

  // (x,y) Axial Coordinate of all TLDs
  G4double x_coord_Tld_1 = 0.7*cm; //RED
	G4double y_coord_Tld_1 = 8.0*cm;

  G4double x_coord_Tld_2 = 2.8*cm; //Violet
	G4double y_coord_Tld_2 = 8.0*cm;

  G4double x_coord_Tld_3 = 4.8*cm; //Red
	G4double y_coord_Tld_3 = 7.5*cm;

  G4double x_coord_Tld_4 = 6.6*cm; //Violet
	G4double y_coord_Tld_4 = 6.2*cm;

  G4double x_coord_Tld_5 = 0.7*cm; //orange
	G4double y_coord_Tld_5 = 5.0*cm;

  G4double x_coord_Tld_6 = 2.8*cm; //blue
	G4double y_coord_Tld_6 = 5.0*cm;

  G4double x_coord_Tld_7 = 4.8*cm; 
	G4double y_coord_Tld_7 = 5.0*cm;

  G4double x_coord_Tld_8 = 8.3*cm; 
	G4double y_coord_Tld_8 = 3.3*cm;

	G4double x_coord_Tld_9 = 6.6*cm; 
	G4double y_coord_Tld_9 = 3.0*cm;

	G4double x_coord_Tld_10 = 0.7*cm; 
	G4double y_coord_Tld_10 = 2.5*cm;

	G4double x_coord_Tld_11 = 2.8*cm; 
	G4double y_coord_Tld_11 = 2.5*cm;

  G4double x_coord_Tld_12 = 4.8*cm;
	G4double y_coord_Tld_12 = 2.5*cm;

  G4double x_coord_Tld_13 = 0.7*cm; 
	G4double y_coord_Tld_13 = 0.0*cm;

  G4double x_coord_Tld_14 = 2.8*cm; 
	G4double y_coord_Tld_14 = 0.0*cm;

  G4double x_coord_Tld_15 = 4.8*cm; 
	G4double y_coord_Tld_15 = 0.0*cm;

  G4double x_coord_Tld_16 = 6.6*cm; 
	G4double y_coord_Tld_16 = 0.0*cm;

  G4double x_coord_Tld_17 = 8.3*cm; 
	G4double y_coord_Tld_17 = 0.0*cm;

	G4double x_coord_Tld_18 = 9.8*cm; 
	G4double y_coord_Tld_18 = 0.0*cm;

	G4double x_coord_Tld_19 = 0.7*cm; 
	G4double y_coord_Tld_19 = -2.5*cm;

	G4double x_coord_Tld_20 = 2.8*cm; 
	G4double y_coord_Tld_20 = -2.5*cm;

	G4double x_coord_Tld_21 = 4.8*cm; 
	G4double y_coord_Tld_21 = -2.5*cm;

  G4double x_coord_Tld_22 = 6.6*cm;
	G4double y_coord_Tld_22 = -3.0*cm;

  G4double x_coord_Tld_23 = 8.3*cm; 
	G4double y_coord_Tld_23 = -3.3*cm;

  G4double x_coord_Tld_24 = 0.7*cm; 
	G4double y_coord_Tld_24 = -5.0*cm;

  G4double x_coord_Tld_25 = 2.8*cm; 
	G4double y_coord_Tld_25 = -5.0*cm;

  G4double x_coord_Tld_26 = 4.8*cm; 
	G4double y_coord_Tld_26 = -5.0*cm;

  G4double x_coord_Tld_27 = 6.6*cm; 
	G4double y_coord_Tld_27 = -6.2*cm;

	G4double x_coord_Tld_28 = 4.8*cm; 
	G4double y_coord_Tld_28 = -7.5*cm;

	G4double x_coord_Tld_29 = 0.7*cm; 
	G4double y_coord_Tld_29 = -8.0*cm;

	G4double x_coord_Tld_30 = 2.8*cm; 
	G4double y_coord_Tld_30 = -8.0*cm;

	G4ThreeVector Position_Tld_1;
	G4ThreeVector Position_Tld_2;
	G4ThreeVector Position_Tld_3;
	G4ThreeVector Position_Tld_4;
	G4ThreeVector Position_Tld_5;
	G4ThreeVector Position_Tld_6;
	G4ThreeVector Position_Tld_7;
	G4ThreeVector Position_Tld_8;
	G4ThreeVector Position_Tld_9;
	G4ThreeVector Position_Tld_10;
	G4ThreeVector Position_Tld_11;
	G4ThreeVector Position_Tld_12;
	G4ThreeVector Position_Tld_13;
	G4ThreeVector Position_Tld_14;
	G4ThreeVector Position_Tld_15;
	G4ThreeVector Position_Tld_16;
	G4ThreeVector Position_Tld_17;
	G4ThreeVector Position_Tld_18;
	G4ThreeVector Position_Tld_19;
	G4ThreeVector Position_Tld_20;
	G4ThreeVector Position_Tld_21;
	G4ThreeVector Position_Tld_22;
	G4ThreeVector Position_Tld_23;
	G4ThreeVector Position_Tld_24;
	G4ThreeVector Position_Tld_25;
	G4ThreeVector Position_Tld_26;
	G4ThreeVector Position_Tld_27;
	G4ThreeVector Position_Tld_28;
	G4ThreeVector Position_Tld_29;
	G4ThreeVector Position_Tld_30;

	Position_Tld_1 = G4ThreeVector(x_coord_Tld_1,- imagerSizeY/2.0+ y_coord_Tld_1, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_2 = G4ThreeVector(x_coord_Tld_2, - imagerSizeY/2.0+ y_coord_Tld_2, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_3 = G4ThreeVector(x_coord_Tld_3, - imagerSizeY/2.0+ y_coord_Tld_3, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_4 = G4ThreeVector(x_coord_Tld_4, - imagerSizeY/2.0+ y_coord_Tld_4, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_5 = G4ThreeVector(x_coord_Tld_5, - imagerSizeY/2.0+ y_coord_Tld_5, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_6 = G4ThreeVector(x_coord_Tld_6, - imagerSizeY/2.0+ y_coord_Tld_6, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_7 = G4ThreeVector(x_coord_Tld_7, - imagerSizeY/2.0+ y_coord_Tld_7, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_8 = G4ThreeVector(x_coord_Tld_8, - imagerSizeY/2.0+ y_coord_Tld_8, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_9 = G4ThreeVector(x_coord_Tld_9, - imagerSizeY/2.0+ y_coord_Tld_9, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_10 = G4ThreeVector(x_coord_Tld_10, - imagerSizeY/2.0+ y_coord_Tld_10, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_11 = G4ThreeVector(x_coord_Tld_11, - imagerSizeY/2.0+ y_coord_Tld_11, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_12 = G4ThreeVector(x_coord_Tld_12, - imagerSizeY/2.0+ y_coord_Tld_12, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_13 = G4ThreeVector(x_coord_Tld_13, - imagerSizeY/2.0+ y_coord_Tld_13, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_14 = G4ThreeVector(x_coord_Tld_14, - imagerSizeY/2.0+ y_coord_Tld_14, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_15 = G4ThreeVector(x_coord_Tld_15,- imagerSizeY/2.0+  y_coord_Tld_15, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_16 = G4ThreeVector(x_coord_Tld_16, - imagerSizeY/2.0+ y_coord_Tld_16, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_17 = G4ThreeVector(x_coord_Tld_17, - imagerSizeY/2.0+ y_coord_Tld_17, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_18 = G4ThreeVector(x_coord_Tld_18, - imagerSizeY/2.0+ y_coord_Tld_18, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_19 = G4ThreeVector(x_coord_Tld_19, - imagerSizeY/2.0+ y_coord_Tld_19, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_20 = G4ThreeVector(x_coord_Tld_20, - imagerSizeY/2.0+ y_coord_Tld_20, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_21 = G4ThreeVector(x_coord_Tld_21,- imagerSizeY/2.0+ y_coord_Tld_21, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_22 = G4ThreeVector(x_coord_Tld_22, - imagerSizeY/2.0+ y_coord_Tld_22, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_23 = G4ThreeVector(x_coord_Tld_23, - imagerSizeY/2.0+ y_coord_Tld_23, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_24 = G4ThreeVector(x_coord_Tld_24, - imagerSizeY/2.0+ y_coord_Tld_24, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_25 = G4ThreeVector(x_coord_Tld_25, - imagerSizeY/2.0+ y_coord_Tld_25, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_26 = G4ThreeVector(x_coord_Tld_26, - imagerSizeY/2.0+ y_coord_Tld_26, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_27 = G4ThreeVector(x_coord_Tld_27, - imagerSizeY/2.0+ y_coord_Tld_27, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_28 = G4ThreeVector(x_coord_Tld_28, - imagerSizeY/2.0+ y_coord_Tld_28, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_29 = G4ThreeVector(x_coord_Tld_29, - imagerSizeY/2.0+ y_coord_Tld_29, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);
	Position_Tld_30 = G4ThreeVector(x_coord_Tld_30, - imagerSizeY/2.0+ y_coord_Tld_30, airGap +  breastPhantomSize_Inferior.z() + detector_deviceHalfSize);

//---------------------------------------------- TLD 1

   solidTld_1 = new G4Box("solidTld_1", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_1 = new G4LogicalVolume(solidTld_1, MaterialTld, "logicTld_1", 0, 0, 0);
   physiTld_1 = new G4PVPlacement(0,		// no rotation
				Position_Tld_1,		
				logicTld_1,  		      // its logical volume
				"physiTld_1",         //its name
				logicWorld,           // its mother  volume
				false,                // no boolean operations
		        0,	              // copy number
			      overlapCheck);	  // perform overlap check
  // Attributes Tld 1
  G4VisAttributes* Tld_1VisAtt= new G4VisAttributes(); // RED
	Tld_1VisAtt->SetColor(1., 0., 0.);
	Tld_1VisAtt->SetVisibility(true);
 	//Tld_1VisAtt->SetForceWireframe(true);
	Tld_1VisAtt->SetForceSolid(true);
	logicTld_1->SetVisAttributes(Tld_1VisAtt);
//---------------------------------------------- 

   solidTld_2 = new G4Box("solidTld_2", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_2 = new G4LogicalVolume(solidTld_2, MaterialTld, "logicTld_2", 0, 0, 0);
   physiTld_2 = new G4PVPlacement(0,Position_Tld_2,logicTld_2,"physiTld_2",logicWorld,false,0,overlapCheck);
  // Attributes Tld 2
  G4VisAttributes* Tld_2VisAtt= new G4VisAttributes(); // VIOLET
	Tld_2VisAtt->SetColor(0.5, 0.5, 1.);
	Tld_2VisAtt->SetVisibility(true);
	Tld_2VisAtt->SetForceSolid(true);
	logicTld_2->SetVisAttributes(Tld_2VisAtt); 

//----------------------------------------------

solidTld_3 = new G4Box("solidTld_3", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_3 = new G4LogicalVolume(solidTld_3, MaterialTld, "logicTld_3", 0, 0, 0);
   physiTld_3 = new G4PVPlacement(0,Position_Tld_3,logicTld_3,"physiTld_3",logicWorld,false,0,overlapCheck);
  // Attributes Tld 3
  G4VisAttributes* Tld_3VisAtt= new G4VisAttributes(); // red
	Tld_3VisAtt->SetColor(1, 0., 0.);
	Tld_3VisAtt->SetVisibility(true);
	Tld_3VisAtt->SetForceSolid(true);
	logicTld_3->SetVisAttributes(Tld_3VisAtt); 

//----------------------------------------------
solidTld_4 = new G4Box("solidTld_4", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_4 = new G4LogicalVolume(solidTld_4, MaterialTld, "logicTld_4", 0, 0, 0);
   physiTld_4 = new G4PVPlacement(0,Position_Tld_4,logicTld_4,"physiTld_4",logicWorld,false,0,overlapCheck);
  // Attributes Tld 4
  G4VisAttributes* Tld_4VisAtt= new G4VisAttributes(); // VIOLET
	Tld_4VisAtt->SetColor(0.5, 0.5, 1.);
	Tld_4VisAtt->SetVisibility(true);
	Tld_4VisAtt->SetForceSolid(true);
	logicTld_4->SetVisAttributes(Tld_4VisAtt); 

//----------------------------------------------
solidTld_5 = new G4Box("solidTld_5", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_5 = new G4LogicalVolume(solidTld_5, MaterialTld, "logicTld_5", 0, 0, 0);
   physiTld_5 = new G4PVPlacement(0,Position_Tld_5,logicTld_5,"physiTld_5",logicWorld,false,0,overlapCheck);
  // Attributes Tld 5
  G4VisAttributes* Tld_5VisAtt= new G4VisAttributes(); // orange
	Tld_5VisAtt->SetColor(1.0, 0.5, 0.);
	Tld_5VisAtt->SetVisibility(true);
	Tld_5VisAtt->SetForceSolid(true);
	logicTld_5->SetVisAttributes(Tld_5VisAtt); 

//----------------------------------------------
solidTld_6 = new G4Box("solidTld_6", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_6 = new G4LogicalVolume(solidTld_6, MaterialTld, "logicTld_6", 0, 0, 0);
   physiTld_6 = new G4PVPlacement(0,Position_Tld_6,logicTld_6,"physiTld_6",logicWorld,false,0,overlapCheck);
  // Attributes Tld 6
  G4VisAttributes* Tld_6VisAtt= new G4VisAttributes(); // Blue
	Tld_6VisAtt->SetColor(0., 0., 1.);
	Tld_6VisAtt->SetVisibility(true);
	Tld_6VisAtt->SetForceSolid(true);
	logicTld_6->SetVisAttributes(Tld_6VisAtt); 

//----------------------------------------------
solidTld_7 = new G4Box("solidTld_7", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_7 = new G4LogicalVolume(solidTld_7, MaterialTld, "logicTld_7", 0, 0, 0);
   physiTld_7 = new G4PVPlacement(0,Position_Tld_7,logicTld_7,"physiTld_7",logicWorld,false,0,overlapCheck);
  // Attributes Tld 7
  G4VisAttributes* Tld_7VisAtt= new G4VisAttributes(); // orange
	Tld_7VisAtt->SetColor(1.0, 0.5, 0.);
	Tld_7VisAtt->SetVisibility(true);
	Tld_7VisAtt->SetForceSolid(true);
	logicTld_7->SetVisAttributes(Tld_7VisAtt); 

//----------------------------------------------
solidTld_8 = new G4Box("solidTld_8", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_8 = new G4LogicalVolume(solidTld_8, MaterialTld, "logicTld_8", 0, 0, 0);
   physiTld_8 = new G4PVPlacement(0,Position_Tld_8,logicTld_8,"physiTld_8",logicWorld,false,0,overlapCheck);
  // Attributes Tld 8
  G4VisAttributes* Tld_8VisAtt= new G4VisAttributes(); // orange
	Tld_8VisAtt->SetColor(1.0, 0.5, 0.);
	Tld_8VisAtt->SetVisibility(true);
	Tld_8VisAtt->SetForceSolid(true);
	logicTld_8->SetVisAttributes(Tld_8VisAtt); 

//----------------------------------------------
solidTld_9 = new G4Box("solidTld_9", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_9 = new G4LogicalVolume(solidTld_9, MaterialTld, "logicTld_9", 0, 0, 0);
   physiTld_9 = new G4PVPlacement(0,Position_Tld_9,logicTld_9,"physiTld_9",logicWorld,false,0,overlapCheck);
  // Attributes Tld 9
  G4VisAttributes* Tld_9VisAtt= new G4VisAttributes(); // Blue
	Tld_9VisAtt->SetColor(0., 0., 1.);
	Tld_9VisAtt->SetVisibility(true);
	Tld_9VisAtt->SetForceSolid(true);
	logicTld_9->SetVisAttributes(Tld_9VisAtt); 

//----------------------------------------------
solidTld_10 = new G4Box("solidTld_10", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_10 = new G4LogicalVolume(solidTld_10, MaterialTld, "logicTld_10", 0, 0, 0);
   physiTld_10 = new G4PVPlacement(0,Position_Tld_10,logicTld_10,"physiTld_10",logicWorld,false,0,overlapCheck);
  // Attributes Tld 10
  G4VisAttributes* Tld_10VisAtt= new G4VisAttributes(); // yellow
	Tld_10VisAtt->SetColor(1.0, 1.0, 0.);
	Tld_10VisAtt->SetVisibility(true);
	Tld_10VisAtt->SetForceSolid(true);
	logicTld_10->SetVisAttributes(Tld_10VisAtt); 

//----------------------------------------------
solidTld_11 = new G4Box("solidTld_11", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_11 = new G4LogicalVolume(solidTld_11, MaterialTld, "logicTld_11", 0, 0, 0);
   physiTld_11 = new G4PVPlacement(0,Position_Tld_11,logicTld_11,"physiTld_11",logicWorld,false,0,overlapCheck);
  // Attributes Tld 11
  G4VisAttributes* Tld_11VisAtt= new G4VisAttributes(); // cyan
	Tld_11VisAtt->SetColor(0., 1.0, 1.);
	Tld_11VisAtt->SetVisibility(true);
	Tld_11VisAtt->SetForceSolid(true);
	logicTld_11->SetVisAttributes(Tld_11VisAtt); 

//----------------------------------------------
solidTld_12 = new G4Box("solidTld_12", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_12 = new G4LogicalVolume(solidTld_12, MaterialTld, "logicTld_12", 0, 0, 0);
   physiTld_12 = new G4PVPlacement(0,Position_Tld_12,logicTld_12,"physiTld_12",logicWorld,false,0,overlapCheck);
  // Attributes Tld 12
  G4VisAttributes* Tld_12VisAtt= new G4VisAttributes(); // yellow
	Tld_12VisAtt->SetColor(1., 1., 0.);
	Tld_12VisAtt->SetVisibility(true);
	Tld_12VisAtt->SetForceSolid(true);
	logicTld_12->SetVisAttributes(Tld_12VisAtt); 

//----------------------------------------------
solidTld_13 = new G4Box("solidTld_13", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_13 = new G4LogicalVolume(solidTld_13, MaterialTld, "logicTld_13", 0, 0, 0);
   physiTld_13 = new G4PVPlacement(0,Position_Tld_13,logicTld_13,"physiTld_13",logicWorld,false,0,overlapCheck);
  // Attributes Tld 13
  G4VisAttributes* Tld_13VisAtt= new G4VisAttributes(); // green
	Tld_13VisAtt->SetColor(0., 1., 0.);
	Tld_13VisAtt->SetVisibility(true);
	Tld_13VisAtt->SetForceSolid(true);
	logicTld_13->SetVisAttributes(Tld_13VisAtt); 

//----------------------------------------------
solidTld_14 = new G4Box("solidTld_14", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_14 = new G4LogicalVolume(solidTld_14, MaterialTld, "logicTld_14", 0, 0, 0);
   physiTld_14 = new G4PVPlacement(0,Position_Tld_14,logicTld_14,"physiTld_14",logicWorld,false,0,overlapCheck);
  // Attributes Tld 14
  G4VisAttributes* Tld_14VisAtt= new G4VisAttributes(); // green
	Tld_14VisAtt->SetColor(0., 1., 0.);
	Tld_14VisAtt->SetVisibility(true);
	Tld_14VisAtt->SetForceSolid(true);
	logicTld_14->SetVisAttributes(Tld_14VisAtt); 

//----------------------------------------------
solidTld_15 = new G4Box("solidTld_15", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_15 = new G4LogicalVolume(solidTld_15, MaterialTld, "logicTld_15", 0, 0, 0);
   physiTld_15 = new G4PVPlacement(0,Position_Tld_15,logicTld_15,"physiTld_15",logicWorld,false,0,overlapCheck);
  // Attributes Tld 15
  G4VisAttributes* Tld_15VisAtt= new G4VisAttributes(); // green
	Tld_15VisAtt->SetColor(0., 1., 0.);
	Tld_15VisAtt->SetVisibility(true);
	Tld_15VisAtt->SetForceSolid(true);
	logicTld_15->SetVisAttributes(Tld_15VisAtt); 

//----------------------------------------------
solidTld_16 = new G4Box("solidTld_16", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_16 = new G4LogicalVolume(solidTld_16, MaterialTld, "logicTld_16", 0, 0, 0);
   physiTld_16 = new G4PVPlacement(0,Position_Tld_16,logicTld_16,"physiTld_16",logicWorld,false,0,overlapCheck);
  // Attributes Tld 16
  G4VisAttributes* Tld_16VisAtt= new G4VisAttributes(); // cyan
	Tld_16VisAtt->SetColor(0., 1., 1.);
	Tld_16VisAtt->SetVisibility(true);
	Tld_16VisAtt->SetForceSolid(true);
	logicTld_16->SetVisAttributes(Tld_16VisAtt); 

//----------------------------------------------
solidTld_17 = new G4Box("solidTld_17", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_17 = new G4LogicalVolume(solidTld_17, MaterialTld, "logicTld_17", 0, 0, 0);
   physiTld_17 = new G4PVPlacement(0,Position_Tld_17,logicTld_17,"physiTld_17",logicWorld,false,0,overlapCheck);
  // Attributes Tld 17
  G4VisAttributes* Tld_17VisAtt= new G4VisAttributes(); // red
	Tld_17VisAtt->SetColor(1., 0.0, 0.);
	Tld_17VisAtt->SetVisibility(true);
	Tld_17VisAtt->SetForceSolid(true);
	logicTld_17->SetVisAttributes(Tld_17VisAtt); 

//----------------------------------------------
solidTld_18 = new G4Box("solidTld_18", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_18 = new G4LogicalVolume(solidTld_18, MaterialTld, "logicTld_18", 0, 0, 0);
   physiTld_18 = new G4PVPlacement(0,Position_Tld_18,logicTld_18,"physiTld_18",logicWorld,false,0,overlapCheck);
  // Attributes Tld 18
  G4VisAttributes* Tld_18VisAtt= new G4VisAttributes(); // yellow
	Tld_18VisAtt->SetColor(1., 1., 0.);
	Tld_18VisAtt->SetVisibility(true);
	Tld_18VisAtt->SetForceSolid(true);
	logicTld_18->SetVisAttributes(Tld_18VisAtt); 

//----------------------------------------------
solidTld_19 = new G4Box("solidTld_19", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_19 = new G4LogicalVolume(solidTld_19, MaterialTld, "logicTld_19", 0, 0, 0);
   physiTld_19 = new G4PVPlacement(0,Position_Tld_19,logicTld_19,"physiTld_19",logicWorld,false,0,overlapCheck);
  // Attributes Tld 19
  G4VisAttributes* Tld_19VisAtt= new G4VisAttributes(); // cyan
	Tld_19VisAtt->SetColor(0., 1., 1.);
	Tld_19VisAtt->SetVisibility(true);
	Tld_19VisAtt->SetForceSolid(true);
	logicTld_19->SetVisAttributes(Tld_19VisAtt); 

//----------------------------------------------
solidTld_20 = new G4Box("solidTld_20", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_20 = new G4LogicalVolume(solidTld_20, MaterialTld, "logicTld_20", 0, 0, 0);
   physiTld_20 = new G4PVPlacement(0,Position_Tld_20,logicTld_20,"physiTld_20",logicWorld,false,0,overlapCheck);
  // Attributes Tld 20
  G4VisAttributes* Tld_20VisAtt= new G4VisAttributes(); // yellow
	Tld_20VisAtt->SetColor(1., 1., 0.);
	Tld_20VisAtt->SetVisibility(true);
	Tld_20VisAtt->SetForceSolid(true);
	logicTld_20->SetVisAttributes(Tld_20VisAtt); 

//----------------------------------------------
solidTld_21 = new G4Box("solidTld_21", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_21 = new G4LogicalVolume(solidTld_21, MaterialTld, "logicTld_21", 0, 0, 0);
   physiTld_21 = new G4PVPlacement(0,Position_Tld_21,logicTld_21,"physiTld_21",logicWorld,false,0,overlapCheck);
  // Attributes Tld 21
  G4VisAttributes* Tld_21VisAtt= new G4VisAttributes(); // cyan
	Tld_21VisAtt->SetColor(0., 1., 1.);
	Tld_21VisAtt->SetVisibility(true);
	Tld_21VisAtt->SetForceSolid(true);
	logicTld_21->SetVisAttributes(Tld_21VisAtt); 

//----------------------------------------------
solidTld_22 = new G4Box("solidTld_22", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_22 = new G4LogicalVolume(solidTld_22, MaterialTld, "logicTld_22", 0, 0, 0);
   physiTld_22 = new G4PVPlacement(0,Position_Tld_22,logicTld_22,"physiTld_22",logicWorld,false,0,overlapCheck);
  // Attributes Tld 22
  G4VisAttributes* Tld_22VisAtt= new G4VisAttributes(); // green
	Tld_22VisAtt->SetColor(0., 1., 0.);
	Tld_22VisAtt->SetVisibility(true);
	Tld_22VisAtt->SetForceSolid(true);
	logicTld_22->SetVisAttributes(Tld_22VisAtt); 

//----------------------------------------------
solidTld_23 = new G4Box("solidTld_23", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_23 = new G4LogicalVolume(solidTld_23, MaterialTld, "logicTld_23", 0, 0, 0);
   physiTld_23 = new G4PVPlacement(0,Position_Tld_23,logicTld_23,"physiTld_23",logicWorld,false,0,overlapCheck);
  // Attributes Tld 23
  G4VisAttributes* Tld_23VisAtt= new G4VisAttributes(); // orange
	Tld_23VisAtt->SetColor(1., 0.5, 0.);
	Tld_23VisAtt->SetVisibility(true);
	Tld_23VisAtt->SetForceSolid(true);
	logicTld_23->SetVisAttributes(Tld_23VisAtt); 

//----------------------------------------------
solidTld_24 = new G4Box("solidTld_24", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_24 = new G4LogicalVolume(solidTld_24, MaterialTld, "logicTld_24", 0, 0, 0);
   physiTld_24 = new G4PVPlacement(0,Position_Tld_24,logicTld_24,"physiTld_24",logicWorld,false,0,overlapCheck);
  // Attributes Tld 24
  G4VisAttributes* Tld_24VisAtt= new G4VisAttributes(); // blu
	Tld_24VisAtt->SetColor(0., 0., 1.);
	Tld_24VisAtt->SetVisibility(true);
	Tld_24VisAtt->SetForceSolid(true);
	logicTld_24->SetVisAttributes(Tld_24VisAtt); 

//----------------------------------------------
solidTld_25 = new G4Box("solidTld_25", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_25 = new G4LogicalVolume(solidTld_25, MaterialTld, "logicTld_25", 0, 0, 0);
   physiTld_25 = new G4PVPlacement(0,Position_Tld_25,logicTld_25,"physiTld_25",logicWorld,false,0,overlapCheck);
  // Attributes Tld 25
  G4VisAttributes* Tld_25VisAtt= new G4VisAttributes(); // orange
	Tld_25VisAtt->SetColor(1, 0.5, 0.);
	Tld_25VisAtt->SetVisibility(true);
	Tld_25VisAtt->SetForceSolid(true);
	logicTld_25->SetVisAttributes(Tld_25VisAtt); 

//----------------------------------------------
solidTld_26 = new G4Box("solidTld_26", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_26 = new G4LogicalVolume(solidTld_26, MaterialTld, "logicTld_26", 0, 0, 0);
   physiTld_26 = new G4PVPlacement(0,Position_Tld_26,logicTld_26,"physiTld_26",logicWorld,false,0,overlapCheck);
  // Attributes Tld 26
  G4VisAttributes* Tld_26VisAtt= new G4VisAttributes(); // blu
	Tld_26VisAtt->SetColor(0., 0., 1.);
	Tld_26VisAtt->SetVisibility(true);
	Tld_26VisAtt->SetForceSolid(true);
	logicTld_26->SetVisAttributes(Tld_26VisAtt); 

//----------------------------------------------
solidTld_27 = new G4Box("solidTld_27", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_27 = new G4LogicalVolume(solidTld_27, MaterialTld, "logicTld_27", 0, 0, 0);
   physiTld_27 = new G4PVPlacement(0,Position_Tld_27,logicTld_27,"physiTld_27",logicWorld,false,0,overlapCheck);
  // Attributes Tld 27
  G4VisAttributes* Tld_27VisAtt= new G4VisAttributes(); // yellow
	Tld_27VisAtt->SetColor(1., 1., 0.);
	Tld_27VisAtt->SetVisibility(true);
	Tld_27VisAtt->SetForceSolid(true);
	logicTld_27->SetVisAttributes(Tld_27VisAtt); 

//----------------------------------------------
solidTld_28 = new G4Box("solidTld_28", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_28 = new G4LogicalVolume(solidTld_28, MaterialTld, "logicTld_28", 0, 0, 0);
   physiTld_28 = new G4PVPlacement(0,Position_Tld_28,logicTld_28,"physiTld_28",logicWorld,false,0,overlapCheck);
  // Attributes Tld 28
  G4VisAttributes* Tld_28VisAtt= new G4VisAttributes(); // VIOLET
	Tld_28VisAtt->SetColor(0.5, 0.5, 1.);
	Tld_28VisAtt->SetVisibility(true);
	Tld_28VisAtt->SetForceSolid(true);
	logicTld_28->SetVisAttributes(Tld_28VisAtt); 

//----------------------------------------------
solidTld_29 = new G4Box("solidTld_29", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_29 = new G4LogicalVolume(solidTld_29, MaterialTld, "logicTld_29", 0, 0, 0);
   physiTld_29 = new G4PVPlacement(0,Position_Tld_29,logicTld_29,"physiTld_29",logicWorld,false,0,overlapCheck);
  // Attributes Tld 29
  G4VisAttributes* Tld_29VisAtt= new G4VisAttributes(); // VIOLET
	Tld_29VisAtt->SetColor(0.5, 0.5, 1.);
	Tld_29VisAtt->SetVisibility(true);
	Tld_29VisAtt->SetForceSolid(true);
	logicTld_29->SetVisAttributes(Tld_29VisAtt); 

//----------------------------------------------

solidTld_30 = new G4Box("solidTld_30", tldHalfSize.x(), tldHalfSize.y(), tldHalfSize.z());
   logicTld_30 = new G4LogicalVolume(solidTld_30, MaterialTld, "logicTld_30", 0, 0, 0);
   physiTld_30 = new G4PVPlacement(0,Position_Tld_30,logicTld_30,"physiTld_30",logicWorld,false,0,overlapCheck);
  // Attributes Tld 30
  G4VisAttributes* Tld_30VisAtt= new G4VisAttributes(); // red
	Tld_30VisAtt->SetColor(1., 0., 0.);
	Tld_30VisAtt->SetVisibility(true);
	Tld_30VisAtt->SetForceSolid(true);
	logicTld_30->SetVisAttributes(Tld_30VisAtt); 

   }  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructSurfaceAirKerma()
{
  //------------------------------ 
  // Surface for scoring the AK
  //------------------------------

  G4Material* SurfaceAKLayer;
 	SurfaceAKLayer= G4Material::GetMaterial("G4_AIR");

  G4double x_half_dimension_AK = 3.0*cm/2.0; 
	G4double y_half_dimension_AK = 3.0*cm/2.0; 
	z_half_dimension_AK     = 0.000001*mm/2.0; // 1 nm Initialize the half_size for the AK    



   G4ThreeVector SurfaceAKLayerHalfSize = G4ThreeVector(x_half_dimension_AK, y_half_dimension_AK, z_half_dimension_AK);

	G4ThreeVector PositionAirKerma;	
	PositionAirKerma = G4ThreeVector (imagerSizeX/2.0, - imagerSizeY/2.0, airGap + breastPhantomSize_Inferior.z() + detector_deviceHalfSize*2.0 + breastPhantomSize_Superior.z()+SurfaceAKLayerHalfSize.z());

  solidAirKerma = new G4Box("solidAirKerma", SurfaceAKLayerHalfSize.x(), SurfaceAKLayerHalfSize.y(),       SurfaceAKLayerHalfSize.z());
   logicAirKerma = new G4LogicalVolume(solidAirKerma, SurfaceAKLayer, "logicAirKerma", 0, 0, 0);

        physiAirKerma = new G4PVPlacement(0,		// no rotation
				PositionAirKerma,		
				logicAirKerma,  		// its logical volume
				"physiAirKerma",    //its name
				logicWorld,         // its mother  volume
				false,              // no boolean operations
		    0,	                // copy number
        overlapCheck);	    // perform overlap check


	// Attributes active layer
  G4VisAttributes* AirKermaVisAtt= new G4VisAttributes(); // Yellow
	AirKermaVisAtt->SetColor(1., 1., 0.);
	AirKermaVisAtt->SetVisibility(true);
	AirKermaVisAtt->SetForceSolid(true);
  logicAirKerma->SetVisAttributes(AirKermaVisAtt);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructImager()
{ 
  //------------------------------ 
  // Imager including compression plate, support plate, and imager cover
  // Note that this calls another method DetectorConstruction::ConstructDetector()
  //------------------------------

	// Calculation of Dimensions
	
   G4ThreeVector imagerSize = G4ThreeVector(imagerSizeX, imagerSizeY, 1.0*cm);
   G4ThreeVector plateSize = G4ThreeVector(imagerSizeX, imagerSizeY+5.0*cm, paddleThickness);

	// Attributes
  G4VisAttributes* platesAttrib= new G4VisAttributes();
	platesAttrib->SetVisibility(true);
	platesAttrib->SetColor(0/255., 255/255., 0/255., 0.4);
	platesAttrib->SetForceAuxEdgeVisible(true);
 	platesAttrib->SetForceWireframe(true);

	if (compressionPlate ) {
	  G4Material* compressionMaterial = G4Material::GetMaterial("G4_POLYCARBONATE"); 
      
		solidCompression = new G4Box("solidCompression", plateSize.x()/2., plateSize.y()/2., plateSize.z()/2.);
	  logicCompression = new G4LogicalVolume(solidCompression , compressionMaterial, "logicCompression",0,0,0);  

	  physiCompression = new G4PVPlacement(0,                 // no rotation
	                          					  G4ThreeVector(plateSize.x()/2.0, -imagerSize.y()/2.0, airGap + detector_deviceHalfSize*2.0 + breastPhantomSize_Inferior.z() + breastPhantomSize_Superior.z()  + z_half_dimension_AK*2.0+ plateSize.z()/2.0),   // at (x,y,z)
	                          			  		logicCompression,     // its logical volume				  
	                           			  		"physiCompression",   // its name
	                          			  		logicWorld,      			// its mother  volume
	                           			  		false,           			// no boolean operations
					                           		0,	             			// copy number
																				overlapCheck);				// perform overlap check
		logicCompression->SetVisAttributes(platesAttrib);
}

	if (supportPlate) {
	  G4Material* supportMaterial = G4Material::GetMaterial("CarbonFiber");  
		solidSupport = new G4Box("solidSupport", plateSize.x()/2., plateSize.y()/2., 1.7*mm/2.);
	  logicSupport = new G4LogicalVolume(solidSupport , supportMaterial, "logicSupport",0,0,0);  
	  physiSupport = new G4PVPlacement(0,                 // no rotation
	                          				G4ThreeVector(plateSize.x()/2.0, -imagerSize.y()/2.0, airGap - plateSize.z()/2.0),   // at (x,y,z)
	                          			  logicSupport,     // its logical volume				  
	                           			  "physiSupport",   // its name
	                          			  logicWorld,      // its mother  volume
	                           			  false,           // no boolean operations
					                       		0,	             // copy number
																		overlapCheck);	 // perform overlap check
		logicSupport->SetVisAttributes(platesAttrib);
	}

	ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructDetector()
{
  //------------------------------
  // Detector Sub-volume containing the cover plates and the Imager
  //------------------------------
  
  // Determine angle of detector based on angle of x-ray source
  //G4RunManager* runManager = G4RunManager::GetRunManager();
  //PrimaryGeneratorAction * generatorAction = (PrimaryGeneratorAction *) runManager->GetUserPrimaryGeneratorAction();

	// Coverplate sizes
	G4double AlCoverThick;
	G4double CarbCoverThick;
	AlCoverThick = 0.025*mm;
  CarbCoverThick = 0.084*mm;
  G4double ImagerThick = 1.0*cm;

  // Calculation of Dimensions
  G4ThreeVector imagerSize = G4ThreeVector(imagerSizeX, imagerSizeY, ImagerThick);
  G4ThreeVector AlCoverSize = G4ThreeVector(imagerSizeX, imagerSizeY, AlCoverThick);
  G4ThreeVector CarbCoverSize = G4ThreeVector(imagerSizeX, imagerSizeY, CarbCoverThick);
  G4ThreeVector physiImager_orig_pos = G4ThreeVector( 0, 0, -(AlCoverThick+CarbCoverThick/2.0));

  // Creation of ImagerHolder
  G4Material* holderMaterial = G4Material::GetMaterial("G4_AIR");

  // remember: holder comprises imager, two AL cover plates and one Carb cover plate
  // remember: BCT: holder comprises imager, one AL cover plates and one Carb cover plate
  solidImagerHolder = new G4Box("solidImagerHolder", imagerSize.x()/2.0, imagerSize.y()/2.0, (imagerSize.z() + 2.0 * AlCoverThick + CarbCoverThick)/2.0);
  logicImagerHolder = new G4LogicalVolume(solidImagerHolder, holderMaterial, "logicImagerHolder", 0, 0, 0);
	physiImagerHolder = new G4PVPlacement(0, 
																				G4ThreeVector(imagerSize.x()/2.0, -imagerSize.y()/2.0, -imagerSize.z()/2.0),
																				logicImagerHolder, 
																				"physiImagerHolder",
																				logicWorld,
																				false,
									                   		0,
																				overlapCheck);
																				
  // Creation of Imager
  G4Material* imagerMaterial = G4Material::GetMaterial("G4_Galactic");  
  solidImager = new G4Box("solidImager", imagerSize.x()/2., imagerSize.y()/2., imagerSize.z()/2.);
  logicImager = new G4LogicalVolume(solidImager , imagerMaterial, "logicImager",0,0,0);  
  physiImager = new G4PVPlacement(0,                      // NO rotation // 090708-SJF
																	physiImager_orig_pos,   // 090708 - holder volume
																	logicImager,            // its logical volume				  
																	"physiImager",          // its name
																	logicImagerHolder,      // its mother  volume 
																	false,                  // no boolean operations
			                         		0,	                    // copy number
																	overlapCheck);	        // perform overlap check

  // Attributes
  G4VisAttributes* imagerAttrib= new G4VisAttributes();
  imagerAttrib->SetVisibility(true);
	imagerAttrib->SetColor(0/255., 255/255., 0/255., 0.4);
	imagerAttrib->SetForceAuxEdgeVisible(true);
 	imagerAttrib->SetForceWireframe(true);
	logicImager->SetVisAttributes(imagerAttrib);


  if (coverPlate) {

	  G4Material* coverAlMaterial = G4Material::GetMaterial("G4_Al");
	  G4Material* coverCMaterial = G4Material::GetMaterial("G4_C");

		// creation of Cover Plate Solids
		solidCoverAl = new G4Box("solidCoverAl", imagerSize.x()/2., imagerSize.y()/2., AlCoverThick/2.);
		solidCoverC  = new G4Box("solidCoverC" , imagerSize.x()/2., imagerSize.y()/2., CarbCoverThick/2.);
	  
	  logicCoverAl = new G4LogicalVolume(solidCoverAl, coverAlMaterial, "logicCoverAl",0,0,0);
	  logicCoverC  = new G4LogicalVolume(solidCoverC,  coverCMaterial, "logicCoverC",0,0,0);
	  
	  physiCoverAlUnder = new G4PVPlacement (0,                 // added tomo rotation //090704-SJF
																					 G4ThreeVector(0,0, physiImager_orig_pos.z()+AlCoverThick/2.0+ImagerThick/2.0),
																					 logicCoverAl,          // its logical volume				  
																					 "physiCoverAlUnder",   // its name
																					 logicImagerHolder,     // its mother  volume
																					 false,                 // no boolean operations
												               		 0,	                    // copy number
																					 overlapCheck);	        // perform overlap check

	  physiCoverC = new G4PVPlacement (0,                 // added rotation /090704-SJF
																		 G4ThreeVector(0, 0, physiImager_orig_pos.z() +AlCoverThick + CarbCoverThick/2.0+ImagerThick/2.0),
																		 logicCoverC,           // its logical volume				  
																		 "physiCoverC",         // its name
																		 logicImagerHolder,     // its mother  volume
																		 false,                 // no boolean operations
							                     	 0,	                    // copy number
																		 overlapCheck);	        // perform overlap check

		
			physiCoverAlOver = new G4PVPlacement(0,                 // added tomo rotation //090704-SJF
																					 G4ThreeVector(0,0, physiImager_orig_pos.z()+AlCoverThick + CarbCoverThick + AlCoverThick/2.0+ImagerThick/2.0),
																					 logicCoverAl,     // its logical volume				  
																					 "physiCoverAlOver",   // its name
																					 logicImagerHolder,      // its mother  volume
																					 false,           // no boolean operations
													               	 0,	             // copy number
																					 overlapCheck);	 // perform overlap check
		
		
		G4VisAttributes* CarbCoverAttrib= new G4VisAttributes();
		CarbCoverAttrib->SetVisibility(true);
		CarbCoverAttrib->SetColor(255/255., 0/255., 0/255., 0.4);
		CarbCoverAttrib->SetForceAuxEdgeVisible(true);
	 	CarbCoverAttrib->SetForceWireframe(true);

		G4VisAttributes* AlCoverAttrib= new G4VisAttributes();
		AlCoverAttrib->SetVisibility(true);
		AlCoverAttrib->SetColor(0/255., 0/255., 255/255., 0.4);
		AlCoverAttrib->SetForceAuxEdgeVisible(true);
	 	AlCoverAttrib->SetForceWireframe(true);

	  logicCoverAl->SetVisAttributes(AlCoverAttrib);
	  logicCoverC->SetVisAttributes(CarbCoverAttrib);
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::DefineBreastMaterial(G4double glandularity)
{ 
  //============================
  // Breast Material Definitions
  //============================

	char materialName[50];
	sprintf(materialName,"BreastMaterial_%02.0f", glandularity);

  G4Material* breastMaterial = G4Material::GetMaterial(materialName);
	if (breastMaterial == NULL)	{

	  G4Material* adiposeMaterial = G4Material::GetMaterial("AdiposeMaterial");
	  G4Material* glandularMaterial = G4Material::GetMaterial("GlandularMaterial");

		G4double  rho0 = 0.9240,
							rho100 = 1.0400;
		G4double rho = 1.0/(glandularity/100.0/rho100 + (100.0 - glandularity)/100.0/rho0);

		breastMaterial = new G4Material(materialName, rho*g/cm3, 2, kStateSolid, 293.15*kelvin, 1.0*atmosphere );
		breastMaterial->AddMaterial(adiposeMaterial, 1.0 - glandularity/100.0);
		breastMaterial->AddMaterial(glandularMaterial, glandularity/100.0);
	}
	
	return breastMaterial;	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 

  //=====================
  // Material Definitions
  //=====================
  //  
  //-------- NIST Materials ----------------------------------------------------
  //  Material Information imported from NIST database.
  //
  G4NistManager* NISTman = G4NistManager::Instance();
  NISTman->FindOrBuildMaterial("G4_AIR");
  NISTman->FindOrBuildMaterial("G4_Galactic");
  NISTman->FindOrBuildMaterial("G4_WATER");
  NISTman->FindOrBuildMaterial("G4_POLYCARBONATE");
  NISTman->FindOrBuildMaterial("G4_C");
  NISTman->FindOrBuildMaterial("G4_Al");
  NISTman->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  NISTman->FindOrBuildMaterial("G4_KAPTON");
  NISTman->FindOrBuildMaterial("G4_PLEXIGLASS");

  G4Material* LiF = NISTman->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");

  G4Element* FindOrBuildElement(const G4String& symb, G4bool isotopes=true);

  G4Element* elementH = NISTman->FindOrBuildElement("H");
  G4Element* elementC = NISTman->FindOrBuildElement("C");
  G4Element* elementN = NISTman->FindOrBuildElement("N");
  G4Element* elementO = NISTman->FindOrBuildElement("O");
  G4Element* elementP = NISTman->FindOrBuildElement("P");
  G4Element* elementMg = NISTman->FindOrBuildElement("Mg");
  G4Element* elementS = NISTman->FindOrBuildElement("S");
  G4Element* elementK = NISTman->FindOrBuildElement("K");
  G4Element* elementCa = NISTman->FindOrBuildElement("Ca");
  G4Element* elementCl = NISTman->FindOrBuildElement("Cl");
  G4Element* elementAl = NISTman->FindOrBuildElement("Al");
  G4Element* elementCu = NISTman->FindOrBuildElement("Cu");
 
	
  G4Material* carbonFiber = new G4Material("CarbonFiber",  1.200*g/cm3, 1, kStateSolid, 293.15*kelvin, 1.0*atmosphere );
	carbonFiber->AddElement( elementC, 1.0 );

  G4Material* tldMaterial = new G4Material("tldMaterial",  2.48*g/cm3, 4,  kStateSolid, 293.15*kelvin, 1.0*atmosphere  );
	tldMaterial->AddMaterial( LiF, 99.5*perCent );	
	tldMaterial->AddElement( elementMg, 0.2*perCent );
	tldMaterial->AddElement( elementCu, 0.004*perCent );
	tldMaterial->AddElement( elementP, 0.296*perCent );

  // CIRS MATERIAL FOLLOWING LITTERATURE

G4Material* adiposeMaterial = new G4Material("AdiposeMaterial",  0.9240*g/cm3, 5,kStateSolid, 293.15*kelvin, 1.0*atmosphere  );//, kStateSolid, 293.15*kelvin, 1.0*atmosphere );
	adiposeMaterial->AddElement( elementH, 0.1176 );
	adiposeMaterial->AddElement( elementC, 0.7595 );
	adiposeMaterial->AddElement( elementN, 0.0123 );
	adiposeMaterial->AddElement( elementO, 0.0982 );
	adiposeMaterial->AddElement( elementCl, 0.0117 );
	

	G4Material* glandularMaterial = new G4Material("GlandularMaterial",  1.0400*g/cm3, 7, kStateSolid, 293.15*kelvin, 1.0*atmosphere );
	glandularMaterial->AddElement( elementH, 0.1093 );
	glandularMaterial->AddElement( elementC, 0.7021 );
	glandularMaterial->AddElement( elementN, 0.0115 );
	glandularMaterial->AddElement( elementO, 0.1251 );
	glandularMaterial->AddElement( elementCl, 0.011 );
	glandularMaterial->AddElement( elementCa, 0.0061 );
  glandularMaterial->AddElement( elementAl, 0.0346 );


   G4Material* breastMaterialCIRS = new G4Material("breastMaterialCIRS",  0.9819*g/cm3, 2,  kStateSolid, 293.15*kelvin, 1.0*atmosphere  );
	breastMaterialCIRS->AddMaterial( adiposeMaterial, 50.0*perCent );	
	breastMaterialCIRS->AddMaterial( glandularMaterial, 50.0*perCent );

    
	G4Material* skinMaterial = new G4Material("SkinMaterial",  1.0900*g/cm3, 8, kStateSolid, 293.15*kelvin, 1.0*atmosphere );
	skinMaterial->AddElement( elementH, 0.098 );
	skinMaterial->AddElement( elementC, 0.178 );
	skinMaterial->AddElement( elementN, 0.050 );
	skinMaterial->AddElement( elementO, 0.667 );
	skinMaterial->AddElement( elementS, 0.00175 );
	skinMaterial->AddElement( elementP, 0.00175 );
	skinMaterial->AddElement( elementK, 0.00175 );
	skinMaterial->AddElement( elementCa, 0.00175 );
    	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineDetectors()
{ 
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4RunManager* runManager = G4RunManager::GetRunManager();
	EventAction * eventAction = (EventAction *) runManager->GetUserEventAction();

if (DetectorDeviceSense && detector_device != NO  ) {

if (detector_device == TLD){

		G4String TldSDname = "/TldSD";
		TldSD* aTldSD = new TldSD(TldSDname, this, eventAction);
		SDman->AddNewDetector( aTldSD );

		} 
   }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::AssignDetectors()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

//this detector is used to calculate the Air Kerma
  G4String surfacefluxSDname = "SurfaceFluxSD";
  G4MultiFunctionalDetector* aSurfaceFluxSD = new G4MultiFunctionalDetector(surfacefluxSDname);
  SDman->AddNewDetector( aSurfaceFluxSD );
  Kerma_Evaluation* theSurfaceFluxSD = new Kerma_Evaluation("theSurfaceFluxSD",this,2);
  
  aSurfaceFluxSD->RegisterPrimitive(theSurfaceFluxSD);
  logicAirKerma ->SetSensitiveDetector(aSurfaceFluxSD);

	if (DetectorDeviceSense && detector_device != NO) {

	if (detector_device == TLD){

		TldSD* theTldSD = (TldSD* ) SDman->FindSensitiveDetector("/TldSD", 1);
  	logicTld_1->SetSensitiveDetector( theTldSD  );
		logicTld_2->SetSensitiveDetector( theTldSD  );
		logicTld_3->SetSensitiveDetector( theTldSD  );
		logicTld_4->SetSensitiveDetector( theTldSD  );
		logicTld_5->SetSensitiveDetector( theTldSD  );
		logicTld_6->SetSensitiveDetector( theTldSD  );
		logicTld_7->SetSensitiveDetector( theTldSD  );
		logicTld_8->SetSensitiveDetector( theTldSD  );
		logicTld_9->SetSensitiveDetector( theTldSD  );
		logicTld_10->SetSensitiveDetector( theTldSD  );
		logicTld_11->SetSensitiveDetector( theTldSD  );
		logicTld_12->SetSensitiveDetector( theTldSD  );
		logicTld_13->SetSensitiveDetector( theTldSD  );
		logicTld_14->SetSensitiveDetector( theTldSD  );
		logicTld_15->SetSensitiveDetector( theTldSD  );
    logicTld_16->SetSensitiveDetector( theTldSD  );
		logicTld_17->SetSensitiveDetector( theTldSD  );
		logicTld_18->SetSensitiveDetector( theTldSD  );
    logicTld_19->SetSensitiveDetector( theTldSD  );
    logicTld_20->SetSensitiveDetector( theTldSD  );
    logicTld_21->SetSensitiveDetector( theTldSD  );
		logicTld_22->SetSensitiveDetector( theTldSD  );
		logicTld_23->SetSensitiveDetector( theTldSD  );
		logicTld_24->SetSensitiveDetector( theTldSD  );
		logicTld_25->SetSensitiveDetector( theTldSD  );
		logicTld_26->SetSensitiveDetector( theTldSD  );
		logicTld_27->SetSensitiveDetector( theTldSD  );
		logicTld_28->SetSensitiveDetector( theTldSD  );
		logicTld_29->SetSensitiveDetector( theTldSD  );
		logicTld_30->SetSensitiveDetector( theTldSD  );
		
		} 
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintGeometryParameters()
{
  //G4RunManager* runManager = G4RunManager::GetRunManager();  // Get the run manager
	//PrimaryGeneratorAction * generatorAction = (PrimaryGeneratorAction *) runManager->GetUserPrimaryGeneratorAction();

  std::ostringstream detectro_deviceName;
	std::ostringstream airGapString;
	std::ostringstream free_in_airName;
	
	switch(detector_device){

		case NO:
			detectro_deviceName << "NO Detector Device";
		break;

    case TLD:
			detectro_deviceName << "TLD Device";
		break;
		

	}

	/*
  G4cout << "\n===================================================================="
	       << "\n### There is " << detectro_deviceName.str()  
				 << "\n### The SID is set to " << generatorAction->GetSID()/cm << " cm."
				 << "\n### The source distance from the chestwall is  " << generatorAction->GetDistFromCW()/cm << " cm."
				 << "\n### The x-ray energy is set to " << generatorAction->GetParticleGun()->GetParticleEnergy()/keV << " keV."
				 << airGapString.str()
				 << "\n### The imager is " << imagerSizeX/cm << " x " << imagerSizeY/cm << " cm "
				 << "\n====================================================================\n";*/

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DumpGeometricalTree(G4VPhysicalVolume* aVolume,G4int depth)
{
  for(G4int isp=0; isp<depth; isp++)
  { G4cout << "  "; }
  G4cout << aVolume->GetName() << "[" << aVolume->GetCopyNo() << "] "
         << aVolume->GetLogicalVolume()->GetName() << " "
         << aVolume->GetLogicalVolume()->GetNoDaughters() << " "
         << aVolume->GetLogicalVolume()->GetMaterial()->GetName();
  if(aVolume->GetLogicalVolume()->GetSensitiveDetector())
  {
    G4cout << " " << aVolume->GetLogicalVolume()->GetSensitiveDetector()
                            ->GetFullPathName();
  }
  G4cout << G4endl;
  G4int number_daughters= static_cast<G4int>(aVolume->GetLogicalVolume()->GetNoDaughters());
  for(G4int i=0;i< number_daughters;i++)
  {DumpGeometricalTree(aVolume->GetLogicalVolume()->GetDaughter(i),depth+1); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// --------------------------------------------------------
// FUNCTIONS TO APPLY UI COMMANDS
// --------------------------------------------------------


void DetectorConstruction::SetImagerSizeX(G4double val)
{
  // change x size of the imager
	imagerSizeX = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetImagerSizeY(G4double val)
{
  // change y size of the imager
	imagerSizeY = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructGeometry());
	AssignDetectors();

  //G4RunManager* runManager = G4RunManager::GetRunManager();
  //PrimaryGeneratorAction * generatorAction = (PrimaryGeneratorAction *) runManager->GetUserPrimaryGeneratorAction();

  G4cout << G4endl << "The geometrical tree defined is : " << G4endl << G4endl;
  DumpGeometricalTree(physiWorld);
  PrintGeometryParameters();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetVisualization(G4bool vis)
{
	visualization = vis;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
