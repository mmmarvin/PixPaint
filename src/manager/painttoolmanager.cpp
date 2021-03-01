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
#include "painttoolmanager.h"

#include "../env/imageenvironment.h"
#include "../env/guienvironment.h"
#include "../helper/selection_helpers.h"
#include "../helper/tool_helpers.h"
#include "../tool/painttoolbase.h"
#include "../registrar/painttoolregistrar.h"
#include "../window/imageeditorview.h"
#include "../window/lefttoolbox.h"
#include "../window/selectionwidget.h"
#include "../window/zoomablescrollarea.h"
#include "../assert.h"
#include "imagemanager.h"

namespace pixpaint
{
  PaintToolManager::PaintToolManager() noexcept :
    m_currentToolIndex(-1),
    m_previousToolIndex(-1)
  {
  }

  void PaintToolManager::setCurrentTool(size_t index) noexcept
  {
    if(index >= getPaintToolRegistrar().size()) {
      return;
    }

    if(m_currentToolIndex != -1) {
      m_previousToolIndex = m_currentToolIndex;
    }
    m_currentToolIndex = index;

    selection_helpers::tryFinalizeTextSelection(true);
    tool_helpers::updateViewToolCursor();

    this->notifyObservers();
//    getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::NORMAL);

    tool_helpers::onToolChange(getCurrentTool(), &PaintToolHandlerBase::onToolChange);
    getImageEnvironment().getView().setFocus();
  }

  void PaintToolManager::setToPreviousTool() noexcept
  {
    if(m_previousToolIndex != -1) {
      m_currentToolIndex = m_previousToolIndex;
      m_previousToolIndex = -1;

      selection_helpers::tryFinalizeTextSelection(true);
      tool_helpers::updateViewToolCursor();

      this->notifyObservers();
//      getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::NORMAL);

      tool_helpers::onToolChange(getCurrentTool(), &PaintToolHandlerBase::onToolChange);
      getImageEnvironment().getView().setFocus();
    }
  }

  bool PaintToolManager::currentToolSet() const noexcept
  {
    return m_currentToolIndex != -1;
  }

  bool PaintToolManager::previousToolSet() const noexcept
  {
    return m_previousToolIndex != -1;
  }

  PaintToolHandlerBase& PaintToolManager::getCurrentTool() noexcept
  {
    PIXPAINT_ASSERT(m_currentToolIndex >= 0, "Current tool was not initialized");
    return (getPaintToolRegistrar().begin() + m_currentToolIndex)->getTool();
  }

  const PaintToolHandlerBase& PaintToolManager::getCurrentTool() const noexcept
  {
    PIXPAINT_ASSERT(m_currentToolIndex >= 0, "Current tool was not initialized");
    return (getPaintToolRegistrar().begin() + m_currentToolIndex)->getTool();
  }

  std::make_signed_t<size_t> PaintToolManager::getCurrentToolIndex() const noexcept
  {
    return m_currentToolIndex;
  }

  PIXPAINT_SINGLETON_FUNC_DEF(PaintToolManager)
}
