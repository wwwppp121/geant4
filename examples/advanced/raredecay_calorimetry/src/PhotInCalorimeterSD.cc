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
// $Id: PhotInCalorimeterSD.cc,v 1.1 2005-05-11 10:37:19 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "PhotInCalorimeterSD.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Track.hh"
#include "G4ios.hh"

G4int PhotInCalorimeterSD::numberOfLayers = 40;

PhotInCalorimeterSD::PhotInCalorimeterSD(G4String name)
:G4VSensitiveDetector(name),AbsCollID(-1),GapCollID(-1)
{
  collectionName.insert("AbsCollection");
  collectionName.insert("GapCollection");
}

PhotInCalorimeterSD::~PhotInCalorimeterSD()
{;}

void PhotInCalorimeterSD::Initialize(G4HCofThisEvent*HCE)
{
  AbsCollection = new PhotInCalorHitsCollection
                      (SensitiveDetectorName,collectionName[0]); 
  for(int i=0;i<numberOfLayers;i++)
  { AbsCollection->insert(new PhotInCalorHit); }
  if(AbsCollID<0)
  { AbsCollID = GetCollectionID(0); }
  HCE->AddHitsCollection(AbsCollID,AbsCollection);

  GapCollection = new PhotInCalorHitsCollection
                      (SensitiveDetectorName,collectionName[1]); 
  for(int j=0;j<numberOfLayers;j++)
  { GapCollection->insert(new PhotInCalorHit); }
  if(GapCollID<0)
  { GapCollID = GetCollectionID(1); }
  HCE->AddHitsCollection(GapCollID,GapCollection);
}

G4bool PhotInCalorimeterSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  
  G4double stepl = aStep->GetStepLength();
  G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
      
  G4TouchableHistory* theTouchable
    = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
    
  G4int LayerNumber = theTouchable->GetReplicaNumber();

  G4int copyNo = LayerNumber/2;
  if(LayerNumber%2)
  { // odd number corresponds to Gap
    (*GapCollection)[copyNo]->AddEnergy(edep);
    if(particle==G4Electron::ElectronDefinition() 
     || particle==G4Positron::PositronDefinition())
     (*GapCollection)[copyNo]->AddStep(stepl);
  }
  else
  { // even number corresponds to Abs
    (*AbsCollection)[copyNo]->AddEnergy(edep);
    if(particle==G4Electron::ElectronDefinition() 
     || particle==G4Positron::PositronDefinition())
     (*AbsCollection)[copyNo]->AddStep(stepl);
  }
  
  return true;
}

void PhotInCalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{;}

void PhotInCalorimeterSD::clear()
{;} 

void PhotInCalorimeterSD::DrawAll()
{;} 

void PhotInCalorimeterSD::PrintAll()
{;} 


