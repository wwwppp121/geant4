
#include "QGSPWrapper.hh"

#include "G4TheoFSGenerator.hh"

#include "G4QGSMFragmentation.hh"
#include "G4LundStringFragmentation.hh"

#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"
#include "G4PreCompoundModel.hh"

#include "G4SystemOfUnits.hh"

QGSPWrapper::QGSPWrapper( const G4String& name, G4ProcessType type )
   : ProcessWrapper(name,type)
{
}

void QGSPWrapper::Compose()
{

   G4TheoFSGenerator* gen = new G4TheoFSGenerator();

   // fInteractionModel = new G4TheoFSGenerator();

   fStringModel      = new G4QGSModel< G4QGSParticipants >; // specific
   
   fCascade          = new G4GeneratorPrecompoundInterface(); // common
   
   fCascade->SetDeExcitation( new G4PreCompoundModel( new G4ExcitationHandler() ) ); // specific

   // There're these 2 options for modeling string fragmentation.
   // The Lund one is "traditionally" used with FTF, and 
   // the QGSM one (older code) is typically used with QGS.
   // However, replacing the QGSM one with Lund seems to give 
   // BETTER performance at higher energy (158GeV).
   // At 31GeV it doesn't really matter, or maybe the QGSM
   // is even a little better.
   //
   if ( fUseLundStrFragm )
   {
      fStringDecay      = new G4ExcitedStringDecay(new G4LundStringFragmentation());
   }
   else
   {
      fStringDecay      = new G4ExcitedStringDecay( new G4QGSMFragmentation() ); // specific
   }
   fStringModel->SetFragmentationModel( fStringDecay ); // common interface but different input

   gen->SetQuasiElasticChannel( new G4QuasiElasticChannel() ); // specific
   gen->SetTransport( fCascade );
   gen->SetHighEnergyGenerator( fStringModel );
   
   fInteractionModel = gen;
   
   fInteractionModel->SetMinEnergy(GeV);  // common
   fInteractionModel->SetMaxEnergy(100.*TeV);   
   
   return; 

}