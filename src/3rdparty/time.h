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
#ifndef GUARD_MAR_duration_H
#define GUARD_MAR_duration_H

namespace mar
{
    class time
    {
        using nanoseconds_value_type = long long int;
        using microseconds_value_type = long int;
        using milliseconds_value_type = int;
        using seconds_value_type = double;

        static constexpr auto NANOSECONDS_MULTIPLIER = 1;
        static constexpr auto MICROSECONDS_MULTIPLIER = 1000;
        static constexpr auto MILLISECONDS_MULTIPLIER = 1000000;
        static constexpr auto SECONDS_MULTIPLIER = 1000000000.0;

    public:
        class nanoseconds
        {
        public:
            using value_type = nanoseconds_value_type;

            explicit nanoseconds(nanoseconds_value_type value) : m_value(value) {}

        private:
            friend class time;

            nanoseconds_value_type raw_value() const { return m_value; }

            nanoseconds_value_type m_value;
        };

        class microseconds : nanoseconds
        {
            using base_type = nanoseconds;

            friend class time;

        public:
            using value_type = microseconds_value_type;

            explicit microseconds(microseconds_value_type value) : base_type(value * MICROSECONDS_MULTIPLIER) {}
        };

        class milliseconds : nanoseconds
        {
            using base_type = nanoseconds;

            friend class time;

        public:
            using value_type = milliseconds_value_type;

            explicit milliseconds(milliseconds_value_type value) : base_type(value * MILLISECONDS_MULTIPLIER) {}
        };

        class seconds : nanoseconds
        {
            using base_type = nanoseconds;

            friend class time;

        public:
            using value_type = seconds_value_type;

            explicit seconds(seconds_value_type value) : base_type(static_cast<nanoseconds_value_type>(value * SECONDS_MULTIPLIER)) {}
        };

        constexpr time() noexcept : m_value(0) {}
        time(nanoseconds nas) : m_value(nas.raw_value()) {}
        time(microseconds mis) : m_value(mis.raw_value()) {}
        time(milliseconds mls) : m_value(mls.raw_value()) {}
        time(seconds sec) : m_value(sec.raw_value()) {}

        nanoseconds_value_type as_nanoseconds() const { return m_value; }
        microseconds_value_type as_microseconds() const { return static_cast<microseconds_value_type>(m_value / MICROSECONDS_MULTIPLIER); }
        milliseconds_value_type as_milliseconds() const { return static_cast<milliseconds_value_type>(m_value / MILLISECONDS_MULTIPLIER); }
        seconds_value_type as_seconds() const { return static_cast<seconds_value_type>(m_value) / static_cast<seconds_value_type>(SECONDS_MULTIPLIER); }
        double as_minute() const { return as_seconds() / 60.0; }
        double as_hour() const { return as_minute() / 24; }

    private:
        friend time& operator+=(time& lhs, const time& rhs);
        friend time& operator-=(time& lhs, const time& rhs);
        template<class T> friend time& operator*=(time& lhs, const T& rhs);
        template<class T> friend time& operator/=(time& lhs, const T& rhs);
        friend bool operator<(const time& lhs, const time& rhs);
        friend bool operator>(const time& lhs, const time& rhs);
        friend bool operator==(const time& lhs, const time& rhs);

        long long int m_value;
    };

    time& operator+=(time& lhs, const time& rhs);
    time operator+(const time& lhs, const time& rhs);
    time& operator-=(time& lhs, const time& rhs);
    time operator-(const time& lhs, const time& rhs);
    template<class T> time& operator*=(time& lhs, const T& rhs);
    template<class T> time operator*(const time& lhs, const T& rhs);
    template<class T> time& operator/=(time& lhs, const T& rhs);
    template<class T> time operator/(const time& lhs, const T& rhs);
    bool operator<(const time& lhs, const time& rhs);
    bool operator>(const time& lhs, const time& rhs);
    bool operator==(const time& lhs, const time& rhs);
}

// definitions
#include "time.inl"

#endif // GUARD_MAR_duration_H
