#include "sslrequest.h"

namespace requests
{
  template<size_t Size, class Func>
  boost::beast::error_code SSLRequest::read_response_chunks(Func func)
  {
    boost::beast::flat_buffer buffer;
    boost::beast::http::parser<false, boost::beast::http::buffer_body> p;

    boost::beast::error_code ec;
    boost::beast::http::read_header(m_stream, buffer, p, ec);
    if(ec) {
      return ec;
    }

    while(!p.is_done()) {
      std::array<char, Size> buff;
      p.get().body().data = buff.data();
      p.get().body().size = buff.size();

      boost::beast::http::read(m_stream, buffer, p, ec);
      if(ec == boost::beast::http::error::need_buffer) {
        ec = {};
      }

      if(ec) {
        return ec;
      }

      func(buff);
    }

    return boost::beast::error_code();
  }
}