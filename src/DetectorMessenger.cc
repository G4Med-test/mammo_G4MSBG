// DetectorMessenger.cc
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* det)
:detectorConst(det)
{ 
  appDir = new G4UIdirectory("/app/");
  appDir->SetGuidance("UI commands specific to this application.");
  
  breastDir = new G4UIdirectory("/app/breast/");
  breastDir->SetGuidance("Target control.");
  
  detDir = new G4UIdirectory("/app/det/");
  detDir->SetGuidance("Detector control.");

  detector_deviceDir = new G4UIdirectory("/app/detector_device/");
  detector_deviceDir->SetGuidance("Detector device control.");
  
  NOTypeCmd = new G4UIcmdWithoutParameter("/app/detector_device/setNOType",this);
  NOTypeCmd->SetGuidance("Set NO detector device.");
  NOTypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  TLDTypeCmd = new G4UIcmdWithoutParameter("/app/detector_device/setTLDType",this);
  TLDTypeCmd->SetGuidance("Set TLD detector device.");
  TLDTypeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  airGapCmd = new G4UIcmdWithADoubleAndUnit("/app/breast/setAirGap",this);
  airGapCmd->SetGuidance("Set air gap.");
  airGapCmd->SetParameterName("Size",false);
  airGapCmd->SetRange("Size>=0.");
  airGapCmd->SetUnitCategory("Length");
  airGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  paddleThicknessCmd = new G4UIcmdWithADoubleAndUnit("/app/breast/setPaddleThickness",this);
  paddleThicknessCmd->SetGuidance("Set Paddle Thickness.");
  paddleThicknessCmd->SetParameterName("Size",false);
  paddleThicknessCmd->SetRange("Size>=0.");
  paddleThicknessCmd->SetUnitCategory("Length");
  paddleThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  breastPhantom_InferiorCmd = new G4UIcmdWithADoubleAndUnit("/app/breast/setBreastPhantom_Inferior",this);
  breastPhantom_InferiorCmd->SetGuidance("Set Breast Phantom Inferior Thickness.");
  breastPhantom_InferiorCmd->SetParameterName("Size",false);
  breastPhantom_InferiorCmd->SetRange("Size>=0.");
  breastPhantom_InferiorCmd->SetUnitCategory("Length");
  breastPhantom_InferiorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  breastPhantom_SuperiorCmd = new G4UIcmdWithADoubleAndUnit("/app/breast/setBreastPhantom_Superior",this);
  breastPhantom_SuperiorCmd->SetGuidance("Set Breast Phantom Superior Thickness.");
  breastPhantom_SuperiorCmd->SetParameterName("Size",false);
  breastPhantom_SuperiorCmd->SetRange("Size>=0.");
  breastPhantom_SuperiorCmd->SetUnitCategory("Length");
  breastPhantom_SuperiorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  imagerXCmd = new G4UIcmdWithADoubleAndUnit("/app/det/setImagerX",this);
  imagerXCmd->SetGuidance("Set x dimension of imager.");
  imagerXCmd->SetParameterName("Size",false);
  imagerXCmd->SetRange("Size>=0.");
  imagerXCmd->SetUnitCategory("Length");
  imagerXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  imagerYCmd = new G4UIcmdWithADoubleAndUnit("/app/det/setImagerY",this);
  imagerYCmd->SetGuidance("Set y dimension of imager.");
  imagerYCmd->SetParameterName("Size",false);
  imagerYCmd->SetRange("Size>=0.");
  imagerYCmd->SetUnitCategory("Length");
  imagerYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  supportCmd = new G4UIcmdWithABool("/app/det/useSupportPlate",this);
  supportCmd->SetGuidance("Detector includes breast support plate.");
	supportCmd->SetDefaultValue(true);
  supportCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  compressionCmd = new G4UIcmdWithABool("/app/det/useCompressionPlate",this);
  compressionCmd->SetGuidance("Detector includes breast compression plate.");
	compressionCmd->SetDefaultValue(true);
  compressionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  

  coverCmd = new G4UIcmdWithABool("/app/det/useCoverPlate",this);
  coverCmd->SetGuidance("Detector includes imager cover plate.");
	coverCmd->SetDefaultValue(true);
  coverCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
 
	airCmd = new G4UIcmdWithABool("/app/det/airInWorld",this);
	airCmd->SetGuidance("Set true to include air in the world, otherwise vaccuum.");
	airCmd->SetDefaultValue(true);
	airCmd->AvailableForStates(G4State_Idle);
 
  imagerKillCmd = new G4UIcmdWithABool("/app/det/killerImager",this);
  imagerKillCmd->SetGuidance("Set sensitivity of imager to just kill entering particle.");
	imagerKillCmd->SetDefaultValue(true);
  imagerKillCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  detector_deviceCmd = new G4UIcmdWithABool("/app/detector_device/detector_device",this);
  detector_deviceCmd->SetGuidance("Able for detector device");
  detector_deviceCmd->SetDefaultValue(true);
  detector_deviceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  

  updateCmd = new G4UIcmdWithoutParameter("/app/update",this);
  updateCmd->SetGuidance("Update simulation geometry.");
  updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  updateCmd->SetGuidance("if you changed geometrical value(s).");
  updateCmd->AvailableForStates(G4State_Idle);

  visualizeCmd = new G4UIcmdWithABool("/app/visualize",this);
  visualizeCmd->SetGuidance("Create geometry for visualization.");
	visualizeCmd->SetDefaultValue(false);
  visualizeCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
	delete visualizeCmd;
	delete updateCmd;
  delete imagerKillCmd;
	delete airCmd;
	delete coverCmd;
	delete compressionCmd;
	delete supportCmd;
	delete imagerYCmd;
	delete imagerXCmd;
  delete paddleThicknessCmd;
  delete breastPhantom_InferiorCmd;
  delete breastPhantom_SuperiorCmd;
	delete airGapCmd;
	delete breastDir;
  delete detDir;
  delete appDir;
  delete TLDTypeCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
 
  if( command == NOTypeCmd )
   { detectorConst->SetDetectorDevice(NO);}
 
  if( command == TLDTypeCmd )
   { detectorConst->SetDetectorDevice(TLD);}

  if( command == airGapCmd )
   { detectorConst->SetAirGap(airGapCmd->GetNewDoubleValue(newValue));}

  if( command == paddleThicknessCmd )
   { detectorConst->SetPaddleThickness(paddleThicknessCmd->GetNewDoubleValue(newValue));}

	if( command == breastPhantom_InferiorCmd )
   { detectorConst->SetBreastPhantom_Inferior(breastPhantom_InferiorCmd->GetNewDoubleValue(newValue));}

	if( command == breastPhantom_SuperiorCmd )
   { detectorConst->SetBreastPhantom_Superior(breastPhantom_SuperiorCmd->GetNewDoubleValue(newValue));}

  if( command == imagerXCmd )
   { detectorConst->SetImagerSizeX(imagerXCmd->GetNewDoubleValue(newValue));}

  if( command == imagerYCmd )
   { detectorConst->SetImagerSizeY(imagerYCmd->GetNewDoubleValue(newValue));}

  if( command == coverCmd )
   { detectorConst->SetCoverPlate(coverCmd->GetNewBoolValue(newValue)); }   
 
	if( command == airCmd )
	 { detectorConst->SetAirInWorld(airCmd->GetNewBoolValue(newValue)); }   

  if( command == compressionCmd )
   { detectorConst->SetCompressionPlate(compressionCmd->GetNewBoolValue(newValue)); }   

  if( command == supportCmd )
   { detectorConst->SetSupportPlate(supportCmd->GetNewBoolValue(newValue)); }   
   
  if( command == imagerKillCmd )
   { detectorConst->SetKillerImager(imagerKillCmd->GetNewBoolValue(newValue)); }   

  if( command == detector_deviceCmd )
   { detectorConst->SetDetectorDeviceSensitivity(detector_deviceCmd->GetNewBoolValue(newValue)); } 

  if( command == updateCmd )
   { detectorConst->UpdateGeometry(); }   

  if( command == visualizeCmd )
   { detectorConst->SetVisualization(visualizeCmd->GetNewBoolValue(newValue)); }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String DetectorMessenger::GetCurrentValue(G4UIcommand* command)
{
	G4String cv;

  if( command == NOTypeCmd )
   { cv = (detectorConst->GetDetectorDevice() == NO ? "True":"False");}

  if( command == TLDTypeCmd )
   { cv = (detectorConst->GetDetectorDevice() == TLD ? "True":"False");}

  if( command == airGapCmd )
   { cv = airGapCmd->ConvertToString(detectorConst->GetAirGap());}

	if( command == paddleThicknessCmd )
   { cv = paddleThicknessCmd->ConvertToString(detectorConst->GetPaddleThickness());}

	if( command == breastPhantom_InferiorCmd )
   { cv = breastPhantom_InferiorCmd->ConvertToString(detectorConst->GetBreastPhantom_Inferior());}

	if( command == breastPhantom_SuperiorCmd )
   { cv = breastPhantom_SuperiorCmd->ConvertToString(detectorConst->GetBreastPhantom_Superior());}

  if( command == imagerXCmd )
   { cv = imagerXCmd->ConvertToString(detectorConst->GetImagerSizeX());}

  if( command == imagerYCmd )
   { cv = imagerYCmd->ConvertToString(detectorConst->GetImagerSizeY());}

  if( command == coverCmd )
   { cv = coverCmd->ConvertToString(detectorConst->GetCoverPlate());}

  if( command == compressionCmd )
   { cv = compressionCmd->ConvertToString(detectorConst->GetCompressionPlate());}

  if( command == supportCmd )
   { cv = supportCmd->ConvertToString(detectorConst->GetSupportPlate());}

  if( command == imagerKillCmd )
   { cv = imagerKillCmd->ConvertToString(detectorConst->GetKillerImager());}

  if( command == visualizeCmd )
   { cv = visualizeCmd->ConvertToString(detectorConst->GetVisualization()); }   

  return cv; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
