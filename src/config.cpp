#include "config.h"

namespace pixpaint
{
  std::string getAppAndVersion()
  {
    return getApp() + getVersion();
  }

  std::string getApp()
  {
    return APP_TITLE;
  }

  std::string getVersion()
  {
    auto ver = std::string(" v.") +
               std::to_string(APP_VERSION_MAJOR) +
               std::string(".") +
               std::to_string(APP_VERSION_MINOR) +
               std::to_string(APP_VERSION_PATCH);

    if(!std::string(APP_RELEASE_TYPE).empty()) {
      ver += std::string("-") +
             std::string(APP_RELEASE_TYPE) +
             std::string(".") +
             std::to_string(APP_VERSION_RELEASE);
    }

    return ver;
  }
}
