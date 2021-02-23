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
#include "eventmanager.h"

namespace pixpaint
{
namespace event_manager_detail
{
  template<class EventT>
  void EventManager::registerListener(EventListener<EventT>& listener)
  {
    auto id = typeid(EventT).hash_code();
    m_listeners[id].push_back(&listener);
  }

  template<class EventT>
  void EventManager::emit(const EventT& event)
  {
    auto id = typeid(EventT).hash_code();
    auto findIt = m_listeners.find(id);
    if(findIt != m_listeners.end()) {
      auto e = static_cast<const void*>(&event);
      for(auto* listener : findIt->second) {
        listener->emit(e);
      }
    }
  }
}
  template<class EventT>
  EventListener<EventT>::EventListener()
  {
    event_manager_detail::getEventManager().registerListener(*this);
  }

  template<class EventT>
  void EventListener<EventT>::emit(const void* event)
  {
    onEmit(*static_cast<const EventT*>(event));
  }

  template<class EventT>
  void emitEvent(const EventT& event)
  {
    event_manager_detail::getEventManager().emit(event);
  }
}
