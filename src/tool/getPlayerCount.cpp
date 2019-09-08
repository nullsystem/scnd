#include "tool/getPlayerCount.h"

#include <sstream>
#include <algorithm>

// No need for json parser library, json raw data is very simple
int tool::getPlayerCount(std::string jsonRawData)
{
  int number = -1;
  std::stringstream ss;
  std::string temp;

  // Turn data to string stream than get first number (player count) out
  std::replace(jsonRawData.begin(), jsonRawData.end(), ':', ' ');
  std::replace(jsonRawData.begin(), jsonRawData.end(), ',', ' ');
  ss << jsonRawData;
  ss >> temp >> temp >> number;

  return number;
}
