/*
 *
 */
// https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid=244630

#include <iostream>
#include "wrapper/curl.h"
#include "wrapper/notify.h"

int main(int argc, char **argv)
{
  wrapper::curl curlJob("https://api.steampowered.com/ISteamUserStats/GetNumberOfCurrentPlayers/v1/?appid=244630");

  curlJob.perform();
  curlJob.getHttpResponseCode();

  wrapper::notify::init();
  wrapper::notify notifyJob("NEOTOKYO", curlJob.getHttpData());
  notifyJob.show();

  return 0;
}

