#include "wrapper/curl.h"

// Static method
std::size_t wrapper::curl::callback(const char *in, std::size_t size, std::size_t num, std::string *out)
{
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

wrapper::curl::curl(std::string_view newUrl, int timeout)
{
  this->url = newUrl;
  this->httpData = std::make_unique<std::string>();

  this->curlPointer = curl_easy_init();
  curl_easy_setopt(this->curlPointer, CURLOPT_URL, this->url.data());
  curl_easy_setopt(this->curlPointer, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
  curl_easy_setopt(this->curlPointer, CURLOPT_TIMEOUT, timeout);
  curl_easy_setopt(this->curlPointer, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(this->curlPointer, CURLOPT_WRITEFUNCTION, wrapper::curl::callback);
  curl_easy_setopt(this->curlPointer, CURLOPT_WRITEDATA, httpData.get());
}

wrapper::curl::~curl()
{
  curl_easy_cleanup(this->curlPointer);
}

void wrapper::curl::perform()
{
  curl_easy_perform(this->curlPointer);
  curl_easy_getinfo(this->curlPointer, CURLINFO_RESPONSE_CODE, &this->httpResponseCode);
}

int wrapper::curl::getHttpResponseCode()
{
  return this->httpResponseCode;
}

std::string wrapper::curl::getHttpData()
{
  return *this->httpData.get();
}
