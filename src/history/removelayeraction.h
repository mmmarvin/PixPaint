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
#ifndef REMOVELAYERACTION_H
#define REMOVELAYERACTION_H

#include <memory>
#include <string>
#include "historyaction.h"

namespace pixpaint
{
  class MaskablePixelData;
  class RemoveLayerAction : public HistoryAction
  {
  public:
    RemoveLayerAction(size_t layerIndex);

    EActionResult undo() override final;
    EActionResult redo() override final;

  private:
    std::unique_ptr<MaskablePixelData>  m_layer;
    std::string                         m_layerName;
    size_t                              m_index;
  };
}

#endif // REMOVELAYERACTION_H
