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

  int getVersionID()
  {
    auto get_release_type_id = [] {
      switch(APP_RELEASE_TYPE) {
      case 'a':
        return 0;
      case 'b':
        return 1;
      case 'rc':
        return 2;
      default:
        break;
      }

      return 3;
    };

    return (APP_VERSION_MAJOR * 10000) +
           (APP_VERSION_MINOR * 1000) +
           (APP_VERSION_PATCH * 100) +
           (get_release_type_id * 10) +
           APP_VERSION_RELEASE;
  }
}
