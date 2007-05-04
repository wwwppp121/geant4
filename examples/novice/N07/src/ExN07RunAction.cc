//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: ExN07RunAction.cc,v 1.7 2007-05-04 01:49:28 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "ExN07RunAction.hh"
#include "ExN07Run.hh"

#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"

#include "G4VSteppingVerbose.hh"
#include "ExN07SteppingVerbose.hh"

ExN07RunAction::ExN07RunAction()
{;}

ExN07RunAction::~ExN07RunAction()
{;}

G4Run* ExN07RunAction::GenerateRun()
{ return new ExN07Run; }

void ExN07RunAction::BeginOfRunAction(const G4Run*)
{
  ExN07SteppingVerbose* sv = (ExN07SteppingVerbose*)(G4VSteppingVerbose::GetInstance());
  sv->InitializeTimers();
}

void ExN07RunAction::EndOfRunAction(const G4Run* aRun)
{
  static G4String regName[3] = {"Calor-A","Calor-B","Calor-C"};

  const ExN07Run* theRun = (const ExN07Run*)aRun;
  
  G4cout
    << "############################################################" << G4endl;
  G4cout 
    << " Run Summary - Number of events : " << theRun->GetNumberOfEvent() 
    << G4endl;
  G4cout
    << "############################################################" << G4endl;

  G4double nEvt = (G4double)(theRun->GetNumberOfEvent());
  for(size_t i=0;i<3;i++)
  {
    size_t ih1 = 2*i;
    size_t ih2 = 2*i+1;

    G4Region* region = G4RegionStore::GetInstance()->GetRegion(regName[i]);
    G4ProductionCuts* cuts = region->GetProductionCuts();
    G4cout << "Region : " << region->GetName() << G4endl;
    G4cout << " Production thresholds :" << G4endl << "   "
           << " gamma " << G4BestUnit(cuts->GetProductionCut("gamma"),"Length")
           << "    e- " << G4BestUnit(cuts->GetProductionCut("e-"),"Length")
           << "    e+ " << G4BestUnit(cuts->GetProductionCut("e+"),"Length")
           << G4endl;
    G4cout << " Energy deposition in an event :" << G4endl << "   "
           << " Absorber " << G4BestUnit((theRun->GetTotalE(ih1))/nEvt,"Energy")
           << "      Gap " << G4BestUnit((theRun->GetTotalE(ih2))/nEvt,"Energy")
           << G4endl;
    G4cout << " Number of secondaries in an event :" << G4endl << "   "
           << " gamma in Absorber " << (theRun->GetNGamma(ih1))/nEvt
           << "    in Gap " << (theRun->GetNGamma(ih2))/nEvt << G4endl << "   "
           << " e-    in Absorber " << (theRun->GetNElectron(ih1))/nEvt
           << "    in Gap " << (theRun->GetNElectron(ih2))/nEvt << G4endl << "   "
           << " e+    in Absorber " << (theRun->GetNPositron(ih1))/nEvt
           << "    in Gap " << (theRun->GetNPositron(ih2))/nEvt << G4endl;
    G4cout << " Minimum kinetic energy of generated secondaries :" << G4endl << "   "
           << " gamma in Absorber " << G4BestUnit(theRun->GetEMinGamma(ih1),"Energy")
           << "    in Gap " << G4BestUnit(theRun->GetEMinGamma(ih2),"Energy")
           << G4endl << "   "
           << " e-    in Absorber " << G4BestUnit(theRun->GetEMinElectron(ih1),"Energy")
           << "    in Gap " << G4BestUnit(theRun->GetEMinElectron(ih2),"Energy")
           << G4endl << "   "
           << " e+    in Absorber " << G4BestUnit(theRun->GetEMinPositron(ih1),"Energy")
           << "    in Gap " << G4BestUnit(theRun->GetEMinPositron(ih2),"Energy")
           << G4endl;
    G4cout << " Total track length of e+/e- in an event :" << G4endl << "   "
           << " Absorber " << G4BestUnit((theRun->GetTotalL(ih1))/nEvt,"Length")
           << "      Gap " << G4BestUnit((theRun->GetTotalL(ih2))/nEvt,"Length")
           << G4endl;
    G4cout << " Total number of steps of e+/e- in an event :" << G4endl << "   "
           << " Absorber " << (theRun->GetNStep(ih1))/nEvt
           << "      Gap " << (theRun->GetNStep(ih2))/nEvt
           << G4endl;
    G4cout
    << "------------------------------------------------------------" << G4endl;
    G4cout << "Scores in parallel geometry" << G4endl;
    G4cout << "layer  eDep/evt   nGamma/evt   nElec/evt   nPosi/evt   stepLen/evt   nStep/evt"
           << G4endl;
    for(size_t k=0;k<20;k++)
    {
      G4cout << "  " << k;
      for(size_t j=0;j<6;j++)
      { G4cout << "  " << (theRun->GetParaValue(i,j,k))/nEvt; }
      G4cout << G4endl;
    }
    G4cout
    << "############################################################" << G4endl;
  }
  ExN07SteppingVerbose* sv = (ExN07SteppingVerbose*)(G4VSteppingVerbose::GetInstance());
  sv->Report();
}

