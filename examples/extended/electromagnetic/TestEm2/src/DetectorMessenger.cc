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
// $Id: DetectorMessenger.cc,v 1.6 2004-06-18 15:43:41 maire Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
:Detector(Det)
{
  testemDir = new G4UIdirectory("/testem/");
  testemDir->SetGuidance(" detector control.");

  MaterCmd = new G4UIcmdWithAString("/testem/det/setMat",this);
  MaterCmd->SetGuidance("Select Material.");
  MaterCmd->SetParameterName("material",false);
  MaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  LBinCmd = new G4UIcmdWith3Vector("/testem/det/setLbin",this);
  LBinCmd->SetGuidance("set longitudinal bining");
  LBinCmd->SetGuidance("nb of bins; bin thickness (in radl)");
  LBinCmd->SetParameterName("nLtot","dLradl"," ",true);
  LBinCmd->SetRange("nLtot>=1 && dLradl>0");
  LBinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  RBinCmd = new G4UIcmdWith3Vector("/testem/det/setRbin",this);
  RBinCmd->SetGuidance("set radial bining");
  RBinCmd->SetGuidance("nb of bins; bin thickness (in radl)");
  RBinCmd->SetParameterName("nRtot","dRradl"," ",true);
  RBinCmd->SetRange("nRtot>=1 && dRradl>0");
  RBinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  FieldCmd = new G4UIcmdWithADoubleAndUnit("/testem/det/setField",this);
  FieldCmd->SetGuidance("Define magnetic field.");
  FieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  FieldCmd->SetParameterName("Bz",false);
  FieldCmd->SetUnitCategory("Magnetic flux density");
  FieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/testem/det/update",this);
  UpdateCmd->SetGuidance("Update geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete MaterCmd;
  delete LBinCmd;
  delete RBinCmd;
  delete FieldCmd;
  delete UpdateCmd;
  delete testemDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == MaterCmd )
   { Detector->SetMaterial(newValue);}
   
  if( command == LBinCmd )
   { Detector->SetLBining(LBinCmd->GetNew3VectorValue(newValue));}

  if( command == RBinCmd )
   { Detector->SetRBining(RBinCmd->GetNew3VectorValue(newValue));}

  if( command == FieldCmd )
   { Detector->SetMagField(FieldCmd->GetNewDoubleValue(newValue));}

  if( command == UpdateCmd )
   { Detector->UpdateGeometry();}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
