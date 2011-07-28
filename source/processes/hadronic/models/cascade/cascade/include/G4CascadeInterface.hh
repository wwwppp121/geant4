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
// $Id: G4CascadeInterface.hh,v 1.23 2010-09-23 18:13:32 mkelsey Exp $
// Defines an interface to Bertini (BERT) cascade
// based on INUCL  intra-nuclear transport.models 
// with bullet hadron energy ~< 10 GeV
//
// 20100405  M. Kelsey -- Fix constness of op== and op!=
// 20100519  M. Kelsey -- Remove Collider data members
// 20100617  M. Kelsey -- Make G4InuclCollider a local data member
// 20100723  M. Kelsey -- Move G4CollisionOutput here for reuse
// 20100916  M. Kelsey -- Add functions to encapsulate ApplyYourself() actions,
//		make colliders pointers (don't expose dependencies)
// 20100922  M. Kelsey -- Add functions to select de-excitation method
// 20110224  M. Kelsey -- Add createTarget() for use with Propagate(); split
//		conservation law messages to separate function.  Move verbose
//		setting to .cc file, and apply to all member objects.
// 20110301  M. Kelsey -- Add copyPreviousCascade() for use with Propagate()  
//		along with new buffers and related particle-conversion  
//		functions.  Encapulate buffer deletion in clear()
// 20110303  M. Kelsey -- Change "bulletList" name to "inputFragments"
// 20110304  M. Kelsey -- Drop conversion of Propagate() arguments; pass
//		directly to collider for processing.  Rename makeReactionProduct
//		to makeDynamicParticle.
// 20110502  M. Kelsey -- Add filename string to capture random seeds.
// 20110720  M. Kelsey -- Discard elastic-cut array (no longer needed),
//		discard local "theFinalState" (avail in base class).

#ifndef G4CASCADEINTERFACE_H
#define G4CASCADEINTERFACE_H 1

#include "G4VIntraNuclearTransportModel.hh"
#include "G4FragmentVector.hh"
#include "G4LorentzRotation.hh"
#include "G4Nucleon.hh"
#include "G4Nucleus.hh"
#include "G4ParticleChange.hh"
#include "G4ReactionProduct.hh"
#include "G4ReactionProductVector.hh"
#include <vector>

class G4CascadParticle;
class G4CascadeCheckBalance;
class G4CollisionOutput;
class G4DynamicParticle;
class G4HadFinalState;
class G4InuclCollider;
class G4InuclElementaryParticle;
class G4InuclNuclei;
class G4InuclParticle;
class G4V3DNucleus;


class G4CascadeInterface : public G4VIntraNuclearTransportModel {

public:
  G4CascadeInterface(const G4String& name = "Bertini Cascade");

  virtual ~G4CascadeInterface();

  G4ReactionProductVector* Propagate(G4KineticTrackVector* theSecondaries,
				     G4V3DNucleus* theNucleus);
  
  G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack,
				 G4Nucleus& theNucleus);

  void setVerboseLevel(G4int verbose);

  // Select betweeen different post-cascade de-excitation models
  void useCascadeDeexcitation();
  void usePreCompoundDeexcitation();

protected:
  void clear();			// Delete previously created particles

  // Convert input projectile and target to Bertini internal types
  void createBullet(const G4HadProjectile& aTrack);
  void createTarget(G4Nucleus& theNucleus);
  void createTarget(G4V3DNucleus* theNucleus);
  void createTarget(G4int A, G4int Z);

  // Evaluate whether any outgoing particles penetrated Coulomb barrier
  G4bool coulombBarrierViolation() const;

  // Conditions for rejecting cascade attempt
  G4bool retryInelasticProton() const;
  G4bool retryInelasticNucleus() const;

  // Transfer Bertini internal final state to hadronics interface
  void copyOutputToHadronicResult();
  G4ReactionProductVector* copyOutputToReactionProducts();

  // Replicate input particles onto output
  G4HadFinalState* NoInteraction(const G4HadProjectile& aTrack,
				 G4Nucleus& theNucleus);

  // Report violations of conservation laws in original frame
  void checkFinalResult();

  // Terminate job because of energy/momentum/etc. violations
  void throwNonConservationFailure();

  // Convert between Bertini and external particle types
  G4DynamicParticle* makeDynamicParticle(const G4InuclElementaryParticle& iep) const;
  G4DynamicParticle* makeDynamicParticle(const G4InuclNuclei& inuc) const;


private:
  G4int operator==(const G4CascadeInterface& right) const {
    return (this == &right);
  }

  G4int operator!=(const G4CascadeInterface& right) const {
    return (this != &right);
  }

  static const G4String randomFile;	// Filename to capture random seeds

  static const G4int maximumTries;	// Number of iterations for inelastic

  G4int verboseLevel;
  G4int numberOfTries;

  G4InuclCollider* collider;
  G4CascadeCheckBalance* balance;

  G4InuclParticle* bullet;
  G4InuclParticle* target;
  G4CollisionOutput* output;

  G4LorentzRotation bulletInLabFrame;
};

#endif // G4CASCADEINTERFACE_H
