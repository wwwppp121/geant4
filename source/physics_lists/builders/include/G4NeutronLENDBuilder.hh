#ifndef G4NeutronLENDBuilder_h
#define G4NeutronLENDBuilder_h 1

#include "globals.hh"

#include "G4HadronElasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4VNeutronBuilder.hh"

#include "G4LENDElasticCrossSection.hh"
#include "G4LENDElastic.hh"
#include "G4LENDInelastic.hh"
#include "G4LENDInelasticCrossSection.hh"
#include "G4LENDFission.hh"
#include "G4LENDFissionCrossSection.hh"
#include "G4LENDCapture.hh"
#include "G4LENDCaptureCrossSection.hh"

class G4NeutronLENDBuilder : public G4VNeutronBuilder
{
  public: 
    G4NeutronLENDBuilder(G4String eva="");
    virtual ~G4NeutronLENDBuilder();

  public: 
    virtual void Build(G4HadronElasticProcess * aP);
    virtual void Build(G4HadronFissionProcess * aP);
    virtual void Build(G4HadronCaptureProcess * aP);
    virtual void Build(G4NeutronInelasticProcess * aP);

    void SetMinEnergy(G4double aM) 
    {
      theMin=aM;
      theIMin = theMin;
    }
    void SetMinInelasticEnergy(G4double aM) 
    {
      theIMin=aM;
    }
    void SetMaxEnergy(G4double aM) 
    {
      theIMax = aM;
      theMax=aM;
    }
    void SetMaxInelasticEnergy(G4double aM)
    {
      theIMax = aM;
    }


  private:

    G4double theMin;
    G4double theIMin;
    G4double theMax;
    G4double theIMax;

    G4LENDElastic * theLENDElastic;
    G4LENDElasticCrossSection * theLENDElasticCrossSection;
    G4LENDInelastic * theLENDInelastic;
    G4LENDInelasticCrossSection * theLENDInelasticCrossSection;
    G4LENDFission * theLENDFission;
    G4LENDFissionCrossSection * theLENDFissionCrossSection;
    G4LENDCapture * theLENDCapture;
    G4LENDCaptureCrossSection * theLENDCaptureCrossSection;

    G4String evaluation;
};

#endif

