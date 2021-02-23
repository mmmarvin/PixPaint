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
#ifndef GUARD_MAR_high_resolution_clock_H
#define GUARD_MAR_high_resolution_clock_H

// standard headers
#include <chrono>
#include <utility>

// custom headers
#include "time.h"

namespace mar
{
    class high_resolution_clock
    {
    public:
      high_resolution_clock();

      time get_elapsed() const;
      time restart();

    private:
      std::pair<time, std::chrono::high_resolution_clock::time_point> get_elapsed_impl() const;
      time restart_impl();

      std::chrono::high_resolution_clock::time_point  m_startTime;
    };

    class high_resolution_stopwatch : public high_resolution_clock
    {
    public:
        high_resolution_stopwatch();

        void start();
        void stop() noexcept;
        time restart();

        bool is_running() const noexcept { return m_isrunning; }

    private:
        bool                                            m_isrunning;
    };
}

#endif // GUARD_MAR_high_resolution_clock_H
