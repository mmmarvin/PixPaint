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
#ifndef MAINMENU_H
#define MAINMENU_H

#include <QAction>
#include <QMenu>
#include "../image/maskablepixeldata.h"
#include "../image/rect.h"

namespace pixpaint
{
  class MainWindow;
  struct Point;

  struct MainMenu
  {
    struct FileMenu
    {
      QMenu*                menu;
      QMenu*                importMenu;
      QMenu*                exportMenu;

      QAction*              newFileAction;
      QAction*              openFileAction;
      QMenu*                openRecentMenu;
      QAction*              saveFileAction;
      QAction*              saveAsFileAction;
      QAction*              importImageFileAction;
      QAction*              exportImageFileAction;
      QAction*              importAnimationFileAction;
      QAction*              exportAnimationFileAction;
      QAction*              closeFileAction;
      QAction*              closeAllFileAction;
      QAction*              exitFileAction;
    };

    struct EditMenu
    {
      QMenu*                menu;

      QAction*              undoEditAction;
      QAction*              redoEditAction;
      QAction*              copyEditAction;
      QAction*              cutEditAction;
      QAction*              pasteEditAction;
      QAction*              deleteEditAction;
      QAction*              preferencesEditAction;
    };

    struct ViewMenu
    {
      QMenu*                menu;

      QAction*              leftToolbar;
      QAction*              rightToolbar;
      QAction*              frameToolbar;
      QAction*              consoleToolbar;
      QAction*              showGridAction;
      QAction*              boxGridAction;
      QAction*              zoomInAction;
      QAction*              zoomOutAction;
    };

    struct ImageMenu
    {
      QMenu*                menu;

      QAction*              resizeImageAction;
      QAction*              flipImageAction;
      QAction*              mirrorImageAction;
      QAction*              rotateImageAction;
      QAction*              flattenImageAction;
      QAction*              invertImageAction;
      QAction*              clearImageAction;
    };

    struct SelectMenu
    {
      QMenu*                menu;

      QMenu*                resizeMenu;
      QAction*              resizeScaleAction;
      QAction*              resizeSmoothScaleAction;
      QAction*              resizeResizeAction;

      QAction*              normalSelectionAction;
      QAction*              resizeSelectionAction;
      QAction*              moveSelectionAction;

      QAction*              selectAllEditAction;
      QAction*              deselectEditAction;
    };

    struct EffectMenu
    {
      QMenu*                menu;

      std::vector<QAction*> effectsActionList;
    };

    struct HelpMenu
    {
      QMenu*                menu;

      QAction*              aboutHelpAction;
    };

    FileMenu      fileMenu;
    EditMenu      editMenu;
    ViewMenu      viewMenu;
    ImageMenu     imageMenu;
    SelectMenu    selectMenu;
    EffectMenu    effectMenu;
    HelpMenu      helpMenu;
  };

  void popFileMenu(MainMenu::FileMenu* fileMenu, QWidget* parent, bool menu);
  void popEditMenu(MainMenu::EditMenu* editMenu, QWidget* parent, bool menu);
  void popViewMenu(MainMenu::ViewMenu* viewMenu, QWidget* parent, bool menu);
  void popImageMenu(MainMenu::ImageMenu* imageMenu, QWidget* parent, bool menu);
  void popSelectMenu(MainMenu::SelectMenu* selectMenu, QWidget* parent, bool menu);
  void popEffectMenu(MainMenu::EffectMenu* effectMenu, QWidget* parent, bool menu);
  void popHelpMenu(MainMenu::HelpMenu* helpMenu, QWidget* parent, bool menu);
}

#endif // MAINMENU_H
