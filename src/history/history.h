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
#ifndef HISTORY_H
#define HISTORY_H

#include <list>
#include <memory>
#include "../history/historyaction.h"

namespace pixpaint
{
  class History
  {
  public:
    History() {}

    template<class ActionT> void addAction(ActionT&& action);

    HistoryAction::EActionResult undo();
    HistoryAction::EActionResult redo();

    bool canUndo() const;
    bool canRedo() const;
    const std::string& getNextUndoName() const;
    const std::string& getNextRedoName() const;

  private:
    void trySetIndexes(HistoryAction& action);
    void tryGetIndexes(HistoryAction& action);
    void tryAddUndo(std::unique_ptr<HistoryAction> action);
    void tryAddRedo(std::unique_ptr<HistoryAction> action);

    std::list<std::unique_ptr<HistoryAction>> m_undo;
    std::list<std::unique_ptr<HistoryAction>> m_redo;
  };
}

#include "history.inl"

#endif // HISTORY_H
