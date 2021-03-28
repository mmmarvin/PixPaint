#include "updater.h"

#include <boost/beast/version.hpp>
#include "../3rdparty/jsonxx/jsonxx.h"
#include "../version.h"
#include "../macro.h"
#include "sslrequest.h"

namespace basio = boost::asio;
namespace bbeast = boost::beast;
namespace bhttp = bbeast::http;
namespace bssl = basio::ssl;
using btcp = basio::ip::tcp;

namespace pixpaint
{
  std::tuple<int, std::string, std::string> Updater::updateAvailable() const
  {
    try {
      requests::SSLRequest r(HOST, PORT);

      bhttp::request<bhttp::string_body> req(bhttp::verb::get, TARGET, 11);
      req.set(bhttp::field::host, HOST);
      req.set(bhttp::field::user_agent, BOOST_BEAST_VERSION_STRING);

      r.write(req);

      auto res = r.read_response();
      if(res.result() == bhttp::status::ok) {
        jsonxx::Object o;
        if(o.parse(requests::to_string(res.body()))) {
          auto latest_version_id = o.get<jsonxx::Object>("latest").get<jsonxx::Number>("version");
          if(latest_version_id > getVersionID()) {
#if defined(LINUX_VERSION)
            const auto& linux_object = o.get<jsonxx::Object>("latest").get<jsonxx::Object>("bin").get<jsonxx::Object>("linux");
#elif defined(WINDOWS_VERSION)
            const auto& win_object = o.get<jsonxx::Object>("latest").get<jsonxx::Object>("bin").get<jsonxx::Object>("win");
#endif // defined(LINUX_VERSION)

            std::string url =
#if defined(LINUX_VERSION)
                linux_object.get<jsonxx::String>("url");
#elif defined(WINDOWS_VERSION)
                win_object.get<jsonxx::String>("url");
#endif // defined(LINUX_VERSION)
            std::string checksum =
#if defined(LINUX_VERSION)
                    linux_object.get<jsonxx::String>("checksum");
#elif defined(WINDOWS_VERSION)
                    win_object.get<jsonxx::String>("checksum");
#endif // defined(LINUX_VERSION)
            return { 1, url, checksum };
          }

          return { 0, "", "" };
        }
      }
    } catch(...) {}

    return { -1, "", "" };
  }

  PIXPAINT_SINGLETON_FUNC_DEF(Updater)
}
