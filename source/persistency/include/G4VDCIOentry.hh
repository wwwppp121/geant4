// $Id: G4VDCIOentry.hh,v 1.2 2002-12-04 10:25:49 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// File: G4VDCIOentry.hh
//
// History:
//   '01.09.12  Youhei Morita  Initial creation

#ifndef VDCIO_ENTRY_T_HH
#define VDCIO_ENTRY_T_HH 1

#include "G4Types.hh"
#include <string>
#include "G4PersistencyCenter.hh"

// Class Description:
//   Abstract base class for digits collection I/O manager entry

class G4VDCIOentry
{
    public: // With description
      G4VDCIOentry(G4std::string n);
      // Constructor

      virtual ~G4VDCIOentry() {};
      // Destructor

    public: // With description
      void SetVerboseLevel(int v) { m_verbose = v; };
      // Set verbose level.

      G4std::string GetName() { return m_name; };
      // Returns the name of the DC I/O manager entry

      virtual void CreateDCIOmanager(G4std::string detName, G4std::string colName) {};
      // virtual method for creating DC I/O manager for the detector

    protected:
      int m_verbose;

    private:
      G4std::string m_name;

}; // End of class G4VDCIOentry

#endif

