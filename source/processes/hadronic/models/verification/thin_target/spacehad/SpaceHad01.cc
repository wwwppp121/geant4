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
/////////////////////////////////////////////////////////////////
///
///     GEANT 4 class file --- Copyright CERN 1998
///      CERN Geneva Switzerland
///
///
///      File name:     SpaceHad01
///
///      Authors: A. Blondel, S.Gilardoni, V. Grichine        
///    (based on Test30 developed by V.Ivanchenko) 
/// 
///      Creation date: 10 March 2003
///
///      Modifications: 
///
/////////////////////////////////////////////////////////////////////-

#include "globals.hh"
#include "G4ios.hh"
#include <fstream>
#include <strstream>
#include <iomanip>
#include <string.h>

#include "G4Material.hh"
#include "G4ElementVector.hh"
#include "Sh01Material.hh"
#include "Sh01Physics.hh"
#include "G4VContinuousDiscreteProcess.hh"
#include "G4ProcessManager.hh"
#include "G4VParticleChange.hh"
#include "G4ParticleChange.hh"
#include "G4HadronCrossSections.hh"
#include "G4VCrossSectionDataSet.hh"
#include "G4ProtonInelasticCrossSection.hh"
#include "G4NeutronInelasticCrossSection.hh"
#include "G4HadronInelasticDataSet.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleChange.hh"
#include "G4DynamicParticle.hh"
#include "G4AntiProton.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4PionZero.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4Alpha.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4ForceCondition.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4Step.hh"
#include "G4GRSVolume.hh"

#include "G4UnitsTable.hh"

#include "../../hadronic_data_set/example/include/G4HadFileSpec.hh"
#include "../../hadronic_data_set/example/include/G4HadFileFinder.hh"
#include "../../hadronic_data_set/example/include/G4ReadHadDoubleDiffXSC.hh"



//////////////////////////////////////////////////////////////////
///
/// If G4ANALYSIS_USE=1, one can use histograms or ntuples for analysis  
///

#ifdef G4ANALYSIS_USE

// New Histogramming (from AIDA and Anaphe):
#include <memory> // for the auto_ptr(T>

#include "AIDA/AIDA.h"

/*
#include "AIDA/IAnalysisFactory.h"

#include "AIDA/ITreeFactory.h"
#include "AIDA/ITree.h"

#include "AIDA/IHistogramFactory.h"
#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"

#include "AIDA/ITupleFactory.h"
#include "AIDA/ITuple.h"
*/

#endif

#include "G4Timer.hh"

int main(int argc, char** argv)
{
  //  HepTupleManager* hbookManager = 0;

  ////////////////////////////////////////////
  ///
  /// Setup

  G4String  namePart = "proton";
  G4String  nameMat  = "Si";
  G4String  nameGen  = "stringCHIPS";
  G4bool    logx     = false;
  G4bool    usePaw   = false;

  ///////////////////////////////////////
  ///
  /// Boolean variable for writing data to arrays, followed by 
  /// sending them to files

  G4bool    useAscii  = true;
  G4bool    manyFiles  = true;

  G4bool    inclusive= true;
  G4int     verbose  = 0;
  G4double  energy   = 100.*MeV;
  G4double  sigmae   = 0.0;
  G4double  elim     = 30.*MeV;
  G4double  dangl    = 5.0;

  G4int     nevt     = 1000;
  G4int     nbins    = 100;
  G4int     nbinsa   = 40;
  G4int     nbinse   = 80;
  G4int     nbinsd   = 20;
  G4int     nbinspi  = 20;
  G4int     nangl    = 0;
  G4int     nanglpi  = 0;

  G4String hFile     = "";
  G4double theStep   = 0.01*micrometer;
  G4double range     = 1.0*micrometer;
  G4double  emax     = 160.*MeV;
  G4double  emaxpi   = 200.*MeV;
  G4double ebinlog   = 2.0*MeV;
  G4Material* material = 0;

  G4double ang[15] = {0.0};
  G4double bng1[15] = {0.0};
  G4double bng2[15] = {0.0};
  G4double cng[15] = {0.0};
  G4double angpi[10] = {0.0};
  G4double bngpi1[10] = {0.0};
  G4double bngpi2[10] = {0.0};
  G4double cngpi[10] = {0.0};

  float bestZ[250] = {
    0.0, 1.0, 1.0, 2.0, 2.0, 0.0, 0.0, 4.0, 0.0, 0.0,   //0 
    4.0, 0.0, 0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 9.0, 0.0,   //10 
    10.0, 10.0, 11.0, 0.0, 11.0, 0.0, 13.0, 0.0, 0.0, 0.0,   //20 
    0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //30 
    0.0, 0.0, 0.0, 0.0, 21.0, 0.0, 21.0, 21.0, 23.0, 0.0,   //40 
    0.0, 24.0, 25.0, 0.0, 25.0, 0.0, 27.0, 27.0, 27.0, 26.0,   //50 
    27.0, 0.0, 0.0, 0.0, 0.0, 30.0, 31.0, 31.0, 32.0, 32.0,   //60 
    33.0, 33.0, 33.0, 34.0, 33.0, 34.0, 35.0, 35.0, 0.0, 36.0,   //70 
    36.0, 37.0, 36.0, 37.0, 37.0, 38.0, 39.0, 39.0, 41.0, 40.0,   //80 
    41.0, 39.0, 41.0, 38.0, 39.0, 40.0, 40.0, 39.0, 40.0, 0.0,   //90 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //100 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //110 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //120 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //130 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //140 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //150 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   //160
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,          //170 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,          //180 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,          //190 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,          //200 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,          //210 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,          //220 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,          //230 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };        //240 
  


  // Track

  G4ThreeVector aPosition = G4ThreeVector(0.,0.,0.);
  G4double      aTime     = 0. ;
  G4ThreeVector aDirection      = G4ThreeVector(0.0,0.0,1.0);
  G4double nx = 0.0, ny = 0.0, nz = 0.0;
 


  ///////////////////////////////////////////////////////////////////
  ///
  /// Materials definition 

  Sh01Material*  mate = new Sh01Material();
  Sh01Physics*   phys = new Sh01Physics();
	
  /// Geometry 

  G4double dimX = 100.0*cm;
  G4double dimY = 100.0*cm;
  G4double dimZ = 100.0*cm;  

  G4Box* sFrame = new G4Box ("Box",dimX, dimY, dimZ);
  G4LogicalVolume* lFrame = new G4LogicalVolume(sFrame,material,"Box",0,0,0);
  G4PVPlacement* pFrame = new G4PVPlacement(0,G4ThreeVector(),"Box",
                                            lFrame,0,false,0);

  assert(pFrame);


  G4cout.setf( std::ios::scientific, std::ios::floatfield );
  std::ifstream* fin = new std::ifstream();
  G4String fname;
  G4bool fG4HDS = false;
 
  ///////////////////////////////////////////////////////////////////
  ///
  /// Control on input

  if( argc < 2 ) // no input file after SpaceHad01: G4HDS case
  {
    fG4HDS = true;
    fname = "g4hds.mac";
    fin->open(fname.c_str());
  
    if( !fin->is_open()) 
    {
      G4cout << "Input file <" << fname << "> does not exist! Exit" << G4endl;
      exit(1);
    }
    // Reading from G4HDS for a process
  }
  else  // shibata and other data case
  {
    fname = argv[1];
    fin->open(fname.c_str());
  
    if( !fin->is_open()) 
    {
      G4cout << "Input file <" << fname << "> does not exist! Exit" << G4endl;
      exit(1);
    }
  }
  ///////////////////////////////////////////////////////////////
  ///
  /// Read input file
  ///

  /*
  G4cout << "Available commands are: " << G4endl;
  G4cout << "#events" << G4endl;
  G4cout << "#exclusive" << G4endl;
  G4cout << "#inclusive" << G4endl;
  G4cout << "#logx" << G4endl;
  G4cout << "#nbins" << G4endl;
  G4cout << "#nbinsa" << G4endl;
  G4cout << "#nbinse" << G4endl;
  G4cout << "#nbinsd" << G4endl;
  G4cout << "#nbinspi" << G4endl;
  G4cout << "#nanglepi" << G4endl;
  G4cout << "#anglespi" << G4endl;
  G4cout << "#nangle" << G4endl;
  G4cout << "#angles" << G4endl;
  G4cout << "#dangle" << G4endl;
  G4cout << "#particle" << G4endl;
  G4cout << "#energy(MeV)" << G4endl;
  G4cout << "#sigmae(MeV)" << G4endl;
  G4cout << "#emax(MeV)" << G4endl;
  G4cout << "#emaxpi(MeV)" << G4endl;
  G4cout << "#elim(MeV)" << G4endl;
  G4cout << "#ebinlog(MeV)" << G4endl;
  G4cout << "#range(mm)" << G4endl;
  G4cout << "#step(mm)" << G4endl;
  G4cout << "#material" << G4endl;
  G4cout << "#generator" << G4endl;
  G4cout << "#paw" << G4endl;
  G4cout << "#verbose" << G4endl;
  G4cout << "#position(mm)" << G4endl;
  G4cout << "#direction" << G4endl;
  G4cout << "#time(ns)" << G4endl;
  G4cout << "#run" << G4endl;
  G4cout << "#exit" << G4endl;
  */


  G4String line, line1;
  G4bool end = true;
  

    do 
    {
      (*fin) >> line;
      G4cout << "Next line " << line << G4endl;

      if(line == "#particle") 
      {
        (*fin) >> namePart;
      } 
      else if(line == "#energy(MeV)") 
      {
        (*fin) >> energy;
        energy *= MeV;
        emax    = energy;
      } 
      else if(line == "#sigmae(MeV)") 
      {
        (*fin) >> sigmae;
        sigmae *= MeV;
      } 
      else if(line == "#emax(MeV)") 
      {
        (*fin) >> emax;
        emax *= MeV;
      } 
      else if(line == "#emaxpi(MeV)") 
      {
        (*fin) >> emaxpi;
        emaxpi *= MeV;
      } 
      else if(line == "#elim(MeV)") 
      {
        (*fin) >> elim;
        elim *= MeV;
      } 
      else if(line == "#ebinlog(MeV)") 
      {
        (*fin) >> ebinlog;
	if (ebinlog < 1.1) ebinlog = 1.1;
        ebinlog *= MeV;
      }
      else if(line == "#events")      (*fin) >> nevt; 
      else if(line == "#exclusive")   inclusive = false;  
      else if(line == "#inclusive")   inclusive = true;     
      else if(line == "#nbins")       (*fin) >> nbins;
      else if(line == "#nbinse")      (*fin) >> nbinse;
      else if(line == "#nbinsa")      (*fin) >> nbinsa;
      else if(line == "#nbinsd")      (*fin) >> nbinsd;
      else if(line == "#nbinspi")     (*fin) >> nbinspi;
      else if(line == "#nangle")      (*fin) >> nangl;
      else if(line == "#nanglepi")    (*fin) >> nanglpi;
      else if(line == "#anglespi") 
      {
        for(int k=0; k<nanglpi; k++)  (*fin) >> angpi[k];
      } 
      else if(line == "#dangle")      (*fin) >> dangl;
      else if(line == "#angles") 
      {
        for(int k=0; k<nangl; k++)    (*fin) >> ang[k];
      } 
      else if(line == "#range(mm)") 
      {
        (*fin) >> range;
        range *= mm;
      } 
      else if(line == "#step(mm)") 
      {
        (*fin) >> theStep;
        theStep *= mm;
      } 
      else if(line == "#material")    (*fin) >> nameMat;
      else if(line == "#particle")    (*fin) >> namePart;
      else if(line == "#generator")   (*fin) >> nameGen;
      else if(line == "#paw")
      { 
        usePaw = true;
        (*fin) >> hFile;
      } 
      else if(line == "#run")         break;
      else if(line == "#verbose")     (*fin) >> verbose;
      else if(line == "#position(mm)") 
      {
        (*fin) >> nx >> ny >> nz;
        aPosition = G4ThreeVector(nx*mm, ny*mm, nz*mm);
      } 
      else if(line == "#direction") 
      {
        (*fin) >> nx >> ny >> nz;

        if(nx*nx + ny*ny + nz*nz > 0.0) 
        {
          aDirection = G4ThreeVector(nx, ny, nz);
          aDirection = aDirection.unit();
	}
      } 
      else if(line == "#time(ns)") 
      {
        (*fin) >> aTime;
        aTime *= ns;
      } 
      else if(line == "#logx")        logx = true;
      else if(line == "#exit") 
      {
        end = false;
        break;
      }
    } while(end);

    if( !end ) 
    {
      G4cout<<"Exit due to wrong end of macro file"<<G4endl;
      exit(1);
    }


    //
    // Start run and initialisation of material, physics etc ...
    //

    G4cout << "###### Start new run # " << G4endl; 
 
    material = mate->GetMaterial(nameMat);
    
    if( !material ) 
    {
      G4cout << "Material <" << nameMat  
	     << "> is not found out" 
	     << G4endl;			
      exit(1);
    }
    G4ParticleDefinition* part = (G4ParticleTable::GetParticleTable())->
                                  FindParticle(namePart);

    G4VProcess* proc = phys->GetProcess(nameGen, namePart, material);
    G4double amass = phys->GetNucleusMass();
				
    const G4ParticleDefinition* proton = G4Proton::Proton();
    const G4ParticleDefinition* neutron = G4Neutron::Neutron();
    const G4ParticleDefinition* pin = G4PionMinus::PionMinus();
    const G4ParticleDefinition* pip = G4PionPlus::PionPlus();
    const G4ParticleDefinition* pi0 = G4PionZero::PionZero();
    const G4ParticleDefinition* deu = G4Deuteron::DeuteronDefinition();
    const G4ParticleDefinition* tri = G4Triton::TritonDefinition();	
    const G4ParticleDefinition* alp = G4Alpha::AlphaDefinition();


    if(fG4HDS)
    {

      char* pathstart = getenv("G4HADATASET");
      G4String temp;
      G4String pathstarttwo(pathstart);      
      G4String process2="dd";
      G4String pName(proton->GetParticleName());     
      G4String nName(neutron->GetParticleName());     
       
      G4HadFileSpec fileIwant( pName,
                               material,
                               nName,
                               process2 );
       
       //G4cout << fileIwant.G4HDSFilename()<< G4endl;
       //G4cout << fileIwant.G4HDSFilepath()<< G4endl;
       
       temp = fileIwant.G4HDSFilename();
       
       // G4HadFileFinder* findit=new G4HadFileFinder(pathstart,temp); 
       
       G4ReadHadDoubleDiffXSC* readExforAl = new G4ReadHadDoubleDiffXSC(fileIwant);    

       std::vector<G4PhysicsTable*>* doubleDiffXscBank = readExforAl->GetDoubleDiffXscBank();
       std::vector<G4DataVector*>* angleDdTable = readExforAl->GetAngleDdTable(); 
       G4DataVector*  TkinVector       = readExforAl->GetTkinVector();
       G4DataVector*  angleVector      = readExforAl->GetAngleVector();
       G4DataVector   energyUnitVector = readExforAl->GetEnergyUnitVector();
       G4DataVector   angleUnitVector  = readExforAl->GetAngleUnitVector();
       G4DataVector   ddXscUnitVector  = readExforAl->GetDdXscUnitVector();

       G4cout<<G4endl<<"External Output of G4HDS DoubleDiffXscBank data"<<G4endl;
       G4cout<<"DoubleDiffXscBank->size() = "<<doubleDiffXscBank->size()<<G4endl<<G4endl;
       size_t jAngle=0; 

       for(size_t i = 0; i < doubleDiffXscBank->size(); ++i)
       {
         G4cout<<G4endl<<"(*doubleDiffXscBank)["<<i<<"]->size() = "
               <<(*doubleDiffXscBank)[i]->size()<<G4endl;

         for(  size_t j = 0; j < (*doubleDiffXscBank)[i]->size(); ++j )
         {
            G4cout<<G4endl<<"(*(*doubleDiffXscBank)["<<i<<"])("<<j<<")->GetVectorLength() = "
                  <<(*(*doubleDiffXscBank)[i])(j)->GetVectorLength()<<G4endl;
            G4cout<<G4endl<<"Tkin"<<"\t"<<"angle"<<"\t"
                  <<"omega"<<"\t"<<"ddXsc"<<G4endl<<G4endl;

            for(size_t k = 0; k < (*(*doubleDiffXscBank)[i])(j)->GetVectorLength(); ++k)
            {
              G4cout<<(*TkinVector)[i]/energyUnitVector[i]<<"\t"

	  //  <<(*angleVector)[jAngle]/angleUnitVector[i]<<"\t"
              <<(*(*angleDdTable)[i])[j]/angleUnitVector[i]<<"\t"

              <<(*(*doubleDiffXscBank)[i])(j)->GetLowEdgeEnergy(k)/energyUnitVector[i]
              <<"\t"
	      <<(*(*(*doubleDiffXscBank)[i])(j))(k)/ddXscUnitVector[i]<<G4endl;
          }
          jAngle++;
        } 
      }

    }

		
    if(!proc) 
    {
      G4cout << "For particle: " << part->GetParticleName() 
	     << " generator " << nameGen << " is unavailable"
	     << G4endl;
      exit(1);
    }

    G4int maxn = (G4int)((*(material->GetElementVector()))[0]->GetN()) + 1;

    // G4int maxz = (G4int)((*(material->GetElementVector()))[0]->GetZ()) + 1; 
		
    G4cout << "The particle:  " << part->GetParticleName() << G4endl;
    G4cout << "The material:  " << material->GetName() << "  Amax= " 
           << maxn << G4endl;
    G4cout << "The step:      " << theStep/mm << " mm" << G4endl;
    G4cout << "The position:  " << aPosition/mm << " mm" << G4endl;
    G4cout << "The direction: " << aDirection << G4endl;
    G4cout << "The time:      " << aTime/ns << " ns" << G4endl;

    //  const G4int nhisto = 56; 
    G4double mass = part->GetPDGMass();
    G4double pmax = sqrt(energy*(energy + 2.0*mass));
    // SG changed by me
    // G4double binlog = log10(ebinlog);
    // G4int nbinlog = (G4int)(log10(2.0*emax)/binlog);
    // G4double logmax = binlog*nbinlog;
    // G4double bine = emax/(G4double)nbinse;
    // G4double bind = emax/(G4double)nbinsd;

    //    G4cout << "nbinlog     " << nbinlog << G4endl;

#ifdef G4ANALYSIS_USE


    // Creating the analysis factory
std::auto_ptr< AIDA::IAnalysisFactory > af( AIDA_createAnalysisFactory() );

    // Creating the tree factory
std::auto_ptr< AIDA::ITreeFactory > tf( af->createTreeFactory() );

    // Creating a tree mapped to a new hbook file.
std::auto_ptr< AIDA::ITree > tree( tf->create( hFile, "hbook", false, true) );
std::cout << "Tree store : " << tree->storeName() << std::endl;
 
// Creating a tuple factory, whose tuples will be handled by the tree
//   G4std::auto_ptr< AIDA::ITupleFactory > 
//  tpf( af->createTupleFactory( *tree ) );

    AIDA::IHistogram1D* h[nhisto];
    
     //    AIDA::IHistogram2D* h2;
    //AIDA::ITuple* ntuple1 = 0;
		
    if(usePaw) 
    {

 // Creating a histogram factory, whose histograms will be handled by the tree
        G4std::auto_ptr< AIDA::IHistogramFactory > 
               hf( af->createHistogramFactory( *tree ) );

      // Creating an 1-dimensional histogram in the root directory of the tree
  
      // ---- Book a histogram and ntuples
      
      G4cout << "Hbook file name: <" << hFile << ">" << G4endl;      
      G4cout << "energy = " << energy/MeV << " MeV" << G4endl;
      G4cout << "emax   = " << emax/MeV << " MeV" << G4endl;
      G4cout << "pmax   = " << pmax/MeV << " MeV" << G4endl;

	/// G4cout << "I amhere 1" << G4endl;

      h[0]=hf->createHistogram1D("1","Number of Secondaries",50,-0.5,49.5);
      h[1]=hf->createHistogram1D("2","Type of secondary",10,-0.5,9.5);
      h[2]=hf->createHistogram1D("3","Phi(degrees) of Secondaries",
                        90,-180.0,180.0);
      h[3]=hf->createHistogram1D("4","Pz (MeV) for protons",100,-pmax,pmax);
      h[4]=hf->createHistogram1D("5","Pz (MeV) for pi-",100,-pmax,pmax);
      h[5]=hf->createHistogram1D("6","Pz (MeV) for pi+",100,-pmax,pmax);
      h[6]=hf->createHistogram1D("7","Pz (MeV) for neutrons",100,-pmax,pmax);
      h[7]=hf->createHistogram1D("8","Pt (MeV) for protons",100,0.,pmax);
      h[8]=hf->createHistogram1D("9","Pt (MeV) for pi-",100,0.,pmax);
      h[9]=hf->createHistogram1D("10","Pt (MeV) for pi+",100,0.,pmax);
      h[10]=hf->createHistogram1D("11","Pt (MeV) for neutrons",100,0.,pmax);
      h[11]=hf->createHistogram1D("12","E (MeV) for protons",100,0.,energy);
      h[12]=hf->createHistogram1D("13","E (MeV) for pi-",100,0.,energy);
      h[13]=hf->createHistogram1D("14","E (MeV) for pi+",100,0.,energy);
      h[14]=hf->createHistogram1D("15","E (MeV) for neutrons",100,0.,energy);
      h[15]=hf->createHistogram1D("16","delta E (MeV)",20,-1.,1.);
      h[16]=hf->createHistogram1D("17","delta Pz (GeV)",20,-1.,1.);
      h[17]=hf->createHistogram1D("18","delta Pt (GeV)",20,-1.,1.);
      
      h[18]=hf->createHistogram1D("19","E (MeV) for pi0",100,0.,energy);
      h[19]=hf->createHistogram1D("20","Pz (MeV) for pi0",100,-pmax,pmax);
      h[20]=hf->createHistogram1D("21","Pt (MeV) for pi0",100,0.,pmax);
      
      h[21]=hf->createHistogram1D("22","E(MeV) protons",nbinse,0.,emax);
      h[22]=hf->createHistogram1D("23","E(MeV) neutrons",nbinse,0.,emax);

      h[23]=hf->createHistogram1D("24","Phi(degrees) of neutrons",
                           90,-180.0,180.0);

      h[24]=hf->createHistogram1D("25","cos(theta) protons",nbinsa,-1.,1.);
      h[25]=hf->createHistogram1D("26","cos(theta) neutrons",nbinsa,-1.,1.);

      h[26]=hf->createHistogram1D("27","Baryon number (mbn)",
                  maxn,-0.5,(G4double)maxn + 0.5);

      if(nangl>0)
       h[27]=hf->createHistogram1D("28","ds/dE for neutrons at theta = 0",
                     nbinsd,0.,emax);
      if(nangl>1)
       h[28]=hf->createHistogram1D("29","ds/dE for neutrons at theta = 1",
                                        nbinsd,0.,emax);
      if(nangl>2)
       h[29]=hf->createHistogram1D("30","ds/dE for neutrons at theta = 2",
                       nbinsd,0.,emax);
      if(nangl>3)
       h[30]=hf->createHistogram1D("31","ds/dE for neutrons at theta = 3",
                          nbinsd,0.,emax);
      if(nangl>4)
       h[31]=hf->createHistogram1D("32","ds/dE for neutrons at theta = 4",
                        nbinsd,0.,emax);
      if(nangl>5)
       h[32]=hf->createHistogram1D("33","ds/dE for neutrons at theta = 5",
                          nbinsd,0.,emax);
      if(nangl>6)
       h[33]=hf->createHistogram1D("34","ds/dE for neutrons at theta = 6",
                       nbinsd,0.,emax);
      if(nangl>7)
       h[34]=hf->createHistogram1D("35","ds/dE for neutrons at theta = 7",
                    nbinsd,0.,emax);
      if(nangl>8)
       h[35]=hf->createHistogram1D("36","ds/dE for neutrons at theta = 8",
                                     nbinsd,0.,emax);
      if(nangl>9)
       h[36]=hf->createHistogram1D("37","ds/dE for neutrons at theta = 9",
                              nbinsd,0.,emax);
      if(nangl>10)
       h[37]=hf->createHistogram1D("38","ds/dE for neutrons at theta = 10",
                                nbinsd,0.,emax);
      if(nangl>11)
       h[38]=hf->createHistogram1D("39","ds/dE for neutrons at theta = 11",
                                     nbinsd,0.,emax);
      if(nangl>12)
       h[39]=hf->createHistogram1D("40","ds/dE for neutrons at theta = 12",
                         nbinsd,0.,emax);

      if(nanglpi>0)
       h[40]=hf->createHistogram1D("41","ds/dE for pi- at theta = 0",
                          nbinspi,0.,emaxpi);
      if(nanglpi>1)
       h[41]=hf->createHistogram1D("42","ds/dE for pi- at theta = 1",
                       nbinspi,0.,emaxpi);
      if(nanglpi>2)
       h[42]=hf->createHistogram1D("43","ds/dE for pi- at theta = 2",
                          nbinspi,0.,emaxpi);
      if(nanglpi>3)
       h[43]=hf->createHistogram1D("44","ds/dE for pi- at theta = 3",
                        nbinspi,0.,emaxpi);
      if(nanglpi>4)
       h[44]=hf->createHistogram1D("45","ds/dE for pi- at theta = 4",
                     nbinspi,0.,emaxpi);
      if(nanglpi>0)
       h[45]=hf->createHistogram1D("46","ds/dE for pi+ at theta = 0",
                             nbinspi,0.,emaxpi);
      if(nanglpi>1)
       h[46]=hf->createHistogram1D("47","ds/dE for pi+ at theta = 1",
                       nbinspi,0.,emaxpi);
      if(nanglpi>2)
       h[47]=hf->createHistogram1D("48","ds/dE for pi+ at theta = 2",
                      nbinspi,0.,emaxpi);
      if(nanglpi>3)
       h[48]=hf->createHistogram1D("49","ds/dE for pi+ at theta = 3",
                          nbinspi,0.,emaxpi);
      // SG changed by me 
      //     if(nanglpi>4)
      //h[49]=hf->createHistogram1D("50","ds/dE for pi+ at theta = 4",
      //               nbinspi,0.,emaxpi);
      //
      //h[50]=hf->createHistogram1D("51","E(MeV) neutrons",nbinlog,0.,logmax);
      //if(nangl>0)
      //  h[51]=hf->createHistogram1D("52","ds/dE for neutrons at theta = 0",
      //          nbinlog,0.,logmax);
      //if(nangl>1)
      //  h[52]=hf->createHistogram1D("53","ds/dE for neutrons at theta = 1",
      //               nbinlog,0.,logmax);
      //if(nangl>2)
      //  h[53]=hf->createHistogram1D("54","ds/dE for neutrons at theta = 2",
      //              nbinlog,0.,logmax);
      //if(nangl>3)
      //  h[54]=hf->createHistogram1D("55","ds/dE for neutrons at theta = 3",
      //              nbinlog,0.,logmax);
      //if(nangl>4)
      //  h[55]=hf->createHistogram1D("56","ds/dE for neutrons at theta = 4",
                      nbinlog,0.,logmax);

      G4cout << "Histograms is initialised nbins=" << nbins
             << G4endl;
    }		

#endif

    //    if( useAscii )
    // {
      /////////////////////////////////////////////////////////
      ///
      /// Similar to historgrams h[0]->h0 etc ...
      /// The hadronic data are sent to arrays followed by file filling
      G4int i;
	/// G4cout << "I amhere 2" << G4endl;
      G4double h0[50],  h1[50],  h2[50],  h3[90],  h4[100],
	       h5[100], h6[100], h7[100], h8[100], h9[100];
      for( i = 0 ; i < 100 ; i++ )
      {
	if( i<50 ) h0[i]=h1[i]=h2[i]=0.;
	if( i<90 ) h3[i]=0.;
        h4[i]=h5[i]=h6[i]=h7[i]=h8[i]=h9[i]=0.;
      }
      G4double h10[100], h11[100], h12[100], h13[100], h14[100],
	       h15[20],  h16[20],  h17[20],  h18[100],  h19[100];
      for( i = 0 ; i < 100 ; i++ )
      {
	if( i<20 ) h15[i]=h16[i]=h17[i]=0.;
        h10[i]=h11[i]=h12[i]=h13[i]=h14[i]=h18[i]=h19[i]=0.;
      }
      const G4int nbinseC=nbinse;
      G4double h20[100]; 
      std::vector<G4double> h21(nbinseC), h22(nbinseC), h23(90), h24(nbinsa),
	                    h25(nbinsa), h26(maxn), h27(nbinsd), h28(nbinsd), h29(nbinsd);
      for( i = 0 ; i < 100 ; i++ )
      {
	if( i<90 ) h23[i]=0.;
        h20[i]=0.;
      }
      for( i = 0 ; i < nbinse ; i++ ) h21[i]=h22[i]=0.;
      for( i = 0 ; i < nbinsa ; i++ ) h24[i]=h25[i]=0.;
      for( i = 0 ; i < maxn   ; i++ ) h26[i]=0.;
      for( i = 0 ; i < nbinsd ; i++ ) h27[i]=h28[i]=h29[i]=0.;

      std::vector<G4double> h30(nbinsd), h31(nbinsd), h32(nbinsd), h33(nbinsd), h34(nbinsd),
	       h35(nbinsd), h36(nbinsd), h37(nbinsd), h38(nbinsd), h39(nbinsd);
      for( i = 0 ; i < nbinsd ; i++ )
      {
	h30[i]=h31[i]=h32[i]=h33[i]=h34[i]=0.;
        h35[i]=h36[i]=h37[i]=h38[i]=h39[i]=0.;
      }
      std::vector<G4double> h40(nbinspi), h41(nbinspi), h42(nbinspi), h43(nbinspi), h44(nbinspi),
	        h45(nbinspi), h46(nbinspi), h47(nbinspi), h48(nbinspi), h49(nbinspi);
      for( i = 0 ; i < nbinspi ; i++ )
      {
	h40[i]=h41[i]=h42[i]=h43[i]=h44[i]=0.;
        h45[i]=h46[i]=h47[i]=h48[i]=h49[i]=0.;
      }

      //// SG changed by my
      ///G4double h50[nbinlog],h51[nbinlog],h52[nbinlog],h53[nbinlog],h54[nbinlog],
      //       h55[nbinlog];
      //for( i = 0 ; i < nbinlog ; i++ )
      //{
      //h50[i]=h51[i]=h52[i]=h53[i]=h54[i]=h55[i]=0.;
      //}
      //    }
      
	/// G4cout << "I amhere 3" << G4endl;

    ////////////////////////////////////////////////////////////////////
    ///
    /// Create a DynamicParticle  
    ///

    G4DynamicParticle dParticle(part,aDirection,energy);
    G4VCrossSectionDataSet* cs = 0;
    G4double cross_sec = 0.0;

    if(part == proton && material->GetElement(0)->GetZ() > 1.5) 
    {
      cs = new G4ProtonInelasticCrossSection();
    } 
    else if(part == neutron && material->GetElement(0)->GetZ() > 1.5) 
    {
      cs = new G4NeutronInelasticCrossSection();
    } 
    else 
    {
      cs = new G4HadronInelasticDataSet();
    }
    if(cs) 
    {
      cs->BuildPhysicsTable(*part);
      cross_sec = cs->GetCrossSection(&dParticle, material->GetElement(0));
    } 
    else 
    {
      cross_sec = (G4HadronCrossSections::Instance())->
        GetInelasticCrossSection(&dParticle, material->GetElement(0));
    }
    G4double factor = cross_sec*MeV*1000.0*(G4double)nbinse/
                       (energy*barn*(G4double)nevt);
    G4double factora= cross_sec*MeV*1000.0*(G4double)nbinsa/
                     (twopi*2.0*barn*(G4double)nevt);
    G4double factorb= cross_sec*1000.0/(barn*(G4double)nevt);

    G4cout << "### factor  = " << factor
           << "### factora = " << factor 
           << "    cross(b)= " << cross_sec/barn << G4endl;
  
	/// G4cout << "I amhere 4" << G4endl;

    if(nangl > 0) 
    {
      for(G4int k=0; k<nangl; k++) 
      {
        if(nangl == 1) 
        {
          bng1[0] = std::max(0.0,ang[0] - dangl);
          bng2[0] = std::min(180., ang[0] + dangl);
        } 
        else if(k == 0) 
        {
          bng1[0] = std::max(0.0,ang[0] - dangl);
          bng2[0] = std::min(0.5*(ang[0] + ang[1]), ang[0] + dangl);
        } 
        else if(k < nangl-1) 
        {
          bng1[k] = std::max(bng2[k-1], ang[k]-dangl);
          bng2[k] = std::min(0.5*(ang[k] + ang[k+1]), ang[k] + dangl);
        } 
        else 
        {
          bng1[k] = std::max(bng2[k-1], ang[k]-dangl);
          bng2[k] = std::min(180., ang[k] + dangl);
        }
        cng[k] = cross_sec*MeV*1000.0*(G4double)nbinsd/
                (twopi*(cos(degree*bng1[k]) - cos(degree*bng2[k]))*
                       barn*emax*(G4double)nevt);
      }
    }
    if(nanglpi > 0) 
    {
      for(G4int k=0; k<nanglpi; k++) 
      {  
        if(nangl == 1) 
        {
          bngpi1[0] = std::max(0.0,angpi[0] - dangl);
          bngpi2[0] = std::min(180., angpi[0] + dangl);
        } 
        else if(k == 0) 
        {
          bngpi1[0] = std::max(0.0,angpi[0] - dangl);
          bngpi2[0] = std::min(0.5*(angpi[0] + angpi[1]), angpi[0] + dangl);
        } 
        else if(k < nanglpi-1) 
        {
          bngpi1[k] = std::max(bngpi2[k-1], angpi[k]-dangl);
          bngpi2[k] = std::min(0.5*(angpi[k] + angpi[k+1]), angpi[k] + dangl);
        } 
        else 
        {
          bngpi1[k] = std::max(bngpi2[k-1], angpi[k]-dangl);
          bngpi2[k] = std::min(180., angpi[k] + dangl);
        }
        cngpi[k] = cross_sec*MeV*1000.0*(G4double)nbinspi/
         (twopi*(cos(degree*bngpi1[k]) - cos(degree*bngpi2[k]))*
                 barn*emax*(G4double)nevt);
      }
    }
    ///////////////////////////////////////////////////////////////
    ///
    /// G4Track object for created dynamic particle, time, and position

    G4Track* gTrack = new G4Track(&dParticle,aTime,aPosition);
 
    // Step 

    G4Step* step = new G4Step();  
    step->SetTrack(gTrack);

	/// G4cout << "I amhere 5" << G4endl;

    G4StepPoint *aPoint, *bPoint;
    aPoint = new G4StepPoint();
    aPoint->SetPosition(aPosition);
    aPoint->SetMaterial(material);
    G4double safety = 10000.*cm;
    aPoint->SetSafety(safety);
    step->SetPreStepPoint(aPoint);

    bPoint = aPoint;
    G4ThreeVector bPosition = aDirection*theStep;
    bPosition += aPosition;
    bPoint->SetPosition(bPosition);
    step->SetPostStepPoint(bPoint);
    step->SetStepLength(theStep);

    G4RotationMatrix* rot  = new G4RotationMatrix();
    G4double phi0 = aDirection.phi();
    G4double theta0 = aDirection.theta();
    rot->rotateZ(-phi0);
    rot->rotateY(-theta0);

    G4cout << "Test rotation= " << (*rot)*(aDirection) << G4endl;

    ///////////////////////////////////////////////////////////////////
    ///
    /// Create and start timer for performance estimation
    ///

    G4Timer* timer = new G4Timer();
    timer->Start();

    const G4DynamicParticle* sec = 0;
    G4ParticleDefinition* pd;
    G4ThreeVector  mom;
    G4LorentzVector labv, fm;
    G4double e, p, m, px, py, pz, pt, theta;
    G4VParticleChange* aChange = 0;
			
    for (G4int iter = 0 ; iter < nevt ; iter++ ) 
    {
      if(verbose>1) 
      {
        G4cout << "### " << iter << "-th event start " << G4endl;
      }
      G4double e0 = energy;
	/// G4cout << "I amhere 6" << G4endl;

      do 
      {
        if(sigmae > 0.0) e0 = G4RandGauss::shoot(energy,sigmae);
      /// G4cout << "I amhere 6.1" << G4endl;


      } while (e0 < 0.0);

      dParticle.SetKineticEnergy(e0);
/// G4cout << "I amhere 6.2" << G4endl;


      gTrack->SetStep(step);
	/// G4cout << "I amhere 6.3" << G4endl;

 
      gTrack->SetKineticEnergy(energy); 

      labv = G4LorentzVector(0.0, 0.0, pmax, energy + mass + amass);

	/// G4cout << "I amhere 6.4" << G4endl;


      ////////////////////////////////////////////////////////////
      ///
      /// Preparation of the hadronic reaction results in aChange

	
      aChange = proc->PostStepDoIt(*gTrack,*step);
/// G4cout << "I amhere 6.5" << G4endl;


      G4double de = aChange->GetLocalEnergyDeposit();
/// G4cout << "I amhere 6.6" << G4endl;

     
 G4int n = aChange->GetNumberOfSecondaries();
	/// G4cout << "I amhere 6.7" << G4endl;

     
 G4int nn= n;
			
	/// G4cout << "I amhere 7" << G4endl;

      if(iter == 1000*(iter/1000)) 
      {
        G4cerr << "##### " << iter << "-th event  #####" << G4endl;
      }	

      G4int nbar = 0;
 
      for(G4int j=0; j<n+1; j++) 
      {

	if( j < n ) 
        {
	   sec = aChange->GetSecondary(j)->GetDynamicParticle();
	   pd  = sec->GetDefinition();	
				
           if(pd->GetPDGMass() > 100.*MeV) nbar++;
	} 
        else 
        {
	   if(aChange->GetStatusChange() == fStopAndKill) break;
           nn++;
	}
      }
		 					
      for(G4int i=0; i<nn; i++) 
      {
	if(i<n) 
        {
	   sec = aChange->GetSecondary(i)->GetDynamicParticle();
	   pd  = sec->GetDefinition();
	   mom = sec->GetMomentumDirection();
	   e   = sec->GetKineticEnergy();

	} 
        else 
        {
	   pd = part;
	   G4ParticleChange* bChange = dynamic_cast<G4ParticleChange*>(aChange);
	   mom = *(bChange->GetMomentumDirectionChange());
	   e   = bChange->GetEnergyChange();	
	}
	if (e < 0.0) 
        {
           e = 0.0;
	}
	///////////////////////////////////////////////////////////
	///
	/// for exclusive reaction 2 particles in final state

        if(!inclusive && nbar != 2) break;

        m = pd->GetPDGMass();
	p = sqrt(e*(e + 2.0*m));
	mom *= p;
        m  = pd->GetPDGMass();
        fm = G4LorentzVector(mom, e + m);
        labv -= fm;
        mom = (*rot)*mom;
        px = mom.x();
        py = mom.y();
        pz = mom.z();
        p  = sqrt(px*px +py*py + pz*pz);
        pt = sqrt(px*px +py*py);

        theta = mom.theta();
        G4double thetad = theta/degree;

#ifdef G4ANALYSIS_USE				

	if(usePaw && e > 0.0 && pt > 0.0) 
        {
                            h[2]-> fill(mom.phi()/degree,1.0);
          if(pd == neutron) h[23]->fill(mom.phi()/degree,1.0);
	}				
#endif	
        de += e;

        if(verbose>0 || abs(mom.phi()/degree - 90.) < 0.01) 
        {
          G4cout << i << "-th secondary  " 
		 << pd->GetParticleName() << "   Ekin(MeV)= "
                 << e/MeV
		 << "   p(MeV)= " << mom/MeV
		 << "   m(MeV)= " << m/MeV
		 << "   Etot(MeV)= " << (e+m)/MeV
		 << "   pt(MeV)= " << pt/MeV
                 << G4endl;
        }
	//  G4double ee,e1,e2,f;
	//  G4int    nb ;
        float N,Z,Z0;				
 
#ifdef G4ANALYSIS_USE
			
	if(usePaw) 
        {
          if(pd) 
          {
            N = pd->GetBaryonNumber();
            Z = pd->GetPDGCharge()/eplus;
            Z0= bestZ[(int)N];

		/// G4cout << "I amhere 8" << G4endl;

            if(abs(Z0 - Z) < 0.1 || Z0 == 0.0) h[26]->fill(N, factorb);
	  }
          if(pd == proton) 
          { 				
            h[1]->fill(1.0, 1.0);						
            h[3]->fill(pz/MeV, 1.0); 
            h[7]->fill(pt/MeV, 1.0);
            h[11]->fill(e/MeV, 1.0);
            h[11]->fill(e/MeV, 1.0);
	    h[21]->fill(e/MeV, factor);
	    h[24]->fill(cos(theta), factora);	
          } 
          else if(pd == pin) 
          {
	    h[1]->fill(4.0, 1.0);
            h[4]->fill(pz/MeV, 1.0); 
            h[8]->fill(pt/MeV, 1.0);
            h[12]->fill(e/MeV, 1.0);
            for(G4int kk=0; kk<nanglpi; kk++) 
            {
              if(bngpi1[kk] <= thetad && thetad <= bngpi2[kk]) 
              {
                h[40+kk]->fill(e/MeV, cngpi[kk]); 
                break;
	      }
	    }				
          } 
          else if(pd == pip) 
          {
	    h[1]->fill(3.0, 1.0);		
            h[5]->fill(pz/MeV, 1.0); 
            h[9]->fill(pt/MeV, 1.0);
            h[13]->fill(e/MeV, 1.0);

            for(G4int kk=0; kk<nanglpi; kk++) 
            {
              if(bngpi1[kk] <= thetad && thetad <= bngpi2[kk]) 
              {
                h[45+kk]->fill(e/MeV, cngpi[kk]); 
                break;
	      }
	    }
	  } 
          else if(pd == pi0) 
          {    
	    h[1]->fill(5.0, 1.0);	
	    h[18]->fill(e/MeV, 1.0);		
	    h[19]->fill(pz/MeV, 1.0); 
	    h[20]->fill(pt/MeV, 1.0);
	  } 
          else if(pd == neutron) 
          {    
	    h[1]->fill(2.0, 1.0);	  
            h[6]->fill(pz/MeV, 1.0); 
            h[10]->fill(pt/MeV, 1.0);
            h[14]->fill(e/MeV, 1.0);
	    h[22]->fill(e/MeV, factor);
	    /// SG changed by me
	    ///            ee = log10(e/MeV);
	    ///            nb = (G4int)(ee/binlog);
	    ///            e1 = binlog*nb;
	    ///            e2 = e1 + binlog;
	    ///           e1 = pow(10., e1);
	    ///            e2 = pow(10., e2) - e1;
	    ///            f  = factor*bine/e2;
	    
	    ///	    h[50]->fill(ee, f);

            /// G4cout << "I am here 9" << G4endl;

	    if(e >= elim) h[25]->fill(cos(theta), factora);

            for(G4int kk=0; kk<nangl; kk++) 
            {
              if(bng1[kk] <= thetad && thetad <= bng2[kk]) 
              {
                h[27+kk]->fill(e/MeV, cng[kk]);
		/// SG changed by me
                /// if(kk < 5) h[51+kk]->fill(ee, cng[kk]*bind/e2);
                break;
	      }
	    }
	  } 
          else if(pd == deu)   h[1]->fill(6.0, 1.0);	
          else if(pd == tri)   h[1]->fill(7.0, 1.0);	
          else if(pd == alp)   h[1]->fill(8.0, 1.0);   	       	
          else                 h[1]->fill(9.0, 1.0);	
	}
#endif								
        // if( i < n ) delete aChange->GetSecondary(i);

	if(useAscii) 
        {
          G4int ibin;
          if(pd) 
          {
            N = pd->GetBaryonNumber();
            Z = pd->GetPDGCharge()/eplus;
            Z0= bestZ[(G4int)N];

            if( (abs(Z0 - Z) < 0.1 || Z0 == 0.0) && N < maxn ) 
                       h26[(G4int)N] += factorb;
	  }
          if(pd == proton) 
          { 				
     	
            h1[0] +=1.;					
   
            ibin = (G4int)(((pz/MeV)+pmax)*100/2/pmax);
	    if(ibin>=0 && ibin<100) h3[ibin] += 1.; 

            ibin = (G4int)((pt/MeV)*100/pmax);
	    if(ibin>=0 && ibin<100) h7[ibin] += 1.; 

            ibin = (G4int)((e/MeV)*100/energy);
	    if(ibin>=0 && ibin<100) h11[ibin] += 1.;
 
            ibin = (G4int)((e/MeV)*nbinse/emax);
	    if(ibin>=0 && ibin<nbinse) h21[ibin] += factor;
	
            ibin = (G4int)(cos(theta)*nbinsa/2);
	    if(ibin>=0 && ibin<nbinsa) h24[ibin] += factora;



	    //////////////////////////////////////////
	    /////////// SG Here the proton cross section
	    ///////////////////////////////////////////
	    /*
	      SG Commented by me

	    if(e >= elim) 
	    {
              ibin = (G4int)(cos(theta)*nbinsa/2);
	      if(ibin>=0 && ibin<nbinsa) h25[ibin] += factora;
	    }
            for(G4int kk=0; kk<nangl; kk++) 
            {
              if(bng1[kk] <= thetad && thetad <= bng2[kk]) 
              {
                ibin = (G4int)((e/MeV)*nbinsd/emax);
                if(kk == 0)  h27[ibin] += cng[kk]; 
                if(kk == 1)  h28[ibin] += cng[kk]; 
                if(kk == 2)  h29[ibin] += cng[kk]; 
                if(kk == 3)  h30[ibin] += cng[kk]; 
                if(kk == 4)  h31[ibin] += cng[kk]; 
                if(kk == 5)  h32[ibin] += cng[kk]; 
                if(kk == 6)  h33[ibin] += cng[kk]; 
                if(kk == 7)  h34[ibin] += cng[kk]; 
                if(kk == 8)  h35[ibin] += cng[kk]; 
                if(kk == 9)  h36[ibin] += cng[kk]; 
                if(kk == 10) h37[ibin] += cng[kk]; 
                if(kk == 11) h38[ibin] += cng[kk]; 
                if(kk == 12) h39[ibin] += cng[kk]; 

                if(kk < 5)
		{
		  ///                  ibin = (G4int)(ee*nbinlog/logmax);
		  /// SG changed by me
		  
		  ///                  if(kk == 0)  h51[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 1)  h52[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 2)  h53[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 3)  h54[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 4)  h55[ibin] += cng[kk]*bind/e2; 
		}
                break;
	      }
	    }
	    */
	    //////////////////////////////////////////////////////////////////////
          } 
          else if(pd == pin) 
          {
            h1[3] +=1.;					

            ibin = (G4int)(((pz/MeV)+pmax)*100/2/pmax);
	    if(ibin>=0 && ibin<100) h4[ibin] += 1.; 
         
            ibin = (G4int)((pt/MeV)*100/pmax);
	    if(ibin>=0 && ibin<100) h8[ibin] += 1.; 
         
            ibin = (G4int)((e/MeV)*100/energy);
	    if(ibin>=0 && ibin<100) h12[ibin] += 1.;

            for(G4int kk=0; kk<nanglpi; kk++) 
            {
              ibin = (G4int)((e/MeV)*nbinspi/emaxpi);

              if(bngpi1[kk] <= thetad && thetad <= bngpi2[kk]) 
              {
                if(kk == 0) h40[ibin] += cngpi[kk]; 
                if(kk == 1) h41[ibin] += cngpi[kk]; 
                if(kk == 2) h42[ibin] += cngpi[kk]; 
                if(kk == 3) h43[ibin] += cngpi[kk]; 
                if(kk == 4) h44[ibin] += cngpi[kk]; 
                break;
	      }
	    }				
          } 
          else if(pd == pip) 
          {
            h1[2] +=1.;					

	    /// G4cout << "I amhere 11" << G4endl;

            ibin = (G4int)(((pz/MeV)+pmax)*100/2/pmax);
	    if(ibin>=0 && ibin<100) h5[ibin] += 1.; 
         
            ibin = (G4int)((pt/MeV)*100/pmax);
	    if(ibin>=0 && ibin<100) h9[ibin] += 1.; 
         
            ibin = (G4int)((e/MeV)*100/energy);
	    if(ibin>=0 && ibin<100) h13[ibin] += 1.;

            for(G4int kk=0; kk<nanglpi; kk++) 
            {
              ibin = (G4int)((e/MeV)*nbinspi/emaxpi);
              if(bngpi1[kk] <= thetad && thetad <= bngpi2[kk]) 
              { 
                if(kk == 0) h45[ibin] += cngpi[kk]; 
                if(kk == 1) h46[ibin] += cngpi[kk]; 
                if(kk == 2) h47[ibin] += cngpi[kk]; 
                if(kk == 3) h48[ibin] += cngpi[kk]; 
                if(kk == 4) h49[ibin] += cngpi[kk]; 
                break;
	      }
	    }
	  } 
          else if(pd == pi0) 
          {    
            h1[4] +=1.;					

            ibin = (G4int)(((pz/MeV)+pmax)*100/2/pmax);
	    if(ibin>=0 && ibin<100) h19[ibin] += 1.; 
         
            ibin = (G4int)((pt/MeV)*100/pmax);
	    if(ibin>=0 && ibin<100) h20[ibin] += 1.; 
         
            ibin = (G4int)((e/MeV)*100/energy);
	    if(ibin>=0 && ibin<100) h18[ibin] += 1.;
	  } 

          else if(pd == neutron) 
          {    
            h1[1] +=1.;					
   
            ibin = (G4int)(((pz/MeV)+pmax)*100/2/pmax);
	    if(ibin>=0 && ibin<100) h6[ibin] += 1.; 

            ibin = (G4int)((pt/MeV)*100/pmax);
	    if(ibin>=0 && ibin<100) h10[ibin] += 1.; 

            ibin = (G4int)((e/MeV)*100/energy);
	    if(ibin>=0 && ibin<100) h14[ibin] += 1.;
 
            ibin = (G4int)((e/MeV)*nbinse/emax);
	    if(ibin>=0 && ibin<nbinse) h22[ibin] += factor;

            //ee = log10(e/MeV);
            //nb = (G4int)(ee/binlog);
            //e1 = binlog*nb;
            //e2 = e1 + binlog;
            //e1 = pow(10., e1);
            //e2 = pow(10., e2) - e1;
	    //f  = factor*bine/e2;

            //ibin = (G4int)(ee*nbinlog/logmax);
	    /// SG changed by me
	    ///	    if(ibin>=0 && ibin<nbinlog) h50[ibin] += f;


	      if(e >= elim) 
	    {
              ibin = (G4int)(cos(theta)*nbinsa/2);
	      if(ibin>=0 && ibin<nbinsa) h25[ibin] += factora;
	    }
            for(G4int kk=0; kk<nangl; kk++) 
            {
              if(bng1[kk] <= thetad && thetad <= bng2[kk]) 
              {
                ibin = (G4int)((e/MeV)*nbinsd/emax);
                if(kk == 0)  h27[ibin] += cng[kk]; 
                if(kk == 1)  h28[ibin] += cng[kk]; 
                if(kk == 2)  h29[ibin] += cng[kk]; 
                if(kk == 3)  h30[ibin] += cng[kk]; 
                if(kk == 4)  h31[ibin] += cng[kk]; 
                if(kk == 5)  h32[ibin] += cng[kk]; 
                if(kk == 6)  h33[ibin] += cng[kk]; 
                if(kk == 7)  h34[ibin] += cng[kk]; 
                if(kk == 8)  h35[ibin] += cng[kk]; 
                if(kk == 9)  h36[ibin] += cng[kk]; 
                if(kk == 10) h37[ibin] += cng[kk]; 
                if(kk == 11) h38[ibin] += cng[kk]; 
                if(kk == 12) h39[ibin] += cng[kk]; 

                if(kk < 5)
		{
		  ///                  ibin = (G4int)(ee*nbinlog/logmax);
		  /// SG changed by me
		  
		  ///                  if(kk == 0)  h51[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 1)  h52[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 2)  h53[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 3)  h54[ibin] += cng[kk]*bind/e2; 
		  ///                  if(kk == 4)  h55[ibin] += cng[kk]*bind/e2; 
		  }
                break;
		}
		}
	  } 
          else if(pd == deu)   h1[5] +=1.;	
          else if(pd == tri)   h1[6] +=1.;
          else if(pd == alp)   h1[7] +=1.;       	
          else                 h1[8] +=1.;
	}								
        if( i < n ) delete aChange->GetSecondary(i);
      }
      if(verbose > 0) 
      {
        G4cout << "Energy/Momentum balance= " << labv << G4endl;
      }	
      px = labv.px();	
      py = labv.py();
      pz = labv.pz();							
      p  = sqrt(px*px +py*py + pz*pz);
      pt = sqrt(px*px +py*py);

#ifdef G4ANALYSIS_USE

      if(usePaw) 
      {
        h[0]->fill((float)nn,1.0);
	h[15]->fill(labv.e()/MeV, 1.0);
	h[16]->fill(pz/GeV, 1.0);
	h[17]->fill(pt/GeV, 1.0);
      }	

#endif
      if(useAscii) 
      {
        G4int ibin;
        h0[nn] += 1.;

        ibin = (G4int)((labv.e()/MeV)*20/2);
	if(ibin>=0 && ibin<20) h15[ibin] += 1.; 

        ibin = (G4int)(((pz/GeV)+pmax)*20/2);
	if(ibin>=0 && ibin<20) h16[ibin] += 1.; 

        ibin = (G4int)((pt/GeV)*20/2);
	if(ibin>=0 && ibin<20) h17[ibin] += 1.; 
      }	
      aChange->Clear();	
    }
    timer->Stop();
    G4cout << "  "  << *timer << G4endl;
    delete timer;
    /// G4cout << "I amhere 12" << G4endl;


    // Committing the transaction with the tree

#ifdef G4ANALYSIS_USE				

    if(usePaw) 
    {
      G4std::cout << "Committing..." << G4std::endl;
      tree->commit();
      G4std::cout << "Closing the tree..." << G4std::endl;
      tree->close();
    }

#endif


    ////////////////////////////////////////////////////////////////////
    ///
    /// Output to ascii files, either many or few groupped

    if(manyFiles) 
    {
      std::ofstream outh0("h0.dat", std::ios::out ) ;
      outh0.setf( std::ios::scientific, std::ios::floatfield );
      std::ofstream outh1("h1.dat", std::ios::out ) ;
      outh1.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh2("h2.dat", std::ios::out ) ;
      outh2.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh3("h3.dat", std::ios::out ) ;
      outh3.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh4("h4.dat", std::ios::out ) ;
      outh4.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh5("h5.dat", std::ios::out ) ;
      outh5.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh6("h6.dat", std::ios::out ) ;
      outh6.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh7("h7.dat", std::ios::out ) ;
      outh7.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh8("h8.dat", std::ios::out ) ;
      outh8.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh9("h9.dat", std::ios::out ) ;
      outh9.setf( std::ios::scientific, std::ios::floatfield );
      ///
      std::ofstream outh10("h10.dat", std::ios::out ) ;
      outh10.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh11("h11.dat", std::ios::out ) ;
      outh11.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh12("h12.dat", std::ios::out ) ;
      outh12.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh13("h13.dat", std::ios::out ) ;
      outh13.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh14("h14.dat", std::ios::out ) ;
      outh14.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh15("h15.dat", std::ios::out ) ;
      outh15.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh16("h16.dat", std::ios::out ) ;
      outh16.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh17("h17.dat", std::ios::out ) ;
      outh17.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh18("h18.dat", std::ios::out ) ;
      outh18.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh19("h19.dat", std::ios::out ) ;
      outh19.setf( std::ios::scientific, std::ios::floatfield );
      ///
      std::ofstream outh20("h20.dat", std::ios::out ) ;
      outh20.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh21("h21.dat", std::ios::out ) ;
      outh21.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh22("h22.dat", std::ios::out ) ;
      outh22.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh23("h23.dat", std::ios::out ) ;
      outh23.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh24("h24.dat", std::ios::out ) ;
      outh24.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh25("h25.dat", std::ios::out ) ;
      outh25.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh26("h26.dat", std::ios::out ) ;
      outh26.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh27("h27.dat", std::ios::out ) ;
      outh27.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh28("h28.dat", std::ios::out ) ;
      outh28.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh29("h29.dat", std::ios::out ) ;
      outh29.setf( std::ios::scientific, std::ios::floatfield );
      ///
      std::ofstream outh30("h30.dat", std::ios::out ) ;
      outh30.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh31("h31.dat", std::ios::out ) ;
      outh31.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh32("h32.dat", std::ios::out ) ;
      outh32.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh33("h33.dat", std::ios::out ) ;
      outh33.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh34("h34.dat", std::ios::out ) ;
      outh34.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh35("h35.dat", std::ios::out ) ;
      outh35.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh36("h36.dat", std::ios::out ) ;
      outh36.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh37("h37.dat", std::ios::out ) ;
      outh37.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh38("h38.dat", std::ios::out ) ;
      outh38.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh39("h39.dat", std::ios::out ) ;
      outh39.setf( std::ios::scientific, std::ios::floatfield );
      ///
      std::ofstream outh40("h40.dat", std::ios::out ) ;
      outh40.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh41("h41.dat", std::ios::out ) ;
      outh41.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh42("h42.dat", std::ios::out ) ;
      outh42.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh43("h43.dat", std::ios::out ) ;
      outh43.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh44("h44.dat", std::ios::out ) ;
      outh44.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh45("h45.dat", std::ios::out ) ;
      outh45.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh46("h46.dat", std::ios::out ) ;
      outh46.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh47("h47.dat", std::ios::out ) ;
      outh47.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh48("h48.dat", std::ios::out ) ;
      outh48.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh49("h49.dat", std::ios::out ) ;
      outh49.setf( std::ios::scientific, std::ios::floatfield );
      ///
      std::ofstream outh50("h50.dat", std::ios::out ) ;
      outh50.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh51("h51.dat", std::ios::out ) ;
      outh51.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh52("h52.dat", std::ios::out ) ;
      outh52.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh53("h53.dat", std::ios::out ) ;
      outh53.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh54("h54.dat", std::ios::out ) ;
      outh54.setf( std::ios::scientific, std::ios::floatfield );

      std::ofstream outh55("h55.dat", std::ios::out ) ;
      outh55.setf( std::ios::scientific, std::ios::floatfield );
      for(i=0;i<100;i++) 
      {
	/// 0-9
        if(i<50) 
        {    
          outh0<<h0[i]<<G4endl;
          outh1<<h1[i]<<G4endl;
          outh2<<h2[i]<<G4endl;
	}
        if(i<90) outh3<<h3[i]<<G4endl;
        outh4<<h4[i]<<G4endl;
        outh5<<h5[i]<<G4endl;
        outh6<<h6[i]<<G4endl;
        outh7<<h7[i]<<G4endl;
        outh8<<h8[i]<<G4endl;
        outh9<<h9[i]<<G4endl;
	/// 10-19
        if(i<20) 
        {    
          outh15<<h15[i]<<G4endl;
          outh16<<h16[i]<<G4endl;
          outh17<<h17[i]<<G4endl;
	}	
        outh10<<h10[i]<<G4endl;
        outh11<<h11[i]<<G4endl;
        outh12<<h12[i]<<G4endl;
        outh13<<h13[i]<<G4endl;
        outh14<<h14[i]<<G4endl;
        outh18<<h18[i]<<G4endl;
        outh19<<h19[i]<<G4endl;
	/// 20-29
        outh20<<h20[i]<<G4endl;
        if(i<90) outh23<<h23[i]<<G4endl;
        if(i<nbinse) 
        {    
          outh21<<h21[i]<<G4endl;
          outh22<<h22[i]<<G4endl;
	}	
        if(i<nbinsa) 
        {    
          outh24<<h24[i]<<G4endl;
          outh25<<h25[i]<<G4endl;
	}	
        if(i<maxn) outh26<<h26[i]<<G4endl;
        if(i<nbinsd) 
        {    
          outh27<<h27[i]<<G4endl;
          outh28<<h28[i]<<G4endl;
          outh29<<h29[i]<<G4endl;
	}	
	/// 30-39
        if(i<nbinspi) 
        {    
          outh30<<h30[i]<<G4endl;
          outh31<<h31[i]<<G4endl;
          outh32<<h32[i]<<G4endl;
          outh33<<h33[i]<<G4endl;
          outh34<<h34[i]<<G4endl;
          outh35<<h35[i]<<G4endl;
          outh36<<h36[i]<<G4endl;
          outh37<<h37[i]<<G4endl;
          outh38<<h38[i]<<G4endl;
          outh39<<h39[i]<<G4endl;
	}
	/// 40-49
        if(i<nbinspi) 
        {    
          outh40<<h40[i]<<G4endl;
          outh41<<h41[i]<<G4endl;
          outh42<<h42[i]<<G4endl;
          outh43<<h43[i]<<G4endl;
          outh44<<h44[i]<<G4endl;
          outh45<<h45[i]<<G4endl;
          outh46<<h46[i]<<G4endl;
          outh47<<h47[i]<<G4endl;
          outh48<<h48[i]<<G4endl;
          outh49<<h49[i]<<G4endl;
	}
	/// 50-55
/// SG changed by me
///        if(i<nbinlog) 
///        {    
///          outh50<<h50[i]<<G4endl;
///          outh51<<h51[i]<<G4endl;
///          outh52<<h52[i]<<G4endl;
///          outh53<<h53[i]<<G4endl;
///          outh54<<h54[i]<<G4endl;
///          outh55<<h55[i]<<G4endl;
///	}	
      }
    }
    else
    {
      /////////////////////////////////////////////////////////////////
      ///
      /// Here writing in few suitable files can be implemented.
      /// Say, using similar groups of data

    }
  G4cout << "###### End of run # " << G4endl;


  fin->close();
  delete mate;
  delete fin;
  delete phys;

  G4cout << "###### End of test #####" << G4endl;
  return 1;
}
