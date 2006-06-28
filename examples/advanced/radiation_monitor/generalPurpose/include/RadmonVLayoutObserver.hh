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
// File name:     RadmonVLayoutObserver.hh
// Creation date: Sep 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonVLayoutObserver.hh,v 1.2 2006-06-28 13:52:16 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Observer class of the observer-subjectmodel for the detector
//                layout
//

#ifndef   RADMONVLAYOUTOBSERVER_HH
 #define  RADMONVLAYOUTOBSERVER_HH
 
 class RadmonVLayoutSubject;
 
 class RadmonVLayoutObserver
 {
  public:
   virtual void                                 OnLayoutChange(void) = 0;

  protected:
   inline                                       RadmonVLayoutObserver();
   inline                                      ~RadmonVLayoutObserver();

  private:
  // Hidden constructors and operators
                                                RadmonVLayoutObserver(const RadmonVLayoutObserver & copy);
   RadmonVLayoutObserver &                      operator=(const RadmonVLayoutObserver & copy);
 };
 
 // Inline implementations
 #include "RadmonVLayoutObserver.icc"
#endif /* RADMONVLAYOUTOBSERVER_HH */
