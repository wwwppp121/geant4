// Copyright FreeHEP, 2005.
#ifndef CHEPREP_DEFAULTHEPREPACTION_H
#define CHEPREP_DEFAULTHEPREPACTION_H 1

#include "cheprep/config.h"

#include <string>

#include "HEPREP/HepRepAction.h"

/**
 * @author Mark Donszelmann
 * @version $Id: DefaultHepRepAction.h,v 1.2 2005-05-25 23:21:59 duns Exp $
 */
namespace cheprep {

class DefaultHepRepAction : public virtual HEPREP::HepRepAction {

    private:
        std::string name;
        std::string expression;

    public:
        DefaultHepRepAction(std::string name, std::string expression);
        ~DefaultHepRepAction();

        std::string getName();
        std::string getExpression();
        HEPREP::HepRepAction* copy();
};

} // cheprep


#endif
