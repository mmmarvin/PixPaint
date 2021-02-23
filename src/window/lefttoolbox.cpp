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
#include "lefttoolbox.h"

#include <QButtonGroup>
#include <QGridLayout>
#include "../env/guienvironment.h"
#include "../event/gui/tab_events.h"
#include "../helper/selection_helpers.h"
#include "../helper/tool_helpers.h"
#include "../layout/flowlayout.h"
#include "../registrar/painttoolregistrar.h"
#include "../utility/qt_utility.h"
#include "../manager/colormanager.h"
#include "../manager/imagemanager.h"
#include "../manager/painttoolmanager.h"
#include "../manager/previewmanager.h"
#include "../gui_define.h"
#include "imageeditorview.h"
#include "statusbar.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  LeftToolbox::LeftToolbox(QWidget* parent) :
    QWidget(parent)
  {
    m_optionFrame = static_cast<PaintToolOptionFrame*>(createOptionFrame());
    auto* toolbox = createToolbox();

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(toolbox, 1);
    layout->addWidget(m_optionFrame, 9);
    this->setLayout(layout);
  }

  void LeftToolbox::switchToDefaultTool()
  {
    m_defaultTool->click();
  }

  void LeftToolbox::switchToDefaultSelectionTool()
  {
    m_defaultSelectionTool->click();
  }

  PaintToolOptionFrame& LeftToolbox::getOptionFrame()
  {
    return *m_optionFrame;
  }

  void LeftToolbox::onEmit(const gui_events::TabAddEvent&)
  {
    if(!this->isEnabled()) {
      this->setEnabled(true);
    }
  }

  void LeftToolbox::onEmit(const gui_events::TabRemoveEvent& event)
  {
    if(!event.numberOfTabs) {
      this->setEnabled(false);
    }
  }

  QWidget* LeftToolbox::createToolbox()
  {
    auto& paintToolManager = getPaintToolManager();
    auto& paintToolRegistrar = getPaintToolRegistrar();

    auto* widget = new QWidget(this);
    auto* buttonGroup = new QButtonGroup(widget);

    auto* layout = new FlowLayout(widget);
    layout->setSpacing(1);
    layout->setAlignment(Qt::AlignTop);

    for(auto& paintToolInformation : paintToolRegistrar) {
      auto* toolBtn = new QPushButton(widget);
      toolBtn->setToolTip(paintToolInformation.getName().c_str());
      paintToolInformation.getTool().m_buttonPair = toolBtn;
      QObject::connect(toolBtn, &QPushButton::clicked,
      [&paintToolManager, &paintToolInformation]() {
        auto& paintTool = paintToolInformation.getTool();
        if(paintToolManager.currentToolSet()) {
          if(&paintToolManager.getCurrentTool() != &paintTool) {
            paintToolManager.setCurrentTool(paintTool);
            tool_helpers::onToolChange(paintTool, &PaintToolHandlerBase::onToolChange);
          }
        } else {
          paintToolManager.setCurrentTool(paintTool);
          tool_helpers::onToolChange(paintTool, &PaintToolHandlerBase::onToolChange);
        }
      });
      toolBtn->setMaximumSize(PAINTTOOLBOX_BUTTON_WIDTH, PAINTTOOLBOX_BUTTON_HEIGHT);
      toolBtn->setIcon(QIcon(paintToolInformation.getIconFilename().c_str()));
      toolBtn->setIconSize(QSize(PAINTTOOLBOX_BUTTON_WIDTH, PAINTTOOLBOX_BUTTON_HEIGHT));
      toolBtn->setCheckable(true);

      buttonGroup->addButton(toolBtn);
      layout->addWidget(toolBtn);

      if(layout->count() == 1) {
        m_defaultSelectionTool = toolBtn;
      } else if(layout->count() == 2) {
        m_defaultTool = toolBtn;
      }
    }
    widget->setLayout(layout);
    widget->move(5, 5);

    return widget ;
  }

  QWidget* LeftToolbox::createOptionFrame()
  {
    return new PaintToolOptionFrame(this);
  }
}
