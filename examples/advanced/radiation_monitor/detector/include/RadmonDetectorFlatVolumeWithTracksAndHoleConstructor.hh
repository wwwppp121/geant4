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
// File name:     RadmonDetectorFlatVolumeWithTracksAndHoleConstructor.hh
// Creation date: Sep 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonDetectorFlatVolumeWithTracksAndHoleConstructor.hh,v 1.2 2006-06-28 13:47:18 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Builds a box with tracks and a hole
//

#ifndef   RADMONDETECTORFLATVOLUMEWITHTRACKSANDHOLECONSTRUCTOR_HH
 #define  RADMONDETECTORFLATVOLUMEWITHTRACKSANDHOLECONSTRUCTOR_HH

 // Include files
 #include "RadmonTDetectorLayerConstructor.hh"
 #include "RadmonTDetectorVolumesWithTracksDecorator.hh"
 #include "RadmonTDetectorVolumesWithHoleDecorator.hh"
 #include "RadmonDetectorFlatVolumeComponent.hh"
 
 class RadmonDetectorFlatVolumeWithTracksAndHoleConstructor : public RadmonTDetectorLayerConstructor<RadmonTDetectorVolumesWithTracksDecorator<RadmonTDetectorVolumesWithHoleDecorator<RadmonDetectorFlatVolumeComponent> > >
 {
  public:
   inline                                       RadmonDetectorFlatVolumeWithTracksAndHoleConstructor();
   inline virtual                              ~RadmonDetectorFlatVolumeWithTracksAndHoleConstructor();

  private:
  // Hidden constructors and operators
                                                RadmonDetectorFlatVolumeWithTracksAndHoleConstructor(const RadmonDetectorFlatVolumeWithTracksAndHoleConstructor & copy);
   RadmonDetectorFlatVolumeWithTracksAndHoleConstructor & operator=(const RadmonDetectorFlatVolumeWithTracksAndHoleConstructor & copy);
 };
 
 // Inline implementations
 #include "RadmonDetectorFlatVolumeWithTracksAndHoleConstructor.icc"
#endif /* RADMONDETECTORFLATVOLUMEWITHTRACKSANDHOLECONSTRUCTOR_HH */
