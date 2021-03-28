#include "version.h"

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

  int getVersionID()
  {
    auto get_release_type_id = [] {
      auto release_type = std::string(APP_RELEASE_TYPE);
      if(release_type == "a") {
        return 0;
      } else if(release_type == "b") {
        return 1;
      } else if(release_type == "rc") {
        return 2;
      }

      return 3;
    };

    return (APP_VERSION_MAJOR * 10000) +
           (APP_VERSION_MINOR * 1000) +
           (APP_VERSION_PATCH * 100) +
           (get_release_type_id() * 10) +
           APP_VERSION_RELEASE;
  }
}
