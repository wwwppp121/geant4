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
// $Id$
//
// Author: Mathieu Karamitros, kara@cenbg.in2p3.fr

// The code is developed in the framework of the ESA AO7146
//
// We would be very happy hearing from you, so do not hesitate to send us your feedback!
//
// In order for Geant4-DNA to be maintained and still open-source, article citations are crucial. 
// If you use Geant4-DNA chemistry and you publish papers about your software, in addition to the general paper on Geant4-DNA:
//
// The Geant4-DNA project, S. Incerti et al., Int. J. Model. Simul. Sci. Comput. 1 (2010) 157–178
//
// we ask that you please cite the following papers reference papers on chemistry:
//
// Diﬀusion-controlled reactions modelling in Geant4-DNA, M. Karamitros et al., 2014 (submitted)
// Modeling Radiation Chemistry in the Geant4 Toolkit, M. Karamitros et al., Prog. Nucl. Sci. Tec. 2 (2011) 503-508


#ifndef G4AllITManager_h
#define G4AllITManager_h 1
#include "globals.hh"
#include "G4ITType.hh"
#include "G4ThreeVector.hh"
#include "G4KDTreeResult.hh"

#include <map>
#include <vector>
#include <memory>

class G4IT;
class G4VITManager;
class G4ITBox;
class G4Track;
template<typename T> class G4ITManager;

/**
  * Holds all IT Manager, and take care of deleting them
  * when AllITManager is deleted
  * Set general verbose for all IT Manager
  */

class G4AllITManager
{
public :
    static G4AllITManager* Instance();
    static void DeleteInstance();
    /**
      * To delete the Instance you should use DeleteInstance()
      * rather than the destructor
      */

    ~G4AllITManager();

    template<typename T>  G4ITManager<T>* Instance();

    G4VITManager* GetInstance(G4ITType);
    G4ITBox* GetBox(const G4Track*);

    void RegisterManager(G4VITManager* manager);
    void Push(G4Track* track);

    /**
     * Set General verbose for all IT Manager
     * See ITManager builder
     */
    void SetVerboseLevel(G4int level)
    {
        fVerbose = level;
    }
    G4int GetVerboseLevel()
    {
        return fVerbose;
    }

    void UpdatePositionMap();
    void CreateTree();

    template<typename T> inline G4KDTreeResultHandle FindNearest(const G4ThreeVector& pos, const T* it);
    template<typename T> inline G4KDTreeResultHandle FindNearest(const T* it0, const T* it);
    template<typename T> inline G4KDTreeResultHandle FindNearestInRange(const G4ThreeVector& pos,
    																const T* it, G4double range);
    template<typename T> inline G4KDTreeResultHandle FindNearestInRange(const T* it0, const T* it, G4double range);

private :
    G4AllITManager();
    static G4ThreadLocal G4AllITManager* fpInstance;
    std::map<G4ITType, G4VITManager*> fITSubManager ;

    int fVerbose ;
};

template<typename T>
inline G4KDTreeResultHandle G4AllITManager::FindNearest(const G4ThreeVector& pos, const T* it)
{
    return G4ITManager<T>::Instance()->FindNearest(pos,it);
}

template<typename T>
inline G4KDTreeResultHandle G4AllITManager::FindNearest(const T* it0, const T* it)
{
    return G4ITManager<T>::Instance()->FindNearest(it0, it) ;
}

template<typename T>
inline G4KDTreeResultHandle G4AllITManager::FindNearestInRange(const G4ThreeVector& pos, const T* it, G4double range)
{
    return G4ITManager<T>::Instance()->FindNearestInRange(pos, it, range);
}

template<typename T>
inline G4KDTreeResultHandle G4AllITManager::FindNearestInRange(const T* it0, const T* it, G4double range)
{
    return G4ITManager<T>::Instance()->FindNearestInRange(it0, it, range);
}

#endif
