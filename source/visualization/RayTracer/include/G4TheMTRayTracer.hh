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
// $Id: G4TheMTRayTracer.hh 66264 2012-12-14 10:17:44Z allison $
//
//


#ifndef G4TheMTRayTracer_H
#define G4TheMTRayTracer_H 1

// class description:
//
// G4TheMTRayTracer
//   This is a graphics driver of Geant4 which generates a figure file by
// ray tracing technique. The format of output figure file can be selected
// by assigning a pointer of G4VFigureFileMaker concrete class object.
//   The main entry of ray tracing is Trace() method, which is available
// only at Idle state. G4TheMTRayTracer shoots rays and controls its own event
// loop. It generates G4Event objects used for its own purpose. When ray
// tracing is working, all sensitive detectors are inactivated and all
// user action classes are swapped out. Still, verbosities set to Geant4
// manager classes are concerned. Thus, it is recommended to set verbosities
// to minimum (usually zero).
//   G4TheMTRayTracer can visualise absolutely all kinds of geometrical shapes
// which G4Navigator can deal with. Instead, it can NOT visualise hits
// nor trajectories generated by usual simulation.

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Colour.hh"

class G4RTMessenger;
class G4RayShooter;
class G4VFigureFileMaker;
class G4VisAttributes;
class G4VRTScanner;
class G4UserWorkerInitialization;
class G4RTWorkerInitialization;
class G4UserRunAction;
class G4RTRunAction;
class G4RTPrimaryGeneratorAction;
class G4RTRun;
class G4RayTracerViewer;

#include "G4TheRayTracer.hh"

class G4TheMTRayTracer : public G4TheRayTracer
{
    friend class G4RTPrimaryGeneratorAction;
    friend class G4RTRun;
    friend class G4RayTracerViewer;

  private:
    static G4TheMTRayTracer* theInstance;

  public: // with description
    G4TheMTRayTracer(G4VFigureFileMaker* figMaker = 0,
		   G4VRTScanner* scanner = 0);
    // Constructor. The argument is the pointer to G4VFigureFileMaker
    // concrete class object. If it is not set and
    // SetFigureFileMaker() method is not invoked before Trace()
    // command is invoked, then G4RTJpegMaker will be used and JPEG
    // file will be generated.  The second argument is a scanner that
    // produces a sequence of window coordinates.  If it is not set
    // here or if SetScanner is not invoked before Trace(), a default
    // G4RTSimpleScanner will be used.

  public:
    virtual ~G4TheMTRayTracer();

  public: // with description
    virtual void Trace(G4String fileName);
    // The main entry point which triggers ray tracing. "fileName" is output
    // file name, and it must contain extention (e.g. myFigure.jpg). This
    // method is available only if Geant4 is at Idle state.

  protected:
    virtual G4bool CreateBitMap();
    // Event loop

  protected:
    virtual void StoreUserActions();
    virtual void RestoreUserActions();

  protected:
    const G4UserWorkerInitialization * theUserWorkerInitialization;
    G4RTWorkerInitialization * theRTWorkerInitialization;
    const G4UserRunAction * theUserRunAction;
    G4RTRunAction * theRTRunAction;

};

#endif