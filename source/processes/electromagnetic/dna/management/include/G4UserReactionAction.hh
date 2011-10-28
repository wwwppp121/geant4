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
// WARNING : This class is released as a prototype.
// It might strongly evolve or even disapear in the next releases.
//
// Author: Mathieu Karamitros (kara@cenbg.in2p3.fr)
//
// History:
// -----------
// 10 Oct 2011 M.Karamitros created
//
// -------------------------------------------------------------------

#ifndef G4VUSERITACTION_H
#define G4VUSERITACTION_H

#include "globals.hh"
#include "G4Track.hh"
#include "G4TrackFastVector.hh"

typedef G4FastVector<const G4Track, G4TrackFastVectorSize> G4ConstTrackFastVector;

class G4UserReactionAction
{
    //--------
    public: // with description
    //--------

    // Constructor and destructors
       G4UserReactionAction();
       G4UserReactionAction(const G4UserReactionAction& /*other*/);
       virtual ~G4UserReactionAction();

    // Member functions
       virtual void UserReactionAction(const G4Track& /*trackA*/,const G4Track& /*trackB*/,
                                       const G4ConstTrackFastVector& /*products*/,
                                       int /*nbProducts*/){;}
};

#endif // G4VUSERITACTION_H