// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4UserTrackingAction.hh,v 1.2 1999-03-24 04:45:33 tsasaki Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//
//---------------------------------------------------------------
//
// G4UserTrackingAction.hh
//
// Description:
//   This class represents actions taken place by the user at each
//   end of stepping. 
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
//---------------------------------------------------------------

class G4UserTrackingAction;

#ifndef G4UserTrackingAction_h
#define G4UserTrackingAction_h 1

class G4TrackingManager;              // Forward declaration

///////////////////////////
class G4UserTrackingAction 
///////////////////////////
{

//--------
   public:
//--------

// Constructor & Destructor
   G4UserTrackingAction(){};
   virtual ~G4UserTrackingAction(){}

// Member functions
   void SetTrackingManagerPointer(G4TrackingManager* pValue);
   virtual void PreUserTrackingAction(){}
   virtual void PostUserTrackingAction(){}

//----------- 
   protected:
//----------- 

// Member functions
   inline const G4TrackingManager* GetTrackingManager() {
      return fpTrackingManager;
   }
   inline G4TrackingManager* GetOmnipotentTrackingManager() {
      return fpTrackingManager;
   }

//--------- 
   private:
//--------- 

// Member data
   G4TrackingManager* fpTrackingManager;

};

#endif


