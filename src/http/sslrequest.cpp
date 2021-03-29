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
#include "sslrequest.h"

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>


namespace basio = boost::asio;
namespace bbeast = boost::beast;
namespace bhttp = bbeast::http;
namespace bssl = basio::ssl;
using btcp = basio::ip::tcp;

namespace requests
{
namespace
{
  bssl::context get_ssl_context(bssl::context_base::method context_method)
  {
    bssl::context ctx(context_method);
    ctx.set_default_verify_paths();
    ctx.set_verify_mode(bssl::verify_peer);

    return ctx;
  }
}
  SSLRequest::SSLRequest(const char* host,
                         const char* port,
                         bssl::context_base::method context_method) :
    m_ctx(get_ssl_context(context_method)),
    m_stream(m_ioc, m_ctx)
  {
    btcp::resolver resolver(m_ioc);

    if(!SSL_set_tlsext_host_name(m_stream.native_handle(), host)) {
      bbeast::error_code ec;
      throw bbeast::system_error(ec);
    }

    const auto results = resolver.resolve(host, port);
    bbeast::get_lowest_layer(m_stream).connect(results);
    m_stream.handshake(bssl::stream_base::client);
  }

  SSLRequest::~SSLRequest()
  {
    close();
  }

  void SSLRequest::close()
  {
    auto& socket = bbeast::get_lowest_layer(m_stream).socket();

    socket.cancel();
    socket.close();
  }

  void SSLRequest::write(const boost::beast::http::request<boost::beast::http::string_body>& request)
  {
    bhttp::write(m_stream, request);
  }

  boost::beast::http::response<boost::beast::http::dynamic_body> SSLRequest::read_response()
  {
    bbeast::flat_buffer buff;
    bhttp::response<bhttp::dynamic_body> response;

    bhttp::read(m_stream, buff, response);

    return response;
  }

  std::string to_string(const boost::beast::multi_buffer& buff)
  {
    return bbeast::buffers_to_string(buff.data());
  }
}
