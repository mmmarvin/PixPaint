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
// custom headers
#include "high_resolution_clock.h"

namespace mar
{
    high_resolution_clock::high_resolution_clock() :
        m_startTime(std::chrono::high_resolution_clock::now())
    {
    }

    time high_resolution_clock::get_elapsed() const
    {
        return get_elapsed_impl().first;
    }

    time high_resolution_clock::restart()
    {
        return restart_impl();
    }

    std::pair<time, std::chrono::high_resolution_clock::time_point> high_resolution_clock::get_elapsed_impl() const
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        std::chrono::high_resolution_clock::time_point prev = m_startTime;

        time elapsed;
        elapsed += time(time::nanoseconds(std::chrono::duration_cast<std::chrono::nanoseconds>(now - prev).count()));

        return { elapsed, now };
    }

    time high_resolution_clock::restart_impl()
    {
        auto current_time = get_elapsed_impl();
        m_startTime = current_time.second;

        return current_time.first;
    }

    high_resolution_stopwatch::high_resolution_stopwatch() :
        m_isrunning(false)
    {
    }

    void high_resolution_stopwatch::start()
    {
        if(!m_isrunning) {
            restart();
            m_isrunning = true;
        }
    }

    void high_resolution_stopwatch::stop() noexcept
    {
        m_isrunning = false;
    }

    time high_resolution_stopwatch::restart()
    {
        if(!m_isrunning)
          m_isrunning = true;

        return high_resolution_clock::restart();
    }
}
