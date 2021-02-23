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
#include <QApplication>
#include <iostream>
#include "3rdparty/gengine/configuration.h"
//#include "utility/settings_file_utility.h"
#include "window/globalkeyreader.h"
#include "window/mainwindow.h"
#include "define.h"

void initializeThreadPool();
void saveConfigurations();

int main(int argc, char** argv)
{
  int fargc = 1; std::array<const char*, 1> fargv = { argv[0] };
  QApplication app(fargc, const_cast<char**>(fargv.data()));

  initializeThreadPool();
  auto* mainWindow = new pixpaint::MainWindow(pixpaint::getRunParam(argc, argv));
  mainWindow->show();

  QObject::connect(&app, &QApplication::aboutToQuit, mainWindow, &pixpaint::MainWindow::exit);
  app.installEventFilter(new pixpaint::GlobalKeyReader);
  return app.exec();
}

void initializeThreadPool()
{
//  volatile auto& threadPool = pixpaint::getThreadPool();
//  threadPool;
}
