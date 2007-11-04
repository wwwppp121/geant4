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
// $Id: G4ScoringManager.hh,v 1.18 2007-11-04 04:06:09 asaim Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#ifndef G4ScoringManager_h
#define G4ScoringManager_h 1

#include "globals.hh"
#include "G4VScoringMesh.hh"
#include <vector>
class G4ScoringMessenger;
class G4ScoreQuantityMessenger;
class G4VHitsCollection;
class G4VScoreWriter;
class G4VScoreColorMap;

// class description:
//
//  This is a singleton class which manages the interactive scoring.
// The user cannot access to the constructor. The pointer of the
// only existing object can be got via G4ScoringManager::GetScoringManager()
// static method. The first invokation of this static method makes
// the singleton object.
//

typedef std::vector<G4VScoringMesh*> MeshVec;
typedef std::vector<G4VScoringMesh*>::iterator MeshVecItr;
typedef std::vector<G4VScoringMesh*>::const_iterator MeshVecConstItr;

class G4ScoringManager 
{
  public: // with description
      static G4ScoringManager* GetScoringManager();
      // Returns the pointer to the singleton object.
  public:
      static G4ScoringManager* GetScoringManagerIfExist();

  protected:
      G4ScoringManager();

  public:
      ~G4ScoringManager();

  public:
      void Accumulate(G4VHitsCollection* map);
      G4VScoringMesh* FindMesh(G4String);
      void List() const;
      void Dump() const;
      void DrawMesh(G4String meshName,G4String psName,G4String colorMapName,G4int axflg=111);
      void DumpQuantityToFile(G4String meshName, G4String psName,G4String fileName, G4String option = "");
      void DumpAllQuantitiesToFile(G4String meshName, G4String fileName, G4String option = "");

  private: 
      static G4ScoringManager * fSManager;
      G4int verboseLevel;
      G4ScoringMessenger* fMessenger;
      G4ScoreQuantityMessenger* fQuantityMessenger;

      MeshVec fMeshVec;
      G4VScoringMesh* fCurrentMesh;

      G4VScoreWriter * writer;
      G4VScoreColorMap * fDefaultLinearColorMap;

  public:
      inline void SetCurrentMesh(G4VScoringMesh* cm)
      { fCurrentMesh = cm; }
      inline G4VScoringMesh* GetCurrentMesh() const
      { return fCurrentMesh; }
      inline void CloseCurrentMesh()
      { fCurrentMesh = 0; }
      inline void SetVerboseLevel(G4int vl) 
      {
        verboseLevel = vl;
        for(MeshVecItr itr = fMeshVec.begin(); itr != fMeshVec.end(); itr++) {
         (*itr)->SetVerboseLevel(vl);
        }
      }
      inline G4int GetVerboseLevel() const
      { return verboseLevel; }
      inline size_t GetNumberOfMesh() const
      { return fMeshVec.size(); }
      inline void RegisterScoringMesh(G4VScoringMesh * sm)
      {
        sm->SetVerboseLevel(verboseLevel);
        fMeshVec.push_back(sm);
        SetCurrentMesh(sm);
      }
      inline G4VScoringMesh* GetMesh(G4int i) const
      { return fMeshVec[i]; }
      inline G4String GetWorldName(G4int i) const
      { return fMeshVec[i]->GetWorldName(); }

      inline void SetScoreWriter(G4VScoreWriter * sw)
      { writer = sw; }
};




#endif

