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
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto ts = std::string(std::ctime(&t));
    if(ts.back() == '\n')
      ts.pop_back();
    auto cmsg = std::move(ts) + std::string(": ") + std::move(msg);

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
