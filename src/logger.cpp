#include "logger.h"

#include <chrono>
#include <ctime>
#include "utility/settings_file_utility.h"
#include "config.h"
#include "debug_log.h"
#include "os_specific_headers.h"

namespace pixpaint
{
  Logger::Logger()
  {
    settings_file_utils::tryCreateSettingsFilename();
    if(!os_specific::filesystem::exists(settings_file_utils::getLogFilename())) {
      m_out.open(settings_file_utils::getLogFilename(), std::ios_base::out);
      if(m_out.is_open()) {
        m_out << APP_TITLE
              << " v." << APP_VERSION_MAJOR << "." << APP_VERSION_MINOR << APP_VERSION_PATCH
              << "-" << APP_RELEASE_TYPE << "." << APP_VERSION_RELEASE
              << " Log File\n"
              << "=========================\n"
              << std::endl;
      }
    } else {
      m_out.open(settings_file_utils::getLogFilename(), std::ios_base::out);
    }
  }

  void Logger::write(std::string msg)
  {
    auto remove_newline_and_space = [](std::string s) {
      while(s.size() &&
            (isspace(s.back()) || s.back() == '\n')) {
        s.pop_back();
      }

      return s;
    };

    if(m_out.is_open()) {
      auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      auto current_time = std::string(std::ctime(&t));
      m_out << remove_newline_and_space(current_time) << ": " << msg << std::endl;
    }
  }

  PIXPAINT_SINGLETON_FUNC_DEF(Logger)
}
