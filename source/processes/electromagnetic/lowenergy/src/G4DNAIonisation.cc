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
// $Id: G4DNAIonisation.cc,v 1.1 2009-01-12 14:26:03 sincerti Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4DNAIonisation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using namespace std;

G4DNAIonisation::G4DNAIonisation(const G4String& processName,
  G4ProcessType type):G4VEmProcess (processName, type),
    isInitialised(false)
{
  SetProcessSubType(51);
  SetMinKinEnergy(12.61*eV); // Minimum is for electrons
  SetMaxKinEnergy(10*MeV); // Maximum is for protons
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4DNAIonisation::~G4DNAIonisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4DNAIonisation::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) 
  {
    isInitialised = true;
    SetBuildTableFlag(false);
    G4double emin = MinKinEnergy();
    G4double emax = MaxKinEnergy();
//    if(!Model()) SetModel(new G4DNABornIonisationModel);
    if(!Model()) SetModel(new G4DNARuddIonisationModel);
    Model()->SetLowEnergyLimit(emin);
    Model()->SetHighEnergyLimit(emax);
    AddEmModel(1, Model());
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void G4DNAIonisation::PrintInfo()
{
  G4cout
    << " Total cross sections computed from " << Model()->GetName() << " model"
    << G4endl;
}         

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
