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
// $Id: RemSimDetectorConstruction.cc,v 1.9 2004-05-21 13:49:23 guatelli Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//

#include "RemSimDetectorConstruction.hh"
#include "RemSimMaterial.hh"
#include "RemSimDetectorMessenger.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"
#include "RemSimVGeometryComponent.hh"
#include "RemSimVehicle1.hh"
#include "RemSimMoonHabitat.hh"
#include "G4VisAttributes.hh"
#include "RemSimSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "RemSimROGeometry.hh"
#include "RemSimDecorator.hh"
#include "RemSimShieldingDecorator.hh"
#include "RemSimShelterSPEDecorator.hh"
#include "RemSimAstronautDecorator.hh"

RemSimDetectorConstruction::RemSimDetectorConstruction()
  :  experimentalHall_log(0),experimentalHall_phys(0),
     phantomPhys(0),detectorPhys(0)
{
 pMaterial = new RemSimMaterial();

 pVehicle = new RemSimVehicle1();
 pMoon =  new RemSimMoonHabitat();

 messenger = new RemSimDetectorMessenger(this);
 decoratorValue ="Nothing";
 astronautValue ="On";
 decorator = 0;  //pointing to shielding
 decoratorSPE = 0;
 decorator1 = 0; //pointing to astronaut
 moon = false;
 flag = false;
}

RemSimDetectorConstruction::~RemSimDetectorConstruction()
{ 
  delete decorator1;
  delete decoratorSPE;
  delete decorator;
  delete messenger;
  delete pMoon;
  delete pVehicle;
  delete pMaterial;
}

G4VPhysicalVolume* RemSimDetectorConstruction::Construct()
{ 
  pMaterial -> DefineMaterials();
  G4Material* vacuum = pMaterial -> GetMaterial("Galactic");
 
  G4double expHall_x = 30.0*m;
  G4double expHall_y = 30.0*m;
  G4double expHall_z = 30.0*m;
  
  G4Box* experimentalHall_box
    = new G4Box("world",expHall_x,expHall_y,expHall_z);
  experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
                                             vacuum,"world",0,0,0);
  
  experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),"world",
                                      experimentalHall_log,0,
                                      false,0);
  //ConstructVolume();

  
  return experimentalHall_phys;
}
void RemSimDetectorConstruction::ConstructVolume()
{ 
 if (moon == true)
    { 
      pMoon -> ConstructComponent(experimentalHall_phys);
      decorator1 = new RemSimAstronautDecorator(pMoon);
      G4VPhysicalVolume* shelter = pMoon -> GetShelter();
      decorator1 -> ChangeMother(shelter);
      decorator1 -> ConstructComponent(experimentalHall_phys);
      }

  else 
    {
     pVehicle -> ConstructComponent(experimentalHall_phys); 
     decorator1 = new RemSimAstronautDecorator(pVehicle); 
     decorator1 -> ConstructComponent(experimentalHall_phys);
   }
}

void RemSimDetectorConstruction::AddShielding(G4String value)
{ 
  if (moon == false)
    {
  decoratorValue = value;
  decorator = retrieveDecorator();
   
  if (decoratorValue == "On")
	{
	  if (decorator == 0)
	    { 
	      decorator = new RemSimShieldingDecorator(pVehicle);
	      decorator -> ConstructComponent(experimentalHall_phys); 
	      G4RunManager::GetRunManager() -> DefineWorldVolume(experimentalHall_phys);
	    }
	  else  G4cout<<" The Shielding alread exists!"<<G4endl;
	}

      if (decoratorValue == "Off")
	{
	  if (decorator != 0)
	    {
	  decorator -> DestroyComponent(); 
	  G4RunManager::GetRunManager() -> DefineWorldVolume(experimentalHall_phys);
          decorator = 0;
	   }
         
          else  G4cout<<" The Shielding does not exist!"<<G4endl;
	}
    }
}

void RemSimDetectorConstruction::AddShelterSPE(G4String value)
{ 
  if (moon == false)
    { 
      decoratorSPE = retrieveDecoratorSPE();

      if (value == "On")
	{
	if (decoratorSPE == 0)
	  { 
	    decoratorSPE = new RemSimShelterSPEDecorator(pVehicle);
	    decoratorSPE -> ConstructComponent(experimentalHall_phys); 
	    G4RunManager::GetRunManager() -> DefineWorldVolume(experimentalHall_phys);
	  }
	else  G4cout<<" The SPE shelter alread exists!"<<G4endl;
	}

  if (value == "Off")
    {
      if (decoratorSPE != 0)
	{
	  decoratorSPE -> DestroyComponent(); 
	  G4RunManager::GetRunManager() -> DefineWorldVolume(experimentalHall_phys);
          decoratorSPE = 0;
	}
         
      else  G4cout<<" The SPE shelter does not exist!"<<G4endl;
    }
    }
  else  G4cout<< " It is not possible to select SPE shelter in moon habitat configuration" << G4endl;
}

RemSimDecorator* RemSimDetectorConstruction::retrieveDecorator()
{
  return decorator;
}

RemSimDecorator* RemSimDetectorConstruction::retrieveDecorator1()
{
  return decorator1;
}

RemSimDecorator* RemSimDetectorConstruction::retrieveDecoratorSPE()
{
  return decoratorSPE;
}

void RemSimDetectorConstruction::ChangeShieldingMaterial(G4String material)
{
//   decorator = retrieveDecorator();
//   if (decorator != 0)
//     {
//       decorator = retrieveDecorator();
//       decorator -> ChangeMaterial(material);
//     }
//   else G4cout<<" Define the shielding before!"<< G4endl;
}
void RemSimDetectorConstruction::ChangeShieldingThickness(G4double thick)
{  
 if (moon == false)
   {
  decorator = retrieveDecorator();
  if (decorator != 0)
    { 
      decorator = retrieveDecorator();
      decorator -> ChangeThickness(thick);
      G4RunManager::GetRunManager() -> DefineWorldVolume(experimentalHall_phys);
    }
  else G4cout<<" Define the shielding before!"<< G4endl;
   }
}
void RemSimDetectorConstruction:: ChooseConfiguration(G4String value)
{
  if (flag == false)
    { 
      if (value == "vehicle") ConstructVolume();
  
      else if (value == "moon")
	{
	  moon = true;
	  ConstructVolume();
	}
      G4RunManager::GetRunManager() -> DefineWorldVolume(experimentalHall_phys);
      flag = true;
    }
  else G4cout<< "The configuration has been selected!" << G4endl;
}
