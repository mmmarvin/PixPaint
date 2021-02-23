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
#ifndef RENAMELAYERACTION_H
#define RENAMELAYERACTION_H

#include <string>
#include "historyaction.h"

namespace pixpaint
{
  class RenameLayerAction : public HistoryAction
  {
  public:
    RenameLayerAction(std::string from, std::string to);

    EActionResult undo() override final;
    EActionResult redo() override final;

  private:
    std::string m_from;
    std::string m_to;
  };
}

#endif // RENAMELAYERACTION_H
