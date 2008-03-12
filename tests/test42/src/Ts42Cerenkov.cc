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
// $Id: Ts42Cerenkov.cc,v 1.3 2008-03-12 10:12:45 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
////////////////////////////////////////////////////////////////////////
// Cerenkov Radiation Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File:        Ts42Cerenkov.cc 
// Description: Discrete Process -- Generation of Cerenkov Photons
// Version:     2.1
// Created:     1996-02-21  
// Author:      Juliet Armstrong
// Updated:     2007-09-30 by Peter Gumplinger
//              > change inheritance to G4VDiscreteProcess
//              GetContinuousStepLimit -> GetMeanFreePath (StronglyForced)
//              AlongStepDoIt -> PostStepDoIt
//              2005-08-17 by Peter Gumplinger
//              > change variable name MeanNumPhotons -> MeanNumberOfPhotons
//              2005-07-28 by Peter Gumplinger
//              > add G4ProcessType to constructor
//              2001-09-17, migration of Materials to pure STL (mma) 
//              2000-11-12 by Peter Gumplinger
//              > add check on CerenkovAngleIntegrals->IsFilledVectorExist()
//              in method GetAverageNumberOfPhotons 
//              > and a test for MeanNumberOfPhotons <= 0.0 in DoIt
//              2000-09-18 by Peter Gumplinger
//              > change: aSecondaryPosition=x0+rand*aStep.GetDeltaPosition();
//                        aSecondaryTrack->SetTouchable(0);
//              1999-10-29 by Peter Gumplinger
//              > change: == into <= in GetContinuousStepLimit
//              1997-08-08 by Peter Gumplinger
//              > add protection against /0
//              > G4MaterialPropertiesTable; new physics/tracking scheme
//
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "G4Poisson.hh"
#include "Ts42Cerenkov.hh"
#include "HistoManager.hh"
#include "G4RotationMatrix.hh"
#include "G4AffineTransform.hh"

using namespace std;

/////////////////////////
// Class Implementation  
/////////////////////////

        //////////////
        // Operators
        //////////////

// Ts42Cerenkov::operator=(const Ts42Cerenkov &right)
// {
// }

        /////////////////
        // Constructors
        /////////////////

Ts42Cerenkov::Ts42Cerenkov(const G4String& processName, G4ProcessType type)
           : G4VDiscreteProcess(processName, type)
{
  G4cout << "Ts42Cerenkov::Ts42Cerenkov constructor" << G4endl;
  G4cout << "NOTE: this is now a G4VDiscreteProcess!" << G4endl;
  G4cout << "Required change in UserPhysicsList: " << G4endl;
  G4cout << "change: pmanager->AddContinuousProcess(theCerenkovProcess);" << G4endl;
  G4cout << "to:     pmanager->AddProcess(theCerenkovProcess);" << G4endl;
  G4cout << "        pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);" << G4endl;

  fTrackSecondariesFirst = false;
  fMaxPhotons = 0;

  thePhysicsTable = NULL;

  if ( verboseLevel > 0 ) 
  {
    G4cout << GetProcessName() << " is created " << G4endl;
  }
  BuildThePhysicsTable();
}

// Ts42Cerenkov::Ts42Cerenkov(const Ts42Cerenkov &right)
// {
// }

        ////////////////
        // Destructors
        ////////////////

Ts42Cerenkov::~Ts42Cerenkov() 
{
	if (thePhysicsTable != NULL) 
        {
	   thePhysicsTable->clearAndDestroy();
           delete thePhysicsTable;
	}
}

        ////////////
        // Methods
        ////////////

// PostStepDoIt
// 
//
// This routine is called for each tracking Step of a charged particle
// in a radiator. A Poisson-distributed number of photons is generated
// according to the Cerenkov formula, distributed evenly along the track
// segment and uniformly azimuth w.r.t. the particle direction. The 
// parameters are then transformed into the Master Reference System, and 
// they are added to the particle change. 

G4VParticleChange*
Ts42Cerenkov::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{

	//////////////////////////////////////////////////////
	// Should we ensure that the material is dispersive?
	//////////////////////////////////////////////////////

        aParticleChange.Initialize(aTrack);

        const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
        const G4Material* aMaterial = aTrack.GetMaterial();

	G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
	G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();

	G4ThreeVector x0 = pPreStepPoint->GetPosition();
        G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
	G4double t0 = pPreStepPoint->GetGlobalTime();

        G4MaterialPropertiesTable* aMaterialPropertiesTable =
                               aMaterial->GetMaterialPropertiesTable();
        if (!aMaterialPropertiesTable)
           return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

	const G4MaterialPropertyVector* Rindex = 
                aMaterialPropertiesTable->GetProperty("RINDEX"); 
        if (!Rindex) 
	   return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

        // particle charge
        const G4double charge = aParticle->GetDefinition()->GetPDGCharge();

        // particle beta
        const G4double beta = (pPreStepPoint ->GetBeta() +
                               pPostStepPoint->GetBeta())/2.;

	G4double MeanNumberOfPhotons = 
                 GetAverageNumberOfPhotons(charge,beta,aMaterial,Rindex);

        if (MeanNumberOfPhotons <= 0.0) 
        {

                // return unchanged particle and no secondaries

                aParticleChange.SetNumberOfSecondaries(0);
 
                return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

        }

        G4double step_length;
        step_length = aStep.GetStepLength();

	MeanNumberOfPhotons = MeanNumberOfPhotons * step_length;

	G4int NumPhotons = (G4int) G4Poisson(MeanNumberOfPhotons);

	if (NumPhotons <= 0) 
        {

		// return unchanged particle and no secondaries  

		aParticleChange.SetNumberOfSecondaries(0);
		
                return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}

	////////////////////////////////////////////////////////////////

	aParticleChange.SetNumberOfSecondaries(NumPhotons);

        if (fTrackSecondariesFirst) {
           if (aTrack.GetTrackStatus() == fAlive )
                   aParticleChange.ProposeTrackStatus(fSuspend);
        }
	
	////////////////////////////////////////////////////////////////

	G4double Pmin = Rindex->GetMinPhotonMomentum();
	G4double Pmax = Rindex->GetMaxPhotonMomentum();
	G4double dp = Pmax - Pmin;

	G4double nMax = Rindex->GetMaxProperty();

        G4double BetaInverse = 1./beta;

	G4double maxCos = BetaInverse / nMax; 
	G4double maxSin2 = (1.0 - maxCos) * (1.0 + maxCos);

	for (G4int i = 0; i < NumPhotons; i++) 
        {
		// Determine photon momentum

		G4double rand;
		G4double sampledMomentum, sampledRI; 
		G4double cosTheta, sin2Theta;
		
		// sample a momentum 

		do 
                {
			rand = G4UniformRand();	
			sampledMomentum = Pmin + rand * dp; 
			sampledRI = Rindex->GetProperty(sampledMomentum);
			cosTheta = BetaInverse / sampledRI;  

			sin2Theta = (1.0 - cosTheta)*(1.0 + cosTheta);
			rand = G4UniformRand();	

		} while (rand*maxSin2 > sin2Theta);

		// Generate random position of photon on cone surface 
		// defined by Theta 

		rand = G4UniformRand();

		G4double phi = twopi*rand;
		G4double sinPhi = sin(phi);
		G4double cosPhi = cos(phi);

		// calculate x,y, and z components of photon momentum 
		// (in coord system with primary particle direction 
		//  aligned with the z axis)

		G4double sinTheta = sqrt(sin2Theta); 
		G4double px = sinTheta*cosPhi;
		G4double py = sinTheta*sinPhi;
		G4double pz = cosTheta;

		// Create photon momentum direction vector 
		// The momentum direction is still with respect
	 	// to the coordinate system where the primary
		// particle direction is aligned with the z axis  

		G4ParticleMomentum photonMomentum(px, py, pz);

		// Rotate momentum direction back to global reference
		// system 

                photonMomentum.rotateUz(p0);

		// Determine polarization of new photon 

		G4double sx = cosTheta*cosPhi;
		G4double sy = cosTheta*sinPhi; 
		G4double sz = -sinTheta;

		G4ThreeVector photonPolarization(sx, sy, sz);

		// Rotate back to original coord system 

                photonPolarization.rotateUz(p0);

                // Tilt between front and energy expansion in crystal

        const G4RotationMatrix* rotM = pPostStepPoint->GetTouchable()->GetRotation();
        G4ThreeVector transl = pPostStepPoint->GetTouchable()->GetTranslation();
        G4AffineTransform transform = G4AffineTransform(rotM,transl);
        transform.Invert();
        G4ThreeVector localPhMom = transform.TransformAxis(photonMomentum);
        // G4ThreeVector localPol = transform.TransformAxis(photonPolarization);

        if ( (localPhMom.z() > 0.01 || localPhMom.z() < -0.01) &&
             (localPhMom.z() > -0.99 || localPhMom.z() < 0.99) ) 
	{
          G4double ratio = aMaterialPropertiesTable->
                                      GetConstProperty("RINDEXRATIO");
          G4double momPerp = std::sqrt(localPhMom.x()*localPhMom.x()+localPhMom.y()*localPhMom.y() );
          G4double tgTheta = momPerp/localPhMom.z();

          tgTheta *= ratio; // crystal tilting!

          cosTheta = std::sqrt(1/(1+tgTheta*tgTheta));
	  sin2Theta = (1.0 - cosTheta)*(1.0 + cosTheta);

          if(sin2Theta < 0.) sin2Theta = 0.;

          sinTheta = std::sqrt(sin2Theta);
          cosPhi = localPhMom.x()/momPerp;  
          sinPhi = localPhMom.y()/momPerp;
          localPhMom = G4ThreeVector( cosPhi*sinTheta, sinPhi*sinTheta, cosTheta );
          localPhMom.unit();

          transform.Invert();
          G4ThreeVector photonMomentum = transform.TransformAxis(localPhMom);
	}		
                // Generate a new photon:

                G4DynamicParticle* aCerenkovPhoton =
                  new G4DynamicParticle(G4OpticalPhoton::OpticalPhoton(), 
  					                 photonMomentum);
		aCerenkovPhoton->SetPolarization
				     (photonPolarization.x(),
				      photonPolarization.y(),
				      photonPolarization.z());

		aCerenkovPhoton->SetKineticEnergy(sampledMomentum);

                // Generate new G4Track object:

		rand = G4UniformRand();

                G4double delta = rand * aStep.GetStepLength();
		G4double deltaTime = delta /
                       ((pPreStepPoint->GetVelocity()+
                         pPostStepPoint->GetVelocity())/2.);

                G4double aSecondaryTime = t0 + deltaTime;

                G4ThreeVector aSecondaryPosition =
                                    x0 + rand * aStep.GetDeltaPosition();

		G4Track* aSecondaryTrack = 
		new G4Track(aCerenkovPhoton,aSecondaryTime,aSecondaryPosition);

                aSecondaryTrack->SetTouchableHandle((G4VTouchable*)0);

                aSecondaryTrack->SetParentID(aTrack.GetTrackID());

		aParticleChange.AddSecondary(aSecondaryTrack);
	}

	if (verboseLevel>0) {
	G4cout << "\n Exiting from Ts42Cerenkov::DoIt -- NumberOfSecondaries = " 
	     << aParticleChange.GetNumberOfSecondaries() << G4endl;
	}

	return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

// BuildThePhysicsTable for the Cerenkov process
// ---------------------------------------------
//

void Ts42Cerenkov::BuildThePhysicsTable()
{
	if (thePhysicsTable) return;

	const G4MaterialTable* theMaterialTable=
	 		       G4Material::GetMaterialTable();
	G4int numOfMaterials = G4Material::GetNumberOfMaterials();

	// create new physics table
	
	thePhysicsTable = new G4PhysicsTable(numOfMaterials);

	// loop for materials

	for (G4int i=0 ; i < numOfMaterials; i++)
	{
		G4PhysicsOrderedFreeVector* aPhysicsOrderedFreeVector =
					new G4PhysicsOrderedFreeVector();

		// Retrieve vector of refraction indices for the material
		// from the material's optical properties table 

		G4Material* aMaterial = (*theMaterialTable)[i];

		G4MaterialPropertiesTable* aMaterialPropertiesTable =
				aMaterial->GetMaterialPropertiesTable();

		if (aMaterialPropertiesTable) {

		   G4MaterialPropertyVector* theRefractionIndexVector = 
		    	   aMaterialPropertiesTable->GetProperty("RINDEX");

		   if (theRefractionIndexVector) {
		
		      // Retrieve the first refraction index in vector
		      // of (photon momentum, refraction index) pairs 

		      theRefractionIndexVector->ResetIterator();
		      ++(*theRefractionIndexVector);	// advance to 1st entry 

		      G4double currentRI = theRefractionIndexVector->
		  			   GetProperty();

		      if (currentRI > 1.0) {

			 // Create first (photon momentum, Cerenkov Integral)
			 // pair  

			 G4double currentPM = theRefractionIndexVector->
			 			 GetPhotonMomentum();
			 G4double currentCAI = 0.0;

			 aPhysicsOrderedFreeVector->
			 	 InsertValues(currentPM , currentCAI);

			 // Set previous values to current ones prior to loop

			 G4double prevPM  = currentPM;
			 G4double prevCAI = currentCAI;
                	 G4double prevRI  = currentRI;

			 // loop over all (photon momentum, refraction index)
			 // pairs stored for this material  

			 while(++(*theRefractionIndexVector))
			 {
				currentRI=theRefractionIndexVector->	
						GetProperty();

				currentPM = theRefractionIndexVector->
						GetPhotonMomentum();

				currentCAI = 0.5*(1.0/(prevRI*prevRI) +
					          1.0/(currentRI*currentRI));

				currentCAI = prevCAI + 
					     (currentPM - prevPM) * currentCAI;

				aPhysicsOrderedFreeVector->
				    InsertValues(currentPM, currentCAI);

				prevPM  = currentPM;
				prevCAI = currentCAI;
				prevRI  = currentRI;
			 }

		      }
		   }
		}

	// The Cerenkov integral for a given material
	// will be inserted in thePhysicsTable
	// according to the position of the material in
	// the material table. 

	thePhysicsTable->insertAt(i,aPhysicsOrderedFreeVector); 

	}
}

// GetMeanFreePath
// ---------------
//

G4double Ts42Cerenkov::GetMeanFreePath(const G4Track& aTrack,
                                           G4double,
                                           G4ForceCondition* condition)
{
        *condition = StronglyForced;

	// If user has defined an average maximum number of photons to
	// be generated in a Step, then return the Step length for that 
	// number of photons. 
 
	if (fMaxPhotons <= 0) return DBL_MAX;

        const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
        const G4Material* aMaterial = aTrack.GetMaterial();

        G4MaterialPropertiesTable* aMaterialPropertiesTable =
                               aMaterial->GetMaterialPropertiesTable();
        if (!aMaterialPropertiesTable) return DBL_MAX;

        const G4MaterialPropertyVector* Rindex =
                aMaterialPropertiesTable->GetProperty("RINDEX");
        if (!Rindex) return DBL_MAX;

        // particle charge
        const G4double charge = aParticle->GetDefinition()->GetPDGCharge();

        // particle beta
        const G4double beta = aParticle->GetTotalMomentum() /
                              aParticle->GetTotalEnergy();

	G4double MeanNumberOfPhotons = 
                 GetAverageNumberOfPhotons(charge,beta,aMaterial,Rindex);

        if(MeanNumberOfPhotons <= 0.0) return DBL_MAX;

	G4double StepLimit = fMaxPhotons / MeanNumberOfPhotons;

	return StepLimit;
}

// GetAverageNumberOfPhotons
// -------------------------
// This routine computes the number of Cerenkov photons produced per
// GEANT-unit (millimeter) in the current medium. 
//             ^^^^^^^^^^

G4double 
Ts42Cerenkov::GetAverageNumberOfPhotons(const G4double charge,
                              const G4double beta, 
			      const G4Material* aMaterial,
			      const G4MaterialPropertyVector* Rindex) const
{
	const G4double Rfact = 369.81/(eV * cm);

        if(beta <= 0.0)return 0.0;

        G4double BetaInverse = 1./beta;

	// Vectors used in computation of Cerenkov Angle Integral:
	// 	- Refraction Indices for the current material
	//	- new G4PhysicsOrderedFreeVector allocated to hold CAI's
 
	G4int materialIndex = aMaterial->GetIndex();

	// Retrieve the Cerenkov Angle Integrals for this material  

	G4PhysicsOrderedFreeVector* CerenkovAngleIntegrals =
	(G4PhysicsOrderedFreeVector*)((*thePhysicsTable)(materialIndex));

        if(!(CerenkovAngleIntegrals->IsFilledVectorExist()))return 0.0;

	// Min and Max photon momenta  
	G4double Pmin = Rindex->GetMinPhotonMomentum();
	G4double Pmax = Rindex->GetMaxPhotonMomentum();

	// Min and Max Refraction Indices 
	G4double nMin = Rindex->GetMinProperty();	
	G4double nMax = Rindex->GetMaxProperty();

	// Max Cerenkov Angle Integral 
	G4double CAImax = CerenkovAngleIntegrals->GetMaxValue();

	G4double dp, ge;

	// If n(Pmax) < 1/Beta -- no photons generated 

	if (nMax < BetaInverse) {
		dp = 0;
		ge = 0;
	} 

	// otherwise if n(Pmin) >= 1/Beta -- photons generated  

	else if (nMin > BetaInverse) {
		dp = Pmax - Pmin;	
		ge = CAImax; 
	} 

	// If n(Pmin) < 1/Beta, and n(Pmax) >= 1/Beta, then
	// we need to find a P such that the value of n(P) == 1/Beta.
	// Interpolation is performed by the GetPhotonMomentum() and
	// GetProperty() methods of the G4MaterialPropertiesTable and
	// the GetValue() method of G4PhysicsVector.  

	else {
		Pmin = Rindex->GetPhotonMomentum(BetaInverse);
		dp = Pmax - Pmin;

		// need boolean for current implementation of G4PhysicsVector
		// ==> being phased out
		G4bool isOutRange;
		G4double CAImin = CerenkovAngleIntegrals->
                                  GetValue(Pmin, isOutRange);
		ge = CAImax - CAImin;

		if (verboseLevel>0) {
			G4cout << "CAImin = " << CAImin << G4endl;
			G4cout << "ge = " << ge << G4endl;
		}
	}
	
	// Calculate number of photons 
	G4double NumPhotons = Rfact * charge/eplus * charge/eplus *
                                 (dp - ge * BetaInverse*BetaInverse);

	NumPhotons /= HistoManager::GetPointer()->GetPhotonBias();

	return NumPhotons;		
}
