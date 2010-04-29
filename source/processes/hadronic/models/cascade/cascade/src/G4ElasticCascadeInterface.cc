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
// $Id: G4ElasticCascadeInterface.cc,v 1.11 2010-04-29 19:39:55 mkelsey Exp $
// Geant4 tag: $Name: not supported by cvs2svn $
//
// Pekka Kaitaniemi, HIP
// Aatos Heikkinen
//
// 20100114  M. Kelsey -- Remove G4CascadeMomentum, use G4LorentzVector directly
// 20100413  M. Kelsey -- Pass G4CollisionOutput by ref to ::collide()
// 20100428  M. Kelsey -- Use G4InuclParticleNames enum
// 20100429  M. Kelsey -- Change "case gamma:" to "case photon:"

#include "G4ElasticCascadeInterface.hh"
#include "globals.hh"
#include "G4DynamicParticleVector.hh"
#include "G4IonTable.hh"
#include "G4InuclCollider.hh"
#include "G4IntraNucleiCascader.hh"
#include "G4ElementaryParticleCollider.hh"
#include "G4NonEquilibriumEvaporator.hh"
#include "G4EquilibriumEvaporator.hh"
#include "G4Fissioner.hh"
#include "G4BigBanger.hh"
#include "G4InuclElementaryParticle.hh"
#include "G4InuclNuclei.hh"
#include "G4InuclParticle.hh"
#include "G4CollisionOutput.hh"
#include "G4V3DNucleus.hh"
#include "G4Track.hh"
#include "G4Nucleus.hh"
#include "G4NucleiModel.hh"
#include "G4LorentzRotation.hh"
#include "G4InuclParticleNames.hh"

using namespace G4InuclParticleNames;


typedef std::vector<G4InuclElementaryParticle>::iterator particleIterator;
typedef std::vector<G4InuclNuclei>::iterator nucleiIterator;

G4ElasticCascadeInterface::G4ElasticCascadeInterface()
  :verboseLevel(0)
{
  if (verboseLevel > 3) {
    G4cout << " >>> G4ElasticCascadeInterface::G4ElasticCascadeInterface" << G4endl;
  }
}
   
G4ReactionProductVector* G4ElasticCascadeInterface::Propagate(G4KineticTrackVector*, G4V3DNucleus* ) {
  return NULL;
}

// #define debug_G4ElasticCascadeInterface

G4HadFinalState* G4ElasticCascadeInterface::ApplyYourself(const G4HadProjectile& aTrack, 
							  G4Nucleus& theNucleus) {
#ifdef DEBUG_ELASTIC
  static G4int counter(0);
  counter++;
  G4cerr << "Reaction number "<< counter << " "<<aTrack.GetDynamicParticle()->GetDefinition()->GetParticleName()<<" "<< aTrack.GetDynamicParticle()->GetKineticEnergy()<<G4endl;
#endif

  theResult.Clear();

  if (verboseLevel > 3) {
    G4cout << " >>> G4ElasticCascadeInterface::ApplyYourself" << G4endl;
  };

  G4double eInit     = 0.0;
  G4double eTot      = 0.0;
  G4double sumBaryon = 0.0;
  G4double sumEnergy = 0.0;

  // Make conversion between native Geant4 and Bertini cascade classes.
  // NOTE: Geant4 units are MeV = 1 and GeV = 1000. Cascade code by default use GeV = 1.

  G4int bulletType =  G4InuclElementaryParticle::type(aTrack.GetDefinition());

  // Code momentum and energy.
  G4LorentzVector projectileMomentum = aTrack.Get4Momentum();
  G4LorentzRotation toZ;
  toZ.rotateZ(-projectileMomentum.phi());
  toZ.rotateY(-projectileMomentum.theta());
  G4LorentzRotation toLabFrame = toZ.inverse();

  G4LorentzVector momentumBullet(G4ThreeVector(0.,0.,aTrack.GetTotalMomentum()/GeV), projectileMomentum.m());

  G4InuclElementaryParticle *bullet = new G4InuclElementaryParticle(momentumBullet, bulletType); 

  sumEnergy = bullet->getKineticEnergy(); // In GeV 
  sumBaryon += bullet->baryon();

  // Set target
  G4InuclNuclei*   target  = NULL;
  G4InuclParticle* targetH = NULL;
  // and outcoming particles
  G4DynamicParticle* cascadeParticle = NULL;

  G4double theNucleusA = theNucleus.GetN();

  if ( !(G4int(theNucleusA) == 1) ) {
    target  = new G4InuclNuclei(theNucleusA, 
				theNucleus.GetZ());

    eInit = bullet->getEnergy() + target->getEnergy();

    sumBaryon += theNucleusA;

    if (verboseLevel > 2) {
      G4cout << "Bullet:  " << G4endl;  
      bullet->printParticle();
    }
    if (verboseLevel > 2) {
      G4cout << "Target:  " << G4endl;  
      target->printParticle();
    }
  }

  G4CollisionOutput output;

  // Colliders initialisation
  G4ElementaryParticleCollider*   colep = new G4ElementaryParticleCollider;
  G4IntraNucleiCascader*            inc = new G4IntraNucleiCascader; // the actual cascade
  inc->setInteractionCase(1); // Interaction type is particle with nuclei.

  G4NonEquilibriumEvaporator*     noneq = new G4NonEquilibriumEvaporator;
  G4EquilibriumEvaporator*         eqil = new G4EquilibriumEvaporator;
  G4Fissioner*                     fiss = new G4Fissioner;
  G4BigBanger*                     bigb = new G4BigBanger;
  G4InuclCollider*             collider = new G4InuclCollider(colep, inc, noneq, eqil, fiss, bigb);
  
  G4int  maxTries = 10; // maximum tries for elastic collision to avoid infinite loop
  G4int  nTries   = 0;  // try counter

  if (G4int(theNucleusA) == 1) { // special treatment for target H(1,1) (proton)

    targetH = new G4InuclElementaryParticle(1);

    G4float cutElastic[8];
    cutElastic[proton   ] = 1.0; // GeV
    cutElastic[neutron  ] = 1.0;
    cutElastic[pionPlus ] = 0.6;
    cutElastic[pionMinus] = 0.2;
    cutElastic[pionZero ] = 0.2;

    //	Was >
    if (momentumBullet.z() < cutElastic[bulletType]) { // elastic collision possible

      do {   // we try to create elastic collision
	output.reset();
	collider->collide(bullet, targetH, output);
	nTries++;
      } while(
	      (nTries < maxTries)                                           &&
	      (output.getOutgoingParticles().size() != 2                    && 
	       (output.getOutgoingParticles().begin()->type() != bulletType ||
		output.getOutgoingParticles().begin()->type() != proton)
	       )
	      );
      // If there was no elastic collision we just return the original
      // bullet and target proton. The returned bullet particle has
      // the momentum of the original bullet particle.
      if(output.getOutgoingParticles().size() != 2 && 
	 (output.getOutgoingParticles().begin()->type() != bulletType || 
	  output.getOutgoingParticles().begin()->type() != proton)) {
	output = G4CollisionOutput();
	output.addOutgoingParticle(G4InuclElementaryParticle(momentumBullet, bulletType));
	output.addOutgoingParticle(G4InuclElementaryParticle(proton));
      }

    } else {
      // If elastic collision can not happen above then we just output
      //the original bullet particle and the target (in this case the
      //target is proton) output = collider->collide(bullet, targetH);
      output.reset();
      output.addOutgoingParticle(G4InuclElementaryParticle(momentumBullet, bulletType));
      output.addOutgoingParticle(G4InuclElementaryParticle(proton));
    }

    sumBaryon += 1;

    eInit = bullet->getEnergy() + targetH->getEnergy();

    if (verboseLevel > 2) {
      G4cout << "Target:  " << G4endl;
      targetH->printParticle();
    }

  } else {  // treat all other targets excepet H(1,1)

    do  // we try to create elastic collision
      {
	//We try to create one particle (the bullet) and a nuclear
	//fragment that has the same A and Z as the original target.
	output.reset();
	collider->collide(bullet, target, output);
	nTries++;
      } while(
	      (nTries < maxTries)        &&
	      (output.getOutgoingParticles().size() != 1 ||
	       output.getOutgoingParticles().begin()->type() != bullet->type() ||
	       output.getNucleiFragments().size() != 1 ||
	       output.getNucleiFragments().begin()->getA() != target->getA() || 
	       output.getNucleiFragments().begin()->getZ() != target->getZ()) 
	      );

      // If there was no elastic collision we just return the original
      // bullet and target nucleus. The returned bullet particle has
      // the momentum of the original bullet particle.
    if((output.getOutgoingParticles().size() != 1 ||
	output.getOutgoingParticles().begin()->type() != bullet->type() ||
	output.getNucleiFragments().size() != 1 ||
	output.getNucleiFragments().begin()->getA() != target->getA() || 
	output.getNucleiFragments().begin()->getZ() != target->getZ())) { 
	
      output.reset();
      output.addOutgoingParticle(G4InuclElementaryParticle(momentumBullet, bulletType));
      output.addTargetFragment(G4InuclNuclei(target->getA(), target->getZ()));
    }
  }

  if (verboseLevel > 1) 
    {
      G4cout << " Cascade output: " << G4endl;
      output.printCollisionOutput();
    }
  
  // Convert cascade data to use hadronics interface
  std::vector<G4InuclNuclei> nucleiFragments = output.getNucleiFragments();
  std::vector<G4InuclElementaryParticle> particles = output.getOutgoingParticles();

  theResult.SetStatusChange(stopAndKill);

  if (!particles.empty()) { 
    particleIterator ipart;
    G4int outgoingParticle;

    for (ipart = particles.begin(); ipart != particles.end(); ipart++) {
      outgoingParticle = ipart->type();
      eTot += ipart->getEnergy();
      G4double ekin = ipart->getKineticEnergy() * GeV;
      G4ThreeVector aMom(ipart->getMomentum().vect().unit());

      sumBaryon -= ipart->baryon();

      sumEnergy -= ekin / GeV;

      switch(outgoingParticle) {

      case proton: 
#ifdef DEBUG_ELASTIC
	G4cerr << "proton " << counter << " " << aMom << " " << ekin << G4endl;
#endif
	cascadeParticle = 
	  new G4DynamicParticle(G4Proton::ProtonDefinition(), aMom, ekin);
	break; 

      case neutron: 

#ifdef DEBUG_ELASTIC
	G4cerr << "neutron "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	cascadeParticle = 
	  new G4DynamicParticle(G4Neutron::NeutronDefinition(), aMom, ekin);
	break;

      case pionPlus: 
	cascadeParticle = 
	  new G4DynamicParticle(G4PionPlus::PionPlusDefinition(), aMom, ekin);

#ifdef DEBUG_ELASTIC
	G4cerr << "pionPlus "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case pionMinus:
	cascadeParticle = 
	  new G4DynamicParticle(G4PionMinus::PionMinusDefinition(), aMom, ekin);

#ifdef DEBUG_ELASTIC
	G4cerr << "pionMinus "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case pionZero: 
	cascadeParticle = 
	  new G4DynamicParticle(G4PionZero::PionZeroDefinition(), aMom, ekin);

#ifdef DEBUG_ELASTIC
	G4cerr << "pionZero "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      case photon:
	cascadeParticle = 
	  new G4DynamicParticle(G4Gamma::Gamma(), aMom, ekin);

#ifdef DEBUG_ELASTIC
	G4cerr << "photon "<< counter<<" "<<aMom<<" "<<  ekin<<G4endl;
#endif
	break;

      default:
        G4cout << " ERROR: G4ElasticCascadeInterface::Propagate undefined particle type"
	       << G4endl;
      }

      cascadeParticle->Set4Momentum(cascadeParticle->Get4Momentum()*=toLabFrame);
      theResult.AddSecondary(cascadeParticle); 
    }
  }

  // get nuclei fragments
  G4DynamicParticle * aFragment = NULL;
  G4ParticleDefinition * aIonDef = 0;
  G4ParticleTable *theTableOfParticles = G4ParticleTable::GetParticleTable();

  if (!nucleiFragments.empty()) { 
    nucleiIterator ifrag;

    for (ifrag = nucleiFragments.begin(); ifrag != nucleiFragments.end(); ifrag++) 
      {
	G4double eKin = ifrag->getKineticEnergy() * GeV;
        eTot += ifrag->getEnergy();

	G4ThreeVector aMom(ifrag->getMomentum().vect().unit());

	if (verboseLevel > 2) {
	  G4cout << " Nuclei fragment: " << G4endl;
	  ifrag->printParticle();
	}

	G4int A = G4int(ifrag->getA());
	G4int Z = G4int(ifrag->getZ());
	aIonDef = theTableOfParticles->FindIon(Z, A, 0, Z);
      
	aFragment =  new G4DynamicParticle(aIonDef, aMom, eKin);

	sumBaryon -= A;
	sumEnergy -= eKin / GeV;

        aFragment->Set4Momentum(aFragment->Get4Momentum()*=toLabFrame);
	theResult.AddSecondary(aFragment); 
      }
  }

  if (verboseLevel > 2) {
    if (sumBaryon != 0) {
      G4cout << "ERROR: no baryon number conservation, sum of baryons = "
             << sumBaryon << G4endl;
    }

    if (sumEnergy > 0.01 ) {
      G4cout << "Kinetic energy conservation violated by "
	     << sumEnergy << " GeV" << G4endl;
    }
     
    G4cout << "Total energy conservation at level ~"
	   << (eInit - eTot) * GeV << " MeV" << G4endl;
    
    if (sumEnergy < -5.0e-5 ) { // 0.05 MeV
      G4cout << "FATAL ERROR: energy created  "
             << sumEnergy * GeV << " MeV" << G4endl;
    }
  }

  delete bullet;
  delete colep;
  delete inc;
  delete noneq; 
  delete fiss;
  delete eqil;
  delete bigb;
  delete collider;

  if(target != NULL) delete target;
  if(targetH != NULL) delete targetH;
  // if(cascadeParticle != NULL) delete cascadeParticle;
  // if(aFragment != NULL) delete aFragment;

  return &theResult;
}
