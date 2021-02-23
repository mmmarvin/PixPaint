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
#include "previewmanager.h"

#include "../window/previewview.h"

namespace pixpaint
{
  void PreviewManager::registerPreview(PreviewView& view)
  {
    if(m_idGenerator.isThereID()) {
      auto index_id = m_idGenerator.getID();
      m_previews[index_id] = &view;
      m_previewIndex[reinterpret_cast<std::uintptr_t>(&view)] = index_id;
    } else {
      m_previewIndex[reinterpret_cast<std::uintptr_t>(&view)] = m_previews.size();
      m_previews.push_back(&view);
    }
  }

  void PreviewManager::deregisterPreview(PreviewView& view)
  {
    auto view_id = reinterpret_cast<std::uintptr_t>(&view);
    auto find_it = m_previewIndex.find(view_id);
    if(find_it != m_previewIndex.end()) {
      auto index_id = find_it->second;
      m_idGenerator.recycleID(index_id);
      m_previews[index_id] = nullptr;
      m_previewIndex.erase(view_id);
    }
  }

  void PreviewManager::deregisterAll()
  {
    m_previews.clear();
    m_idGenerator.clear();
    m_previewIndex.clear();
  }

  void PreviewManager::refreshAll()
  {
    // if the preview size == id generator size, then we know that all
    // the preview IDs have been recycled, so don't do any refresh
    if(m_idGenerator.count() != m_previews.size()) {
      for(auto* preview : m_previews) {
        if(preview) {
          preview->repaint();
        }
      }
    }
  }

  void PreviewManager::refresh(std::size_t index)
  {
    if(m_previews[index]) {
      m_previews[index]->repaint();
    }
  }

  void PreviewManager::refreshResizeAll()
  {
    // if the preview size == id generator size, then we know that all
    // the preview IDs have been recycled, so don't do any refresh
    if(m_idGenerator.count() != m_previews.size()) {
      for(auto* preview : m_previews) {
        if(preview) {
          preview->refreshResize();
        }
      }
    }
  }

  void PreviewManager::refreshResize(std::size_t index)
  {
    if(m_previews[index]) {
      m_previews[index]->refreshResize();
    }
  }

  PIXPAINT_SINGLETON_FUNC_DEF(PreviewManager)
}
