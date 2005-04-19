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
// $Id: StepMax.hh,v 1.1 2005-04-19 15:01:39 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
/////////////////////////////////////////////////////////////////////////////////.

#ifndef StepMax_h
#define StepMax_h 1

#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"

class Histo;
// class G4VPhysicalVolume;
// class StepMaxMessenger;

////////////////////////////////////////////////////////////////////////////////
//
//

class StepMax : public G4VDiscreteProcess
{
  public:

     StepMax(const G4String& processName = "UserMaxStep");
    ~StepMax();

     G4bool IsApplicable(const G4ParticleDefinition&);

     void SetMaxStep(G4double);

     G4double GetMaxStep() {return MaxChargedStep;};

     G4double PostStepGetPhysicalInteractionLength( const G4Track& track,
			                       G4double previousStepSize,
			                       G4ForceCondition* condition);

     G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

     G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition*)
     {return 0.;};    // it is not needed here !

  private:

     G4double MaxChargedStep;
     G4double ProposedStep;
     G4double thDensity;

  //   StepMaxMessenger*  pMess;
  //  Histo*             histo;
     G4VPhysicalVolume* checkVolume;
     G4VPhysicalVolume* gasVolume;
     G4bool             first;
};

////////////////////////////////////////////////////////////////////////////////////

#endif

