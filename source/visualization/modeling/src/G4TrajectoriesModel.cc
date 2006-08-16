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
// $Id: G4TrajectoriesModel.cc,v 1.20 2006-08-16 16:09:38 allison Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  26th August 1998.
// Model which knows how to draw GEANT4 trajectories.

#include "G4TrajectoriesModel.hh"

#include "G4VGraphicsScene.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

G4TrajectoriesModel::~G4TrajectoriesModel () {}

G4TrajectoriesModel::G4TrajectoriesModel (G4int drawingMode):
fDrawingMode(drawingMode) {
  fGlobalTag = "G4TrajectoriesModel for all trajectories.";
  fGlobalDescription = fGlobalTag;
}

void G4TrajectoriesModel::DescribeYourselfTo (G4VGraphicsScene& sceneHandler) {
  const G4Event* event = 
    G4EventManager::GetEventManager()->GetConstCurrentEvent();
  if (event) {
    G4TrajectoryContainer* TC = event -> GetTrajectoryContainer ();
    if (TC) {
      for (G4int iT = 0; iT < TC->entries(); iT++) {
	G4VTrajectory* pTraj = (*TC) [iT];
	// Debug: pTraj->ShowTrajectory(); G4cout << G4endl;
	sceneHandler.AddCompound (*pTraj);
      }
    }
  }
}
