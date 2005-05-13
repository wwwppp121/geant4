// Copyright FreeHEP, 2005.
#ifndef CHEPREP_ZIPOUTPUTSTREAM_H
#define CHEPREP_ZIPOUTPUTSTREAM_H

#include <string>
#include <iostream>
#include <vector>


/**
 * @author Mark Donszelmann
 * @version $Id: ZipOutputStream.h,v 1.2 2005-05-13 17:25:13 duns Exp $
 */
namespace cheprep {

    class ZipOutputStreamBuffer;

    class ZipOutputStream : public std::ostream {

        public:

            ZipOutputStream(std::ostream& os);
  
            void closeEntry();

            void close();

            void putNextEntry(const std::string& name, bool compress);
            
            void setComment(const std::string& comment);

            virtual ~ZipOutputStream();

        private:
            ZipOutputStreamBuffer* buffer;
    };
 
} // cheprep

#endif // CHEPREP_ZIPOUTPUTSTREAM_H
