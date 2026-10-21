#ifndef Kerma_Evaluation_h
#define Kerma_Evaluation_h 1

#include "G4VPrimitiveScorer.hh"
#include "G4THitsMap.hh"
#include "G4Box.hh"
#include "G4PSDirectionFlag.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description) Modified by Christian Fedon on May 2017
//
//  This is a primitive scorer class for scoring the Air kerma based on the Surface Flux example.
//  Current version assumes only for G4Box shape, and the surface
//  is defined at the -Z plane of the box.
//   The surface flux is given in the unit of area. 
//    e.g.  sum of 1/cos(T)/mm2,  where T is a incident angle of the
//                                track on the surface.
//
//
// Direction      (in paddle) +Z    -Z (exit paddle)   
//   0  IN || OUT              |   ->|<-                fFlux_InOut
//   1  IN                     |     |<-                fFlux_In
//   2  OUT                    |   ->|                  fFlux_Out
// Created: 2005-11-14  Tsukasa ASO, Akinori Kimura.
// 
// 18-Nov-2005  T.Aso,  To use always positive value for anglefactor.
//                      Bug fix. Area definition.
// 29-Mar-2007  T.Aso,  Bug fix for momentum direction at outgoing flux.
// 2010-07-22   Introduce Unit specification.
// 2010-07-22   Add weighted and divideByArea options
///////////////////////////////////////////////////////////////////////////////

class DetectorConstruction;

class Kerma_Evaluation : public G4VPrimitiveScorer
{
  public: // with description
      Kerma_Evaluation(G4String name, DetectorConstruction*, G4int direction, G4int depth=0);
      Kerma_Evaluation(G4String name, DetectorConstruction*, G4int direction, 
			  const G4String& unit, G4int depth=0); 
      virtual ~Kerma_Evaluation();

      inline void Weighted(G4bool flg=true) { weighted = flg; }
      // Multiply track weight

      inline void DivideByArea(G4bool flg=true) { divideByArea = flg; }
      // Divided by Area.

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      G4int IsSelectedSurface(G4Step*,G4Box*);
      G4double AirMassEnergyAbsCoeff(G4double);

  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

      virtual void SetUnit(const G4String& unit);

  protected:
      virtual void DefineUnitAndCategory();

 public:
		 	DetectorConstruction* 	detectorConst;
			G4int  HCID;
      G4int  fDirection;
      G4THitsMap<G4double>* EvtMap;
      G4bool weighted;
      G4bool divideByArea;
   
     
};

#endif

