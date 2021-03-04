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
#include "consolemanager.h"

#include <chrono>
#include "../env/guienvironment.h"
#include "../window/consoletoolbox.h"

namespace pixpaint
{
  void ConsoleManager::writeMessage(std::string msg)
  {
    auto remove_newline_and_space = [](std::string s) {
      while(s.size() &&
            (isspace(s.back()) || s.back() == '\n')) {
        s.pop_back();
      }

      return s;
    };

    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto current_time = std::string(std::ctime(&t));
    auto cmsg = remove_newline_and_space(current_time) + std::string(": ") + std::move(msg);

    writeMessageImpl(std::move(cmsg));
  }

  void ConsoleManager::writeMessageSystem(std::string msg)
  {
    writeMessageImpl(std::move(msg));
  }

  void ConsoleManager::clearMessages()
  {
    getGUIEnvironment().getConsoleToolbox().clearMessages();
  }

  void ConsoleManager::writeMessageImpl(std::string msg)
  {
    getGUIEnvironment().getConsoleToolbox().writeMessage(std::move(msg));
  }

  PIXPAINT_SINGLETON_FUNC_DEF(ConsoleManager)
}
