// Copyright FreeHEP, 2005.

#include <iostream>
#include <ctime>
#include <vector>

#include "cheprep/ZipOutputStreamBuffer.h"
#include "cheprep/ZipOutputStream.h"

/**
 * @author Mark Donszelmann
 * @version $Id: ZipOutputStream.cc,v 1.2 2005-05-13 17:25:13 duns Exp $
 */
namespace cheprep {

    ZipOutputStream::ZipOutputStream(std::ostream& os) : std::ostream(0) {
        buffer = new ZipOutputStreamBuffer(os.rdbuf());
        
        init(buffer);
    }
    
    void ZipOutputStream::closeEntry() {
        buffer->closeEntry();
    }


    void ZipOutputStream::close() {
        buffer->close();
    }

    void ZipOutputStream::putNextEntry(const std::string& name, bool compress) {
        buffer->putNextEntry(name, compress);
    }

    void ZipOutputStream::setComment(const std::string& comment ) {
        buffer->setComment(comment);
    }

    ZipOutputStream::~ZipOutputStream() {
        close();
        delete buffer;
    }

} // cheprep
