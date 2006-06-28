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
// File name:     RadmonGeneratorSourceLayout.hh
// Creation date: Oct 2005
// Main author:   Riccardo Capra <capra@ge.infn.it>
//
// Id:            $Id: RadmonGeneratorSourceLayout.hh,v 1.2 2006-06-28 13:53:09 gunter Exp $
// Tag:           $Name: not supported by cvs2svn $
//
// Description:   Container for the data of a particles source
//

#ifndef   RADMONGENERATORSOURCELAYOUT_HH
 #define  RADMONGENERATORSOURCELAYOUT_HH
 
 // Include files
 #include "G4String.hh"
 #include "RadmonGeneratorSourceAlgorithmLayout.hh"
 #include "RadmonTLabelledCollection.hh"

 class RadmonGeneratorSourceLayout
 {
  public:
   inline                                       RadmonGeneratorSourceLayout();
                                                RadmonGeneratorSourceLayout(const RadmonGeneratorSourceLayout & copy);
   inline                                      ~RadmonGeneratorSourceLayout();

   RadmonGeneratorSourceLayout &                operator=(const RadmonGeneratorSourceLayout & copy);

   inline const G4String &                      GetLabel(void) const;
   inline void                                  SetLabel(const G4String & label);

   inline G4double                              GetIntensity(void) const;
   inline void                                  SetIntensity(G4double intensity);
   
   RadmonGeneratorSourceAlgorithmLayout &       AppendAlgorithm(void);

   inline G4int                                 GetNAlgorithms(void) const;
   inline G4bool                                Empty(void) const;
   
   RadmonGeneratorSourceAlgorithmLayout &       GetAlgorithm(G4int index);
   const RadmonGeneratorSourceAlgorithmLayout & GetAlgorithm(G4int index) const;
   G4bool                                       ExistsAlgorithmByLabel(const G4String & label) const;
   G4int                                        MultiplicityAlgorithmByLabel(const G4String & label) const;
   RadmonGeneratorSourceAlgorithmLayout &       FindAlgorithmByLabel(const G4String & label, G4int count = 0);
   const RadmonGeneratorSourceAlgorithmLayout & FindAlgorithmByLabel(const G4String & label, G4int count = 0) const;

   void                                         RemoveAlgorithmByLabel(const G4String & label, G4int count = 0);
   void                                         RemoveAlgorithmsByLabel(const G4String & label);
   void                                         RemoveAlgorithm(G4int index);
   void                                         RemoveAlgorithmsByRange(G4int first, G4int last);
   void                                         RemoveAllAlgorithms(void);

   void                                         DumpLayout(std::ostream & out, const G4String & indent = G4String()) const;

  private:
   inline G4String &                            GetNullStr() const;

  // Private attributes
   RadmonTLabelledCollection<RadmonGeneratorSourceAlgorithmLayout> algorithmsCollection;
   G4String                                     sourceLabel;
   G4double                                     sourceIntensity;
 };

 // Inline implementations
 #include "RadmonGeneratorSourceLayout.icc"
#endif /* RADMONGENERATORSOURCELAYOUT_HH */
