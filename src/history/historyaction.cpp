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
#include "historyaction.h"

namespace pixpaint
{
  HistoryAction::HistoryAction(std::string name) :
    m_name(std::move(name))
  {
  }

  const std::string& HistoryAction::getName() const
  {
    return m_name;
  }

  HistoryAction::EActionResult HistoryAction::toEActionResult(unsigned int i)
  {
    return static_cast<EActionResult>(i);
  }

  size_t HistoryAction::getFrameIndex() const
  {
    return m_frameIndex;
  }

  size_t HistoryAction::getLayerIndex() const
  {
    return m_layerIndex;
  }
}
