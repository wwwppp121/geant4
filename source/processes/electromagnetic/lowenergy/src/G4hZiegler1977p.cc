// This code implementation is the intellectual property of
// the GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
// For information related to this code contact:
// Geant4 Collaboration
//
// File name:     G4hZiegler1977p
//
// Author:        V.Ivanchenko (Vladimir.Ivanchenko@cern.ch)
// 
// Creation date: 20 July 2000
//
// Modifications: 
// 20/07/2000  V.Ivanchenko First implementation
//
// Class Description: 
//
// Electronic stopping power parametrised according to
// H.H.Andersen & J.F.Ziegler, Hydrogen Stopping Powers and
// Ranges in All Elements, Vol.3, Pergamon Press, 1977
//
// Class Description: End 
//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4hZiegler1977p.hh"
#include "G4UnitsTable.hh"
#include "globals.hh"
#include "G4Material.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hZiegler1977p::G4hZiegler1977p():G4VhElectronicStoppingPower(), 
  protonMassAMU(1.007276)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4hZiegler1977p::~G4hZiegler1977p() 
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4hZiegler1977p::HasMaterial(const G4Material* material) 
{
  if(1 == (material->GetNumberOfElements())) return true;
  return false ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hZiegler1977p::StoppingPower(const G4Material* material,
                                              G4double kineticEnergy) 
{
  G4double ionloss = 0.0 ;

  // pure material (normally not the case for this function)
  if(1 == (material->GetNumberOfElements())) {
    G4double z = material->GetZ() ;
    G4double ionloss = ElectronicStoppingPower( z, kineticEnergy ) ;  
  }
  
  return ionloss;
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4hZiegler1977p::ElectronicStoppingPower(G4double z,
                                                  G4double kineticEnergy) const
{
  G4double ionloss ;
  G4int i = G4int(z)-1 ;  // index of atom
  if(i < 1)  i = 1 ;
  if(i > 92) i = 92 ;
  
  // The data and the fit from: 
  // H.H.Andersen & J.F.Ziegler Hydrogen Stopping Powers and
  // Ranges in All Elements, Vol.3, Pergamon Press, 1977
  // Proton kinetic energy in keV/amu  
  
  G4double kinE = kineticEnergy/(keV*protonMassAMU) ; 
  
  static G4double a[92][12] = {
    1.262,1.440,  242.6,12000.,0.115900,0.0005099,54360.0,-5.0520,2.0490,-0.30440,0.019660,-0.0004659,
    1.229,1.397,  484.5,5873.0,0.052250,0.0010200,24510.0,-2.1580,0.8278,-0.11720,0.007259,-0.000166,
    1.411,1.600,  725.6,3013.0,0.045780,0.0015300,21470.0,-0.5831,0.5620,-0.11830,0.009298,-0.0002498,
    2.248,2.590,  966.0, 153.8,0.034750,0.0020390,16300.0, 0.2779,0.1745,-0.05684,0.005155,-0.0001488,
    2.474,2.815, 1206.0,1060.0,0.028550,0.0025490,13450.0,-2.4450,1.2830,-0.22050,0.015600,-0.0003930,
    2.631,2.989, 1445.0, 957.2,0.028190,0.0030590,13220.0,-4.3800,2.0440,-0.32830,0.022210,-0.0005417,
    2.954,3.350, 1683.0,1900.0,0.025130,0.0035690,11790.0,-5.0540,2.3250,-0.37130,0.025060,-0.0006109,
    2.652,3.000, 1920.0,2000.0,0.022300,0.0040790,10460.0,-6.7340,3.0190,-0.47480,0.031710,-0.0007669,
    2.085,2.352, 2157.0,2634.0,0.018160,0.0045890,8517.0, -5.5710,2.4490,-0.37810,0.024830,-0.0005919,
    1.951,2.199, 2393.0,2699.0,0.015680,0.0050990,7353.0, -4.4080,1.8790,-0.28140,0.017960,-0.0004168,
    2.542,2.869, 2628.0,1854.0,0.014720,0.0056090,6905.0, -4.9590,2.0730,-0.30540,0.019210,-0.0004403,
    3.792,4.293, 2862.0,1009.0,0.013970,0.0061180,6551.0, -5.5100,2.2660,-0.32950,0.020470,-0.0004637,
    4.154,4.739, 2766.0, 164.5,0.020230,0.0066280,6309.0, -6.0610,2.4600,-0.35350,0.021730,-0.0004871,
    4.150,4.700, 3329.0, 550.0,0.013210,0.0071380,6194.0, -6.2940,2.5380,-0.36280,0.022200,-0.0004956,
    3.232,3.647, 3561.0,1560.0,0.012670,0.0076480,5942.0, -6.5270,2.6160,-0.37210,0.022670,-0.0005040,
    3.447,3.891, 3792.0,1219.0,0.012110,0.0081580,5678.0, -6.7610,2.6940,-0.38140,0.023140,-0.0005125,
    5.047,5.714, 4023.0, 878.6,0.011780,0.0086680,5524.0, -6.9940,2.7730,-0.39070,0.023610,-0.0005209,
    5.731,6.500, 4253.0, 530.0,0.011230,0.0091780,5268.0, -7.2270,2.8510,-0.40000,0.024070,-0.0005294,
    5.151,5.833, 4482.0, 545.7,0.011290,0.0096870,5295.0, -7.4400,2.9230,-0.40940,0.024620,-0.0005411,
    5.521,6.252, 4710.0, 553.3,0.011120,0.0102000,5214.0, -7.6530,2.9950,-0.41870,0.025160,-0.0005529,
    5.201,5.884, 4938.0, 560.9,0.009995,0.0107100,4688.0, -8.0120,3.1230,-0.43500,0.026050,-0.0005707,
    4.862,5.496, 5165.0, 568.5,0.009474,0.0112200,4443.0, -8.3710,3.2510,-0.45130,0.026940,-0.0005886,
    4.480,5.055, 5391.0, 952.3,0.009117,0.0117300,4276.0, -8.7310,3.3790,-0.46760,0.027830,-0.0006064,
    3.983,4.489, 5616.0,1336.0,0.008413,0.0122400,3946.0, -9.0900,3.5070,-0.48380,0.028720,-0.0006243,
    3.469,3.907, 5725.0,1461.0,0.008829,0.0127500,3785.0, -9.4490,3.6350,-0.50010,0.029610,-0.0006421,
    3.519,3.963, 6065.0,1243.0,0.007782,0.0132600,3650.0, -9.8090,3.7630,-0.51640,0.030500,-0.0006600,
    3.140,3.535, 6288.0,1372.0,0.007361,0.0137700,3453.0,-10.1700,3.8910,-0.53270,0.031390,-0.0006779,
    3.553,4.004, 6205.0, 555.1,0.008763,0.0142800,3297.0,-10.5300,4.0190,-0.54900,0.032290,-0.0006957,
    3.696,4.175, 4673.0, 387.8,0.021880,0.0147900,3174.0,-11.1800,4.2520,-0.57910,0.033990,-0.0007314,
    4.210,4.750, 6953.0, 295.2,0.006809,0.0153000,3194.0,-11.5700,4.3940,-0.59800,0.035060,-0.0007537,
    5.041,5.697, 7173.0, 202.6,0.006725,0.0158100,3154.0,-11.9500,4.5370,-0.61690,0.036130,-0.0007759,
    5.554,6.300, 6496.0, 110.0,0.009689,0.0163200,3097.0,-12.3400,4.6800,-0.63580,0.037210,-0.0007981,
    5.323,6.012, 7611.0, 292.5,0.006447,0.0168300,3024.0,-12.7200,4.8230,-0.65470,0.038280,-0.0008203,
    5.847,6.656, 7395.0, 117.5,0.007684,0.0173400,3006.0,-13.1100,4.9650,-0.67350,0.039350,-0.0008425,
    5.611,6.335, 8046.0, 365.2,0.006244,0.0178500,2928.0,-13.4000,5.0830,-0.69060,0.040420,-0.0008675,
    6.411,7.250, 8262.0, 220.0,0.006087,0.0183600,2855.0,-13.6900,5.2000,-0.70760,0.041500,-0.0008925,
    5.694,6.429, 8478.0, 292.9,0.006087,0.0188600,2855.0,-13.9200,5.2660,-0.71400,0.041730,-0.0008943,
    6.339,7.159, 8693.0, 330.3,0.006003,0.0193700,2815.0,-14.1400,5.3310,-0.72050,0.041960,-0.0008962,
    6.407,7.234, 8907.0, 367.8,0.005889,0.0198800,2762.0,-14.3600,5.3970,-0.72690,0.042190,-0.0008980,
    6.734,7.603, 9120.0, 405.2,0.005765,0.0203900,2704.0,-14.5900,5.4630,-0.73330,0.042420,-0.0008998,
    6.902,7.791, 9333.0, 442.7,0.005587,0.0209000,2621.0,-16.2200,6.0940,-0.82250,0.047910,-0.0010240,
    6.425,7.248, 9545.0, 480.2,0.005367,0.0214100,2517.0,-17.8500,6.7250,-0.91160,0.053390,-0.0011480,
    6.799,7.671, 9756.0, 517.6,0.005315,0.0219200,2493.0,-17.9600,6.7520,-0.91350,0.053410,-0.001147,
    6.108,6.887, 9966.0, 555.1,0.005151,0.0224300,2416.0,-18.0700,6.7790,-0.91540,0.053420,-0.0011450,
    5.924,6.677,10180.0, 592.5,0.004919,0.0229400,2307.0,-18.1800,6.8060,-0.91730,0.053430,-0.0011430,
    5.238,5.900,10380.0, 630.0,0.004758,0.0234500,2231.0,-18.2800,6.8330,-0.91920,0.053450,-0.0011420,
    5.623,6.354, 7160.0, 337.6,0.013940,0.0239600,2193.0,-18.3900,6.8600,-0.92110,0.053460,-0.0011400,
    5.814,6.554,10800.0, 355.5,0.004626,0.0244700,2170.0,-18.6200,6.9150,-0.92430,0.053400,-0.0011340,
    6.230,7.024,11010.0, 370.9,0.004540,0.0249800,2129.0,-18.8500,6.9690,-0.92750,0.053350,-0.0011270,
    6.410,7.227,11210.0, 386.4,0.004474,0.0254900,2099.0,-19.0700,7.0240,-0.93080,0.053290,-0.0011210,
    7.500,8.480, 8608.0, 348.0,0.009074,0.0260000,2069.0,-19.5700,7.2250,-0.96030,0.055180,-0.0011650,
    6.979,7.871,11620.0, 392.4,0.004402,0.0265100,2065.0,-20.0700,7.4260,-0.98990,0.057070,-0.0012090,
    7.725,8.716,11830.0, 394.8,0.004376,0.0270200,2052.0,-20.5600,7.6270,-1.01900,0.058960,-0.0012540,
    8.231,9.289,12030.0, 397.3,0.004384,0.0275300,2056.0,-21.0600,7.8280,-1.04900,0.060850,-0.0012980,
    7.287,8.218,12230.0, 399.7,0.004447,0.0280400,2086.0,-20.4000,7.5400,-1.00400,0.057820,-0.0012240,
    7.899,8.911,12430.0, 402.1,0.004511,0.0285500,2116.0,-19.7400,7.2520,-0.95880,0.054790,-0.0011510,
    8.041,9.071,12630.0, 404.5,0.004540,0.0290600,2129.0,-19.0800,6.9640,-0.91360,0.051760,-0.0010770,
    7.489,8.444,12830.0, 406.9,0.004420,0.0295700,2073.0,-18.4300,6.6770,-0.86840,0.048720,-0.0010030,
    7.291,8.219,13030.0, 409.3,0.004298,0.0300800,2016.0,-17.7700,6.3890,-0.82330,0.045690,-0.0009292,
    7.098,8.000,13230.0, 411.8,0.004182,0.0305900,1962.0,-17.1100,6.1010,-0.77810,0.042660,-0.0008553,
    6.910,7.786,13430.0, 414.2,0.004058,0.0311000,1903.0,-16.4500,5.8130,-0.73300,0.039630,-0.0007815,
    6.728,7.580,13620.0, 416.6,0.003976,0.0316100,1865.0,-15.7900,5.5260,-0.68780,0.036600,-0.0007077,
    6.551,7.380,13820.0, 419.0,0.003877,0.0321200,1819.0,-15.1300,5.2380,-0.64260,0.033570,-0.0006339,
    6.739,7.592,14020.0, 421.4,0.003863,0.0326300,1812.0,-14.4700,4.9500,-0.59750,0.030530,-0.0005601,
    6.212,6.996,14210.0, 423.9,0.003725,0.0331400,1747.0,-14.5600,4.9840,-0.60220,0.030820,-0.0005668,
    5.517,6.210,14400.0, 426.3,0.003632,0.0336500,1703.0,-14.6500,5.0180,-0.60690,0.031110,-0.0005734,
    5.219,5.874,14600.0, 428.7,0.003498,0.0341600,1640.0,-14.7400,5.0510,-0.61170,0.031410,-0.0005801,
    5.071,5.706,14790.0, 433.0,0.003405,0.0346700,1597.0,-14.8300,5.0850,-0.61640,0.031700,-0.0005867,
    4.926,5.542,14980.0, 433.5,0.003342,0.0351800,1567.0,-14.9100,5.1190,-0.62110,0.031990,-0.0005933,
    4.787,5.386,15170.0, 435.9,0.003292,0.0356900,1544.0,-15.0000,5.1530,-0.62580,0.032280,-0.0006000,
    4.893,5.505,15360.0, 438.4,0.003243,0.0362000,1521.0,-15.0900,5.1860,-0.63050,0.032570,-0.0006066,
    5.028,5.657,15550.0, 440.8,0.003195,0.0367100,1499.0,-15.1800,5.2200,-0.63530,0.032860,-0.0006133,
    4.738,5.329,15740.0, 443.2,0.003186,0.0372200,1494.0,-15.2700,5.2540,-0.64000,0.033150,-0.0006199,
    4.574,5.144,15930.0, 442.4,0.003144,0.0377300,1475.0,-15.6700,5.3920,-0.65770,0.034180,-0.0006426,
    5.200,5.851,16120.0, 441.6,0.003122,0.0382400,1464.0,-16.0700,5.5290,-0.67550,0.035210,-0.0006654,
    5.070,5.704,16300.0, 440.9,0.003082,0.0387500,1446.0,-16.4700,5.6670,-0.69320,0.036240,-0.0006881,
    4.945,5.563,16490.0, 440.1,0.002965,0.0392600,1390.0,-16.8800,5.8040,-0.71100,0.037270,-0.0007109,
    4.476,5.034,16670.0, 439.3,0.002871,0.0397700,1347.0,-17.2800,5.9420,-0.72870,0.038300,-0.0007336,
    4.856,5.460,18320.0, 438.5,0.002542,0.0402800,1354.0,-17.0200,5.8460,-0.71490,0.037400,-0.0007114,
    4.308,4.843,17040.0, 487.8,0.002882,0.0407900,1352.0,-17.8400,6.1830,-0.76590,0.040760,-0.0007925,
    4.723,5.311,17220.0, 537.0,0.002913,0.0413000,1366.0,-18.6600,6.5200,-0.81690,0.044110,-0.0008737,
    5.319,5.982,17400.0, 586.3,0.002871,0.0418100,1347.0,-19.4800,6.8570,-0.86780,0.047470,-0.0009548,
    5.956,6.700,17800.0, 677.0,0.002660,0.0423200,1336.0,-19.5500,6.8710,-0.86860,0.047480,-0.0009544,
    6.158,6.928,17770.0, 586.3,0.002812,0.0428300,1319.0,-19.6200,6.8840,-0.86940,0.047480,-0.0009540,
    6.204,6.979,17950.0, 586.3,0.002776,0.0433400,1302.0,-19.6900,6.8980,-0.87020,0.047490,-0.0009536,
    6.181,6.954,18120.0, 586.3,0.002748,0.0438500,1289.0,-19.7600,6.9120,-0.87100,0.047490,-0.0009532,
    6.949,7.820,18300.0, 586.3,0.002737,0.0443600,1284.0,-19.8300,6.9260,-0.87180,0.047500,-0.0009528,
    7.506,8.448,18480.0, 586.3,0.002727,0.0448700,1279.0,-19.9000,6.9400,-0.87260,0.047510,-0.0009524,
    7.649,8.609,18660.0, 586.3,0.002697,0.0453800,1265.0,-19.9700,6.9530,-0.87330,0.047510,-0.0009520,
    7.710,8.679,18830.0, 586.3,0.002641,0.0458900,1239.0,-20.0400,6.9670,-0.87410,0.047520,-0.0009516,
    7.407,8.336,19010.0, 586.3,0.002603,0.0464000,1221.0,-20.1100,6.9810,-0.87490,0.047520,-0.0009512,
    7.290,8.204,19180.0, 586.3,0.002573,0.0469100,1207.0,-20.1800,6.9950,-0.87570,0.047530,-0.0009508
  };
  
  if ( kinE < 10.0 ) {
    ionloss = a[i][0] * sqrt(kinE) ;
    
  } else if ( kinE < 1000.0 ) {
    G4double slow  = a[i][1] * pow(kinE, 0.45) ;
    G4double shigh = log( 1.0 + a[i][3]/kinE + a[i][4]*kinE ) * a[i][2]/kinE ;
    ionloss = slow*shigh / (slow + shigh) ; 
    
  } else {
    G4double le = log(kinE) ;
    G4double gam = 1.0 + kineticEnergy / proton_mass_c2 ;
    G4double beta2 = 1.0 - 1.0/ (gam*gam) ;
    ionloss = ( log(a[i][6]*beta2/(1.0 - beta2)) - beta2 -
		a[i][7] - a[i][8]*le - a[i][9]*le*le - a[i][10]*le*le*le -
		a[i][11]*le*le*le*le ) * a[i][5]/beta2 ;
  }
  
  if ( ionloss < 0.0) ionloss = 0.0 ;
  
  return ionloss;
}

