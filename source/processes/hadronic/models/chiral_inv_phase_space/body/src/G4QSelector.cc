// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4QSelector.cc,v 1.6 2001-09-13 15:19:42 mkossov Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// -------------------------------------------------------------------
//      GEANT 4 class implementation file
//
//      For information related to this code contact:
//      CERN, CN Division, ASD group
//      ---------------- G4QSelector ----------------
//             by Mikhail Kossov, Sept 1999.
//      class for decay Selection for Hadrons generated by CHIPS Model
// -------------------------------------------------------------------
#include "G4QSelector.hh"

G4QSelector::G4QSelector(){};

G4QSelector::G4QSelector(G4QHadronVector hadronVec) : theHadrons(hadronVec){};

G4bool G4QSelector::SelectPDGSet(G4QPDGCodeVector thePDGCodes)
{
  G4int  ind;
  G4int  tNH = theHadrons.entries();
  G4int* yes = new int[tNH];
  //for (ind=0; ind<tNH; ind++)
  for (ind=tNH-1; ind>=0; ind--) // A Loop to fill
  {
    G4int c=theHadrons[ind]->GetPDGCode(); // PDGCode
  }

  delete yes;
  return false;
}

G4QSelector::~G4QSelector() {}
