#ifndef STREAMERHEPREPTYPETREE_H
#define STREAMERHEPREPTYPETREE_H 1

#include "FreeHepTypes.h"

#include <string>
#include <vector>

#include "HEPREP/HepRep.h"
#include "HEPREP/HepRepTypeTree.h"
#include "HEPREP/HepRepType.h"
#include "HEPREP/HepRepWriter.h"
#include "HEPREP/HepRepTreeID.h"

#include "DefaultHepRepTreeID.h"

/**
 *
 * @author M.Donszelmann
 */
class StreamerHepRepTypeTree : public DefaultHepRepTreeID, public virtual HEPREP::HepRepTypeTree {

    public:
        StreamerHepRepTypeTree(HEPREP::HepRepWriter* streamer, HEPREP::HepRepTreeID* typeTree);
        ~StreamerHepRepTypeTree();

        HEPREP::HepRepTypeTree* copy(HEPREP::HepRep* heprep);
        HEPREP::HepRepTreeID* copy();
        bool addType(HEPREP::HepRepType* type);
        std::vector<HEPREP::HepRepType* >* getTypes();
};

#endif
