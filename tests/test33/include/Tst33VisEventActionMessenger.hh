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
// $Id: Tst33VisEventActionMessenger.hh,v 1.5 2007-06-22 12:47:16 ahoward Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// ----------------------------------------------------------------------
// Class Tst33VisEventActionMessenger
//
// Class description:
//
// Provides comands for messaging in visualization mode.

// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Tst33VisEventActionMessenger_h
#define Tst33VisEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class Tst33VisEventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Tst33VisEventActionMessenger: public G4UImessenger
{
public:
  explicit Tst33VisEventActionMessenger(Tst33VisEventAction*);
  virtual ~Tst33VisEventActionMessenger();
  
  virtual void SetNewValue(G4UIcommand*, G4String);
  
private:
  Tst33VisEventActionMessenger(const Tst33VisEventActionMessenger &);
  Tst33VisEventActionMessenger &
  operator=(const Tst33VisEventActionMessenger &);
  Tst33VisEventAction*   eventAction;   
  G4UIcmdWithAString* DrawCmd;
  G4UIcmdWithAnInteger* PrintCmd;    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
