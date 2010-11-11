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
// $Id: G4NeutronElasticXS.cc,v 1.8 2010-11-11 21:39:16 dennis Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:    G4NeutronElasticXS
//
// Author  Ivantchenko, Geant4, 3-Aug-09
//
// Modifications:
//

#include "G4NeutronElasticXS.hh"
#include "G4Neutron.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4PhysicsLogVector.hh"
#include "G4PhysicsVector.hh"
#include "G4GlauberGribovCrossSection.hh"
#include "G4HadronNucleonXsc.hh"
#include "G4NistManager.hh"
#include "G4Proton.hh"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

G4NeutronElasticXS::G4NeutronElasticXS() 
  :  proton(G4Proton::Proton()), maxZ(92)
{
  //  verboseLevel = 0;
  if(verboseLevel > 0){
    G4cout  << "G4NeutronElasticXS::G4NeutronElasticXS Initialise for Z < " 
	    << maxZ + 1 << G4endl;
  }
  data.resize(maxZ+1, 0);
  coeff.resize(maxZ+1, 1.0);
  ggXsection = new G4GlauberGribovCrossSection();
  fNucleon = new G4HadronNucleonXsc();
  isInitialized = false;
}

G4NeutronElasticXS::~G4NeutronElasticXS()
{
  for(G4int i=0; i<=maxZ; ++i) {
    delete data[i];
  }
}

G4bool 
G4NeutronElasticXS::IsApplicable(const G4DynamicParticle*, 
				 const G4Element*)
{
  return true;
}

G4bool 
G4NeutronElasticXS::IsIsoApplicable(const G4DynamicParticle*,
                                    G4int /*ZZ*/, G4int /*AA*/)
{
  return false;
}

G4double 
G4NeutronElasticXS::GetCrossSection(const G4DynamicParticle* aParticle,
				    const G4Element* elm, G4double)
{
  G4double xs = 0.0;
  G4double ekin = aParticle->GetKineticEnergy();

  G4int Z = G4int(elm->GetZ());
  if(Z < 1 || Z > maxZ) { return xs; }
  G4PhysicsVector* pv = data[Z];
  //  G4cout  << "G4NeutronElasticXS::GetCrossSection e= " << ekin << " Z= " << Z << G4endl;

  // element was not initialised
  if(!pv) {
    Initialise(Z);
    pv = data[Z];
    if(!pv) { return xs; }
  }

  G4double e1 = pv->Energy(0);
  if(ekin <= e1) { return (*pv)[0]; }

  G4int n = pv->GetVectorLength() - 1;
  G4double e2 = pv->Energy(n);

  if(ekin <= e2) { 
    xs = pv->Value(ekin); 
  } else if(1 == Z) { 
    fNucleon->GetHadronNucleonXscPDG(aParticle, proton);
    xs = coeff[1]*fNucleon->GetElasticHadronNucleonXsc();
  } else {          
    ggXsection->GetCrossSection(aParticle, elm);
    xs = coeff[Z]*ggXsection->GetElasticGlauberGribovXsc();
  }

  if(verboseLevel > 0){
    G4cout  << "ekin= " << ekin << ",  XSinel= " << xs << G4endl;
  }
  return xs;
}


void 
G4NeutronElasticXS::BuildPhysicsTable(const G4ParticleDefinition& p)
{
  if(verboseLevel > 0){
    G4cout << "G4NeutronElasticXS::BuildPhysicsTable for " 
	   << p.GetParticleName() << G4endl;
  }
  if(isInitialized || p.GetParticleName() != "neutron") { return; }
  isInitialized = true;

  // check environment variable 
  // Build the complete string identifying the file with the data set
  char* path = getenv("G4NEUTRONXSDATA");
  if (!path){
    G4cout << "G4NEUTRONXSDATA environment variable not set" << G4endl;
  }

  G4DynamicParticle* dynParticle = 
    new G4DynamicParticle(G4Neutron::Neutron(),G4ThreeVector(1,0,0),1);

  // Access to elements
  const G4ElementTable* theElmTable = G4Element::GetElementTable();
  size_t numOfElm = G4Element::GetNumberOfElements();
  if(numOfElm > 0) {
    for(size_t i=0; i<numOfElm; ++i) {
      G4int Z = G4int(((*theElmTable)[i])->GetZ());
      if(Z < 1)         { Z = 1; }
      else if(Z > maxZ) { Z = maxZ; }
      //G4cout << "Z= " << Z << G4endl;
      // Initialisation 
      if(!data[Z]) { Initialise(Z, dynParticle, path); }
    }
  }
  delete dynParticle;
}

void 
G4NeutronElasticXS::DumpPhysicsTable(const G4ParticleDefinition&)
{
}

void 
G4NeutronElasticXS::Initialise(G4int Z, G4DynamicParticle* dp, 
			       const char* p)
{
  if(data[Z]) { return; }
  const char* path = p;
  if(!p) {
    // check environment variable 
    // Build the complete string identifying the file with the data set
    path = getenv("G4NEUTRONXSDATA");
    if (!path) {
      if(verboseLevel > 1) {
	G4cout << "G4NEUTRONXSDATA environment variable not set" << G4endl;
      }
      return;
    }
  }
  G4DynamicParticle* dynParticle = dp;
  if(!dp) {
    dynParticle = 
      new G4DynamicParticle(G4Neutron::Neutron(),G4ThreeVector(1,0,0),1);
  }

  const G4Element* Elem = 
    G4NistManager::Instance()->FindOrBuildElement(Z, true);

  // upload data from file
  data[Z] = new G4PhysicsLogVector();

  std::ostringstream ost;
  ost << path << "/elast" << Z ;
  std::ifstream filein(ost.str().c_str());
  if (!(filein)) {
    G4cout << " file " << ost << "  is not opened" << G4endl;
  }else{
    if(verboseLevel > 1) {
      G4cout << ost << " is opened" << G4endl;
    }
    // retrieve data from DB
    data[Z]->Retrieve(filein, true);

    // smooth transition 
    size_t n      = data[Z]->GetVectorLength() - 1;
    G4double emax = data[Z]->Energy(n);
    G4double sig1 = (*data[Z])[n];
    dynParticle->SetKineticEnergy(emax);
    G4double sig2 = 0.0;
    if(1 == Z) {
      fNucleon->GetHadronNucleonXscPDG(dynParticle, proton);
      sig2 = fNucleon->GetElasticHadronNucleonXsc();
    } else {
      ggXsection->GetCrossSection(dynParticle, Elem);
      sig2 = ggXsection->GetElasticGlauberGribovXsc();
    }
    if(sig2 > 0.) { coeff[Z] = sig1/sig2; } 
  } 
  if(!dp) { delete dynParticle; }
}
