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
#include "string_utility.h"

#include <algorithm>
#include <sstream>

namespace gengine2d
{
namespace string_utils
{
namespace
{
  bool partialMatchSeekForward(const std::string& substr, const std::string& str)
  {
    auto it1 = substr.begin();
    auto it1End = substr.end();
    auto it2 = str.begin();
    for(; it1 != it1End; ) {
      if(*it1++ != *it2++) {
        return false;
      }
    }

    return true;
  }

  bool partialMatchSeekBackward(const std::string& substr, const std::string& str)
  {
    auto it1 = substr.rbegin();
    auto it1End = substr.rend();
    auto it2 = str.rbegin();
    for(; it1 != it1End; ) {
      if(*it1++ != *it2++) {
        return false;
      }
    }

    return true;
  }
}
  std::string toLower(std::string str)
  {
    std::transform(str.begin(), str.end(), str.begin(), [](char c) {
      return static_cast<char>(tolower(static_cast<unsigned char>(c)));
    });

    return str;
  }

  std::string toUpper(std::string str)
  {
    std::transform(str.begin(), str.end(), str.begin(), [](char c) {
      return static_cast<char>(toupper(static_cast<unsigned char>(c)));
    });

    return str;
  }

  std::vector<std::string> tokenize(std::string str, std::vector<char> delimiters)
  {
    std::vector<std::string> ret;
    std::size_t i = 0;
    while(i < str.size()) {
      while(i < str.size() && isspace(str[i])) {
        ++i;
      }

      char endChar = ' ';
      auto firstChar = str[i];
      for(auto c : delimiters) {
        if(firstChar == c) {
          endChar = firstChar;
          ++i;
          break;
        }
      }

      auto j = i;
      while(j < str.size() && str[j] != endChar) {
        ++j;
      }

      if(i < j) {
        ret.push_back(str.substr(i, j - i));
        i = j + 1;
      }
    }

    return ret;
  }

  std::vector<std::string> tokenize2(std::string str, char delimiter)
  {
    return tokenizeIf(str, [delimiter](char c) { return c == delimiter; });
  }

  std::string combineTokens(const std::vector<std::string>& tokens,
                         char delimiter,
                         std::size_t start,
                         std::size_t end)
  {
    std::string ret;
    if(start < tokens.size()) {
      ret += tokens[start];
      for(std::size_t i = start + 1; i < end; ++i) {
        ret.push_back(delimiter);
        ret += tokens[i];
      }
    }

    return ret;
  }

  std::wstring toWideString(std::string str)
  {
    return std::wstring(str.begin(), str.end());
  }

  std::string toNarrowString(std::wstring str)
  {
    return std::string(str.begin(), str.end());
  }

  std::string tryRemoveQuotations(std::string str)
  {
    if(str.size() > 2 && str.front() == '\"' && str.back() == '\"') {
      str.erase(str.begin());
      str.pop_back();
    }

    return str;
  }

  bool partialMatch(const std::string& substr, const std::string& str, ESeekDirection dir)
  {
    if(substr.size() <= str.size()) {
      if(dir == ESeekDirection::SD_BEG) {
        return partialMatchSeekForward(substr, str);
      } else if(dir == ESeekDirection::SD_END) {
        return partialMatchSeekBackward(substr, str);
      }
    }

    return false;
  }

  bool matchBeginning(const std::string& substr, const std::string& str)
  {
    return partialMatch(substr, str, ESeekDirection::SD_BEG);
  }

  bool matchEnding(const std::string& substr, const std::string& str)
  {
    return partialMatch(substr, str, ESeekDirection::SD_END);
  }
}
}
