#include "os_specific_functions.h"

#include <boost/process.hpp>

namespace pixpaint
{
namespace os_specific
{
  template<class... Args>
  bool callProcessArgs(const std::string& process_name,
                       Args&&... args)
  {
    if(sizeof...(Args) == 0) {
      try {
        boost::process::child c(process_name);

        if(c.running()) {
          c.detach();
          return true;
        }
      } catch(...) {}

      return false;
    }

    try {
      boost::process::child c(process_name,
                              std::forward<Args>(args)...);

      if(c.running()) {
        c.detach();
        return true;
      }
    } catch(...) {}

    return false;
  }
}
}
