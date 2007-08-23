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
// $Id: G4NeutronEvaporationProbability.hh,v 1.6 2007-08-23 16:14:02 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// Hadronic Process: Nuclear De-excitations
// by V. Lara (Nov 1999) 
//



#ifndef G4NeutronEvaporationProbability_h
#define G4NeutronEvaporationProbability_h 1


#include "G4EvaporationProbability.hh"


class G4NeutronEvaporationProbability : public G4EvaporationProbability
{
public:
  // Only available constructor
  G4NeutronEvaporationProbability();
		
  ~G4NeutronEvaporationProbability() {}
private:  
  // Copy constructor
  G4NeutronEvaporationProbability(const G4NeutronEvaporationProbability &right);

  const G4NeutronEvaporationProbability & operator=(const G4NeutronEvaporationProbability &right);
  G4bool operator==(const G4NeutronEvaporationProbability &right) const;
  G4bool operator!=(const G4NeutronEvaporationProbability &right) const;
  

private:

  virtual G4double CalcAlphaParam(const G4Fragment & fragment) const 
  { return 0.76+2.2/std::pow(static_cast<G4double>(fragment.GetA()-GetA()),1.0/3.0);}
	
  virtual G4double CalcBetaParam(const G4Fragment & fragment) const 
  { return (2.12/std::pow(static_cast<G4double>(fragment.GetA()-GetA()),2.0/3.0) - 0.05)*MeV/
      CalcAlphaParam(fragment); }

  virtual G4double CalcRjParam(const G4Fragment & ) const 
  { return 1.0; }


  // Excitation energy levels 
  std::vector<G4double> ExcitEnergies;
  // Spin of excitation energy levels 
  std::vector<G4int> ExcitSpins;

};


#endif
