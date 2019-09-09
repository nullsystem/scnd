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
    curl();
    ~curl();

    void perform();

    // Setters
    void setUrl(std::string_view newUrl);
    void setTimeout(int timeout = 10);

    // Getters
    int getHttpResponseCode();
    std::string getHttpData();

    // Removers
    void clearHttpData();
  };
}

#endif

