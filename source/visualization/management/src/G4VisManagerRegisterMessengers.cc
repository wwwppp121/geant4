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
// $Id: G4VisManagerRegisterMessengers.cc,v 1.38 2001-08-17 23:03:05 johna Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// G4VisManager::RegisterMessengers - John Allison 30/July/1998.

#include "G4VisManager.hh"
#include "G4VVisCommand.hh"
#include "G4VisCommands.hh"
#include "G4VisCommandsCompound.hh"
#include "G4VisCommandsScene.hh"
#include "G4VisCommandsSceneAdd.hh"
#include "G4VisCommandsSceneHandler.hh"
#include "G4VisCommandsViewer.hh"
#include "G4VisCommandsViewerSet.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"

void G4VisManager::RegisterMessengers () {

/******************************************************************

(Extract for working note.)

NI means "not implemented yet".


Introduction
============

This note defines the concepts of scene, scene handler and viewer.

A scene handler can handle only one scene at any one time but can have
any number of viewers.

A scene can be attached to any number of scene handlers.

It is important to realise that there is also the concept of a "current"
scene, "current" scene handler and "current" viewer which are maintained
by the G4VisManager.  It is usually the last object created or selected
or operated upon.

Note that there is also a concept of a "standard view" which is that which
comfortably includes all components of the scene.  From this follow the
concept of standard target point, etc.  It is the responsibility of
each viewer to apply its view parameters relatively, taking into account
the scene it represents.  This is a dynamic operation and the scene handler
and its viewers must be smart enough to know when to recalculate
graphics-system-dependent quantities when the scene or the view parameters
change.

See G4VisManager.hh and G4ViewParameters.hh for a more extended description.

Scenes
======

A scene is a list of visualizable objects, such as detector components,
hits, trajectories, axes, etc.

Scenes are graphics-system-independent.

The G4VisManager has a list of scenes.

Unless otherwise stated, commands affect the current scene only.

/vis/scene/add/axes [<x0>] [<y0>] [<z0>] [<length>] [<unit>]
  default:             0      0      0       1         m
  Draws axes at (x0, y0, z0) of given length.

NI /vis/scene/add/ghost [<particle>] [<physical-volume-name>]
NI                      [<copy-no>] [<depth>]

/vis/scene/add/ghosts [<particle>]
  default:                 all

/vis/scene/add/hits [<sensitive-volume-name>]
  default:              (argument not impl'd yet.)
  Causes hits, if any, to be drawn at the end of processiing an event.

/vis/scene/add/logicalVolume <logical-volume-name> [<depth>] 
  default:                                             1

/vis/scene/add/scale [<length> <length-unit>] [x|y|z] [<red>] [<green>] [<blue>] [auto|manual] [<xmid> <ymid> <zmid> <unit>]
  default:                1          m           x       1         1        1        auto          0      0      0     m
  Adds an annotated scale line to the current scene.  See G4Scale.hh
  for further description.

/vis/scene/add/text

/vis/scene/add/trajectories [<sensitive-volume-name>]
  default:                      (argument not impl'd yet.)
  Causes trajectories, if any, to be drawn at the end of processiing an event.

NI /vis/scene/add/transientObjects ???????????? (JA 9/Aug/01)

/vis/scene/add/volume [<physical-volume-name>] [<copy-no>] [<depth>]
  default:                     world                -1         -1
  Adds physical volume to current scene.
  If copy-no is negative, first occurrence of physical-volume-name is
    selected.
  If depth is negative, search is made to all depths.

/vis/scene/create [<scene-name>]
  default       auto-generated name
  This scene becomes current.

NI /vis/scene/edit  (Just make a new one? JA 9/Aug/01)

/vis/scene/endOfEventAction [accumulate|refresh]
  default:                        refresh
  The scene handler for this scene will, when a viewer of this scene is
  current, be requested to accumulate "transient" objects, such as hits,
  event by event or to erase them (refresh the screen) before drawing the
  transient objects of the next event.  Any "run-persistent" objects, such
  as dectector geometry components, are unaffected and remain in the viewer.

/vis/scene/list [<scene-name>] [<verbosity>]
  default:           all             0
  Current scene remains current.

/vis/scene/notifyHandlers
  Clears and refreshes all viewers of current scene.
  Does not issue "update" (see /vis/viewer/update).

/vis/scene/remove <scene-name>
  Current scene can change or become invalid.

/vis/scene/select [<scene-name>]
  default:      current scene name
  This scene becomes current.

NI /vis/scene/set/hitOption accumulate|byEvent
  default:                   byEvent
  (Implemented as see /vis/scene/accumulate true|false.))

NI /vis/scene/set/modelingStyle [<modeling-style>]

NI /vis/scene/set/notifyOption immediate|delayed


Scene Handlers
==============

A scene handler is an object which knows how to interpret a scene for a
specific graphics system.

Each scene handler handles one scene and has, in general, any number of
viewers.

The G4VisManager has a list of scene handlers.

/vis/sceneHandler/attach [<scene-name>]
  default:             current scene name
  Attaches scene to current scene handler.

/vis/sceneHandler/create [<graphics-system-name>] [<scene-handler-name>]
  default:                     error               auto-generated name
  (The first default simply triggers a list of possibilities.)
  This scene handler becomes current.
  The current scene, if any, is attached.

/vis/sceneHandler/list [<scene-handler-name>] [<verbosity>]
  default:                     all                  0
  Current scene handler remains current.

NI /vis/sceneHandler/notifyEndOfProcessing
  Issues "update" for each viewer of current scene handler.

NI /vis/sceneHandler/processScene
  Refreshes all viewers of current scene handler.
  Does not issue "update" (see /vis/viewer/update).

/vis/sceneHandler/remove <scene-handler-name>
  Current scene handler can change or become invalid.

/vis/sceneHandler/select [<scene-handler-name>]
  default:            current scene handler name
  This scene handler becomes current.


Viewers
=======

A viewer opens windows and draws to the screen or writes to file for
off-line viewing or hardcopy, etc.  It can be dumb (a non-interactive
window) or intelligent (respond to mouse clicks, spawn other windows,
change viewpoint, etc.).

Most viewer commands respond to the viewer "short name", which is the
name up to the first space character, if any.  Thus, a viewer name can
contain spaces but must be unique up to the first space.

Unless otherwise stated, commands affect the current viewer only.

/vis/viewer/clear

NI /vis/viewer/clone
  Creates a clone.  Clone becomes current viewer.

/vis/viewer/create [<scene-handler>]           [<viewer-name>]     [<pixels>]
  default:     current scene handler name    auto-generated name      600
  Pixel size of square window (hint only).
  This viewer becomes current.

/vis/viewer/dolly [<increment>] [<unit>]
  default:        current-value    m
  Moves the camera incrementally in by this distance.

/vis/viewer/dollyTo [<distance>] [<unit>]
  default:         current-value    m
  Moves the camera in this distance relative to standard target point.

/vis/viewer/lightsThetaPhi [<theta>] [<phi>] [deg|rad]
  default: current as default.
/vis/viewer/lightsVector [<x>] [<y>] [<z>]
  default: current as default.

/vis/viewer/list   [<viewer-name>]    [<verbosity>]
  default:             all                  0
  Current viewer remains current.

/vis/viewer/pan [<right-increment>] [<up-increment>] [<unit>]
  default:                 0           0
  Moves the camera incrementally right and up by these amounts.

/vis/viewer/panTo [<right>] [<up>] [<unit>]
  default:                0     0
  Moves the camera to this position right and up relative to standard target
    point.

/vis/viewer/select <viewer-name>
  default:          no default
  This viewer becomes current.

/vis/viewer/refresh [<viewer-name>]
  default:       current viewer name
  Re-traverses graphical data, which is enough in some cases to refresh the
  view.  In some cases post-processing is required to complete the view -
  see /vis/viewer/update.
  This viewer becomes current.

/vis/viewer/remove <viewer-name>
  default:          no default
  Current viewer can change or become invalid.

/vis/viewer/reset [<viewer-name>]
  default:      current viewer name
  Resets view parameters to defaults.
  This viewer becomes current.

/vis/viewer/viewpointThetaPhi [<theta>] [<phi>] [deg|rad]
  default: current as default.
/vis/viewer/viewpointVector [<x>] [<y>] [<z>]
  default: current as default.
  Set direction from target to camera.  Also changes lightpoint direction if
  lights are set to move with camera.

/vis/viewer/zoom [<factor>]
  default:            1
  Multiplies magnification by this factor.

/vis/viewer/zoomTo [<factor>]
  default:             1
  Magnifies by this factor relative to standard view.

/vis/viewer/set/all <from-viewer-name>
  Copies view parameters from from-viewer to current viewer.

/vis/viewer/set/autoRefresh [true|false]
  default:                    false
  View is automatically refreshed after a change of view parameters.

/vis/viewer/set/culling
  g[lobal]|c[overedDaughters]|i[nvisible]|d[ensity] [true|false]
  [density] [unit]
  default: none true 0.01 g/cm3

NI /vis/viewer/set/cutawayPlane ...
  Set plane(s) for cutaway views.

/vis/viewer/set/edge [true|false]
  default:              true

/vis/viewer/set/hiddenEdge  [true|false]
  default:                       true

/vis/viewer/set/hiddenMarker  [true|false]
  default:                       true

/vis/viewer/set/lightsMove with-camera|with-object

/vis/viewer/set/lineSegmentsPerCircle        [<number-of-sides-per-circle>]
  default:                                         24
  Number of sides per circle in polygon/polyhedron graphical representation
    of objects with curved lines/surfaces.

/vis/viewer/set/projection
  o[rthogonal]|p[erspective] [<field-half-angle>] [deg|rad]
  default: none                       30             deg

/vis/viewer/set/sectionPlane ...
  Set plane for drawing section (DCUT).  Specify plane by x y z units nx ny nz,
  e.g., for a y-z plane at x = 1 cm:
  /vis/viewer/set/section_plane on 1 0 0 cm 1 0 0

/vis/viewer/set/style w[ireframe]|s[urface]

NI /vis/viewer/notifyOption immediate|delayed ?Issue of "update" after "set"?

NI /vis/viewer/notifyHandler ??

/vis/viewer/update [<viewer-name>]
  default:     current viewer name
  Initiates post-processing if required.  This viewer becomes current.


Attributes (nothing implemented yet)
==========

The G4VisManager also keeps a list of visualization attributes which can
be created and changed and attributed to visualizable objects.

Unless otherwise stated, commands affect the visualization attributes of
the current viewer only.

NI /vis/attributes/create [<vis-attributes-name>]
  default:                 auto-generated name
  These attributes are passed to the current viewer???????????

NI /vis/attributes/set/colour [<vis-attributes-name>] [<r>]  [<g>] [<b>] [<o>]
NI /vis/attributes/set/color  [<vis-attributes-name>] [<r>]  [<g>] [<b>] [<o>]
  default:                   current attributes name   1      1     1     1
  Sets colour (red, green, blue, opacity).

NI /vis/scene/set/attributes <logical-volume-name>
  Associates current vis attributes with logical volume.  (Do we need to
    provide possibility of resetting to original attributes?)
  (Move to scene when implemented.)


General Commands
================

/vis/enable [true|false]
  default:       true
/vis/disable
  Enables/disables visualization system.

/vis/verbose [<verbosity>]
  default:     warnings
  Simple graded message scheme - give first letter or a digit:
    0) quiet,         // Nothing is printed.
    1) startup,       // Startup and endup messages are printed...
    2) errors,        // ...and errors...
    3) warnings,      // ...and warnings...
    4) confirmations, // ...and confirming messages...
    5) parameters,    // ...and parameters of scenes and views...
    6) all            // ...and everything available.


Compound Commands
=================

NI /vis/draw <physical-volume-name> clashes with old /vis~/draw/, so...

/vis/drawTree [<physical-volume-name>] [<system>]
default:           world                ATree
  /vis/open $2
  /vis/drawVolume $1

/vis/drawVolume [<physical-volume-name>]
default:             world
  /vis/scene/create
  /vis/scene/add/volume $1
  /vis/sceneHandler/attach
  /vis/viewer/refresh

/vis/drawView [<theta-deg>] [<phi-deg>]
              [<pan-right>] [<pan-up>] [<pan-unit>]
              [<zoom-factor>]
              [<dolly>] [<dolly-unit>]
default: 0 0 0 0 cm 1 0 cm
  /vis/viewer/viewpointThetaPhi $1 $2 deg
  /vis/viewer/panTo $3 $4 $5
  /vis/viewer/zoomTo $6
  /vis/viewer/dollyTo $7 $8

/vis/open [<graphics-system-name>] [<[pixels>]
default:          error                600
  /vis/scene/create
  /vis/sceneHandler/create $1
  /vis/viewer/create ! ! $2

/vis/specify <logical-volume-name>
  /geometry/print $1
  /vis/scene/create
  /vis/scene/add/logicalVolume $1
  /vis/sceneHandler/attach
  /vis/viewer/refresh

******************************************************************/

  // Instantiate individual messengers/commands (usually one command
  // per messenger).

  G4VVisCommand::SetVisManager (this);

  G4UIcommand* directory;

  directory = new G4UIdirectory ("/vis/");
  directory -> SetGuidance ("Visualization commands.");
  fDirectoryList.push_back (directory);
  fMessengerList.push_back (new G4VisCommandEnable);
  fMessengerList.push_back (new G4VisCommandVerbose);

  directory = new G4UIdirectory ("/vis/scene/");
  directory -> SetGuidance ("Operations on Geant4 scenes.");
  fDirectoryList.push_back (directory);
  fMessengerList.push_back (new G4VisCommandSceneCreate);
  fMessengerList.push_back (new G4VisCommandSceneEndOfEventAction);
  fMessengerList.push_back (new G4VisCommandSceneList);
  fMessengerList.push_back (new G4VisCommandSceneNotifyHandlers);
  fMessengerList.push_back (new G4VisCommandSceneRemove);
  fMessengerList.push_back (new G4VisCommandSceneSelect);

  directory = new G4UIdirectory ("/vis/scene/add/");
  directory -> SetGuidance ("Add model to current scene.");
  fDirectoryList.push_back (directory);
  fMessengerList.push_back (new G4VisCommandSceneAddAxes);
  fMessengerList.push_back (new G4VisCommandSceneAddGhosts);
  fMessengerList.push_back (new G4VisCommandSceneAddHits);
  fMessengerList.push_back (new G4VisCommandSceneAddLogicalVolume);
  fMessengerList.push_back (new G4VisCommandSceneAddScale);
  fMessengerList.push_back (new G4VisCommandSceneAddText);
  fMessengerList.push_back (new G4VisCommandSceneAddTrajectories);
  fMessengerList.push_back (new G4VisCommandSceneAddVolume);

  directory = new G4UIdirectory ("/vis/sceneHandler/");
  directory -> SetGuidance ("Operations on Geant4 scene handlers.");
  fDirectoryList.push_back (directory);
  fMessengerList.push_back (new G4VisCommandSceneHandlerAttach);
  fMessengerList.push_back (new G4VisCommandSceneHandlerCreate);
  fMessengerList.push_back (new G4VisCommandSceneHandlerList);
  fMessengerList.push_back (new G4VisCommandSceneHandlerRemove);
  fMessengerList.push_back (new G4VisCommandSceneHandlerSelect);

  directory = new G4UIdirectory ("/vis/viewer/");
  directory -> SetGuidance ("Operations on Geant4 viewers.");
  fDirectoryList.push_back (directory);
  fMessengerList.push_back (new G4VisCommandViewerClear);
  fMessengerList.push_back (new G4VisCommandViewerCreate);
  fMessengerList.push_back (new G4VisCommandViewerDolly);
  fMessengerList.push_back (new G4VisCommandViewerLights);
  fMessengerList.push_back (new G4VisCommandViewerList);
  fMessengerList.push_back (new G4VisCommandViewerPan);
  fMessengerList.push_back (new G4VisCommandViewerRefresh);
  fMessengerList.push_back (new G4VisCommandViewerRemove);
  fMessengerList.push_back (new G4VisCommandViewerReset);
  fMessengerList.push_back (new G4VisCommandViewerSelect);
  fMessengerList.push_back (new G4VisCommandViewerUpdate);
  fMessengerList.push_back (new G4VisCommandViewerViewpoint);
  fMessengerList.push_back (new G4VisCommandViewerZoom);

  directory = new G4UIdirectory ("/vis/viewer/set/");
  directory -> SetGuidance ("Set view parameters of current viewer.");
  fDirectoryList.push_back (directory);
  fMessengerList.push_back (new G4VisCommandsViewerSet);

  // Compound commands...
  fMessengerList.push_back (new G4VisCommandDrawTree);
  fMessengerList.push_back (new G4VisCommandDrawVolume);
  fMessengerList.push_back (new G4VisCommandDrawView);
  fMessengerList.push_back (new G4VisCommandOpen);
  fMessengerList.push_back (new G4VisCommandSpecify);

}
