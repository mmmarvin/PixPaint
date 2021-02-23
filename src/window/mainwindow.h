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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../manager/previewmanager.h"
#include "../filetype/imagefiletypebase.h"
#include "../animationfiletype/animationfiletypebase.h"
#include "../manager/colormanager.h"
#include "../manager/imagemanager.h"
#include "../registrar/painttoolregistrar.h"
#include "../manager/painttoolmanager.h"
#include "mainmenu.h"
#include "statusbar.h"

namespace pixpaint
{
  struct RunParam
  {
    RunParam() :
      filename(""),
      usePython(true)
    {
    }

    std::string filename;
    bool        usePython;
  };

  class DocumentPanel;

  class MainWindow : public QMainWindow
  {
  public:
    MainWindow(const RunParam& runParam = RunParam());
    void exit();

  protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

  private:
    void createConsoleWidget();
    void createMainWidget();
    void createDocks();
    void createMenus();
    void createActions();
    void createActionShortcuts();
    void createStatusBar();
    void createRecentFiles();
    void checkRecentFiles();
    void initializeConfigValues();
    void loadConfigValues();

  private:
    friend void popFileMenu(MainMenu::FileMenu*, QWidget*, bool);
    friend class DocumentPanel;

    void slotOpenFile(bool);
    void slotSaveFile(bool);
    void slotSaveAsFile(bool);
    void slotImportAnimationFile(bool);
    void slotExportAnimationFile(bool);
    void slotImportImageFile(bool);
    void slotExportImageFile(bool);
    bool slotCloseAll(bool);

    void openProject(std::string filename);
    void openImage(std::string filename);
    void openAnimation(std::string filename);
    void saveProject(std::string filename, std::string mimeType);
    void saveImage(std::string filename, std::string mimeType);
    void saveAnimation(std::string filename, std::string mimeType);

    template<class RegistrarGetterT, class FileOpenerT, class FileTypeSetterT>
    bool openImpl(std::string filename,
                  bool shouldSetFilename,
                  bool shouldAddToRecent,
                  bool showError = true);
    template<class RegistrarGetterT, class FileSaverT, class FileTypeSetterT, class FilePreSaverT, class ModifySetterT>
    void saveImpl(std::string filename,
                  std::string mimeType,
                  bool shouldSetFilename,
                  bool shouldSetTabText,
                  bool shouldAddToRecent);

    void addToRecent(std::string filename);
    void tryOpenFile(const std::string& filename);
    void updateRecentActions();

    int                       m_lastImageWidth;
    int                       m_lastImageHeight;
    std::vector<std::string>  m_recentFiles;
    std::vector<QAction*>     m_recentFileActions;
    DocumentPanel*            m_documentPanel;
  };

  RunParam getRunParam(int argc, char** argv);
}

#include "mainwindow.inl"

#endif // MAINWINDOW_H
