//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: Tst33ScorerBuilder.cc,v 1.5 2002-11-20 09:38:26 dressel Exp $
// GEANT4 tag 
//
// ----------------------------------------------------------------------
// GEANT 4 class source file
//
// Tst33ScorerBuilder.cc
//
// ----------------------------------------------------------------------

#include "Tst33ScorerBuilder.hh"
#include "G4CellScorerStore.hh"
#include "G4CellScorer.hh"
#include "Tst33VGeometry.hh"

Tst33ScorerBuilder::Tst33ScorerBuilder()
{}

Tst33ScorerBuilder::~Tst33ScorerBuilder()
{}


G4CellScorerStore *Tst33ScorerBuilder::
CreateScorer(Tst33VGeometry *samplegeo, 
	     const G4CellScorer **specialCellScorer){
  G4GeometryCell gWorldCell(samplegeo->GetWorldVolume(), -1);
  
  G4CellScorerStore *cs_store = new G4CellScorerStore();
  if (!cs_store) {
    G4Exception("Tst33ScorerBuilder::CreateScorer: new failed to create G4CellScorerStore!");
  }
  cs_store->AddCellScorer(gWorldCell);
  
  
  G4int i = 1;
  for (i=1; i <= 19; i++) {
    G4GeometryCell gCell(samplegeo->GetGeometryCell(i));
    const G4CellScorer *s = cs_store->AddCellScorer(gCell);
    if (i==18) {
      *specialCellScorer = s;
    }
  }
  return cs_store;
}
