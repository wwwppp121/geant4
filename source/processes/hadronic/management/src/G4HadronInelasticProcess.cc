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
//
 // Hadronic Inelastic Process Class
 // J.L. Chuma, TRIUMF, 24-Mar-1997
 // Last modified: 27-Mar-1997
 // J.P. Wellisch: Bug hunting, 23-Apr-97
 // Modified by J.L.Chuma 8-Jul-97 to eliminate possible division by zero for sigma
//
// 14-APR-98 F.W.Jones: variant G4HadronInelastic process for
// G4CrossSectionDataSet/DataStore class design.
//
// 17-JUN-98 F.W.Jones: removed extraneous code causing core dump.
//
 
#include "G4HadronInelasticProcess.hh"
#include "G4GenericIon.hh"
 
 G4double G4HadronInelasticProcess::GetMeanFreePath(
  const G4Track &aTrack,
  G4double ,
  G4ForceCondition *)
  {
    const G4DynamicParticle *aParticle = aTrack.GetDynamicParticle();
    if( aParticle->GetDefinition() != theParticle && 
        theParticle != G4GenericIon::GenericIon())
      G4Exception( this->GetProcessName()+
                   " called for "+
                   aParticle->GetDefinition()->GetParticleName() );
    G4Material *aMaterial = aTrack.GetMaterial();
    G4int nElements = aMaterial->GetNumberOfElements();
    
    // returns the mean free path in GEANT4 internal units
    
    const G4double *theAtomicNumDensityVector =
      aMaterial->GetAtomicNumDensityVector();
    
    G4double aTemp = aMaterial->GetTemperature();
        
    G4double sigma = 0.0;
    for( G4int i=0; i<nElements; ++i )
    {
      G4double xSection =
        GetMicroscopicCrossSection( aParticle, (*aMaterial->GetElementVector())[i], aTemp);
      sigma += theAtomicNumDensityVector[i] * xSection;
    }
    sigma *= aScaleFactor;
    theLastCrossSection = sigma;
    if( sigma > 0.0 )
      return 1.0/sigma;
    else
      return DBL_MAX;
  }
 
 void 
  G4HadronInelasticProcess::BuildThePhysicsTable()
  {
   if (!theCrossSectionDataStore) {
     //      G4Exception("G4HadronInelasticProcess::BuildThePhysicsTable: "
     //                  "no CrossSectionDataStore");
      return;
   }

   theCrossSectionDataStore->BuildPhysicsTable(*theParticle);

   //    G4int numberOfElements = G4Element::GetNumberOfElements();
   //    thePhysicsTable = new G4PhysicsTable( numberOfElements );
   //    
   //    // make a PhysicsVector for each element
   //    
   //    static const G4ElementTable *theElementTable = G4Element::GetElementTable();
   //    for( G4int i=0; i<numberOfElements; ++i )
   //      (*thePhysicsTable)(i) =
   //        theCrossSectionData.MakePhysicsVector( *this, *theParticle,
   //                                               (*theElementTable)[i] );
  }
 
 G4double G4HadronInelasticProcess::GetMicroscopicCrossSection(
  const G4DynamicParticle *aParticle,
  const G4Element *anElement,
  G4double aTemp)
  {
    // returns the microscopic cross section in GEANT4 internal units
    
   if (!theCrossSectionDataStore) {
      G4Exception("G4HadronInelasticProcess::GetMicroscopicCrossSection:"
                  "no CrossSectionDataStore");
      return DBL_MIN;
   }
   return theCrossSectionDataStore->GetCrossSection(aParticle, anElement, aTemp);

   //    G4bool isOutRange;
   //    G4int j = anElement->GetIndex();
   //    
   //    G4double s = (*((*thePhysicsTable)(j))).GetValue(
   //     aParticle->GetTotalMomentum()/GeV, isOutRange );
   //    return s;
  }
 
 /* end of file */
