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
// $Id: G4BoundingSphereScene.cc,v 1.6 2001-07-22 00:59:40 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// John Allison  7th June 1997
// An artificial scene to reuse G4VScene code to calculate a bounding sphere.

#include "G4BoundingSphereScene.hh"

#include "G4VSolid.hh"
#include "G4Vector3D.hh"

G4BoundingSphereScene::G4BoundingSphereScene ():
  fRadius (-1.),
  fpObjectTransformation (0)
{}

G4BoundingSphereScene::~G4BoundingSphereScene () {}

void G4BoundingSphereScene::PreAddThis
(const G4Transform3D& objectTransformation,
 const G4VisAttributes&) {
  fpObjectTransformation = &objectTransformation;
}

G4VisExtent G4BoundingSphereScene::GetBoundingSphereExtent () {
  return G4VisExtent (fCentre, fRadius);
}

void G4BoundingSphereScene::Accrue (const G4VSolid& solid) {
  const G4VisExtent& thisExtent = solid.GetExtent ();
  G4Point3D thisCentre = thisExtent.GetExtentCentre ();
  if (fpObjectTransformation) {
    thisCentre.transform (*fpObjectTransformation);
  }
  const G4double thisRadius = thisExtent.GetExtentRadius ();
  AccrueBoundingSphere (thisCentre, thisRadius);
  /***********************************************
    G4cout << "G4BoundingSphereScene::Accrue: centre: " << fCentre
           << ", radius: " << fRadius << G4endl;
  ***********************************************/
}

void G4BoundingSphereScene::ResetBoundingSphere () {
  fCentre = G4Point3D ();
  fRadius = -1.;
  fpObjectTransformation = 0;
}

void G4BoundingSphereScene::AccrueBoundingSphere
(const G4Point3D& thisCentre,
 G4double thisRadius) {

  if (fRadius < 0 ) {  // First time.
    fCentre = thisCentre;
    fRadius = thisRadius;
  }
  else {
    G4Vector3D join = thisCentre - fCentre;
    if (join == G4Vector3D (0., 0., 0.)) {             // Centres coincide.
      if (fRadius < thisRadius) fRadius = thisRadius;
    }
    else if (join.mag () + thisRadius <= fRadius) {  // Inside accrued sphere.
      // Do nothing.
    }
    else {
      G4Vector3D unitJoin = join.unit ();
      G4Point3D oldExtremity1 = fCentre - fRadius * unitJoin;
      G4Point3D newExtremity1 = thisCentre - thisRadius * unitJoin;
      G4Point3D oldExtremity2 = fCentre + fRadius * unitJoin;
      G4Point3D newExtremity2 = thisCentre + thisRadius * unitJoin;
      G4Point3D extremity1;
      if (oldExtremity1 * unitJoin < newExtremity1 * unitJoin) {
	extremity1 = oldExtremity1;
      }
      else {
	extremity1 = newExtremity1;
      }
      G4Point3D extremity2;
      if (oldExtremity2 * unitJoin > newExtremity2 * unitJoin) {
	extremity2 = oldExtremity2;
      }
      else {
	extremity2 = newExtremity2;
      }
      fCentre = 0.5 * (extremity2 + extremity1);
      fRadius = 0.5 * (extremity2 - extremity1).mag ();
    }
  }
}

