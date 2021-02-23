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
#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

#include <string>
#include <vector>
#include "base.h"

namespace gengine2d
{
namespace string_utils
{
  std::string toLower(std::string str);
  std::string toUpper(std::string str);

  std::vector<std::string> tokenize(std::string str, std::vector<char> delimiters = {});
  std::vector<std::string> tokenize2(std::string str, char delimiter);
  template<class Func> std::vector<std::string> tokenizeIf(std::string str, Func&& func);

  std::string combineTokens(const std::vector<std::string>& tokens,
                         char delimiter,
                         std::size_t start,
                         std::size_t end);

  std::wstring toWideString(std::string str);
  std::string toNarrowString(std::wstring str);

  std::string tryRemoveQuotations(std::string str);

  bool partialMatch(const std::string& substr, const std::string& str, ESeekDirection dir = ESeekDirection::SD_BEG);
  bool matchBeginning(const std::string& substr, const std::string& str);
}
}

#include "string_utility.inl"

#endif // STRING_UTILITY_H
