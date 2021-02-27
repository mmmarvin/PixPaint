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
#include "mainwindow.h"

#include <functional>
#include <fstream>
#include <clip/clip.h>
#include <QCloseEvent>
#include <QDockWidget>
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTabBar>
#include <QStatusBar>
#include "../3rdparty/gengine/configuration.h"
#include "../dialog/optiondialog.h"
#include "../embed/headerstream.h"
#include "../embed/script_utility.h"
#include "../embed/script_exports.h"
#include "../env/imageenvironment.h"
#include "../env/guienvironment.h"
#include "../event/gui/tab_events.h"
#include "../helper/selection_helpers.h"
#include "../helper/tool_helpers.h"
#include "../image/image.h"
#include "../manager/consolemanager.h"
#include "../manager/documentmanager.h"
#include "../manager/imagemanager.h"
#include "../manager/selectionmanager.h"
#include "../manager/textselectionmanager.h"
#include "../manager/textmanager.h"
#include "../registrar/animationfiletyperegistrar.h"
#include "../registrar/imagefiletyperegistrar.h"
#include "../registrar/projectfiletyperegistrar.h"
#include "../utility/filetype_utility.h"
#include "../utility/qt_utility.h"
#include "../utility/settings_file_utility.h"
#include "../config.h"
#include "../debug_log.h"
#include "../define.h"
#include "../default_color_template.h"
#include "../animation_filetype_importer.h"
#include "../image_filetype_importer.h"
#include "../project_filetype_importer.h"
#include "../effect_importer.h"
#include "../os_specific_headers.h"
#include "../tool_importer.h"
#include "colortoolbox.h"
#include "consoletoolbox.h"
#include "documentpanel.h"
#include "frametoolbox.h"
#include "imageeditorview.h"
#include "layerwidget.h"
#include "lefttoolbox.h"
#include "righttoolbox.h"
#include "zoomablescrollarea.h"

namespace pixpaint
{
namespace
{
  void do_save(MainWindow* parent,
               const std::string& filter,
               const std::string& dialogTitle,
               std::function<void(const std::string&, const std::string&)> saveFunc)
  {
    if(getImageEnvironment().isViewSet()) {
      QFileDialog dialog(parent,
                         QObject::tr(dialogTitle.c_str()),
                         gengine2d::getConfigurationManager().getString(CONFIG_SECTION_SETTINGS, "last_location")->c_str(),
                         filter.c_str());
      dialog.setFileMode(QFileDialog::AnyFile);
      dialog.setAcceptMode(QFileDialog::AcceptSave);
      if(dialog.exec()) {
        auto filenames = dialog.selectedFiles();
        if(filenames.size() == 1) {
          saveFunc(std::string(filenames[0].toUtf8().constData()),
                   std::string(dialog.selectedNameFilter().toUtf8().constData()));
        }
      }
    }
  }

  void do_open(QMainWindow* parent,
               const std::string& filter,
               const std::string& dialogTitle,
               std::function<void(const std::string&)> openFunc)
  {
    QFileDialog dialog(parent,
                       QObject::tr(dialogTitle.c_str()),
                       gengine2d::getConfigurationManager().getString(CONFIG_SECTION_SETTINGS, "last_location")->c_str(),
                       filter.c_str());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.selectNameFilter(QObject::tr("All Files (*.*)"));
    if(dialog.exec()) {
      auto filenames = dialog.selectedFiles();
      if(filenames.size() == 1) {
        openFunc(std::string(filenames[0].toUtf8().constData()));
      }
    }
  }

  struct ProjectRegistrarGetter
  {
    ProjectFileTypeRegistrar& operator()() { return getProjectFileTypeRegistrar(); }
  };

  struct ProjectOpener
  {
    int operator()(const std::string& filename, AnimationFileTypeHandlerBase& handle, Document& document)
    {
      return handle.openAnimation(filename, document.getAnimation());
    }
  };

  struct ProjectSaver
  {
    int operator()(const std::string& filename, AnimationFileTypeHandlerBase& handle, Document& document)
    {
      return handle.saveAnimation(filename, document.getAnimation());
    }
  };

  struct ProjectFileTypeSetter
  {
    void operator()(DocumentManager& documentManager, AnimationFileTypeHandlerBase& handle)
    {
      documentManager.getDocument().setFileTypeInformation(&handle);
    }
  };

  struct ImageRegistrarGetter
  {
    ImageFileTypeRegistrar& operator()() { return getImageFileTypeRegistrar(); }
  };

  struct ImageOpener
  {
    int operator()(const std::string& filename, ImageFileTypeHandlerBase& handle, Document& document)
    {
      return handle.openImage(filename, document.getAnimation().getFrame(0));
    }
  };

  struct ImageSaver
  {
    int operator()(const std::string& filename, ImageFileTypeHandlerBase& handle, Document& document)
    {
      return handle.saveImage(filename, document.getAnimation().getCurrentFrame());
    }
  };

  struct ImageFiletypeSetter
  {
    void operator()(DocumentManager&, ImageFileTypeHandlerBase&) {}
  };

  struct ImagePreSaver
  {
    void operator()(DocumentManager& document_manager, ImageFileTypeHandlerBase& handle)
    {
      handle.onPreSave(document_manager.getDocument().getAnimation().getCurrentFrame());
    }
  };

  struct AnimationRegistrarGetter
  {
    AnimationFileTypeRegistrar& operator()() { return getAnimationFileTypeRegistrar(); }
  };

  struct AnimationOpener
  {
    int operator()(const std::string& filename, AnimationFileTypeHandlerBase& handle, Document& document)
    {
      return handle.openAnimation(filename, document.getAnimation());
    }
  };

  struct AnimationSaver
  {
    int operator()(const std::string& filename, AnimationFileTypeHandlerBase& handle, Document& document)
    {
      return handle.saveAnimation(filename, document.getAnimation());
    }
  };

  struct AnimationFiletypeSetter
  {
    void operator()(DocumentManager&, AnimationFileTypeHandlerBase&) {}
  };

  struct AnimationPreSaver
  {
    void operator()(DocumentManager& document_manager, AnimationFileTypeHandlerBase& handle)
    {
      handle.onPreSave(document_manager.getDocument().getAnimation());
    }
  };

  struct ModifySetter
  {
    void operator()(Animation& animation)
    {
      for(size_t i = 0, isize = animation.getFrameCount(); i < isize; ++i) {
        animation.getFrame(i).setModified(false);
      }
    }
  };

  struct DummyModifySetter
  {
    void operator()(Animation&)
    {
    }
  };
}
  MainWindow::MainWindow(const RunParam& runParam) :
    QMainWindow(nullptr)
  {
    auto app_name = (APP_TITLE +
                     std::string(" v.") +
                     std::to_string(APP_VERSION_MAJOR) +
                     std::string(".") +
                     std::to_string(APP_VERSION_MINOR) +
                     std::to_string(APP_VERSION_PATCH) +
                     std::string(" [") +
                     APP_RELEASE_TYPE +
                     std::string("]"));

    this->setMinimumSize(640, 480);
    this->setWindowState(Qt::WindowState::WindowMaximized);
    this->setWindowTitle(app_name.c_str());
    this->setWindowIcon(QIcon("res/pixpaint.png"));

    createConsoleWidget();
    getConsoleManager().writeMessageSystem(app_name + std::string(" Console"));
    getConsoleManager().writeMessageSystem("=================================");

    // initialize built-in config values
    initializeConfigValues();

    // register the paint tools
    register_fixed_tool();
    register_fixed_project_file_types();
    register_fixed_image_file_types();
    register_fixed_animation_file_types();
    register_fixed_effects();

    if(runParam.usePython) {
      // register custom tools, effects, etc...
      initPythonExports();
    }

    // load the config values
    loadConfigValues();

    // create widgets
    createStatusBar();
    createMainWidget();
    createDocks();
    createMenus();
    createActions();
    createActionShortcuts();

    // create recent files
    createRecentFiles();
    checkRecentFiles();
    updateRecentActions();

    auto& configurationManager = gengine2d::getConfigurationManager();
    m_lastImageWidth = *configurationManager.getInteger(CONFIG_SECTION_SETTINGS,
                                                       "image_width");
    m_lastImageHeight = *configurationManager.getInteger(CONFIG_SECTION_SETTINGS,
                                                       "image_height");
    m_lastImageWidth = m_lastImageWidth <= 0 ? DEFAULT_IMAGE_WIDTH : m_lastImageWidth ;
    m_lastImageHeight = m_lastImageHeight <= 0 ? DEFAULT_IMAGE_HEIGHT : m_lastImageHeight;

    emitEvent(gui_events::TabChangeEvent { -1, 0});
    this->installEventFilter(this);

    if(!runParam.filename.empty()) {
      tryOpenFile(runParam.filename);
    }
  }

  void MainWindow::exit()
  {
    auto& configurationManager = gengine2d::getConfigurationManager();
    for(size_t i = 0; i < NUMBER_OF_RECENT_FILES; ++i) {
      if(i < m_recentFiles.size()) {
        configurationManager.setVariable(CONFIG_SECTION_RECENT_FILES,
                                         std::string("File") + std::to_string(i + 1),
                                         m_recentFiles[i]);
      } else {
        configurationManager.setVariable(CONFIG_SECTION_RECENT_FILES,
                                         std::string("File") + std::to_string(i + 1),
                                         "");
      }
    }
    configurationManager.setVariable(CONFIG_SECTION_SETTINGS,
                                     "image_width",
                                     std::to_string(m_lastImageWidth));
    configurationManager.setVariable(CONFIG_SECTION_SETTINGS,
                                     "image_height",
                                     std::to_string(m_lastImageHeight));

    pixpaint::settings_file_utils::tryCreateSettingsFilename();
    gengine2d::getConfigurationManager().saveToFile(pixpaint::settings_file_utils::getSettingsFilename());
  }

  void MainWindow::keyPressEvent(QKeyEvent* event)
  {
    if(event->key() == Qt::Key_Escape) {
      selection_helpers::tryFinalizeAllSelections(true);
    } else {
      auto& imageEnv = getImageEnvironment();
      auto& imageManager = getImageManager();
      if(imageEnv.isViewSet()) {
        auto& currentPaintTool = getPaintToolManager().getCurrentTool();
        auto& view = imageEnv.getView();

        if(getTextSelectionManager().selectionExists()) {
          getTextManager().onKeyPress(static_cast<EKey>(event->key()),
                                      event->modifiers() & Qt::ShiftModifier);
          auto pixelSize = view.getPixelSize();
          auto refreshRect = view.getSmallestDrawableRegion(castTo<double>(getTextSelectionManager().getSelectionRect()) * pixelSize);
          view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refreshRect)));
        } else {
          const bool shift_down = (event->modifiers() & Qt::ShiftModifier) != 0;
          const bool ctrl_down = (event->modifiers() & Qt::ControlModifier) != 0;
          auto res = currentPaintTool.onKeyPress(static_cast<EKey>(event->key()),
                                                 getColorManager().getForegroundColor(),
                                                 ControlState { shift_down, ctrl_down },
                                                 view.getPreviewLayer(),
                                                 imageManager.getImage().getCurrentLayer());

          if(res & PaintToolBase::EChangeResult::ECCR_UPDATEIMAGE) {
            auto pixelSize = view.getPixelSize();
            auto refreshRect = view.getSmallestDrawableRegion(castTo<double>(currentPaintTool.getDrawRect()) * pixelSize);
            view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refreshRect)));
          }
          if(res & PaintToolBase::EChangeResult::ECCR_UPDATECURSOR) {
            tool_helpers::updateViewToolCursor();
          }
        }
      }
    }
  }

  void MainWindow::keyReleaseEvent(QKeyEvent* event)
  {
    if(event->key() != Qt::Key_Escape) {
      auto& imageEnv = getImageEnvironment();
      auto& imageManager = getImageManager();
      if(imageEnv.isViewSet()) {
        auto& currentPaintTool = getPaintToolManager().getCurrentTool();
        auto& view = imageEnv.getView();

        const bool shift_down = (event->modifiers() & Qt::ShiftModifier) != 0;
        const bool ctrl_down = (event->modifiers() & Qt::ControlModifier) != 0;
        auto res = currentPaintTool.onKeyRelease(static_cast<EKey>(event->key()),
                                                 getColorManager().getForegroundColor(),
                                                 ControlState { shift_down, ctrl_down },
                                                 view.getPreviewLayer(),
                                                 imageManager.getImage().getCurrentLayer());

        if(res & PaintToolBase::EChangeResult::ECCR_UPDATEIMAGE) {
          auto pixelSize = view.getPixelSize();
          auto refreshRect = view.getSmallestDrawableRegion(castTo<double>(currentPaintTool.getDrawRect()) * pixelSize);
          view.repaint(qt_utils::convertToQTRect(castTo<position_t>(refreshRect)));
        }
        if(res & PaintToolBase::EChangeResult::ECCR_UPDATECURSOR) {
          tool_helpers::updateViewToolCursor();
        }
      }
    }
  }

  void MainWindow::closeEvent(QCloseEvent* event)
  {
    if(!slotCloseAll(false)) {
      event->ignore();
    } else {
      QMainWindow::closeEvent(event);
    }
  }

  bool MainWindow::eventFilter(QObject* w, QEvent* e)
  {
    if(e->type() == QEvent::ShortcutOverride) {
      QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
      auto& selectionManager = getSelectionManager();
      auto& textSelectionManager = getTextSelectionManager();
      auto& imageEnv = getImageEnvironment();

      if(selectionManager.selectionExists()) {
        // if selection maanger is set, we ignore arrow keys to allow
        // it to be moved
        auto key = keyEvent->key();
        switch(key) {
        case Qt::Key_Escape:
        case Qt::Key_Left:
        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Down:
          e->accept();
          return true;
        default:
          break;
        }
      } else if(textSelectionManager.selectionExists() && imageEnv.isViewSet()) {
        // if the text selection manager is set, we ignore all shortcut keys
        auto key = keyEvent->key();
        auto c = static_cast<char>(key);
        if(isalnum(c) || ispunct(c)) {
          auto modifier = keyEvent->modifiers();
          if(!(modifier & Qt::ControlModifier) &&
             !(modifier & Qt::ShiftModifier)) {
            e->accept();
            return true;
          }
        } else {
          switch(key) {
          case EKey::Key_Escape:
          case EKey::Key_Backspace:
          case EKey::Key_Delete:
          case EKey::Key_Space:
          case EKey::Key_Home:
          case EKey::Key_End:
          case EKey::Key_Left:
          case EKey::Key_Right:
          case EKey::Key_Up:
          case EKey::Key_Down:
            e->accept();
            return true;
          default:
            break;
          }
        }
      } else if(getGUIEnvironment().getRightToolbox().getLayerWidget().m_scrollArea->hasFocus()) {
        auto key = keyEvent->key();
        switch(key) {
        case EKey::Key_Delete:
          e->accept();
          return true;
        default:
          break;
        }
      }
    }

    return QMainWindow::eventFilter(w, e);
  }

  void MainWindow::createConsoleWidget()
  {
    auto& gui_env = getGUIEnvironment();

    gui_env.m_consoleToolboxDock = new QDockWidget(tr("Console"), this);
    gui_env.m_consoleToolbox = new ConsoleToolbox(gui_env.m_consoleToolboxDock);
    gui_env.m_consoleToolboxDock->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    gui_env.m_consoleToolboxDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    gui_env.m_consoleToolboxDock->setWidget(gui_env.m_consoleToolbox);
    this->addDockWidget(Qt::LeftDockWidgetArea, gui_env.m_consoleToolboxDock);
    gui_env.m_consoleToolboxDock->hide();
  }

  void MainWindow::createMainWidget()
  {
    m_documentPanel = new DocumentPanel(this);
    this->setCentralWidget(m_documentPanel);
  }

  void MainWindow::createDocks()
  {
    this->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    this->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    this->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    auto& gui_env = getGUIEnvironment();
    gui_env.m_leftToolboxDock = new QDockWidget(tr("Toolbox"), this);
    gui_env.m_leftToolbox = new LeftToolbox(gui_env.m_leftToolboxDock);
    gui_env.m_leftToolbox->switchToDefaultTool();
    gui_env.m_leftToolboxDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    gui_env.m_leftToolboxDock->setWidget(gui_env.m_leftToolbox);
    this->addDockWidget(Qt::LeftDockWidgetArea, gui_env.m_leftToolboxDock);

    gui_env.m_rightToolboxDock = new QDockWidget(tr(""), this);
    gui_env.m_rightToolbox = new RightToolbox(gui_env.m_rightToolboxDock);
    gui_env.m_rightToolboxDock->setWidget(gui_env.m_rightToolbox);
    gui_env.m_rightToolboxDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->addDockWidget(Qt::RightDockWidgetArea, gui_env.m_rightToolboxDock);

    auto* dock = new QDockWidget(tr("Colors Toolbar"), this);
    gui_env.m_colorToolbox = new ColorToolbox(dock, getDefaultColor());
    dock->setFeatures(static_cast<QDockWidget::DockWidgetFeature>(0));
    dock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setWidget(gui_env.m_colorToolbox);
    this->addDockWidget(Qt::BottomDockWidgetArea, dock);

    gui_env.m_frameToolboxDock = new QDockWidget(tr("Frames Toolbar"), this);
    gui_env.m_frameToolbox = new FrameToolbox(dock);
    gui_env.m_frameToolboxDock->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    gui_env.m_frameToolboxDock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    gui_env.m_frameToolboxDock->setWidget(gui_env.m_frameToolbox);
    this->addDockWidget(Qt::TopDockWidgetArea, gui_env.m_frameToolboxDock);
  }

  void MainWindow::createMenus()
  {
    auto& gui_env = getGUIEnvironment();
    gui_env.getMainMenu().fileMenu.menu = this->menuBar()->addMenu(tr("&File"));
    gui_env.getMainMenu().editMenu.menu = this->menuBar()->addMenu(tr("&Edit"));
    gui_env.getMainMenu().selectMenu.menu = this->menuBar()->addMenu(tr("&Select"));
    gui_env.getMainMenu().viewMenu.menu = this->menuBar()->addMenu(tr("&View"));
    gui_env.getMainMenu().imageMenu.menu = this->menuBar()->addMenu(tr("&Image"));
    gui_env.getMainMenu().effectMenu.menu = this->menuBar()->addMenu(tr("Effe&ct"));
    gui_env.getMainMenu().helpMenu.menu = this->menuBar()->addMenu(tr("&Help"));
  }

  void MainWindow::createActions()
  {
    auto* mainMenu = &getGUIEnvironment().getMainMenu();

    // File Actions
    popFileMenu(&mainMenu->fileMenu, this, true);

    // Edit Actions
    popEditMenu(&mainMenu->editMenu, this, true);

    // Selection Actions
    popSelectMenu(&mainMenu->selectMenu, this, true);

    // View Actions
    popViewMenu(&mainMenu->viewMenu, this, true);

    // Image Actions
    popImageMenu(&mainMenu->imageMenu, this, true);

    // Effect Actions
    popEffectMenu(&mainMenu->effectMenu, this, true);

    // Help Actions
    popHelpMenu(&mainMenu->helpMenu, this, true);
  }

  void MainWindow::createStatusBar()
  {
    auto& gui_env = getGUIEnvironment();
    gui_env.getStatusBar().toolDescription = new QLabel(tr("Tool Description"), this->statusBar());
    gui_env.getStatusBar().toolDescription->setToolTip(tr("Tool Description"));
    gui_env.getStatusBar().mousePosition = new QLabel(tr("[0, 0]"), this->statusBar());
    gui_env.getStatusBar().mousePosition->setToolTip(tr("Mouse Position"));
    gui_env.getStatusBar().drawSize = new QLabel(tr("[0, 0]"), this->statusBar());
    gui_env.getStatusBar().imageZoomLevel = new QLabel(tr("0%"), this->statusBar());
    gui_env.getStatusBar().imageZoomLevel->setToolTip(tr("Image Zoom Level"));

    this->statusBar()->insertWidget(0, gui_env.getStatusBar().toolDescription, 10);
    this->statusBar()->insertWidget(1, gui_env.getStatusBar().mousePosition, 1);
    this->statusBar()->insertWidget(2, gui_env.getStatusBar().drawSize, 1);
    this->statusBar()->insertWidget(3, gui_env.getStatusBar().imageZoomLevel, 1);
  }

  void MainWindow::createRecentFiles()
  {
    auto& configuration_manager = gengine2d::getConfigurationManager();
    for(int i = 1; i <= NUMBER_OF_RECENT_FILES; ++i) {
      auto file = *configuration_manager.getString(CONFIG_SECTION_RECENT_FILES,
                                                  std::string("File") + std::to_string(i));
      if(file != "\"\"") {
        m_recentFiles.push_back(file);
      } else {
        break;
      }
    }
  }

  void MainWindow::checkRecentFiles()
  {
    std::vector<std::string> new_recent_files;
    for(const auto& filename : m_recentFiles) {
      if(os_specific::filesystem::exists(filename)) {
        new_recent_files.push_back(filename);
      }
    }

    m_recentFiles = std::move(new_recent_files);
  }

  void MainWindow::slotOpenFile(bool)
  {
    auto filter = projectfiletype_utils::getFilterList();
    if(filter.empty()) {
      filter = "All Files (*.*)";
    } else {
      filter += ";;All Files (*.*)";
    }

//    QFileDialog dialog(this,
//                       tr("Open..."),
//                       tr(gengine2d::getConfigurationManager().getString(CONFIG_SECTION_SETTINGS, "last_location")->c_str()),
//                       filter.c_str());
//    dialog.setFileMode(QFileDialog::ExistingFile);
//    dialog.setAcceptMode(QFileDialog::AcceptOpen);
//    dialog.selectNameFilter(tr("All Files (*.*)"));
//    if(dialog.exec()) {
//      auto filenames = dialog.selectedFiles();
//      if(filenames.size() == 1) {
//        openProject(std::string(filenames[0].toUtf8().constData()));
//      }
//    }
    do_open(this,
            filter,
            "Open Project...",
    [this](const std::string& filename) {
      openProject(filename);
    });
  }

  void MainWindow::slotSaveFile(bool)
  {
    auto& image_env = getImageEnvironment();
    auto& image_manager = getImageManager();
    auto& document_manager = getDocumentManager();
    if(image_env.isViewSet()) {
      if(document_manager.getDocument().getFilename().empty() &&
         !document_manager.getDocument().getFileTypeInformation()) {
        slotSaveAsFile(false);
      } else {
        auto filename = document_manager.getDocument().getFilename();
        auto* fileTypeInformation = document_manager.getDocument().getFileTypeInformation();

        auto res = fileTypeInformation->saveAnimation(filename, document_manager.getDocument().getAnimation());
        if(res == ImageFileTypeBase::EOR_ERROR) {
          QMessageBox::critical(this, tr("Error"), tr("There was an error saving the image!"));
          return;
        }

        m_documentPanel->tabBar()->setTabText(m_documentPanel->getDocumentIndex(document_manager.getDocument()),
                                              filename.c_str());
        image_manager.getImage().setModified(false);
      }
    }
  }

  void MainWindow::slotSaveAsFile(bool)
  {
    do_save(this,
           projectfiletype_utils::getFilterList(),
           "Save Project...",
    [this](const std::string& filename, const std::string& mimeType){
      saveProject(filename, mimeType);
    });
  }

  void MainWindow::slotImportAnimationFile(bool)
  {
    auto filter = animationtype_utils::getFilterList();
    if(filter.empty()) {
      filter = "All Files (*.*)";
    } else {
      filter += ";;All Files (*.*)";
    }

//    QFileDialog dialog(this, tr("Open..."), tr(""), filter.c_str());
//    dialog.setFileMode(QFileDialog::ExistingFile);
//    dialog.setAcceptMode(QFileDialog::AcceptOpen);
//    dialog.selectNameFilter(tr("All Files (*.*)"));
//    if(dialog.exec()) {
//      auto filenames = dialog.selectedFiles();
//      if(filenames.size() == 1) {
//        openAnimation(std::string(filenames[0].toUtf8().constData()));
//      }
//    }
    do_open(this,
            filter,
            "Import Animation...",
    [this](const std::string& filename) {
      openAnimation(filename);
    });
  }

  void MainWindow::slotExportAnimationFile(bool)
  {
    do_save(this,
           animationtype_utils::getFilterList(),
           "Export Animation...",
    [this](const std::string& filename, const std::string& mimeType){
      saveAnimation(filename, mimeType);
    });
  }

  void MainWindow::slotImportImageFile(bool)
  {
    auto filter = imagefiletype_utils::getFilterList();
    if(filter.empty()) {
      filter = "All Files (*.*)";
    } else {
      filter += ";;All Files (*.*)";
    }

//    QFileDialog dialog(this, tr("Open..."), tr(""), filter.c_str());
//    dialog.setFileMode(QFileDialog::ExistingFile);
//    dialog.setAcceptMode(QFileDialog::AcceptOpen);
//    dialog.selectNameFilter(tr("All Files (*.*)"));
//    if(dialog.exec()) {
//      auto filenames = dialog.selectedFiles();
//      if(filenames.size() == 1) {
//        openImage(std::string(filenames[0].toUtf8().constData()));
//      }
//    }
    do_open(this,
            filter,
            "Import Image...",
    [this](const std::string& filename) {
      openImage(filename);
    });
  }

  void MainWindow::slotExportImageFile(bool)
  {
    do_save(this,
           imagefiletype_utils::getFilterList(),
           "Export Image...",
    [this](const std::string& filename, const std::string& mimeType) {
      saveImage(filename, mimeType);
    });
  }

  bool MainWindow::slotCloseAll(bool)
  {
    while(m_documentPanel->getPageCount()) {
      if(!m_documentPanel->closePage(0)) {
        return false;
      }
    }

    return true;
  }

  void MainWindow::openProject(std::string filename)
  {
    openImpl<ProjectRegistrarGetter,
             ProjectOpener,
             ProjectFileTypeSetter>(filename, true, true);
  }

  void MainWindow::openImage(std::string filename)
  {
    openImpl<ImageRegistrarGetter,
             ImageOpener,
             ImageFiletypeSetter>(filename, false, false);
  }

  void MainWindow::openAnimation(std::string filename)
  {
    openImpl<AnimationRegistrarGetter,
             AnimationOpener,
             AnimationFiletypeSetter>(filename, false, false);
  }

  void MainWindow::saveProject(std::string filename, std::string mimeType)
  {
    saveImpl<ProjectRegistrarGetter,
             ProjectSaver,
             ProjectFileTypeSetter,
             AnimationPreSaver,
             ModifySetter>(std::move(filename), std::move(mimeType), true, true, true);
  }

  void MainWindow::saveImage(std::string filename, std::string mimeType)
  {
    saveImpl<ImageRegistrarGetter,
             ImageSaver,
             ImageFiletypeSetter,
             ImagePreSaver,
             DummyModifySetter>(std::move(filename), std::move(mimeType), false, false, false);
  }

  void MainWindow::saveAnimation(std::string filename, std::string mimeType)
  {
    saveImpl<AnimationRegistrarGetter,
             AnimationSaver,
             AnimationFiletypeSetter,
             AnimationPreSaver,
             DummyModifySetter>(std::move(filename), std::move(mimeType), false, false, false);
  }

  void MainWindow::addToRecent(std::string filename)
  {
    if(filename.empty()) {
      return;
    }

    auto findIt = std::find(m_recentFiles.begin(), m_recentFiles.end(), filename);
    if(findIt != m_recentFiles.end()) {
      return;
    }

    if(m_recentFiles.size() > (NUMBER_OF_RECENT_FILES - 1)) {
      auto diff = m_recentFiles.size() - (NUMBER_OF_RECENT_FILES - 1);
      m_recentFiles.erase(m_recentFiles.begin(), m_recentFiles.begin() + diff);
    }

    m_recentFiles.push_back(std::move(filename));
    updateRecentActions();
  }

  void MainWindow::tryOpenFile(const std::string& filename)
  {
    if(!openImpl<ProjectRegistrarGetter, ProjectOpener, ProjectFileTypeSetter>(filename, true, true, false)) {
      if(!openImpl<ImageRegistrarGetter, ImageOpener, ImageFiletypeSetter>(filename, false, false, false)) {
        openImpl<AnimationRegistrarGetter, AnimationOpener, AnimationFiletypeSetter>(filename, false, false);
      }
    }
  }

  void MainWindow::updateRecentActions()
  {
    for(auto* action : m_recentFileActions) {
      delete action;
    }
    m_recentFileActions.clear();

    auto* menu = getGUIEnvironment().getMainMenu().fileMenu.openRecentMenu;
    if(m_recentFiles.empty()) {
      menu->setEnabled(false);
    } else {
      menu->setEnabled(true);
      for(const auto& filename : m_recentFiles) {
        auto* fileAction = menu->addAction(tr(filename.c_str()));
        m_recentFileActions.push_back(fileAction);

        QObject::connect(fileAction, &QAction::triggered, [filename, this](bool) {
          this->openImage(filename);
        });
      }

      menu->addSeparator();
      auto* clearAction = menu->addAction(tr("Clear recent..."));
      m_recentFileActions.push_back(clearAction);

      QObject::connect(clearAction, &QAction::triggered, [this](bool) {
        this->m_recentFiles.clear();
        this->updateRecentActions();
      });
    }
  }

  RunParam getRunParam(int argc, char** argv)
  {
    RunParam param;

    if(argc > 1) {
      for(int i = 1; i < argc; ++i) {
        auto sargv = std::string(argv[i]);
        if(sargv == "--nopython") {
          param.usePython = false;
        } else if(os_specific::filesystem::exists(os_specific::filesystem::path(sargv))) {
          param.filename = sargv;
        }
      }
    }

    return param;
  }
}


