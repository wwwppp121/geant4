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
// $Id: G4EmCorrections.hh,v 1.7 2006-06-02 16:25:27 vnivanch Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4EmCorrections
//
// Author:        Vladimir Ivanchenko
//
// Creation date: 13.01.2005
//
// Modifications:
// 28.04.2006 General cleanup, add finite size corrections (V.Ivanchenko)
// 13.05.2006 Add corrections for ion stopping (V.Ivanhcenko)
//
// Class Description:
//
// This class provides calculation of EM corrections to ionisation
//

// -------------------------------------------------------------------
//

#ifndef G4EmCorrections_h
#define G4EmCorrections_h 1

#include "globals.hh"
#include "G4AtomicShells.hh"
#include "G4ionEffectiveCharge.hh"
#include "G4Material.hh"
#include "G4ParticleDefinition.hh"

class G4NistManager;
class G4VEmModel;
class G4PhysicsVector;

class G4EmCorrections
{

public:

  G4EmCorrections();

  virtual ~G4EmCorrections();

  G4double HighOrderCorrections(const G4ParticleDefinition* p,
                                const G4Material* material,
                                      G4double kineticEnergy);

  G4double Bethe(const G4ParticleDefinition* p,
                 const G4Material* material,
		 G4double kineticEnergy);

  G4double SpinCorrection(const G4ParticleDefinition* p,
                          const G4Material* material,
			  G4double kineticEnergy);

  G4double KShellCorrection(const G4ParticleDefinition* p,
                            const G4Material* material,
			    G4double kineticEnergy);

  G4double LShellCorrection(const G4ParticleDefinition* p,
                            const G4Material* material,
			    G4double kineticEnergy);

  G4double ShellCorrection(const G4ParticleDefinition* p,
                           const G4Material* material,
			   G4double kineticEnergy);

  G4double ShellCorrectionSTD(const G4ParticleDefinition* p,
                              const G4Material* material,
			      G4double kineticEnergy);

  G4double DensityCorrection(const G4ParticleDefinition* p,
                             const G4Material* material,
			     G4double kineticEnergy);

  G4double BarkasCorrection(const G4ParticleDefinition* p,
                            const G4Material* material,
			    G4double kineticEnergy);

  G4double BlochCorrection(const G4ParticleDefinition* p,
                           const G4Material* material,
			   G4double kineticEnergy);

  G4double MottCorrection(const G4ParticleDefinition* p,
                          const G4Material* material,
			  G4double kineticEnergy);

  G4double FiniteSizeCorrection(const G4ParticleDefinition* p,
				const G4Material* material,
				G4double kineticEnergy);

  G4double NuclearDEDX(const G4ParticleDefinition* p,
                       const G4Material* material,
		       G4double kineticEnergy,
		       G4bool fluct = true);

  void AddStoppingData(G4int Z, G4int A, const G4String& materialName,
		       G4PhysicsVector& dVector);

  G4double EffectiveChargeCorrection(const G4ParticleDefinition*,
				     const G4Material*,
				     G4double);

  G4ionEffectiveCharge* GetIonEffectiveCharge(G4VEmModel* m = 0);

  G4int GetNumberOfStoppingVectors();

private:

  void Initialise();

  G4PhysicsVector* InitialiseMaterial(const G4Material* mat);

  void SetupKinematics(const G4ParticleDefinition* p,
		       const G4Material* material,
		       G4double kineticEnergy);

  G4double KShell(G4double theta, G4double eta);

  G4double LShell(G4double theta, G4double eta);

  G4int Index(G4double x, G4double* y, G4int n);

  G4double Value(G4double xv, G4double x1, G4double x2, G4double y1, G4double y2);

  G4double Value2(G4double xv, G4double yv, G4double x1, G4double x2,
                  G4double y1, G4double y2,
                  G4double z11, G4double z21, G4double z12, G4double z22);

  G4double NuclearStoppingPower(G4double e, G4double z1, G4double z2,
                                            G4double m1, G4double m2);

  // hide assignment operator
  G4EmCorrections & operator=(const G4EmCorrections &right);
  G4EmCorrections(const G4EmCorrections&);

  G4double     engBarkas[47];
  G4double     corBarkas[47];
  G4double     ed[104];
  G4double     a[104];
  G4double     theZieglerFactor;
  G4double     alpha2;
  G4bool       lossFlucFlag;

  G4int        verbose;

  G4int        nK;
  G4int        nL;
  G4int        nEtaK;
  G4int        nEtaL;

  G4double     COSEB[14];
  G4double     COSXI[14];
  G4double     ZD[11];

  G4double     TheK[20];
  G4double     SK[20];
  G4double     TK[20];
  G4double     UK[20];
  G4double     VK[20];
  G4double     ZK[20];

  G4double     TheL[26];
  G4double     SL[26];
  G4double     TL[26];
  G4double     UL[26];
  G4double     VL[26];

  G4double     Eta[29];
  G4double     CK[20][29];
  G4double     CL[26][28];
  G4double     HM[53];
  G4double     HN[31];
  G4double     MSH[93];
  G4double     TAU[93];

  const G4ParticleDefinition* particle;
  const G4ParticleDefinition* curParticle;
  const G4Material*           material;
  const G4Material*           curMaterial;
  const G4ElementVector*      theElementVector;
  const G4double*             atomDensity;

  G4int     numberOfElements;
  G4double  kinEnergy;
  G4double  mass;
  G4double  massFactor;
  G4double  tau;
  G4double  gamma;
  G4double  bg2;
  G4double  beta2;
  G4double  beta;
  G4double  ba2;
  G4double  tmax;
  G4double  tmax0;
  G4double  charge;
  G4double  q2;

  G4AtomicShells        shells;
  G4ionEffectiveCharge  effCharge;

  G4NistManager*              nist;
  G4VEmModel*                 ionModel;

  // Ion stopping data
  G4int                       nIons;
  G4int                       idx;
  std::vector<G4int>          Zion;
  std::vector<G4int>          Aion;
  std::vector<G4String>       materialName;

  std::vector<const G4Material*> materialList;
  std::vector<G4PhysicsVector*>  stopData;
  G4PhysicsVector*               curVector;
};

inline G4int G4EmCorrections::Index(G4double x, G4double* y, G4int n)
{
  G4int iddd = n-1;
  do {iddd--;} while (iddd>0 && x<y[iddd]);
  return iddd;
}

inline G4double G4EmCorrections::Value(G4double xv, G4double x1, G4double x2,
                                       G4double y1, G4double y2)
{
  return y1 + (y2 - y1)*(xv - x1)/(x2 - x1);
}

inline G4double G4EmCorrections::Value2(G4double xv, G4double yv, 
					G4double x1, G4double x2,
                                        G4double y1, G4double y2,
					G4double z11, G4double z21, 
					G4double z12, G4double z22)
{
  return (z11*(x2-xv)*(y2-yv) + z22*(xv-x1)*(yv-y1) +
	  0.5*(z12*((x2-xv)*(yv-y1)+(xv-x1)*(y2-yv))+
	       z21*((xv-x1)*(y2-yv)+(yv-y1)*(x2-xv))))
         / ((x2-x1)*(y2-y1));
}

inline void G4EmCorrections::SetupKinematics(const G4ParticleDefinition* p,
					     const G4Material* mat,
					     G4double kineticEnergy)
{
  if(kineticEnergy != kinEnergy || p != particle) {
    particle = p;
    kinEnergy = kineticEnergy;
    mass  = p->GetPDGMass();
    tau   = kineticEnergy / mass;
    gamma = 1.0 + tau;
    bg2   = tau * (tau+2.0);
    beta2 = bg2/(gamma*gamma);
    beta  = std::sqrt(beta2);
    ba2   = beta2/alpha2;
    G4double ratio = electron_mass_c2/mass;
    tmax  = 2.0*electron_mass_c2*bg2 /(1. + 2.0*gamma*ratio + ratio*ratio);
    tmax0 = tmax;
    charge  = p->GetPDGCharge()/eplus;
    if(charge < 1.5)  q2 = charge*charge;
    else {
      q2 = effCharge.EffectiveChargeSquareRatio(p,mat,kinEnergy);
      charge = std::sqrt(q2);
    }
    if(mass > 120.*MeV)
      tmax = std::min(tmax,51200.*electron_mass_c2*std::pow(proton_mass_c2/mass,0.666667));
  }
  if(mat != material) {
    material = mat;
    theElementVector = material->GetElementVector();
    atomDensity  = material->GetAtomicNumDensityVector(); 
    numberOfElements = material->GetNumberOfElements();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif
