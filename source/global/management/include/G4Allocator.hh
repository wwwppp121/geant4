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
// $Id: G4Allocator.hh,v 1.15 2004-05-26 14:43:41 gcosmo Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
// 
// ------------------------------------------------------------
// GEANT 4 class header file 
//
// Class Description:
//
// A class for fast allocation of objects to the heap through a pool of
// chunks organised as linked list. It's meant to be used by associating
// it to the object to be allocated and defining for it new and delete
// operators via MallocSingle() and FreeSingle() methods.
       
//      ---------------- G4Allocator ----------------
//
// Author: G.Cosmo (CERN), November 2000
// ------------------------------------------------------------

#ifndef G4Allocator_h
#define G4Allocator_h 1

#include "G4AllocatorPool.hh"

template <class Type>
class G4Allocator
{
  public:  // with description

    G4Allocator() throw();
    ~G4Allocator() throw();
      // Constructor & destructor

    inline Type* MallocSingle();
    inline void FreeSingle(Type* anElement);
      // Malloc and Free methods to be used when overloading
      // new and delete operators in the client <Type> object

    inline void ResetStorage();
      // Returns allocated storage to the free store, resets
      // allocator and page sizes.
      // Note: contents in memory are lost using this call !

    inline size_t GetAllocatedSize() const;
      // Returns the size of the total memory allocated

  public:  // without description

    // This public section includes standard methods and types
    // required if the allocator is to be used as alternative
    // allocator for STL containers.
    // NOTE: the code below is a trivial implementation to make
    //       this class an STL compliant allocator.
    //       It is anyhow NOT recommended to use this class as
    //       alternative allocator for STL containers !

    typedef Type value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef Type* pointer;
    typedef const Type* const_pointer;
    typedef Type& reference;
    typedef const Type& const_reference;

    template <class U> G4Allocator(const G4Allocator<U>& right) throw()
         { *this = right; }
      // Copy constructor

    pointer address(reference r) const { return &r; }
    const_pointer address(const_reference r) const { return &r; }
      // Returns the address of values

    pointer allocate(size_type n, void* hint = 0)
    {
      // Allocates space for n elements of type Type, but does not initialise
      //
      Type* mem_alloc = 0;
      if (n == 1)
        mem_alloc = MallocSingle();
      else
        mem_alloc = static_cast<Type*>(::operator new(n*sizeof(Type)));
      return mem_alloc;
    }
    void deallocate(pointer p, size_type n)
    {
      // Deallocates n elements of type Type, but doesn't destroy
      //
      if (n == 1)
        FreeSingle(p);
      else
        ::operator delete((void*)p);
      return;
    }

    void construct(pointer p, const Type& val) { new((void*)p) Type(val); }
      // Initialises *p by val
    void destroy(pointer p) { p->~Type(); }
      // Destroy *p but doesn't deallocate

    size_type max_size() const throw()
    {
      // Returns the maximum number of elements that can be allocated
      //
      return 2147483647/sizeof(Type);
    }

    template <class U>
    struct rebind { typedef G4Allocator<U> other; };
      // Rebind allocator to type U

  private:

    G4AllocatorPool mem;
      // Pool of elements of sizeof(Type)
};

// ------------------------------------------------------------
// Inline implementation
// ------------------------------------------------------------

// Initialization of the static pool
//
// template <class Type> G4AllocatorPool G4Allocator<Type>::mem(sizeof(Type));

// ************************************************************
// G4Allocator constructor
// ************************************************************
//
template <class Type>
G4Allocator<Type>::G4Allocator() throw()
  : mem(sizeof(Type))
{
}

// ************************************************************
// G4Allocator destructor
// ************************************************************
//
template <class Type>
G4Allocator<Type>::~G4Allocator() throw()
{
}

// ************************************************************
// MallocSingle
// ************************************************************
//
template <class Type>
Type* G4Allocator<Type>::MallocSingle()
{
  return static_cast<Type*>(mem.Alloc());
}

// ************************************************************
// FreeSingle
// ************************************************************
//
template <class Type>
void G4Allocator<Type>::FreeSingle(Type* anElement)
{
  mem.Free(anElement);
  return;
}

// ************************************************************
// ResetStorage
// ************************************************************
//
template <class Type>
void G4Allocator<Type>::ResetStorage()
{
  // Clear all allocated storage and return it to the free store
  //
  mem.Reset();
  return;
}

// ************************************************************
// GetAllocatedSize
// ************************************************************
//
template <class Type>
size_t G4Allocator<Type>::GetAllocatedSize() const
{
  return mem.Size();
}

// ************************************************************
// operator==
// ************************************************************
//
template <class T1, class T2>
bool operator== (const G4Allocator<T1>&, const G4Allocator<T2>&) throw()
{
  return true;
}

// ************************************************************
// operator!=
// ************************************************************
//
template <class T1, class T2>
bool operator!= (const G4Allocator<T1>&, const G4Allocator<T2>&) throw()
{
  return false;
}

#endif
