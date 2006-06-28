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
//
// $Id: G4DNAProtonBornExcitation.cc,v 1.2 2006-06-28 13:58:37 gunter Exp $
// GEANT4 tag $Name: not supported by cvs2svn $

#include "G4DNAProtonBornExcitation.hh"
#include "G4Proton.hh"

                                        G4DNAProtonBornExcitationEnergyLimitsPolicy :: G4DNAProtonBornExcitationEnergyLimitsPolicy()
:
 lowEnergyLimit(100*eV),
 zeroBelowLowEnergyLimit(true),
 highEnergyLimit(100*MeV),
 zeroAboveHighEnergyLimit(false)
{
}

                                        G4DNAProtonBornExcitationIncomingParticlePolicy :: G4DNAProtonBornExcitationIncomingParticlePolicy()

{
}

                                        G4DNAProtonBornExcitationDataFilePolicy :: G4DNAProtonBornExcitationDataFilePolicy()
:
 lowEnergyLimit(100*eV),
 zeroBelowLowEnergyLimit(true),
 highEnergyLimit(100*MeV),
 zeroAboveHighEnergyLimit(false),
 dataFileEnergyUnit(eV),
 dataFileCrossSectionUnit(m*m),
 dataFileName("ProtonBornExcitationCrossSection")
{
}

const G4ParticleDefinition *            G4DNAProtonBornExcitationIncomingParticlePolicy :: IncomingParticleDefinition(void) const
{
 return G4Proton::Proton();
}

