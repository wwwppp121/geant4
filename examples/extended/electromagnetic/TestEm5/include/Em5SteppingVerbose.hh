// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: Em5SteppingVerbose.hh,v 1.3 2000-02-29 12:23:26 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//  
//---------------------------------------------------------------
//
// Em5SteppingVerbose.hh
//
// Description:
//   This class manages the vervose outputs in G4SteppingManager. 
//   
//
//
//---------------------------------------------------------------

class Em5SteppingVerbose;

#ifndef Em5SteppingVerbose_h
#define Em5SteppingVerbose_h 1

#include "G4SteppingVerbose.hh"

class Em5SteppingVerbose : public G4SteppingVerbose {
public:   
// Constructor/Destructor
  Em5SteppingVerbose();
 ~Em5SteppingVerbose();
//
  void StepInfo();
  void TrackingStarted();
//


};

#endif
