// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: Tst14RunAction.hh,v 1.2 1999-06-14 14:28:35 aforti Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef Tst14RunAction_h
#define Tst14RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class Tst14RunAction : public G4UserRunAction
{
  public:
    Tst14RunAction();
    virtual ~Tst14RunAction();

  public:
    virtual void BeginOfRunAction(const G4Run* aRun);
    virtual void EndOfRunAction(const G4Run* aRun);

};

#endif

