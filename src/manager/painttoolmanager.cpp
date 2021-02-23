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
#include "../window/imageeditorview.h"
#include "../window/lefttoolbox.h"
#include "../window/selectionwidget.h"
#include "../window/zoomablescrollarea.h"
#include "../assert.h"
#include "imagemanager.h"

namespace pixpaint
{
  PaintToolManager::PaintToolManager() noexcept :
    m_currentTool(nullptr),
    m_previousTool(nullptr)
  {
  }

  void PaintToolManager::setToPreviousTool() noexcept
  {
    if(m_previousTool) {
      m_currentTool = m_previousTool;
      m_currentTool->m_button->click();
      m_previousTool = nullptr;

      selection_helpers::tryFinalizeTextSelection(true);
      tool_helpers::updateViewToolCursor();

      updateStatusDescription();

      getGUIEnvironment().getLeftToolbox().getOptionFrame().setOptions(m_currentTool->getOptions());
      getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::NORMAL);
    }
  }

  bool PaintToolManager::currentToolSet() const noexcept
  {
    return m_currentTool != nullptr;
  }

  bool PaintToolManager::previousToolSet() const noexcept
  {
    return m_previousTool != nullptr;
  }

  PaintToolHandlerBase& PaintToolManager::getCurrentTool() noexcept
  {
    PIXPAINT_ASSERT(m_currentTool, "Current tool was not initialized");
    return *m_currentTool;
  }

  const PaintToolHandlerBase& PaintToolManager::getCurrentTool() const noexcept
  {
    PIXPAINT_ASSERT(m_currentTool, "Current tool was not initialized");
    return *m_currentTool;
  }

  void PaintToolManager::setCurrentTool(PaintToolHandlerBase& currentTool) noexcept
  {
    if(m_currentTool) {
      m_previousTool = m_currentTool;
    }
    m_currentTool = &currentTool;
    m_currentTool->m_button->click();

    selection_helpers::tryFinalizeTextSelection(true);
    tool_helpers::updateViewToolCursor();

    updateStatusDescription();

    getGUIEnvironment().getLeftToolbox().getOptionFrame().setOptions(m_currentTool->getOptions());
    getImageEnvironment().getSelection().setMode(SelectionWidget::ESelectionMode::NORMAL);
  }

  void PaintToolManager::updateStatusDescription()
  {
    getGUIEnvironment().getStatusBar().setToolDescription(m_currentTool->getDescription());
  }

  PIXPAINT_SINGLETON_FUNC_DEF(PaintToolManager)
}
