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
#ifndef PREVIEWMANAGER_H
#define PREVIEWMANAGER_H

#include <cinttypes>
#include "../3rdparty/ska/unordered_map.hpp"
#include "../idgenerator.h"
#include "../pixpaint_macro.h"

namespace pixpaint
{
  class PreviewView;

  class PreviewManager
  {
    PIXPAINT_SINGLETON_FUNC_FRIEND(PreviewManager)
    PreviewManager() {}

  public:
    void registerPreview(PreviewView& view);
    void deregisterPreview(PreviewView& view);
    void deregisterAll();

    void refreshAll();
    void refresh(std::size_t index);
    void refreshResizeAll();
    void refreshResize(std::size_t index);

  private:
    ska::unordered_map<std::uintptr_t, std::size_t> m_previewIndex;
    std::vector<PreviewView*>                       m_previews;
    IDGenerator                                     m_idGenerator;
  };

  PIXPAINT_SINGLETON_FUNC(PreviewManager)
}

#endif // PREVIEWMANAGER_H
