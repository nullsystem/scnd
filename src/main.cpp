/* 
 * steamcountsnotifyd
 *
 * Steam counter notiifcation daemon
 * version Pre-Alpha 2019-09-11
 *
 * TODO:
 *  Daemonise
 */

#include <iostream>
#include <thread>
#include <deque>
#include <vector>
#include <functional>

#include "core/counterThread.h"
#include "core/params.h"
#include "tool/args.h"
#include "tool/getPlayerCount.h"
#include "wrapper/curl.h"
#include "wrapper/daemon.h"
#include "wrapper/notify.h"

/* 
 * Main function of steamcountsnotifyd
 */
int main(int argc, char **argv)
{
  std::deque<std::string> args = tool::toArgs(argc, argv);
  args.pop_front();     // Program execution name not needed
  param::config config;
  bool          running = config.setFromArgs(args);

  // If help or version message not used (normal execution)
  if (running)
  {
    //std::function<void(unsigned int, param::appidName_s, const param::config &)> thread::appidRunning;
    wrapper::notify::init("steamcountsnotifyd");

    // Initialise and run threads for each appid/game 
    std::vector<std::thread> appidThreadVector;

    for (const auto &[appid, game] : config.getAppidMap())
    {
      appidThreadVector.emplace_back(std::thread(std::function(cthread::appidRunning), appid, game, config));
    }

    // Join thread if joinable
    for (std::thread &thread : appidThreadVector)
    {
      if (thread.joinable())
      {
        thread.join();
      }
    }

    wrapper::notify::uninit();
  }

  return 0;
}

