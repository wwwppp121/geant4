// Copyright FreeHEP, 2005.
#ifndef CHEPREP_DEFAULTHEPREPTREEID_H
#define CHEPREP_DEFAULTHEPREPTREEID_H 1

#include "cheprep/config.h"

#include <string>

#include "HEPREP/HepRepTreeID.h"

/**
 * @author Mark Donszelmann
 * @version $Id: DefaultHepRepTreeID.h,v 1.2 2005-05-25 23:21:59 duns Exp $
 */
namespace cheprep {

class DefaultHepRepTreeID : public virtual HEPREP::HepRepTreeID {

    private:
        std::string name;
        std::string version;
        std::string qualifier;

    public:
        DefaultHepRepTreeID(std::string name, std::string version, std::string qualifier = "top_level");
        ~DefaultHepRepTreeID();

        std::string getQualifier();
        void setQualifier(std::string qualifier);
        std::string getName();
        std::string getVersion();
};

} // cheprep


#endif
