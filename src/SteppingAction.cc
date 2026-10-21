// SteppingAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Run.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
SteppingAction::SteppingAction(G4RunManager *run, PrimaryGeneratorAction* gen)
:runManager(run), generatorAction(gen)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step * theStep)
{
	G4StepPoint* preStepPoint = theStep->GetPreStepPoint();
	G4StepPoint* postStepPoint = theStep->GetPostStepPoint();

	//G4Material* currentMaterial = preStepPoint->GetMaterial();

	G4ThreeVector startPos = preStepPoint->GetPosition();
	G4ThreeVector endPos = postStepPoint->GetPosition();
	
	//G4double length = sqrt((endPos - startPos).mag2());

	//Run* theRun = (Run*)runManager->GetCurrentRun();
	
//	G4cout << "### Step, Material: " << currentMaterial->GetName() << "  Length: " << length/mm << " Array Indices: " 
//					<< generatorAction->GetPixNumberX() << " " << generatorAction->GetPixNumberY() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

