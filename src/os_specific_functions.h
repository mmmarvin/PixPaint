#ifndef OS_SPECIFIC_FUNCTIONS_H
#define OS_SPECIFIC_FUNCTIONS_H

#include <string>

namespace pixpaint
{
namespace os_specific
{
  bool call_process(const std::string& process_name,
                    const std::string& process_parameters);
}
}

#endif // OS_SPECIFIC_FUNCTIONS_H
