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
// INCL++ intra-nuclear cascade model
// Pekka Kaitaniemi, CEA and Helsinki Institute of Physics
// Davide Mancusi, CEA
// Alain Boudard, CEA
// Sylvie Leray, CEA
// Joseph Cugnon, University of Liege
//
// INCL++ revision: v5.0.3
//
#define INCLXX_IN_GEANT4_MODE 1

#include "globals.hh"

#include "G4INCLCascade.hh"
#include "G4INCLRandom.hh"
#include "G4INCLRanecu.hh"
#include "G4INCLGeant4Random.hh"
#include "G4INCLStandardPropagationModel.hh"
#include "G4INCLParticleTable.hh"
#include "G4INCLGlobalInfo.hh"

#include "G4INCLPauliBlocking.hh"
#include "G4INCLIPauli.hh"
#include "G4INCLPauliStrict.hh"
#include "G4INCLPauliStandard.hh"
#include "G4INCLPauliStrictStandard.hh"
#include "G4INCLPauliGlobal.hh"
#include "G4INCLCDPP.hh"

#include "G4INCLLogger.hh"
#include "G4INCLGlobals.hh"
#include "G4INCLNuclearDensityFactory.hh"

#include "G4INCLCoulombDistortion.hh"
#include "G4INCLICoulomb.hh"
#include "G4INCLCoulombNone.hh"
#include "G4INCLCoulombNonRelativistic.hh"

#include "G4INCLClustering.hh"
#include "G4INCLClusteringModelIntercomparison.hh"
#include "G4INCLClusteringModelNone.hh"

#include "G4INCLRootFinder.hh"

#include <cstring>
#include <cstdlib>

namespace G4INCL {

  INCL::INCL(G4INCL::Config const * const config)
    :propagationModel(0), theA(208), theZ(82), maxImpactParameter(0.),
    theConfig(config)
  {
    // Set the logger object.
    G4INCL::Logger::setLoggerSlave(new G4INCL::LoggerSlave(theConfig->getLogFileName()));
    G4INCL::Logger::setVerbosityLevel(theConfig->getVerbosity());

    // Set the random number generator algorithm. The system can support
    // multiple different generator algorithms in a completely
    // transparent way.
#ifdef INCLXX_IN_GEANT4_MODE
    G4INCL::Random::setGenerator(new G4INCL::Geant4RandomGenerator());
#else
    G4INCL::Random::setGenerator(new G4INCL::Ranecu(theConfig->getRandomSeeds()));
#endif // INCLXX_IN_GEANT4_MODE

    // Select the Pauli blocking algorithm:
    G4INCL::PauliType pauli = theConfig->getPauliType();
    if(pauli == G4INCL::StrictStatisticalPauli)
      G4INCL::Pauli::setBlocker(new G4INCL::PauliStrictStandard);
    else if(pauli == G4INCL::StatisticalPauli)
      G4INCL::Pauli::setBlocker(new G4INCL::PauliStandard);
    else if(pauli == G4INCL::StrictPauli)
      G4INCL::Pauli::setBlocker(new G4INCL::PauliStrict);
    else if(pauli == G4INCL::GlobalPauli)
      G4INCL::Pauli::setBlocker(new G4INCL::PauliGlobal);
    else if(pauli == G4INCL::NoPauli)
      G4INCL::Pauli::setBlocker(NULL);

    if(theConfig->getCDPP())
      G4INCL::Pauli::setCDPP(new G4INCL::CDPP);
    else
      G4INCL::Pauli::setCDPP(NULL);

    // Select the Coulomb-distortion algorithm:
    G4INCL::CoulombType coulombType = theConfig->getCoulombType();
    if(coulombType == G4INCL::NonRelativisticCoulomb)
      G4INCL::CoulombDistortion::setCoulomb(new G4INCL::CoulombNonRelativistic);
    else // if(coulombType == G4INCL::NoCoulomb)
      G4INCL::CoulombDistortion::setCoulomb(new G4INCL::CoulombNone);

    // Select the clustering algorithm:
    G4INCL::ClusterAlgorithmType clusterAlgorithm = theConfig->getClusterAlgorithm();
    if(clusterAlgorithm == G4INCL::IntercomparisonClusterAlgorithm) {
      G4INCL::Clustering::setClusteringModel(new G4INCL::ClusteringModelIntercomparison);
      // Set the maximum mass for the clustering algorithm
      G4INCL::IClusteringModel::maxClusterAlgorithmMass = theConfig->getClusterMaxMass();
    }
    else // if(clusterAlgorithm == G4INCL::NoClusterAlgorithm)
      G4INCL::Clustering::setClusteringModel(new G4INCL::ClusteringModelNone);

    // Initialize the INCL particle table:
    G4INCL::ParticleTable::initialize();

    // Propagation model is responsible for finding avatars and
    // transporting the particles. In principle this step is "hidden"
    // behind an abstract interface and the rest of the system does not
    // care how the transportation and avatar finding is done. This
    // should allow us to "easily" experiment with different avatar
    // finding schemes and even to support things like curved
    // trajectories in the future.
    propagationModel = new G4INCL::StandardPropagationModel(theConfig->getLocalEnergyBBType(),theConfig->getLocalEnergyPiType());
    eventAction = new EventAction();
    propagationAction = new PropagationAction();
    avatarAction = new AvatarAction();

    std::strcpy(theGlobalInfo.cascadeModel, theConfig->getVersionID().c_str());
    std::strcpy(theGlobalInfo.deexcitationModel, "none");

    // Set the target
    if(!theConfig->isNaturalTarget()) {
      setTarget(theConfig->getTargetA(), theConfig->getTargetZ());
      // Fill in the global information
      theGlobalInfo.At = theConfig->getTargetA();
      theGlobalInfo.Zt = theConfig->getTargetZ();
    } else {
      // TODO: support for natural targets
      FATAL("Fatal: natural targets are not supported yet." << std::endl);
      std::exit(EXIT_FAILURE);
    }

#ifndef INCLXX_IN_GEANT4_MODE
    // Echo the input parameters to the log file
    INFO(theConfig->echo() << std::endl);
#endif
  }

  INCL::INCL(IPropagationModel *aPropagationModel)
    :propagationModel(aPropagationModel), theA(208), theZ(82), maxImpactParameter(0.), theConfig(NULL)
  {
    // Set the random number generator algorithm. The system can support
    // multiple different generator algorithms in a completely
    // transparent way.
    G4INCL::Random::setGenerator(new G4INCL::Ranecu());
  }

  INCL::~INCL() {
    G4INCL::Pauli::deleteBlockers();
    G4INCL::CoulombDistortion::deleteCoulomb();
    G4INCL::Random::deleteGenerator();
    G4INCL::ParticleTable::deletePDS();
    G4INCL::Clustering::deleteClusteringModel();
    G4INCL::Logger::deleteLoggerSlave();
    G4INCL::NuclearDensityFactory::clearCache();
    delete avatarAction;
    delete propagationAction;
    delete eventAction;
    delete propagationModel;
    delete theConfig;
  }

  void INCL::setTarget(G4int A, G4int Z) {
    if(A > 0 && A < 300 && Z > 0 && Z < 200) {
      theA = A;
      theZ = Z;
    } else {
      ERROR("Unsupported target: A = " << A << " Z = " << Z << std::endl);
      ERROR("Target configuration rejected." << std::endl);
    }

    // Set the maximum impact parameter
    // TODO: for natural target abundances, make this the largest impact
    // parameter for all the isotopes.
    Nucleus *aNucleus = new Nucleus(A, Z, theConfig);
    G4double kineticEnergy = theConfig->getProjectileKineticEnergy();
    G4INCL::ParticleType projectileType = theConfig->getProjectileType();
    G4double projectileMass = ParticleTable::getMass(projectileType);
    G4double energy = kineticEnergy + projectileMass;
    G4double momentumZ = std::sqrt(energy*energy - projectileMass*projectileMass);
    G4INCL::ThreeVector position(0.0, 0.0, 0.0); // Irrelevant at this point...
    G4INCL::ThreeVector momentum(0.0, 0.0, momentumZ);
    G4INCL::Particle *projectile = new G4INCL::Particle(projectileType, energy,
							momentum, position);
    maxImpactParameter = CoulombDistortion::maxImpactParameter(projectile, aNucleus);

    delete projectile;
    delete aNucleus;

    // Set the geometric cross section
    theGlobalInfo.geometricCrossSection =
      Math::tenPi*std::pow(maxImpactParameter,2);

  }

  G4bool INCL::initializeTarget(G4int A, G4int Z) {
    Nucleus *previousNucleus = propagationModel->getNucleus();
    delete previousNucleus;

    Nucleus *aNucleus = new Nucleus(A, Z, theConfig);
    aNucleus->getStore()->getBook()->reset();
    aNucleus->initializeParticles();

    propagationModel->setNucleus(aNucleus);
    return true;
  }

  const EventInfo &INCL::processEvent(Particle *projectile) {
    initializeTarget(theA, theZ);
    // Usage of the projectile API:

    // Test projectile:
    // G4INCL::ThreeVector position(0.0, 0.0, 0.0);
    // G4INCL::ThreeVector momentum(0.0, 0.0, 2000.0);
    // G4double energy = std::sqrt(momentum.mag2() + G4INCL::ProtonMass * G4INCL::ProtonMass);
    // G4INCL::Particle *projectile = new G4INCL::Particle(G4INCL::Proton, energy,
    // 							momentum, position);

    // composite
    // G4INCL::Nucleus *projectileNucleus = new G4INCL::Nucleus(6, 12);
    //projectileNucleus->initializeParticles();

    // Create a nucleus of Z = 82 and A = 208
    //    G4INCL::Nucleus *theNucleus = new G4INCL::Nucleus(6, 12);

    // Generate the initial distribution of particles
    //    theNucleus->initializeParticles();

    //  theNucleus->shootMe(projectile);
    //  theNucleus->shootMe(projectileNucleus);

    // Manually set the stopping time of the simulation.
    //    propagationModel->setStoppingTime(70.0);

    // Assign the nucleus to the propagation model
    //    propagationModel->setNucleus(theNucleus);

    // Shortcut pointer
    Nucleus *nucleus = propagationModel->getNucleus();

    // Reset theEventInfo
    theEventInfo.reset();

    EventInfo::eventNumber++;

    // Increment the global counter for the number of shots
    theGlobalInfo.nShots++;

    // Fill in the global information
    // TODO: should be moved to the input processing
    theGlobalInfo.Ap = projectile->getA();
    theGlobalInfo.Zp = projectile->getZ();
    theGlobalInfo.Ep = projectile->getKineticEnergy();

    // Fill in the event information
    theEventInfo.projectileType = projectile->getType();
    theEventInfo.Ap = projectile->getA();
    theEventInfo.Zp = projectile->getZ();
    theEventInfo.Ep = projectile->getKineticEnergy();
    theEventInfo.At = nucleus->getA();
    theEventInfo.Zt = nucleus->getZ();

    // Do nothing below the Coulomb barrier
    if(maxImpactParameter<=0.) {
      // Increment the global counter for the number of transparents
      theGlobalInfo.nTransparents++;

      // Fill in the event information
      theEventInfo.transparent = true;

      // Delete the projectile!
      delete projectile;

      return theEventInfo;
    }

    // Randomly draw an impact parameter
    G4double impactParameter = maxImpactParameter * std::sqrt(Random::shoot());
    // Fill in the event information
    theEventInfo.impactParameter = impactParameter;

    G4bool projectileHitsTarget = propagationModel->shootProjectile(projectile, impactParameter);
    if(projectileHitsTarget == false) {
      // Increment the global counter for the number of transparents
      theGlobalInfo.nTransparents++;

      // Fill in the event information
      theEventInfo.transparent = true;

      // Delete the projectile!
      delete projectile;

      return theEventInfo;
    }

    // Fill in the event information
    const G4double effectiveImpactParameter =
      projectile->getTransversePosition().mag();
    theEventInfo.effectiveImpactParameter = effectiveImpactParameter;

    do {
      // Run book keeping actions that should take place before propagation:
      propagationAction->beforePropagationAction(propagationModel);

      // Get the avatar with the smallest time and propagate particles
      // to that point in time.
      G4INCL::IAvatar *avatar = propagationModel->propagate();

      // Run book keeping actions that should take place after propagation:
      propagationAction->afterPropagationAction(propagationModel, avatar);

      if(avatar == 0) break; // No more avatars in the avatar list.

      // Run book keeping actions that should take place before avatar:
      avatarAction->beforeAvatarAction(avatar, nucleus);

      // Channel is responsible for calculating the outcome of the
      // selected avatar. There are different kinds of channels. The
      // class IChannel is, again, an abstract interface that defines
      // the externally observable behavior of all interaction
      // channels.
      // The handling of the channel is transparent to the API.
      // Final state tells what changed...
      G4INCL::FinalState *finalState = avatar->getFinalState();

      // Run book keeping actions that should take place after avatar:
      avatarAction->afterAvatarAction(avatar, nucleus, finalState);

      // So now we must give this information to the nucleus
      nucleus->applyFinalState(finalState);
      // and now we are ready to process the next avatar!

      delete avatar;
      delete finalState;
    } while(continueCascade());

    // Fill in the event information
    theEventInfo.transparent = nucleus->isEventTransparent();

    if(theEventInfo.transparent) {
      // Increment the global counter for the number of transparents
      theGlobalInfo.nTransparents++;
    } else {
      // Check if the nucleus contains deltas
      theEventInfo.deltasInside = nucleus->containsDeltas();

      // Take care of any remaining deltas
      theEventInfo.forcedDeltasOutside = nucleus->decayOutgoingDeltas();
      theEventInfo.forcedDeltasInside = nucleus->decayInsideDeltas();

      // Cluster decay
      theEventInfo.clusterDecay = nucleus->decayOutgoingClusters();

      // Apply Coulomb distortion, if appropriate
      // Note that this will apply Coulomb distortion also on pions emitted by
      // unphysical remnants (see decayInsideDeltas). This is at variance with
      // what INCL4.6 does, but these events are (should be!) so rare that
      // whatever we do doesn't (shouldn't!) make any noticeable difference.
      G4INCL::CoulombDistortion::distortOut(nucleus->getStore()->getOutgoingParticles(), nucleus);

      // Compute recoil momentum, energy and spin of the nucleus
      nucleus->computeRecoilKinematics();

      // Make room for the remnant recoil by rescaling the energies of the
      // outgoing particles.
      if(nucleus->hasRemnant()) rescaleOutgoingForRecoil();

      // Global checks of conservation laws
      globalConservationChecks();

      // Fill the EventInfo structure
      nucleus->fillEventInfo(&theEventInfo);
      //      theEventInfo.fillFromNucleus(nucleus);
      theEventInfo.stoppingTime = propagationModel->getCurrentTime();

      // Check if we have an absorption:
      if(theEventInfo.nucleonAbsorption) theGlobalInfo.nNucleonAbsorptions++;
      if(theEventInfo.pionAbsorption) theGlobalInfo.nPionAbsorptions++;
    }

    return theEventInfo;
  }

  void INCL::rescaleOutgoingForRecoil() {
    class RecoilFunctor : public RootFunctor {
      public:
        /** \brief Prepare for calling the () operator and scaleParticleEnergies
         *
         * The constructor sets the private class members.
         */
        RecoilFunctor(Nucleus * const n, const EventInfo &ei) :
          nucleus(n),
          outgoingParticles(n->getStore()->getOutgoingParticles()),
          theEventInfo(ei) {
            for(ParticleIter p=outgoingParticles.begin(); p!=outgoingParticles.end(); ++p) {
              particleMomenta.push_back((*p)->getMomentum());
              particleKineticEnergies.push_back((*p)->getKineticEnergy());
            }
          }
        virtual ~RecoilFunctor() {}

        /** \brief Compute the energy-conservation violation.
         *
         * \param x scale factor for the particle energies
         * \return the energy-conservation violation
         */
        G4double operator()(const G4double x) const {
          scaleParticleEnergies(x);
          return nucleus->getConservationBalance(theEventInfo).energy;
        }

        /// \brief Clean up after root finding
        void cleanUp(const G4bool success) const {
          if(!success)
            scaleParticleEnergies(1.);
        }

      private:
        /// \brief Pointer to the nucleus
        Nucleus *nucleus;
        /// \brief List of final-state particles.
        ParticleList const &outgoingParticles;
        // \brief Reference to the EventInfo object
        EventInfo const &theEventInfo;
        /// \brief Initial momenta of the outgoing particles
        std::list<ThreeVector> particleMomenta;
        /// \brief Initial kinetic energies of the outgoing particles
        std::list<G4double> particleKineticEnergies;

        /** \brief Scale the kinetic energies of the outgoing particles.
         *
         * \param alpha scale factor
         */
        void scaleParticleEnergies(const G4double rescale) const {
          // Rescale the energies (and the momenta) of the outgoing particles.
          ThreeVector pBalance = nucleus->getIncomingMomentum();
          std::list<ThreeVector>::const_iterator iP = particleMomenta.begin();
          std::list<G4double>::const_iterator iE = particleKineticEnergies.begin();
          for( ParticleIter i = outgoingParticles.begin(); i != outgoingParticles.end(); ++i, ++iP, ++iE)
          {
            const G4double mass = (*i)->getMass();
            const G4double newKineticEnergy = (*iE) * rescale;

            (*i)->setMomentum(*iP);
            (*i)->setEnergy(mass + newKineticEnergy);
            (*i)->adjustMomentumFromEnergy();

            pBalance -= (*i)->getMomentum();
          }

          nucleus->setRecoilMomentum(pBalance);
          const G4double remnantMass = ParticleTable::getMass(nucleus->getA(),nucleus->getZ()) + nucleus->getExcitationEnergy();
          const G4double pRem2 = pBalance.mag2();
          const G4double recoilEnergy = pRem2/
            (std::sqrt(pRem2+remnantMass*remnantMass) + remnantMass);
          nucleus->setRecoilEnergy(recoilEnergy);
        }
    } theRecoilFunctor(propagationModel->getNucleus(), theEventInfo);

    // Apply the root-finding algorithm
    const G4bool success = RootFinder::solve(&theRecoilFunctor, 1.0);
    if(success) {
      std::pair<G4double,G4double> theSolution = RootFinder::getSolution();
      theRecoilFunctor(theSolution.first); // Apply the solution
    } else {
      WARN("Couldn't accommodate remnant recoil while satisfying energy conservation, root-finding algorithm failed." << std::endl);
    }

  }

  void INCL::globalConservationChecks() {
    const Nucleus *nucleus = propagationModel->getNucleus();

    /* FIXME: This version of the energy-conservation check only uses kinetic
       energies, to mimic what INCL4.5 does. This is unsatisfactory because it
       does not take into account the particle masses. At some point, it would
       be nice to have real energy conservation, with real masses. When ready
       to do so, have a look at the status of the code at commit
       aad75d09b8a52d28b8eb1bd38bdf347e63b802db (or possibly simply revert the
       following commit). */
    Nucleus::ConservationBalance theBalance = nucleus->getConservationBalance(theEventInfo);

    // Global conservation checks
    const G4double pLongBalance = theBalance.momentum.getZ();
    const G4double pTransBalance = theBalance.momentum.perp();
    if(theBalance.Z != 0) {
      ERROR("Violation of charge conservation! ZBalance = " << theBalance.Z << std::endl);
    }
    if(theBalance.A != 0) {
      ERROR("Violation of baryon-number conservation! ABalance = " << theBalance.A << std::endl);
    }
    if(std::abs(theBalance.energy)>10.0) {
      WARN("Violation of energy conservation > 10 MeV. EBalance = " << theBalance.energy << std::endl);
    }
    if(std::abs(pLongBalance)>5.0) {
      WARN("Violation of longitudinal momentum conservation > 5.0 MeV. pLongBalance = " << pLongBalance << std::endl);
    }
    if(std::abs(pTransBalance)>5.0) {
      WARN("Violation of transverse momentum conservation > 5.0 MeV. pTransBalance = " << pTransBalance << std::endl);
    }

    // Feed the EventInfo variables
    theEventInfo.EBalance = theBalance.energy;
    theEventInfo.pLongBalance = pLongBalance;
    theEventInfo.pTransBalance = pTransBalance;
  }

  G4bool INCL::continueCascade() {
    Nucleus *nucleus = propagationModel->getNucleus();
    // Stop if we have passed the stopping time
    if(propagationModel->getCurrentTime() > propagationModel->getStoppingTime()) return false;
    // Stop if there are no participants and no pions inside the nucleus
    if(nucleus->getStore()->getBook()->getParticipants()==0 &&
        nucleus->getStore()->getIncomingParticles().empty()) return false;
    // Stop if the remnant has only one nucleon
    if(nucleus->getA() <= 1) return false;
    return true;
  }

  void INCL::finaliseGlobalInfo() {
    theGlobalInfo.nucleonAbsorptionCrossSection = theGlobalInfo.geometricCrossSection *
      ((G4double) theGlobalInfo.nNucleonAbsorptions) / ((G4double) theGlobalInfo.nShots);
    theGlobalInfo.pionAbsorptionCrossSection = theGlobalInfo.geometricCrossSection *
      ((G4double) theGlobalInfo.nPionAbsorptions) / ((G4double) theGlobalInfo.nShots);
    theGlobalInfo.reactionCrossSection = theGlobalInfo.geometricCrossSection *
      ((G4double) (theGlobalInfo.nShots - theGlobalInfo.nTransparents)) /
      ((G4double) theGlobalInfo.nShots);
    theGlobalInfo.errorReactionCrossSection = theGlobalInfo.geometricCrossSection *
      std::sqrt((G4double) (theGlobalInfo.nShots - theGlobalInfo.nTransparents)) /
      ((G4double) theGlobalInfo.nShots);
  }

}
