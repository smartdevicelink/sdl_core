#ifndef THIRDPARTYLIBS_ENCRYPTION_INCLUDE_ENCRYPTION_HASHING_H_
#define THIRDPARTYLIBS_ENCRYPTION_INCLUDE_ENCRYPTION_HASHING_H_

#include <string>
#include <openssl/sha.h>

namespace encryption {

std::string MakeHash(const std::string& input);

}  //  namespace encryption

#endif  //  THIRDPARTYLIBS_ENCRYPTION_INCLUDE_ENCRYPTION_HASHING_H_