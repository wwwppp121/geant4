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
// Author: Patricia Mendez (patricia.mendez@cern.ch)
//
// History:
// -----------
// 14 Feb 2003 Patricia Mendez     Created
//--------------------------------------------------------------------------

#ifdef G4ANALYSIS_USE
#include "FCALAnalysisMessenger.hh"

#include "FCALAnalysisManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

FCALAnalysisMessenger::FCALAnalysisMessenger(FCALAnalysisManager* analysisManager)
  :xrayFluoAnalysis(analysisManager)

{ 
  FCALAnalysisDir = new G4UIdirectory("/analysis/");
  FCALAnalysisDir->SetGuidance("analysis control.");

  ouputFileCommand = new G4UIcmdWithAString("/analysis/outputFile",this);
  ouputFileCommand->SetGuidance("specify the name of the output file (lowercase for Hbook)");
  ouputFileCommand->SetGuidance("default: xrayfluo.hbk");
  ouputFileCommand->SetParameterName("choice",true);
  ouputFileCommand->SetDefaultValue("fcal.his");
  ouputFileCommand->AvailableForStates(G4State_Idle);
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

FCALAnalysisMessenger::~FCALAnalysisMessenger()
{
  
  delete FCALAnalysisDir; 
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void FCALAnalysisMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 

if(command == ouputFileCommand)
    {xrayFluoAnalysis->SetOutputFileName(newValue);}

   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#endif













