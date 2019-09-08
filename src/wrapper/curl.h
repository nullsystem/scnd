#ifndef WRAPPER__CURL_H
#define WRAPPER__CURL_H

#include <cstdint>
#include <string_view>
#include <memory>
#include <curl/curl.h>

namespace wrapper
{
  class curl
  {
  private:
    std::string_view   url;
    CURL              *curlPointer;
    int                httpResponseCode;
    std::size_t static callback(const char *in, std::size_t size, std::size_t num, std::string *out); 
    std::unique_ptr<std::string> httpData;

  public:
    curl(std::string_view newUrl, int timeout = 10);
    ~curl();
    void perform();
    int getHttpResponseCode();
    std::string getHttpData();
  };
}

#endif

