// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
// For information related to this code contact:
// Geant4 Collaboration
//
// File name:     G4hZiegler1977p
//
// Author:        V.Ivanchenko (Vladimir.Ivanchenko@cern.ch)
// 
// Creation date: 20 July 2000
//
// Modifications: 
// 20/07/2000  V.Ivanchenko First implementation
//
// Class Description: 
//
// Electronic stopping power parametrised according to
// H.H.Andersen & J.F.Ziegler, Hydrogen Stopping Powers and
// Ranges in All Elements, Vol.3, Pergamon Press, 1977
//
// Class Description: End 
//
// -------------------------------------------------------------------
//

#ifndef G4hZiegler1977p_h
#define G4hZiegler1977p_h 1

#include "G4VhElectronicStoppingPower.hh"

class G4hZiegler1977p : public G4VhElectronicStoppingPower
{

public:

  G4hZiegler1977p();

  ~G4hZiegler1977p();

  G4bool HasMaterial(const G4Material* material);

  G4double StoppingPower(const G4Material* material,
                               G4double kineticEnergy);

  G4double ElectronicStoppingPower(G4double z,
                                   G4double kineticEnergy) const;
 
protected:

private:
  const G4double protonMassAMU;

};

#endif
