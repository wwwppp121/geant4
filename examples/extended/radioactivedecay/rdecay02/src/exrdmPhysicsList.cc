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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "exrdmPhysicsList.hh"
#include "exrdmPhysicsListMessenger.hh"

#include "exrdmPhysListParticles.hh"
#include "G4EmStandardPhysics.hh"
#include "exrdmPhysListEmLowEnergy.hh"
#include "exrdmPhysListHadron.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "HadronPhysicsQGSP_BERT.hh"
#include "HadronPhysicsQGSP_BIC.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "HadronPhysicsQGSP_BIC_HP.hh"

#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4QStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4DecayPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

exrdmPhysicsList::exrdmPhysicsList() : G4VModularPhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 1.*mm;
  fCutForGamma     = defaultCutValue;
  fCutForElectron  = defaultCutValue;
  fCutForPositron  = defaultCutValue;

  fDetectorCuts = 0;
  fTargetCuts   = 0;

  fPMessenger = new exrdmPhysicsListMessenger(this);

  SetVerboseLevel(1);

  //default physics
  fParticleList = new G4DecayPhysics();

  //default physics
  fRaddecayList = new G4RadioactiveDecayPhysics();

  // EM physics
  fEmPhysicsList = new G4EmStandardPhysics();
  
  // Had physics 
  fHadPhysicsList = 0;
  fNhadcomp = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

exrdmPhysicsList::~exrdmPhysicsList()
{
  delete fPMessenger;
  delete fRaddecayList;
  delete fEmPhysicsList;
  if (fHadPhysicsList) delete fHadPhysicsList;
  if (fNhadcomp > 0) {
    for(G4int i=0; i<fNhadcomp; i++) {
      delete fHadronPhys[i];
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::ConstructParticle()
{
  fParticleList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::ConstructProcess()
{
  AddTransportation();
  // em
  fEmPhysicsList->ConstructProcess();
  // decays
  fParticleList->ConstructProcess();
  fRaddecayList->ConstructProcess();
  // had
  if (fNhadcomp > 0) {
    for(G4int i=0; i<fNhadcomp; i++) {
      (fHadronPhys[i])->ConstructProcess();
    }
  }
  if (fHadPhysicsList) fHadPhysicsList->ConstructProcess();
  G4cout << "### exrdmPhysicsList::ConstructProcess is done" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::SelectPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "exrdmPhysicsList::SelectPhysicsList: <" << name << ">" << G4endl;
  }
  // default  Had physics
  if (name == "Hadron" && !fHadPhysicsList) {
    fHadPhysicsList = new exrdmPhysListHadron("hadron");
  } else if (name == "QGSP_BERT") {
    AddExtraBuilders(false);
    fHadPhysicsList = new HadronPhysicsQGSP_BERT("std-hadron");
  } else if (name == "QGSP_BIC" && !fHadPhysicsList) {
    AddExtraBuilders(false);
    fHadPhysicsList = new HadronPhysicsQGSP_BIC("std-hadron");
  } else if (name == "QGSP_BERT_HP"  && !fHadPhysicsList) {
    AddExtraBuilders(true);
    fHadPhysicsList = new HadronPhysicsQGSP_BERT_HP("std-hadron");
  } else if (name == "QGSP_BIC_HP"  && !fHadPhysicsList) {
    AddExtraBuilders(true);
    fHadPhysicsList = new HadronPhysicsQGSP_BIC_HP("std-hadron");
  } else if (name == "LowEnergy_EM") {
      delete fEmPhysicsList;
      fEmPhysicsList = new exrdmPhysListEmLowEnergy("lowe-em");
  } else if (name == "Standard_EM") {
      delete fEmPhysicsList;
      fEmPhysicsList = new G4EmStandardPhysics();
  } else {
      G4cout << "exrdmPhysicsList WARNING wrong or unkonwn <" 
	     << name << "> Physics " << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::AddExtraBuilders(G4bool flagHP)
{
  fNhadcomp = 5;
  fHadronPhys.push_back( new G4EmExtraPhysics("extra EM"));
  fHadronPhys.push_back( new G4HadronElasticPhysics("elastic",verboseLevel,
						   flagHP));
  fHadronPhys.push_back( new G4QStoppingPhysics("stopping",verboseLevel));
  fHadronPhys.push_back( new G4IonBinaryCascadePhysics("ionBIC"));
  fHadronPhys.push_back( new G4NeutronTrackingCut("Neutron tracking cut"));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::SetCuts()
{
  SetCutValue(fCutForGamma, "gamma");
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForPositron, "e+");
  G4cout << "world cuts are set" << G4endl;

  if( !fTargetCuts ) SetTargetCut(fCutForElectron);
  G4Region* region = (G4RegionStore::GetInstance())->GetRegion("Target");
  region->SetProductionCuts(fTargetCuts);
  G4cout << "Target cuts are set" << G4endl;

  if( !fDetectorCuts ) SetDetectorCut(fCutForElectron);
  region = (G4RegionStore::GetInstance())->GetRegion("Detector");
  region->SetProductionCuts(fDetectorCuts);
  G4cout << "Detector cuts are set" << G4endl;

  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::SetCutForGamma(G4double cut)
{
  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::SetCutForElectron(G4double cut)
{
  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::SetCutForPositron(G4double cut)
{
  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::SetTargetCut(G4double cut)
{
  if( !fTargetCuts ) fTargetCuts = new G4ProductionCuts();

  fTargetCuts->SetProductionCut(cut, idxG4GammaCut);
  fTargetCuts->SetProductionCut(cut, idxG4ElectronCut);
  fTargetCuts->SetProductionCut(cut, idxG4PositronCut);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void exrdmPhysicsList::SetDetectorCut(G4double cut)
{
  if( !fDetectorCuts ) fDetectorCuts = new G4ProductionCuts();

  fDetectorCuts->SetProductionCut(cut, idxG4GammaCut);
  fDetectorCuts->SetProductionCut(cut, idxG4ElectronCut);
  fDetectorCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
