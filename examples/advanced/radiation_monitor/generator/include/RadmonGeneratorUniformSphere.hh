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
// File name:     RadmonGeneratorUniformSphere.hh
// Creation date: Oct 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonGeneratorUniformSphere.hh,v 1.2 2006-06-28 13:53:17 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Abstract class for a primary generator 
//

#ifndef   RADMONGENERATORUNIFORMSPHERE_HH
 #define  RADMONGENERATORUNIFORMSPHERE_HH

 // Include files
 #include "RadmonVGeneratorWithLabel.hh"
 
 // Forward declaration
 class G4String;
 class G4ParticleGun;
 
 class RadmonGeneratorUniformSphere : public RadmonVGeneratorWithLabel
 {
  public:
   inline                                       RadmonGeneratorUniformSphere();
   inline virtual                              ~RadmonGeneratorUniformSphere();

   virtual void                                 ConvolveParticleGun(G4ParticleGun & gun);
   virtual RadmonVGeneratorWithLabel *          New(void) const;

  // Hidden constructors and operators
  private:
                                                RadmonGeneratorUniformSphere(const RadmonGeneratorUniformSphere & copy);
   RadmonGeneratorUniformSphere &               operator=(const RadmonGeneratorUniformSphere & copy);
 };
 
 // Inline implementations
 #include "RadmonGeneratorUniformSphere.icc"
#endif /* RADMONGENERATORUNIFORMSPHERE_HH */
