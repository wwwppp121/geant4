#ifdef  G4ANALYSIS_USE

#include <stdlib.h>
#include "g4std/fstream"
#include "XrayFluoAnalysisManager.hh"
#include "G4VAnalysisSystem.hh"
#include "XrayFluoDetectorConstruction.hh"
#include "XrayFluoAnalysisMessenger.hh"
#include <IHistogramFactory.h>
#include <IHistogram1D.h>

#include <IPlotter.h>
#include <IVector.h>
#include <IVectorFactory.h>

#include "G4LizardSystem.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XrayFluoAnalysisManager::XrayFluoAnalysisManager(XrayFluoDetectorConstruction* XrayFluoDC): 
  Detector(XrayFluoDC), 
  
  histoGammaKenergy(0),
  histoIoniEnergy(0),
  histoPhotoEnergy(0),
  histoBremEnergy(0),
  histoComptEnergy(0),
  histoConvEnergy(0),
  histoRaylEnergy(0),
  histoGammaOutEnergy(0),
  histoElecKenergy(0),
  histoeIoniEnergy(0),
  histoePhotoEnergy(0),
  histoeBremEnergy(0),
  histoeComptEnergy(0),
  histoeConvEnergy(0),
  histoeRaylEnergy(0),
  histoElecOutEnergy(0),
  
  histoFactory(0), pl(0),
  histo1DDraw("disable"),histo1DSave("enable")
  
{
  // Define the messenger and the analysis system
  analysisMessenger = new XrayFluoAnalysisMessenger(this);
  analysisSystem = new G4LizardSystem;
  
  histoFactory = analysisSystem->GetHistogramFactory();  
  
  //   The following lines set the plotter and the vectorfactory that
  //   are needed in this example for a multiple histograms
  //   visualization.
  
  fVectorFactory = createIVectorFactory();          
  pl = createIPlotter();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

XrayFluoAnalysisManager::~XrayFluoAnalysisManager() {
  
  delete histoGammaKenergy;
  delete histoIoniEnergy;
  delete histoPhotoEnergy;
  delete histoBremEnergy;
  delete histoComptEnergy;
  delete histoConvEnergy;
  delete histoRaylEnergy;
  delete histoGammaOutEnergy;
  delete histoElecKenergy;
  delete histoeIoniEnergy;
  delete histoePhotoEnergy;
  delete histoeBremEnergy;
  delete histoeComptEnergy;
  delete histoeConvEnergy;
  delete histoeRaylEnergy;
  delete histoElecOutEnergy;

  delete analysisSystem;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool XrayFluoAnalysisManager::RegisterAnalysisSystem(G4VAnalysisSystem*)
{
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

IHistogramFactory* XrayFluoAnalysisManager::GetHistogramFactory(const G4String& aSystem)
{
  return histoFactory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void XrayFluoAnalysisManager::Store(IHistogram* histo, const G4String& ID)
{
  analysisSystem->Store(histo, ID);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


   void XrayFluoAnalysisManager::Plot(IHistogram* histo = 0)
{
 
  IVector* vgKe = 0;
  IVector* vIoe = 0;
  IVector* vPhe = 0;
  IVector* vBre = 0;
  IVector* vCoe = 0;
  IVector* vCve = 0;
  IVector* vRae = 0;
  IVector* vGoe = 0;
  IVector* veKe = 0;
  IVector* veIoe = 0;
  IVector* vePhe = 0;
  IVector* veBre = 0;
  IVector* veCoe = 0;
  IVector* veCve = 0;
  IVector* veRae = 0;
  IVector* vEoe = 0;

 // We fill them with the histograms

 if(histo1DDraw == "enable")
    {
     
      vgKe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoGammaKenergy)); 
      vIoe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoIoniEnergy));    
      vPhe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoPhotoEnergy));
      vBre = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoBremEnergy));

  
      vCoe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoComptEnergy));
      vCve = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoConvEnergy));
      vRae = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoRaylEnergy));
      vGoe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoGammaOutEnergy));
      veKe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoElecKenergy)); 
      veIoe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeIoniEnergy));    
      vePhe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoePhotoEnergy));
      veBre = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeBremEnergy));

  
      veCoe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeComptEnergy));
      veCve = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeConvEnergy));
      veRae = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeRaylEnergy));
      vEoe = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoElecOutEnergy));



      pl->plot(vgKe);  
      pl->plot(vIoe); 
      pl->plot(vPhe);
      pl->plot(vBre); 
      pl->plot(vCoe);
      pl->plot(vCve); 
      pl->plot(vRae); 
      pl->plot(vGoe); 
      pl->plot(veKe);  
      pl->plot(veIoe); 
      pl->plot(vePhe);
      pl->plot(veBre); 
      pl->plot(veCoe);
      pl->plot(veCve); 
      pl->plot(veRae); 
      pl->plot(vEoe); 
      pl->refresh();
    }


  delete vgKe;
  delete vIoe;
  delete vPhe;
  delete vBre;
  delete vCoe;
  delete vCve;
  delete vRae;
  delete vGoe;
  delete veKe;
  delete veIoe;
  delete vePhe;
  delete veBre;
  delete veCoe;
  delete veCve;
  delete veRae;
  delete vEoe;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


void XrayFluoAnalysisManager::InsertKEnergy(double gKe)
{
  histoGammaKenergy->fill(gKe);
}


void XrayFluoAnalysisManager::InsertIoniEnergy(double Ioe)
{
 histoIoniEnergy->fill(Ioe);
}

void XrayFluoAnalysisManager::InsertPhotoEnergy(double Phe)
{
 histoPhotoEnergy->fill(Phe);
}

void XrayFluoAnalysisManager::InsertBremEnergy(double Bre)
  {
   histoBremEnergy->fill(Bre);
 }

void XrayFluoAnalysisManager::InsertComptEnergy(double Coe)
  {
   histoComptEnergy->fill(Coe);
 }

void XrayFluoAnalysisManager::InsertConvEnergy(double Cve)
  {
   histoConvEnergy->fill(Cve);
 }

void XrayFluoAnalysisManager::InsertRaylEnergy(double Rae)
  {
  histoRaylEnergy->fill(Rae);
  }
void XrayFluoAnalysisManager::InsertOutEnergy(double Goe)
{
 histoGammaOutEnergy->fill(Goe);
}
void XrayFluoAnalysisManager::InserteKEnergy(double eKe)
{
  histoElecKenergy->fill(eKe);
}

void XrayFluoAnalysisManager::InserteIoniEnergy(double eIoe)
{
 histoeIoniEnergy->fill(eIoe);
}

void XrayFluoAnalysisManager::InsertePhotoEnergy(double ePhe)
{
 histoePhotoEnergy->fill(ePhe);
}

void XrayFluoAnalysisManager::InserteBremEnergy(double eBre)
  {
   histoeBremEnergy->fill(eBre);
 }

void XrayFluoAnalysisManager::InserteComptEnergy(double eCoe)
  {
   histoeComptEnergy->fill(eCoe);
 }

void XrayFluoAnalysisManager::InserteConvEnergy(double eCve)
  {
   histoeConvEnergy->fill(eCve);
 }

void XrayFluoAnalysisManager::InserteRaylEnergy(double eRae)
  {
  histoeRaylEnergy->fill(eRae);
  }
void XrayFluoAnalysisManager::InserteOutEnergy(double Eoe)
{
 histoElecOutEnergy->fill(Eoe);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

// This member reset the histograms and it is called at the begin
// of each run; here we put the inizialization so that the histograms have 
// always the right dimensions depending from the detector geometry

void XrayFluoAnalysisManager::BeginOfRun() 
{ 
 
  if (histoFactory) 
    {
 
      histoFactory->destroy(histoGammaKenergy);
      histoGammaKenergy = histoFactory->create1D("Energy of the  gammas at the preStepPoint when created(keV)", 300, 0,5000);
      histoFactory->destroy(histoIoniEnergy);
      histoIoniEnergy = histoFactory->create1D("Energy of the gammas created by LowEIoni(keV)", 300, 0,5000);
      histoFactory->destroy(histoPhotoEnergy);
      histoPhotoEnergy = histoFactory->create1D("Energy of the gammas created by LowEPhotoelec(keV)", 300, 0,5000);
      histoFactory->destroy(histoBremEnergy);
      histoBremEnergy = histoFactory->create1D("Energy of the gammas created by LowEBrem(keV)", 300, 0,5000);
      histoFactory->destroy(histoComptEnergy);
      histoComptEnergy = histoFactory->create1D("Energy of the gammas created by LowECompt(keV)", 300, 0,5000);
      histoFactory->destroy(histoConvEnergy);
      histoConvEnergy = histoFactory->create1D("Energy of the gammas created by LowEGammaConv(keV)", 300, 0,5000);
      histoFactory->destroy(histoRaylEnergy);
      histoRaylEnergy = histoFactory->create1D("Energy of the gammas created by LowERayleigh(keV)", 300, 0,5000);
      histoFactory->destroy(histoGammaOutEnergy);
      histoGammaOutEnergy = histoFactory->create1D("Energy of the outgoing gammas (keV)", 300, 0,5000);
       histoFactory->destroy(histoElecKenergy);
      histoElecKenergy = histoFactory->create1D("All electrons created (keV)", 500, 0,1000);
      histoFactory->destroy(histoeIoniEnergy);
      histoeIoniEnergy = histoFactory->create1D("Energy of the electrons created by LowEIoni(keV)", 500, 0,1000);
      histoFactory->destroy(histoePhotoEnergy);
      histoePhotoEnergy = histoFactory->create1D("Electrons created by LowEPhotoelec(keV)", 500, 0,1000);
      histoFactory->destroy(histoeBremEnergy);
      histoeBremEnergy = histoFactory->create1D("Electrons created by LowEBrem(keV)", 500, 0,1000);
      histoFactory->destroy(histoeComptEnergy);
      histoeComptEnergy = histoFactory->create1D("Electrons created by LowECompt(keV)", 500, 0,1000);
      histoFactory->destroy(histoeConvEnergy);
      histoeConvEnergy = histoFactory->create1D("Electrons created by LowEGammaConv(keV)", 500, 0,1000);
      histoFactory->destroy(histoeRaylEnergy);
      histoeRaylEnergy = histoFactory->create1D("Electrons created by LowERayleigh(keV)", 500, 0,1000);
      histoFactory->destroy(histoElecOutEnergy);
      histoElecOutEnergy = histoFactory->create1D(" Outgoing electrons (keV)", 500, 0,1000);
         
    
      }

  if(histoGammaKenergy)
    histoGammaKenergy->reset();
  if(histoIoniEnergy)
    histoIoniEnergy->reset();
  if(histoPhotoEnergy)
    histoPhotoEnergy->reset();
  if(histoBremEnergy)
    histoBremEnergy->reset();
  if(histoComptEnergy)
    histoComptEnergy->reset();
  if(histoConvEnergy)
    histoConvEnergy->reset();
  if(histoRaylEnergy)
    histoRaylEnergy->reset(); 
  if(histoGammaOutEnergy)
    histoGammaOutEnergy->reset();
  if(histoElecKenergy)
    histoElecKenergy->reset();
  if(histoeIoniEnergy)
    histoeIoniEnergy->reset();
  if(histoePhotoEnergy)
    histoePhotoEnergy->reset();
  if(histoeBremEnergy)
    histoeBremEnergy->reset();
  if(histoeComptEnergy)
    histoeComptEnergy->reset();
  if(histoeConvEnergy)
    histoeConvEnergy->reset();
  if(histoeRaylEnergy)
    histoeRaylEnergy->reset(); 
  if(histoElecOutEnergy)
    histoElecOutEnergy->reset();

  pl->zone(4,4);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
 
//   This member is called at the end of each run 

void XrayFluoAnalysisManager::EndOfRun(G4int n) 
{
  // This variable contains the names of the PS files
   char name[15];
  // We define some vectors
 
   IVector* vgKe   = 0;
   IVector* vIoe   = 0;
   IVector* vPhe   = 0;
   IVector* vBre   = 0;
   IVector* vCoe   = 0;
   IVector* vCve   = 0;
   IVector* vRae   = 0;
   IVector* vGoe   = 0;
   IVector* veKe   = 0;
   IVector* veIoe   = 0;
   IVector* vePhe   = 0;
   IVector* veBre   = 0;
   IVector* veCoe   = 0;
   IVector* veCve   = 0;
   IVector* veRae   = 0;
   IVector* vEoe   = 0;
  
  // Temporary we set one single zone for the plotter
  pl->zone(1,1);
  
  // We now print the histograms, each one in a separate file 

  if(histo1DSave == "enable")
    {
      vgKe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoGammaKenergy));
      vIoe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoIoniEnergy));
      vPhe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoPhotoEnergy));
      vBre   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoBremEnergy));
      vCoe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoComptEnergy));
      vCve   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoConvEnergy));
      vRae   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoRaylEnergy));
      vGoe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoGammaOutEnergy));
      veKe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoElecKenergy));
      veIoe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeIoniEnergy));
      vePhe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoePhotoEnergy));
      veBre   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeBremEnergy));
      veCoe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeComptEnergy));
      veCve   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeConvEnergy));
      veRae   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoeRaylEnergy));
      vEoe   = fVectorFactory->from1D(dynamic_cast<IHistogram1D*>(histoElecOutEnergy));

    
   sprintf(name,"gam_gen.ps", n);
      pl->plot(vgKe);
      pl->psPrint(name);

      sprintf(name,"gam_ion.ps", n);
      pl->plot(vIoe);
      pl->psPrint(name);

      sprintf(name,"gam_ipho.ps", n);
      pl->plot(vPhe);
      pl->psPrint(name);

      sprintf(name,"gam_brem.ps", n);
      pl->plot(vBre);
      pl->psPrint(name);

      sprintf(name,"gam_comp.ps", n);
      pl->plot(vCoe);
      pl->psPrint(name);

      sprintf(name,"gam_conv.ps", n);
      pl->plot(vCve);
      pl->psPrint(name);

      sprintf(name,"gam_ray.ps", n);
      pl->plot(vRae);
      pl->psPrint(name);

      sprintf(name,"gam_out.ps", n);
      pl->plot(vGoe);
      pl->psPrint(name);  

      sprintf(name,"ele_gen.ps", n);
      pl->plot(veKe);
      pl->psPrint(name);

      sprintf(name,"ele_ion.ps", n);
      pl->plot(veIoe);
      pl->psPrint(name);

      sprintf(name,"ele_ipho.ps", n);
      pl->plot(vePhe);
      pl->psPrint(name);

      sprintf(name,"ele_brem.ps", n);
      pl->plot(veBre);
      pl->psPrint(name);

      sprintf(name,"ele_comp.ps", n);
      pl->plot(veCoe);
      pl->psPrint(name);

      sprintf(name,"ele_conv.ps", n);
      pl->plot(veCve);
      pl->psPrint(name);

      sprintf(name,"ele_ray.ps", n);
      pl->plot(veRae);
      pl->psPrint(name);

      sprintf(name,"ele_out.ps", n);
      pl->plot(vEoe);
      pl->psPrint(name);
 
  }
  delete vgKe;
  delete vPhe;
  delete vIoe;
  delete vBre;
  delete vCoe;
  delete vCve;
  delete vRae;
  delete vGoe;
  delete veKe;
  delete vePhe;
  delete veIoe;
  delete veBre;
  delete veCoe;
  delete veCve;
  delete veRae;
  delete vEoe;
  
  
  }
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//  This member is called at the end of every event 
void XrayFluoAnalysisManager::EndOfEvent(G4int flag) 
{
  // The histograms are updated only if there is some
  // hits in the event
  if(flag) Plot();
}

#endif




