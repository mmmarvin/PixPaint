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
#ifndef HASH_UTILITY_H
#define HASH_UTILITY_H

#include <string>

namespace pixpaint
{
namespace crypto_hash_utils
{
  std::string md5_file(const std::string& filename);
  std::string sha256_file(const std::string& filename);
  std::string sha512_file(const std::string& filename);

  std::string md5_string(const std::string& string);
  std::string sha256_string(const std::string& string);
  std::string sha512_string(const std::string& string);
}
}

#endif // HASH_UTILITY_H
