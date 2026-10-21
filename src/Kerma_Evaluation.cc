//
// $Id: Kerma_Evaluation.cc 81087 2014-05-20 15:44:27Z gcosmo $
//
// Kerma_Evaluation
#include "Kerma_Evaluation.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepStatus.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VPVParameterisation.hh"
#include "G4UnitsTable.hh"
#include "G4GeometryTolerance.hh"
#include "DetectorConstruction.hh"

////////////////////////////////////////////////////////////////////////////////
// (Description) Modified by Christian Fedon on May 2017
// 
//   This is a primitive scorer class for scoring Surface Flux after the Compression Paddle
//  Current version assumes only for G4Box shape, and the surface
//  is fixed on the exit plane of the upper paddle (initially define on -Z plane).
//
// Direction      (in surf AK)+Z    -Z (exit surf AK)   
//   0  IN || OUT              |   ->|<-  
//   1  IN                     |     |<-   
//   2  OUT                    |   ->|  
//
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 
// 18-Nov-2005  T.Aso,  To use always positive value for anglefactor.
// 29-Mar-2007  T.Aso,  Bug fix for momentum direction at outgoing flux.
// 2010-07-22   Introduce Unit specification.
// 2010-07-22   Add weighted and divideByAre options
///////////////////////////////////////////////////////////////////////////////

Kerma_Evaluation::Kerma_Evaluation(G4String name, DetectorConstruction* det,
					 G4int direction, G4int depth)
  : G4VPrimitiveScorer(name,depth), detectorConst(det), HCID(-1),fDirection(direction),EvtMap(0),
    weighted(true),divideByArea(true)
{
    DefineUnitAndCategory();
    SetUnit("percm2");
}

Kerma_Evaluation::Kerma_Evaluation(G4String name, DetectorConstruction* det, 
					 G4int direction, 
					 const G4String& unit,
					 G4int depth)
  : G4VPrimitiveScorer(name,depth), detectorConst(det), HCID(-1),fDirection(direction),EvtMap(0),
    weighted(true),divideByArea(true)
{
    DefineUnitAndCategory();
    SetUnit(unit);
}

Kerma_Evaluation::~Kerma_Evaluation()
{;}

G4bool Kerma_Evaluation::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4VPhysicalVolume* physVol = preStep->GetPhysicalVolume();
  G4VPVParameterisation* physParam = physVol->GetParameterisation();
  G4VSolid * solid = 0;
  if(physParam)
  { // for parameterized volume
    G4int idx = ((G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable()))
                ->GetReplicaNumber(indexDepth);
    solid = physParam->ComputeSolid(idx, physVol);
    solid->ComputeDimensions(physParam,idx,physVol);
  }
  else
  { // for ordinary volume
    solid = physVol->GetLogicalVolume()->GetSolid();
  }

  G4Box* boxSolid = (G4Box*)(solid);

  G4int dirFlag =IsSelectedSurface(aStep,boxSolid);
  if ( dirFlag > 0 ) {
    if ( fDirection == fFlux_InOut || fDirection == dirFlag ){

      G4StepPoint* thisStep=0;
      if ( dirFlag == fFlux_In ){
	thisStep = preStep;
      }else if ( dirFlag == fFlux_Out ){
	thisStep = aStep->GetPostStepPoint();
      }else{
	return FALSE;
      }

      G4TouchableHandle theTouchable = thisStep->GetTouchableHandle();
      G4ThreeVector pdirection = thisStep->GetMomentumDirection();
      G4ThreeVector localdir  = 
	theTouchable->GetHistory()->GetTopTransform().TransformAxis(pdirection);
      //
      G4double angleFactor = localdir.z();
      if ( angleFactor < 0 ) angleFactor *= -1.;
      G4double flux = 1.0;
      if ( weighted ) flux *=preStep->GetWeight(); // Current (Particle Weight)
      //
      G4double square = 4.*boxSolid->GetXHalfLength()*boxSolid->GetYHalfLength();
      //
      flux = flux/angleFactor;  // Flux with angle.
      if ( divideByArea ) flux /= square;
      //
      G4int index = GetIndex(aStep);

//Kerma evaluation procedure

G4double e_vertex = aStep->GetTrack()->GetVertexKineticEnergy();
G4double multiply_energy_by_mass_en_rho = e_vertex * AirMassEnergyAbsCoeff(e_vertex);
G4double kerma = flux * multiply_energy_by_mass_en_rho; 

//G4cout << "Flux= "  << flux*(cm*cm) << " halfX " << boxSolid->GetXHalfLength()/cm << " halfY " << boxSolid->GetYHalfLength()/cm << " angle factor " << angleFactor << " area " <<  square/(cm*cm) << " " << e_vertex << " en_rho " << AirMassEnergyAbsCoeff(e_vertex) <<  G4endl;

//G4cout << "Flux= "  << flux*(cm*cm)  << " angle_factor " << angleFactor  <<  G4endl;
      EvtMap->add(index,kerma);
    }
  }
#ifdef debug
    G4cout << " PASSED vol " 
	   << index << " trk "<<trkid<<" len " << fKerma_Evaluation<<G4endl;
#endif

  return TRUE;
}

G4int Kerma_Evaluation::IsSelectedSurface(G4Step* aStep, G4Box* boxSolid){

  G4TouchableHandle theTouchable = 
    aStep->GetPreStepPoint()->GetTouchableHandle();
  G4double kCarTolerance=G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();

  if (aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary ){
    // Entering Geometry
    G4ThreeVector stppos1= aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localpos1 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos1);
    if(std::fabs( localpos1.z() + boxSolid->GetZHalfLength())<kCarTolerance ){
      return fFlux_In;
    }
  }

  if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary ){
    // Exiting Geometry
    G4ThreeVector stppos2= aStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector localpos2 = 
      theTouchable->GetHistory()->GetTopTransform().TransformPoint(stppos2);
    if(std::fabs( localpos2.z() + boxSolid->GetZHalfLength())<kCarTolerance ){
      return fFlux_Out;
    }
  }

  return -1;
}

void Kerma_Evaluation::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
				    GetName());
  if ( HCID < 0 ) HCID = GetCollectionID(0);
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void Kerma_Evaluation::EndOfEvent(G4HCofThisEvent*)
{;}

void Kerma_Evaluation::clear(){
  EvtMap->clear();
}

void Kerma_Evaluation::DrawAll()
{;}

void Kerma_Evaluation::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer" << GetName() <<G4endl; 
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  flux  : " << *(itr->second)/GetUnitValue() 
	   << " [" << GetUnit() <<"]"
	   << G4endl;
  }
}

void Kerma_Evaluation::SetUnit(const G4String& unit)
{
    if ( divideByArea ) {
	CheckAndSetUnit(unit,"Per Unit Surface");
    } else {
	if (unit == "" ){
	    unitName = unit;
	    unitValue = 1.0;
	}else{
	    G4String msg = "Invalid unit ["+unit+"] (Current  unit is [" +GetUnit()+"] ) for " + GetName();
	    G4Exception("Kerma_Evaluation::SetUnit","DetPS0008",JustWarning,msg);
	}
    }
}

void Kerma_Evaluation::DefineUnitAndCategory(){
   // Per Unit Surface
   new G4UnitDefinition("percentimeter2","percm2","Per Unit Surface",(1./cm2));
   new G4UnitDefinition("permillimeter2","permm2","Per Unit Surface",(1./mm2));
   new G4UnitDefinition("permeter2","perm2","Per Unit Surface",(1./m2));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double Kerma_Evaluation::AirMassEnergyAbsCoeff(G4double energy)
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
