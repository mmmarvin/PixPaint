/**********
 *
 *     PixPaint
 *     Copyright 2020-2021 Marvin Manese
 *
 *     This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **********/
#include "hash_utility.h"

#include <array>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <openssl/sha.h>

namespace pixpaint
{
namespace crypto_hash_utils
{
namespace
{
  std::string to_hex(unsigned char c)
  {
    std::stringstream ss;
    ss << std::hex <<
          std::setw(2) <<
          std::setfill('0') <<
          static_cast<int>(c);
    return ss.str();
  }

  template<class HashCtx, size_t DigestLength>
  std::string compute_hash_file(const std::string& filename,
                                int (*init_func)(HashCtx*),
                                int (*update_func)(HashCtx*, const void*, size_t),
                                int (*finalize_func)(unsigned char*, HashCtx*))
  {
    std::ifstream in(filename, std::ios_base::binary);
    if(in.is_open()) {
      std::array<unsigned char, DigestLength> hash;

      HashCtx hash_ctx;
      if(!(*init_func)(&hash_ctx)) {
        return "";
      }

      std::array<unsigned char, 512> buff;
      in.read(reinterpret_cast<char*>(buff.data()), buff.size());
      auto read = in.gcount();
      while(read) {
        if(!(*update_func)(&hash_ctx, buff.data(), read)) {
          return "";
        }

        in.read(reinterpret_cast<char*>(buff.data()), buff.size());
        read = in.gcount();
      }
      if(!(*finalize_func)(hash.data(), &hash_ctx)) {
        return "";
      }

      std::string ret;
      for(size_t i = 0; i < DigestLength; ++i) {
        ret += to_hex(hash[i]);
      }

      return ret;
    }

    return "";
  }

  template<class HashCtx, size_t DigestLength>
  std::string compute_hash_string(const std::string& string,
                                  int (*init_func)(HashCtx*),
                                  int (*update_func)(HashCtx*, const void*, size_t),
                                  int (*finalize_func)(unsigned char*, HashCtx*))
  {
    std::array<unsigned char, DigestLength> hash;

    HashCtx hash_ctx;
    if(!(*init_func)(&hash_ctx) ||
       !(*update_func)(&hash_ctx, string.c_str(), string.size()) ||
       !(*finalize_func)(hash.data(), &hash_ctx)) {
      return "";
    }

    std::string ret;
    for(size_t i = 0; i < DigestLength; ++i) {
      ret += to_hex(hash[i]);
    }

    return ret;
  }
}
  std::string md5_file(const std::string& filename)
  {
    return compute_hash_file<MD5_CTX, MD5_DIGEST_LENGTH>(filename, &MD5_Init, &MD5_Update, &MD5_Final);
  }

  std::string sha256_file(const std::string& filename)
  {
    return compute_hash_file<SHA256_CTX, SHA256_DIGEST_LENGTH>(filename, &SHA256_Init, &SHA256_Update, &SHA256_Final);
  }

  std::string sha512_file(const std::string& filename)
  {
    return compute_hash_file<SHA512_CTX, SHA512_DIGEST_LENGTH>(filename, &SHA512_Init, &SHA512_Update, &SHA512_Final);
  }

  std::string md5_string(const std::string& string)
  {
    return compute_hash_string<MD5_CTX, MD5_DIGEST_LENGTH>(string, &MD5_Init, &MD5_Update, &MD5_Final);
  }

  std::string sha256_string(const std::string& string)
  {
    return compute_hash_string<SHA256_CTX, SHA256_DIGEST_LENGTH>(string, &SHA256_Init, &SHA256_Update, &SHA256_Final);
  }

  std::string sha512_string(const std::string& string)
  {
    return compute_hash_string<SHA512_CTX, SHA512_DIGEST_LENGTH>(string, &SHA512_Init, &SHA512_Update, &SHA512_Final);
  }
}
}
