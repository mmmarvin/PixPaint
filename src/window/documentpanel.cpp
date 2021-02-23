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
#include "documentpanel.h"

#include <QMessageBox>
#include <QWheelEvent>
#include <QTabBar>
#include "../3rdparty/gengine/configuration.h"
#include "../env/imageenvironment.h"
#include "../env/guienvironment.h"
#include "../event/gui/tab_events.h"
#include "../helper/selection_helpers.h"
#include "../manager/colormanager.h"
#include "../define.h"
#include "../manager/documentmanager.h"
#include "../manager/imagemanager.h"
#include "../manager/previewmanager.h"
#include "frametoolbox.h"
#include "imageeditorview.h"
#include "lefttoolbox.h"
#include "mainwindow.h"
#include "righttoolbox.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
  DocumentPanel::DocumentPanel(QWidget* parent) :
    QTabWidget(parent),
    m_currentIndex(0)
  {
    this->tabBar()->setExpanding(false);
    this->tabBar()->setTabsClosable(true);

    m_rightClickEditMenu.menu = new QMenu(this);
    m_rightClickSelectionMenu.menu = m_rightClickEditMenu.menu;
    popEditMenu(&m_rightClickEditMenu, this, false);
    popSelectMenu(&m_rightClickSelectionMenu, this, false);

    auto& configuration_manager = gengine2d::getConfigurationManager();
    auto image_width = *configuration_manager.getInteger(pixpaint::CONFIG_SECTION_SETTINGS,
                                                       "image_width");
    auto image_height = *configuration_manager.getInteger(pixpaint::CONFIG_SECTION_SETTINGS,
                                                       "image_height");
    image_width = image_width <= 0 ? DEFAULT_IMAGE_WIDTH : image_width;
    image_height = image_height <= 0 ? DEFAULT_IMAGE_HEIGHT : image_height;
    m_defaultPixelSize = (image_width > 64 || image_height > 64) ? 8 : 16;
    addPage(Document(image_width, image_height));

    QObject::connect(this,
                     &QTabWidget::currentChanged,
                     [this](int index)
    {
      if(index != -1)  {
        auto& image_env = getImageEnvironment();
        auto& image_manager = getImageManager();
        auto& document_manager = getDocumentManager();
        auto& previewManager = getPreviewManager();
        auto* view = m_imageViews[index];
        auto* scroll_area = m_scrollAreas[index];

        selection_helpers::tryFinalizeAllSelections(true);

        document_manager.setDocument(m_animationDocuments[index].get());
        image_env.setView(view);
        image_env.setScrollArea(scroll_area);

        auto& current_tool = getPaintToolManager().getCurrentTool();
        if(current_tool.getCursor()) {
          view->setCursor(current_tool.getCursor().value());
        } else {
          view->setCursor(Cursor(Cursor::ECT_ARROW));
        }

        emitEvent(gui_events::TabChangeEvent { m_currentIndex, index });
        m_currentIndex = index;

        image_manager.refresh();
        previewManager.refreshAll();
      }
    });
    QObject::connect(this->tabBar(), &QTabBar::tabCloseRequested, this, &DocumentPanel::closePage);
  }

  void DocumentPanel::addPage(Document&& document)
  {
    auto& gui_env = getGUIEnvironment();
    m_animationDocuments.push_back(std::make_unique<Document>(std::move(document)));

    auto& image_document = *m_animationDocuments.back();
    auto page_name = image_document.getFilename();

    auto default_pixel_size = (image_document.getAnimation().getFrame(0).getWidth() > 64 ||
                             image_document.getAnimation().getFrame(0).getHeight() > 64) ?
                             8 :
                             16;
    auto* scroll_area = new ZoomableScrollArea(this);
    auto* view = new ImageEditorView(scroll_area,
                                     m_rightClickEditMenu.menu,
                                     image_document.getAnimation().getFrame(0),
                                     default_pixel_size);
    m_imageViews.push_back(view);
    m_scrollAreas.push_back(scroll_area);
    scroll_area->setWidget(view);
    scroll_area->createResizeHandles();
    scroll_area->createSelectionWidget(&m_rightClickEditMenu);

    this->addTab(scroll_area,
                 page_name.empty() ? std::string("[Untitled]").c_str() : page_name.c_str());
    this->setCurrentIndex(m_animationDocuments.size() - 1);
    emitEvent(gui_events::TabAddEvent{ m_animationDocuments.size() });

    gui_env.getStatusBar().setZoomLevelText(view->getPixelSize() * 100);
    if(gui_env.m_rightToolbox) {
      gui_env.getRightToolbox().setEnabled(true);
    }

    auto& image_env = getImageEnvironment();
    auto& document_manager = getDocumentManager();
    image_env.setView(view);
    image_env.setScrollArea(scroll_area);
    document_manager.setDocument(&image_document);
  }

  bool DocumentPanel::closePage(int index)
  {
    if(index < 0) {
      return false;
    }

    selection_helpers::tryFinalizeAllSelections();

    if(index != this->currentIndex()) {
      this->setCurrentIndex(index);
    }

    auto& image_env = getImageEnvironment();
    auto& document_manager = getDocumentManager();

    auto& document = *m_animationDocuments.at(index).get();
    auto document_filename = document.getFilename();
    if(document_filename.empty()) {
      document_filename = "Untitled";
    }
    for(size_t i = 0, isize = document.getAnimation().getFrameCount(); i < isize; ++i) {
      auto& image = document.getAnimation().getFrame(i);
      if(image.isModified()) {
        auto res = QMessageBox::warning(this,
                                        "Warning",
                                        (std::string("\"") + document_filename + std::string("\" has been modified. Continue without saving?")).c_str(),
                                        QMessageBox::Yes | QMessageBox::No);
        if(res == QMessageBox::Yes) {
          break;
        } else if(res == QMessageBox::No){
          return false;
        }
      }
    }

    if(m_animationDocuments.size() > 1) {
      if(index == m_animationDocuments.size() - 1) {
        this->setCurrentIndex(index - 1);
      } else {
        this->setCurrentIndex(index + 1);
      }
      emitEvent(gui_events::TabRemoveEvent{ m_animationDocuments.size() - 1 });
    } else {
      document_manager.setDocument(nullptr);
      image_env.setView(nullptr);
      image_env.setScrollArea(nullptr);

      emitEvent(gui_events::TabRemoveEvent{ m_animationDocuments.size() - 1 });
    }
    removePage(index);

    return true;
  }

  int DocumentPanel::getPageCount() const
  {
    return m_animationDocuments.size();
  }

  int DocumentPanel::getDocumentIndex(Document& document) const
  {
    for(std::size_t i = 0, isize = m_animationDocuments.size(); i < isize; ++i) {
      auto* image_document = m_animationDocuments[i].get();
      if(&document == image_document) {
        return i;
      }
    }

    return -1;
  }

  void DocumentPanel::removePage(int index)
  {
    m_animationDocuments.erase(m_animationDocuments.begin() + index);
    m_imageViews.erase(m_imageViews.begin() + index);
    m_scrollAreas.erase(m_scrollAreas.begin() + index);
    this->removeTab(index);
  }
}
