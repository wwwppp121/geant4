#ifndef G4FISSION_STORE_HH
#define G4FISSION_STORE_HH

#include "G4FissionConfiguration.hh"

#include "g4std/vector"

class G4FissionStore {

public:

  G4FissionStore();

  void addConfig(G4double a, 
		 G4double z, 
		 G4double ez, 
		 G4double ek, 
		 G4double ev) {
    G4FissionConfiguration config(a, z, ez, ek, ev);
    configurations.push_back(config);
    // config.print();
  };

  G4int size() const { 
    return configurations.size(); 
  };

  G4FissionConfiguration generateConfiguration(G4double amax, 
					       G4double rand) const;

private:
  G4int verboseLevel;

  G4std::vector<G4FissionConfiguration> configurations;

};

#endif // G4FISSION_STORE_HH 



