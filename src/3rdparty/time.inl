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
#include "time.h"

namespace mar
{
    template<class T>
    time& operator*=(time& lhs, const T& rhs)
    {
        lhs.m_value *= static_cast<time::nanoseconds_value_type>(rhs);
        return lhs;
    }

    template<class T>
    time operator*(const time& lhs, const T& rhs)
    {
        auto ret = lhs;
        ret *= rhs;

        return ret;
    }

    template<class T>
    time& operator/=(time& lhs, const T& rhs)
    {
        lhs.m_value /= static_cast<time::nanoseconds_value_type>(rhs);
        return lhs;
    }

    template<class T>
    time operator/(const time& lhs, const T& rhs)
    {
        auto ret = lhs;
        ret /= rhs;

        return ret;
    }
}
