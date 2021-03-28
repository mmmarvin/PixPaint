#ifndef SSLREQUEST_H
#define SSLREQUEST_H

#include <cstddef>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/beast/http/error.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

namespace requests
{
  class SSLRequest
  {
  public:
    SSLRequest(const char* host,
               const char* port,
               boost::asio::ssl::context_base::method context_method = boost::asio::ssl::context::tlsv12_client);
    ~SSLRequest();

    void close();

    void write(const boost::beast::http::request<boost::beast::http::string_body>& request);
    boost::beast::http::response<boost::beast::http::dynamic_body> read_response();
    template<size_t Size, class Func> boost::beast::error_code read_response_chunks(Func func);

  private:
    boost::asio::io_context                            m_ioc;
    boost::asio::ssl::context                          m_ctx;
    boost::beast::ssl_stream<boost::beast::tcp_stream> m_stream;
  };

  std::string to_string(const boost::beast::multi_buffer& buff);
}

#include "sslrequest.inl"

#endif // SSLREQUEST_H
