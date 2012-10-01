#!/usr/bin/env bash
#
export G4INSTALL=/home-cluck/syjun/g4p/work/g4.9.5.p01/geant4.9.5.p01
export G4SYSTEM=Linux-g++
export G4SRC=${G4INSTALL}/source
export G4LIB=${G4INSTALL}/lib
export G4INCLUDE=${G4INSTALL}/include

export G4LIB_BUILD_SHARED=1
export GLOBALLIBS=1

export G4DATA=${G4INSTALL}/data
export G4LEDATA=${G4DATA}/G4EMLOW6.23
export G4LEVELGAMMADATA=${G4DATA}/PhotonEvaporation2.2
export G4NEUTRONHPDATA=${G4DATA}/G4NDL4.0
export G4RADIOACTIVEDATA=${G4DATA}/RadioactiveDecay3.4
export G4ABLADATA=${G4DATA}/G4ABLA3.0
export G4REALSURFACEDATA=${G4DATA}/RealSurface1.0
export G4NEUTRONXSDATA=${G4DATA}/G4NEUTRONXS1.1
export G4PIIDATA=${G4DATA}/G4PII1.3

export G4WORKDIR=/home-cluck/yarba_j/work/test-geant4.9.5.p01/tests
export G4EXE=${G4WORKDIR}/bin/${G4SYSTEM}

export ROOTSYS=/products/root/v5_30_02/Linux64bit+2.6-2.5-gcc46-prof

export XERCESCROOT=/products/xerces_c/v3_1_1/Linux64bit+2.6-2.5-gcc46-prof

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${G4LIB}:${ROOTSYS}/lib:${XERCESCROOT}/lib

export PATH=${PATH}:${G4EXE}:${ROOTSYS}/bin

