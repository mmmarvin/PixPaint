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
#include "updaterwindow.h"

#include <atomic>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <boost/beast/version.hpp>
#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStyle>
#include <QPainter>
#include <QVBoxLayout>
#include "../src/http/http_helper.h"
#include "../src/http/sslrequest.h"
#include "../src/os_specific_functions.h"
#include "../src/version_specific_headers.h"
#include "updater_version.h"

namespace bbeast = boost::beast;
namespace bhttp = bbeast::http;

namespace pixpaint_updater
{
  UpdaterWindow::UpdaterWindow(std::string url, std::string checksum) :
    QDialog(nullptr),
    m_ready(false),
    m_done(false)
  {
    this->setWindowTitle(getAppAndVersion().c_str());
    this->setFixedSize(400, 100);

    m_downloaderThread = std::thread(&UpdaterWindow::dowloaderThread, this, url, checksum);

    try {
//      auto* central_widget = new QWidget(this);

      auto* layout = new QVBoxLayout(this);

      m_label = new QLabel("Downloading update...", this);
      m_progressBar = new QProgressBar(this);
      m_progressBar->setMinimum(0);
      m_progressBar->setMaximum(100);
      m_progressBar->setValue(50);
      m_progressBar->setFormat("");
      m_progressBar->setStyleSheet("QProgressBar::chunk { background: #0000FF; width: 5px; }");

      layout->addSpacerItem(new QSpacerItem(0, 20));
      layout->addWidget(m_label);
      layout->addWidget(m_progressBar);
      layout->addSpacerItem(new QSpacerItem(0, 20));

//      this->setCentralWidget(central_widget);
    } catch(...) {
      m_done.store(true);
      if(m_downloaderThread.joinable()) {
        m_downloaderThread.join();
      }

      throw;
    }
  }

  UpdaterWindow::~UpdaterWindow()
  {
    if(m_downloaderThread.joinable()) {
      m_downloaderThread.join();
    }
  }

  void UpdaterWindow::closeEvent(QCloseEvent* event)
  {
    auto res = QMessageBox::question(this, "Updater", "Update is not finished! Do you want to continue to exit?", QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, QMessageBox::StandardButton::No);
    if(res == QMessageBox::StandardButton::Yes) {
      // Tell the downloader thread to exit
      m_done.store(true);
    }

    event->ignore();
  }

  void UpdaterWindow::showEvent(QShowEvent*)
  {
    m_ready.store(true);
  }

  void UpdaterWindow::dowloaderThread(std::string url, std::string checksum)
  {
    using namespace std::chrono;

    // wait until the window is ready
    while(!m_ready.load()) {
      if(m_done.load()) {
        return;
      }

      std::this_thread::sleep_for(1s);
    }

    // parse the url
    auto url_structure = pixpaint::http_helper::parseURL(url);
    if(url_structure.protocol.empty() ||
       url_structure.hostname.empty() ||
       url_structure.pathname.empty()) {
      QMessageBox::critical(this, "Updater", "There was a problem parsing the update URL!");
      QApplication::exit(1);
    }

//    std::cout << url_structure.protocol << "\n"
//              << url_structure.hostname << "\n"
//              << url_structure.pathname << std::endl;

    // open the file to write to
    auto out_path = (version_specific::filesystem::temporary_path() /
                     version_specific::filesystem::path("pixpaint")).string();
    auto out_filename =  (version_specific::filesystem::path(out_path) /
                          version_specific::filesystem::path("update.zip")).string();

    if(!version_specific::filesystem::exists(out_path)) {
      if(!version_specific::filesystem::create_directory(out_path)) {
        QMessageBox::critical(this, "Updater", "Cannot write to hard disk!");
        QApplication::exit(1);
      }
    }

    if(version_specific::filesystem::exists(out_filename)) {
      if(std::remove(out_filename.c_str())) {
        QMessageBox::critical(this, "Updater", "Cannot write to hard disk!");
        QApplication::exit(1);
      }
    }

    std::ofstream out_file(out_filename, std::ios_base::binary | std::ios_base::app);

//    std::cout << "Saving file to " << (version_specific::filesystem::temporary_path() /
//                                       version_specific::filesystem::path("pixpaint") /
//                                       version_specific::filesystem::path("update.zip")).string() << std::endl;

    if(!out_file.is_open()) {
      QMessageBox::critical(this, "Updater", "Cannot write to hard disk!");
      QApplication::exit(1);
    }

    requests::SSLRequest r(url_structure.hostname.c_str(),
                           url_structure.protocol.c_str());

    bhttp::request<bhttp::string_body> req(bhttp::verb::get, url_structure.pathname.c_str(), 11);
    req.set(bhttp::field::host, url_structure.hostname.c_str());
    req.set(bhttp::field::user_agent, BOOST_BEAST_VERSION_STRING);

    r.write(req);

    auto res = r.read_response_chunks<512>([this, &out_file]
    (const std::array<unsigned char, 512>& buff, size_t read_size) {
      if(m_done.load()) {
        return false;
      }

      out_file.write(reinterpret_cast<const char*>(buff.data()), read_size);
      return true;
    });

    if(!m_done.load()) {
      if(res) {
        throw bbeast::system_error(res);
      }

      // run the extractor
      pixpaint::os_specific::callProcess("extract", out_filename + " .");
      QApplication::exit(0);
    }

    QApplication::exit(1);
  }
}