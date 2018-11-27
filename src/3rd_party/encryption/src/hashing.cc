#include "encryption/hashing.h"
#include <sstream>
#include <iomanip>

namespace encryption {

std::string MakeHash(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha_hasher;
    SHA256_Init(&sha_hasher);
    SHA256_Update(&sha_hasher, input.c_str(), input.size());
    SHA256_Final(hash, &sha_hasher);
    std::stringstream ss;
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

}  //  namespace encryption