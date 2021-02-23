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
#include "script_errors.h"

#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "../env/guienvironment.h"
#include "../window/consoletoolbox.h"

namespace bp = boost::python;

namespace pixpaint
{
  std::string getScriptError()
  {
    PyObject* type, * value, * traceback;
    PyErr_Fetch(&type, &value, &traceback);

    bp::handle<> htraceback(traceback);
    bp::object otraceback(htraceback);
    bp::handle<> htype(type);
    bp::object otype(htype);

    auto err_line_number = bp::extract<long>(otraceback.attr("tb_lineno"))();
    auto err_filename = bp::extract<std::string>(otraceback.attr("tb_frame").attr("f_code").attr("co_filename"))();
    auto err_funcname = bp::extract<std::string>(otraceback.attr("tb_frame").attr("f_code").attr("co_name"))();
    auto err_type = bp::extract<std::string>(otype.attr("__name__"))();
    auto err_msg = bp::extract<std::string>(value)();

    // emulate pytohn error
    auto error_msg = std::string("Traceback (most recent call last):\n") +
                     std::string(" File \"") + err_filename + std::string("\", line ") +
                     std::to_string(err_line_number) + std::string(", in ") + err_funcname + std::string("\n") +
                     err_type + std::string(": ") + err_msg;
    PyErr_Clear();

    return error_msg;
  }

  void printScriptError()
  {
    getGUIEnvironment().getConsoleToolbox().writeMessage(getScriptError());
  }
}
