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
// $Id: Em10DetectorConstruction.cc,v 1.19 2006-02-02 15:42:21 grichine Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 

#include "Em10DetectorConstruction.hh"
#include "Em10DetectorMessenger.hh"
#include "Em10CalorimeterSD.hh"
#include "Em10Materials.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4ProductionCuts.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

/////////////////////////////////////////////////////////////////////////////
//
//

Em10DetectorConstruction::Em10DetectorConstruction()
  :worldchanged(false), fAbsorberMaterial(0),  fGapMat(0),fSetUp("simpleALICE"),
   fWorldMaterial(0),  solidWorld(0),      logicWorld(0),      physiWorld(0),
   fSolidRadSlice(0), fLogicRadSlice(0),  fPhysicRadSlice(0),
   solidRadiator(0),  logicRadiator(0),   physiRadiator(0),
   fRadiatorMat(0),
   solidAbsorber(0),  logicAbsorber(0),   physiAbsorber(0),
   magField(0),       calorimeterSD(0),   fRegGasDet(0), fRadRegion(0), fMat(0)
{
  detectorMessenger = new Em10DetectorMessenger(this);
  fMat = new Em10Materials();
}

//////////////////////////////////////////////////////////////////////////
//
//

Em10DetectorConstruction::~Em10DetectorConstruction()
{ 
  delete detectorMessenger;
}

//////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume* Em10DetectorConstruction::Construct()
{
  return ConstructDetectorXTR();  
}


/////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume* Em10DetectorConstruction::ConstructDetectorXTR()
{
 // Cleanup old geometry

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  if( fSetUp == "simpleALICE" )
  {
    return SimpleSetUpALICE();
  }
  else
  {
    G4cout<<"Experimental setup is unsupported. Check /XTRdetector/setup "<<G4endl;
    return 0;
  }
}



G4VPhysicalVolume* Em10DetectorConstruction::SimpleSetUpALICE()
{
  fWorldSizeZ = 400.*cm; 
  fWorldSizeR = 20.*cm;

  // Radiator and detector parameters


  fRadThickness = 0.020*mm;   
  foilGasRatio  = fRadThickness/(fRadThickness+fGasGap) ;
  fFoilNumber   = 220; 

  fAlphaPlate   = 160.0;
  fAlphaGas     = 160.0;
  fModelNumber  = 0;


  fAbsorberThickness = 38.3*mm; 

  fAbsorberRadius   = 1.*mm;
  zAbsorber        = 136.*cm;

  fWindowThick    = 51.0*micrometer ;
  fElectrodeThick = 10.0*micrometer ;
  fGapThick       =  1.0*mm ;


  fDetThickness =  40.0*mm ;
  fDetLength    = 200.0*cm  ;
  fDetGap       =   0.01*mm ;


  fStartR       = 40*cm  ;
  fStartZ       = 100.0*mm  ;

  fModuleNumber = 1      ;  

  // Preparation of mixed radiator material


  G4Material* Mylar = fMat->GetMaterial("Mylar");
  G4Material* Air   = fMat->GetMaterial("Air");
  G4Material* Al   = fMat->GetMaterial("Al");

  G4double foilDensity =  1.39*g/cm3; // Mylar // 0.91*g/cm3;  // CH2 0.534*g/cm3; //Li     
  G4double gasDensity  =  1.2928*mg/cm3; // Air // 1.977*mg/cm3; // CO2 0.178*mg/cm3; // He  
  G4double totDensity  = foilDensity*foilGasRatio + gasDensity*(1.0-foilGasRatio) ;

  G4double fractionFoil =  foilDensity*foilGasRatio/totDensity ; 
  G4double fractionGas  =  gasDensity*(1.0-foilGasRatio)/totDensity ;  
    
  G4Material* radiatorMat = new G4Material("radiatorMat"  , totDensity, 
                                                  2);
  radiatorMat->AddMaterial( Mylar, fractionFoil ) ;
  radiatorMat->AddMaterial( Air, fractionGas  ) ;
  
  // default materials of the detector and TR radiator

  fRadiatorMat =  radiatorMat; 
  fFoilMat     = Mylar; // CH2; // Kapton; // Mylar ; // Li ; // CH2 ;  
  fGasMat      = Air; // CO2; // He; //   
  
  fWindowMat    = Mylar ;
  fElectrodeMat = Al ;

  fAbsorberMaterial = fMat->GetMaterial("Xe15CO2");
 

  fGapMat          = fAbsorberMaterial;

  fWorldMaterial    = Air; // CO2 ;  

  G4double zModule; 

  solidWorld = new G4Box("World", fWorldSizeR,fWorldSizeR,fWorldSizeZ/2.);
                         
  logicWorld = new G4LogicalVolume(solidWorld,  fWorldMaterial,  "World");	
                                   
  physiWorld = new G4PVPlacement(0, G4ThreeVector(), "World",		
                                 logicWorld,		
                                 0,			
                                 false,			
                                 0);			

  // TR radiator envelope

  G4double radThick = fFoilNumber*(fRadThickness + fGasGap) - fGasGap + fDetGap   ;

  G4double zRad = fStartZ + 0.5*radThick ;
  G4cout<<"zRad = "<<zRad/mm<<" mm"<<G4endl ;

  G4cout<<"radThick = "<<radThick/mm<<" mm"<<G4endl ;
  G4cout<<"fFoilNumber = "<<fFoilNumber<<G4endl ;
  G4cout<<"fRadiatorMat = "<<fRadiatorMat->GetName()<<G4endl ;
  G4cout<<"WorldMaterial = "<<fWorldMaterial->GetName()<<G4endl ;

  solidRadiator = new G4Box("Radiator",1.1*fAbsorberRadius , 
                                              1.1*fAbsorberRadius, 
                                              0.5*radThick             ) ; 
                         
  logicRadiator = new G4LogicalVolume(solidRadiator, fRadiatorMat,      
                                       "Radiator");	       
                                   
  physiRadiator = new G4PVPlacement(0,
                                     G4ThreeVector(0,0,zRad),	        
                                     "Radiator", logicRadiator,		
                                     physiWorld, false,	0       );  	
  fSolidRadSlice = new G4Box("RadSlice",fAbsorberRadius,
                                fAbsorberRadius,0.5*fRadThickness ) ;

  fLogicRadSlice = new G4LogicalVolume(fSolidRadSlice,fRadiatorMat,
                                          "RadSlice",0,0,0);

  zModule = fStartZ ;   //  ??? + fRadThickness ;
 
  G4cout<<"zModule = "<<zModule/mm<<" mm"<<G4endl ;
   
  G4double zWindow = fStartZ + radThick + fWindowThick/2. + 15.0*mm ;    
      			                  
  G4Box* solidWindowR = new G4Box("WindowR",fAbsorberRadius+0.001,
                                          fAbsorberRadius+0.001,
                                          fWindowThick/2.+0.001  ); 
                          
  G4LogicalVolume* logicWindowR = new G4LogicalVolume(solidWindowR,
                                     fWorldMaterial, "WindowR");
 
  G4VPhysicalVolume*    physiWindowR = new G4PVPlacement(0,		   
                        G4ThreeVector(0.,0.,zWindow),        
                              "Window",logicWindowR,physiWorld,false,0);
  // window 
      			                  
  G4Box* solidWindow = new G4Box("Window",fAbsorberRadius,
                                   fAbsorberRadius, fWindowThick/2.); 
                          
  G4LogicalVolume* logicWindow = new G4LogicalVolume(solidWindow,
                                     fWindowMat, "Window"); 
  G4VPhysicalVolume*    physiWindow = new G4PVPlacement(0,		   
                        G4ThreeVector(0.,0.,0.),        
                              "Window",logicWindow,physiWindowR,false,0); 

  // create region for window inside windowR for

  if( fRadRegion != 0 ) delete fRadRegion; 
  if( fRadRegion == 0 )        fRadRegion = new G4Region("XTRradiator");
                               fRadRegion->AddRootLogicalVolume(logicWindowR);                       

  G4Box* solidGap = new G4Box("Gap",fAbsorberRadius,
                                    fAbsorberRadius,
                                    fGapThick/2.     ) ; 
                          
  G4LogicalVolume* logicGap = new G4LogicalVolume(solidGap,fGapMat, "Gap"); 

  G4double zGap = zWindow + fWindowThick/2. + fGapThick/2. + 0.01*mm ;    

  G4Box* solidElectrode = new G4Box("Electrode",fAbsorberRadius,
                                                fAbsorberRadius,
                                                fElectrodeThick/2. ); 
                          
  G4LogicalVolume* logicElectrode = new G4LogicalVolume(solidElectrode,
                                        fElectrodeMat, "Electrode"); 

  G4double zElectrode = zGap + fGapThick/2. + fElectrodeThick/2. + 0.01*mm;    

  // Absorber

  zAbsorber = zElectrode + fElectrodeThick/2. + fAbsorberThickness/2. + 0.01*mm; 

  G4cout<<"zAbsorber = "<<zAbsorber/mm<<" mm"<<G4endl;

  if (fAbsorberThickness > 0.) 
  { 
    solidAbsorber = new G4Box("Absorber", fAbsorberRadius,		
                                 fAbsorberRadius, fAbsorberThickness/2.); 
                          
    logicAbsorber = new G4LogicalVolume(solidAbsorber, fAbsorberMaterial, 
      			                  "Absorber");     
      			                  
    physiAbsorber = new G4PVPlacement(0, G4ThreeVector(0.,0.,zAbsorber),        
                                       "Absorber", logicAbsorber,     
                                        physiWorld,  false,  0);                
                                        
  }
  if( fRegGasDet != 0 ) delete fRegGasDet;
  if( fRegGasDet == 0 )        fRegGasDet = new G4Region("XTRdEdxDetector");  
                               fRegGasDet->AddRootLogicalVolume(logicAbsorber);              
                                 
  // Sensitive Detectors: Absorber 
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if(!calorimeterSD)
  {
    calorimeterSD = new Em10CalorimeterSD("CalorSD",this);
    SDman->AddNewDetector( calorimeterSD );
  }
  if (logicAbsorber)  logicAbsorber->SetSensitiveDetector(calorimeterSD);

  return physiWorld;
}

////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::TestOld()
{
  //  G4double inch = 2.54*cm ;
  // G4double  mil = inch/1000.0 ;

  // create commands for interactive definition of the calorimeter  

  // fGammaCut    = 23*mm; 
  // fElectronCut = 23*mm; 
  // fPositronCut = 23*mm; 

  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;


  //  G4int i, j ; 
  // G4int j ;
  //  G4double zModule, zRadiator, rModule, rRadiator ; 
  
  // complete the Calor parameters definition and Print 

  ComputeCalorParameters();
  PrintCalorParameters();

  // zRadiator ;
      
  // World
  
  // if(solidWorld) delete solidWorld ;
  // if(logicWorld) delete logicWorld ;
  // if(physiWorld) delete physiWorld ;
 
  //  if(solidRadiator) delete solidRadiator;
  //  if(logicRadiator) delete logicRadiator;
  //  if(physiRadiator) delete physiRadiator;

  //  radThick *= 1.02 ;

  //  if(fSolidRadSlice) delete fSolidRadSlice;
  //  if(fLogicRadSlice) delete fLogicRadSlice; 
  //  if(fPhysicRadSlice) delete fPhysicRadSlice; 

  /*
    for(j=0;j<fFoilNumber;j++)
    {  

      zRadiator = zModule + j*(fRadThickness + fGasGap) ;
      G4cout<<zRadiator/mm<<" mm"<<"\t" ;
      //   G4cout<<"j = "<<j<<"\t" ;         
      
      fPhysicRadSlice = new G4PVPlacement(0,G4ThreeVector(0.,0.,zRadiator-zRad),
                                         "RadSlice",fLogicRadSlice,
                                          physiRadiator,false,j);
     }                                 
  G4cout<<G4endl ;
  */
    // fRadRegion->RemoveRootLogicalVolume(logicWindowR);
  // G4ProductionCuts* cutsR = 0;
    // cutsR = new G4ProductionCuts();
    // fRadRegion->SetProductionCuts(cutsR);
  
  // else  // Second time - get a cut object from region
  {   
    // cutsR = fRadRegion->GetProductionCuts();
  }

  // cutsR->SetProductionCut(fGammaCut,"gamma");
  // cutsR->SetProductionCut(fElectronCut,"e-");
  // cutsR->SetProductionCut(fPositronCut,"e+");
      			                  
  // G4VPhysicalVolume*    physiGap = new G4PVPlacement(0,		   
  // 		                       G4ThreeVector(0.,0.,zGap),        
  //                                    "Gap",logicGap,physiWorld,false,0); 

      			                  
  //  G4VPhysicalVolume*    physiElectrode = new G4PVPlacement(0,		   
  //  		                       G4ThreeVector(0.,0.,zElectrode),        
  //                                    "Electrode",logicElectrode,
  //                                     physiWorld,false,0);    
    //  if(solidAbsorber) delete solidAbsorber ;
    //  if(logicAbsorber) delete logicAbsorber ;
    //  if(physiAbsorber) delete physiAbsorber ;

    // fRegGasDet->RemoveRootLogicalVolume(logicAbsorber);
  // G4ProductionCuts* cuts = 0;
    // cuts = new G4ProductionCuts();
    //  fRegGasDet->SetProductionCuts(cuts);
  // else  // Second time - get a cut object from region
  {   
    //  cuts = fRegGasDet->GetProductionCuts();
  }


  // cuts->SetProductionCut(fGammaCut,"gamma");
  // cuts->SetProductionCut(fElectronCut,"e-");
  // cuts->SetProductionCut(fPositronCut,"e+");

}




////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::PrintCalorParameters()
{
  G4cout << "\n The  WORLD   is made of " 
       << fWorldSizeZ/mm << "mm of " << fWorldMaterial->GetName() ;
  G4cout << ", the transverse size (R) of the world is " << fWorldSizeR/mm << " mm. " << G4endl;
  G4cout << " The ABSORBER is made of " 
       << fAbsorberThickness/mm << "mm of " << fAbsorberMaterial->GetName() ;
  G4cout << ", the transverse size (R) is " << fAbsorberRadius/mm << " mm. " << G4endl;
  G4cout << " Z position of the (middle of the) absorber " << zAbsorber/mm << "  mm." << G4endl;
  G4cout << G4endl;
}

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name   
  G4Material* pttoMaterial;

  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J]; 
    
    if(pttoMaterial->GetName() == materialChoice)
    {
      fAbsorberMaterial = pttoMaterial;
      logicAbsorber->SetMaterial(pttoMaterial); 
        // PrintCalorParameters();
    }             
  }
}
///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetRadiatorMaterial(G4String materialChoice)
{
  // get the pointer to the material table

  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name
   
  G4Material* pttoMaterial;
  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J];
     
    if(pttoMaterial->GetName() == materialChoice)
    {
      fRadiatorMat = pttoMaterial;
      fLogicRadSlice->SetMaterial(pttoMaterial); 
      // PrintCalorParameters();
    }             
  }
}

////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name   
  G4Material* pttoMaterial;

  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J];
     
    if(pttoMaterial->GetName() == materialChoice)
    {
      fWorldMaterial = pttoMaterial;
      logicWorld->SetMaterial(pttoMaterial); 
       //  PrintCalorParameters();
    }             
  }
}

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberThickness(G4double val)
{
  // change Absorber thickness and recompute the calorimeter parameters
  fAbsorberThickness = val;
  ComputeCalorParameters();
}  

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetRadiatorThickness(G4double val)
{
  // change XTR radiator thickness and recompute the calorimeter parameters
  fRadThickness = val;
  // ComputeCalorParameters();
}
  
///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetGasGapThickness(G4double val)
{
  // change XTR gas gap thickness and recompute the calorimeter parameters
  fGasGap = val;
  // ComputeCalorParameters();
}  

/////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberRadius(G4double val)
{
  // change the transverse size and recompute the calorimeter parameters
  fAbsorberRadius = val;
  ComputeCalorParameters();
}  

////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetWorldSizeZ(G4double val)
{
  worldchanged=true;
  fWorldSizeZ = val;
  ComputeCalorParameters();
}  

///////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetWorldSizeR(G4double val)
{
  worldchanged=true;
  fWorldSizeR = val;
  ComputeCalorParameters();
}  

//////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetAbsorberZpos(G4double val)
{
  zAbsorber  = val;
  ComputeCalorParameters();
}  

//////////////////////////////////////////////////////////////////////////////
//
//

void Em10DetectorConstruction::SetMagField(G4double)
{
  //apply a global uniform magnetic field along X axis

  /* *********************************************************

  G4FieldManager* fieldMgr 
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    
  if(magField) delete magField;		//delete the existing magn field
  
  if(fieldValue!=0.)			// create a new one if non nul
  { 
    magField = new G4UniformMagField(G4ThreeVector(fieldValue,0.,0.));        
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  } 
  else 
  {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }

  *************************************************************** */

}

///////////////////////////////////////////////////////////////////////////////
//
//
  
void Em10DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetectorXTR());
}

//
//
////////////////////////////////////////////////////////////////////////////

















