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
// $Id: G4VDivisionParameterisation.hh,v 1.1 2003-06-16 15:11:41 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// class G4VDivisionParameterisation
//
// Class description:
//
// ......
//
// Member data:
//

// History:
// 09.05.01 - P.Arce Initial version
// ********************************************************************

#ifndef G4VDivisionParameterisation_H
#define G4VDivisionParameterisation_H 1

#include "globals.hh"
#include "geomdefs.hh"
#include "G4VPVParameterisation.hh"

enum DivisionType { DivNDIVandWIDTH, DivNDIV, DivWIDTH };

class G4VPhysicalVolume;
class HepTransform3D;
class G4VSolid;

class G4VDivisionParameterisation : public G4VPVParameterisation
{ 
  public:  // with description

    G4VDivisionParameterisation( EAxis axis, G4int nDiv, G4double width,
                                 G4double offset, G4VSolid* motherSolid = 0);
    virtual ~G4VDivisionParameterisation();

    virtual void ComputeTransformation(const G4int copyNo,
                                       G4VPhysicalVolume *physVol) const = 0;

    inline const G4String& GetType() const;
    inline EAxis GetAxis() const;
    inline G4int GetNoDiv() const;
    inline G4double GetWidth() const;
    inline G4double GetOffset() const;
    inline G4VSolid* GetMotherSolid() const;
    inline void SetType(const G4String& type);
    inline G4int VolumeFirstCopyNo() const;

  protected:  // with description

    void CheckMotherSolid( const G4VSolid* motherSolid,
                           const G4String& solidType ) const;
    void ChangeRotMatrix( G4VPhysicalVolume* physVol,
                          G4double rotZ = 0. ) const;
    
    G4int CalculateNDiv( G4double motherDim, G4double width,
                         G4double offset ) const;
    G4double CalculateWidth( G4double motherDim, G4int nDiv,
                             G4double offset ) const;

  protected:

    G4String ftype;
    EAxis faxis;
    G4int fnDiv;
    G4double fwidth;
    G4double foffset;
    G4VSolid* fmotherSolid;

    static G4int verbose;
    G4int theVoluFirstCopyNo;
};

#include "G4VDivisionParameterisation.icc"

#endif
