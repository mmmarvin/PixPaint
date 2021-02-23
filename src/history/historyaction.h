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
#ifndef HISTORYACTION_H
#define HISTORYACTION_H

#include <cstddef>
#include <string>

namespace pixpaint
{
  class HistoryAction
  {
  public:
    enum EActionResult : unsigned int
    {
      EAR_REFRESH_IMAGE = 1 << 0,
      EAR_REFRESH_IMAGE_SIZE = 1 << 1,
      EAR_RECREATE_LAYER_LIST = 1 << 2,
      EAR_RECREATE_FRAME_LIST = 1 << 3,
      EAR_REFRESH_LAYER_LIST = 1 << 4
//      EAR_REFRESH_FRAME_LIST = 1 << 5
    };

    explicit HistoryAction(std::string name = "");
    virtual ~HistoryAction() {}

    virtual EActionResult undo() = 0;
    virtual EActionResult redo() = 0;
    const std::string& getName() const;

  protected:
    static EActionResult toEActionResult(unsigned int i);

    size_t getFrameIndex() const;
    size_t getLayerIndex() const;

  private:
    friend class History;

    size_t      m_frameIndex;
    size_t      m_layerIndex;
    std::string m_name;
  };
}

#endif // HISTORYACTION_H
