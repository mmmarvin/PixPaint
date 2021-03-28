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
#include "version_specific_headers.h"

#if defined(USE_CXX14)
namespace version_specific
{
namespace filesystem
{
  bool exists(const path& filename)
  {
    return boost::filesystem::exists(filename);
  }

  bool create_directory(const path& p)
  {
    return boost::filesystem::create_directory(p);
  }

  path current_path()
  {
    return boost::filesystem::current_path();
  }
}
}
#elif defined(USE_CXX17)
namespace version_specific
{
namespace filesystem
{
  bool exists(const path& filename)
  {
    return std::filesystem::exists(filename);
  }

  bool create_directory(const path& p)
  {
    return std::filesystem::create_directory(p);
  }

  path current_path()
  {
    return std::filesystem::current_path();
  }
}
}
#endif // __cplusplus < 201703L || (defined(_MSC_VER) && (_MSC_VER < 1914))
