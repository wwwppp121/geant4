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
/// \file runAndEvent/RE03/src/RE03WorkerInitialization.cc
/// \brief Implementation of the RE03WorkerInitialization class
//
//
// $Id: RE03WorkerInitialization.cc 66780 2013-01-12 14:56:35Z gcosmo $
//

#include "RE03WorkerInitialization.hh"
#include "RE03PrimaryGeneratorAction.hh"

RE03WorkerInitialization::RE03WorkerInitialization()
{;} 

RE03WorkerInitialization::~RE03WorkerInitialization()
{;}

void RE03WorkerInitialization::WorkerStart() const
{
  SetUserAction(new RE03PrimaryGeneratorAction);
}
 
