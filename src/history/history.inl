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

namespace pixpaint
{
  template<class ActionT>
  void History::addAction(ActionT&& action)
  {
    static_assert(std::is_base_of<HistoryAction, ActionT>::value, "Action must inherit from HistoryAction");
    tryGetIndexes(action);

    m_redo.clear();
    m_undo.push_back(std::make_unique<ActionT>(std::forward<ActionT>(action)));
  }
}
