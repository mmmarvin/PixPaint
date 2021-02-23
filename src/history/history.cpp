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
#include "history.h"

#include "../image/animation.h"
#include "../image/image.h"
#include "../event/gui/history_events.h"
#include "../manager/documentmanager.h"
#include "../manager/eventmanager.h"
#include "../manager/imagemanager.h"
#include "../document.h"

namespace pixpaint
{
namespace
{
  void tryAddTo(std::list<std::unique_ptr<HistoryAction>>& l, std::unique_ptr<HistoryAction> action)
  {
    if(l.size() == 10) {
      l.pop_front();
    }

    l.push_back(std::move(action));
  }
}
  HistoryAction::EActionResult History::undo()
  {
    auto a = std::move(m_undo.back());
    m_undo.pop_back();

    // try to switch to the layer index and frame index where the action
    // was made
    trySetIndexes(*a);

    auto res = a->undo();
    tryAddRedo(std::move(a));

    return res;
  }

  HistoryAction::EActionResult History::redo()
  {
    auto a = std::move(m_redo.back());
    m_redo.pop_back();

    // try to switch to the layer index and frame index where the action
    // was made
    trySetIndexes(*a);

    auto res = a->redo();
    tryAddUndo(std::move(a));

    return res;
  }

  bool History::canUndo() const
  {
    return !m_undo.empty();
  }

  bool History::canRedo() const
  {
    return !m_redo.empty();
  }

  const std::string& History::getNextUndoName() const
  {
    return m_undo.back()->getName();
  }

  const std::string& History::getNextRedoName() const
  {
    return m_redo.back()->getName();
  }

  void History::trySetIndexes(HistoryAction& action)
  {
    auto frame_index = action.m_frameIndex;
    auto layer_index = action.m_layerIndex;

    auto& animation = getDocumentManager().getDocument().getAnimation();
    if(frame_index != animation.getCurrentFrameIndex()) {
      if(frame_index < animation.getFrameCount()) {
        animation.setCurrentFrameIndex(frame_index);
        emitEvent(gui_events::HistoryFrameChangeEvent{ frame_index });
      }
    }

    if(layer_index != animation.getCurrentFrame().getCurrentLayerIndex()) {
      if(layer_index < animation.getCurrentFrame().getLayerCount()) {
        animation.getCurrentFrame().setCurrentLayerIndex(layer_index);
        emitEvent(gui_events::HistoryLayerChangeEvent { layer_index });
      }
    }
  }

  void History::tryGetIndexes(HistoryAction& action)
  {
    action.m_frameIndex = getDocumentManager().getDocument().getAnimation().getCurrentFrameIndex();
    action.m_layerIndex = getImageManager().getImage().getCurrentLayerIndex();
  }

  void History::tryAddUndo(std::unique_ptr<HistoryAction> action)
  {
    tryAddTo(m_undo, std::move(action));
  }

  void History::tryAddRedo(std::unique_ptr<HistoryAction> action)
  {
    tryAddTo(m_redo, std::move(action));
  }
}
