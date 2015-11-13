#ifndef THIRDPARTYLIBS_ENCRYPTION_INCLUDE_ENCRYPTION_HASHING_H_
#define THIRDPARTYLIBS_ENCRYPTION_INCLUDE_ENCRYPTION_HASHING_H_

#include <string>
#ifdef OS_WIN32
// do nothing
#else
#include <openssl/sha.h>
#endif

namespace encryption {

std::string MakeHash(const std::string& input);

}  //  namespace encryption

#endif  //  THIRDPARTYLIBS_ENCRYPTION_INCLUDE_ENCRYPTION_HASHING_H_