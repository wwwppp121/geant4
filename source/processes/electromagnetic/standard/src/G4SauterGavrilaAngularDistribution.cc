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
// $Id: G4SauterGavrilaAngularDistribution.cc,v 1.1 2010-10-14 15:17:48 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4SauterGavrilaAngularDistribution
//
// Author:     Vladimir Ivanchenko using Michel Maire algorithm
//             developed for Geant3
// 
// Creation date: 23 July 2012
//
//
// -------------------------------------------------------------------
//

#include "G4SauterGavrilaAngularDistribution.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

G4SauterGavrilaAngularDistribution::G4SauterGavrilaAngularDistribution()
  : G4VEmAngularDistribution("AngularGenSauterGavrila")
{}    

G4SauterGavrilaAngularDistribution::~G4SauterGavrilaAngularDistribution() 
{}

G4ThreeVector& 
G4SauterGavrilaAngularDistribution::SampleDirection(
       const G4DynamicParticle* dp, G4double, G4int, const G4Material*)
{
  G4double gamma = 1. + dp->GetKineticEnergy()/electron_mass_c2;
  const G4double gammalimit = 30.0;

  if (gamma > gammalimit) {
    fLocalDirection = dp->GetMomentumDirection(); 
    // Bugzilla 1120
    // SI on 05/09/2010 as suggested by JG 04/09/10 
  } else {
 
    G4double invgamma = 1.0/gamma;
    G4double beta  = std::sqrt((gamma - 1)*(gamma + 1))*invgamma;
    G4double b     = 0.5*gamma*(gamma-1)*(gamma-2);
    G4double invgamma2 = invgamma*invgamma;
   
    G4double rndm,term,greject,grejsup,costeta,sint2;
    if (gamma < 2.) { grejsup = (1.+b-beta*b)/invgamma2; }
    else            { grejsup = (1.+b+beta*b)/invgamma2; }

    do { 
      rndm    = 1 - 2*G4UniformRand();
      costeta = (rndm + beta)/(rndm*beta + 1);
      term    = invgamma2/(1 + beta*rndm);
      sint2   = (1 - costeta)*(1 + costeta);
      greject = sint2*(1 + b*term)/(term*term);

    } while(greject < G4UniformRand()*grejsup);
       
    G4double sinteta = std::sqrt(sint2);
    G4double phi  = CLHEP::twopi*G4UniformRand(); 

    fLocalDirection.set(sinteta*cos(phi), sinteta*sin(phi), costeta);
    fLocalDirection.rotateUz(dp->GetMomentumDirection());
  }
  return fLocalDirection;
}

void G4SauterGavrilaAngularDistribution::PrintGeneratorInformation() const
{
  G4cout << "\n" << G4endl;
  G4cout << "Non-polarized photoelectric effect angular generator." << G4endl;
  G4cout << "The Sauter-Gavrila distribution for the K-shell is used."<<G4endl;
  G4cout << "Originally developed by M.Maire for Geant3" 
	 << G4endl;
} 