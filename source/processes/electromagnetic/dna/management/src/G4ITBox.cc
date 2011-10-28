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
// Author: Mathieu Karamitros (kara@cenbg.in2p3.fr)
//
// History:
// -----------
// 10 Oct 2011 M.Karamitros created
//
// -------------------------------------------------------------------

#include "G4ITBox.hh"

G4ITBox::G4ITBox() : fNbIT(0), fFirstIT(0), fLastIT(0), fPreviousBox(0), fNextBox(0)
{;}

G4ITBox::~G4ITBox()
{
    if( fNbIT != 0 )
    {
        G4IT * aIT = fFirstIT;
        G4IT * nextIT;

        // delete tracks in the stack
        while( aIT != 0 )
        {
            nextIT = aIT->GetNext();
            delete aIT;
            aIT = nextIT;
        }
    }

    if(fPreviousBox)    fPreviousBox->SetNextBox(fNextBox) ;
    if(fNextBox)        fNextBox->SetPreviousBox(fPreviousBox);
}

const G4ITBox & G4ITBox::operator=(const G4ITBox &right)
{
    fNbIT = right.fNbIT;
    fFirstIT = right.fFirstIT;
    fLastIT = right.fLastIT;
    fPreviousBox = 0;
    fNextBox = 0;
    return *this;
}

void G4ITBox::Push( G4IT * aIT )
{
    if( fNbIT == 0 )
    {
        aIT->SetPrevious( 0 );
        fFirstIT = aIT;
    }
    else
    {
        fLastIT->SetNext( aIT );
        aIT->SetPrevious( fLastIT );
    }
    fLastIT = aIT;
    fNbIT++;
    aIT->SetITBox(this);
}

void G4ITBox::Extract( G4IT * aStackedIT )
{
    //Take off an IT from the box
    if( aStackedIT == fFirstIT )
    {
        fFirstIT = aStackedIT->GetNext();
    }
    else  if( aStackedIT == fLastIT )
    {
        fLastIT = aStackedIT->GetPrevious();

    }

    if( aStackedIT->GetNext())
        aStackedIT->GetNext()->SetPrevious(aStackedIT->GetPrevious());
    if( aStackedIT->GetPrevious())
        aStackedIT->GetPrevious()->SetNext(aStackedIT->GetNext());

    aStackedIT->SetNext(0);
    aStackedIT->SetPrevious(0);
    aStackedIT->SetITBox(0);
    fNbIT--;
}

G4IT* G4ITBox::FindIT(const G4Track& track)
{
    if( fNbIT == 0 ) return 0;

    G4IT * temp = fLastIT;
    G4bool find = false;

    while(!find || temp)
    {
        if(temp-> GetTrack() == &track)
        {
            find = true;
            break;
        }
        temp = temp->GetPrevious();
    }

    return temp;
}

const G4IT* G4ITBox::FindIT(const G4Track& track) const
{
    if( fNbIT == 0 ) return 0;

    const G4IT * temp = fLastIT;
    G4bool find = false;

    while(!find || temp)
    {
        if(temp-> GetTrack() == &track)
        {
            find = true;
            break;
        }
        temp = temp->GetPrevious();
    }

    return temp;
}

void G4ITBox::TransferTo(G4ITBox * aStack)
{
    G4IT * ITToTransfer = fFirstIT;
    while(fNbIT)
    {
        Extract(ITToTransfer);
        aStack->Push(ITToTransfer);
        ITToTransfer = ITToTransfer->GetNext();
        // DEBUG
        // G4cout<<n_IT<<G4endl;
    }
}