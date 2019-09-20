/* 
 * steamcountsnotifyd
 *
 * Steam counter notiifcation daemon
 * version Alpha v0.0.2 - 2019-09-16
 *
 * TODO:
 *  Per-server notification
 */

#include <iostream>
#include <thread>
#include <deque>
#include <vector>
#include <functional>
#include <memory>

#include "core/counterThread.h"
#include "core/params.h"
#include "tool/args.h"
#include "tool/getPlayerCount.h"
#include "wrapper/curl.h"
#include "wrapper/daemon.h"
#include "wrapper/notify.h"

static GMainLoop *loop;

int main(int argc, char **argv)
{
  std::deque<std::string> args = tool::toArgs(argc, argv);
  args.pop_front();     // Program execution name not needed
  param::config config;
  bool          running = config.setFromArgs(args);

  loop = g_main_loop_new(nullptr, FALSE); 

  // If help or version message not used (normal execution)
  if (running)
  {
    // Initialise daemon and notification
    if (config.getDaemonize())
    {
      wrapper::daemon::init();
    }
    wrapper::notify::init("steamcountsnotifyd");

    // Notify that the daemon started up
    {
      wrapper::notify startUpNotify("steamcountsnotifyd", "Started up");
      startUpNotify.setTimeout(5);
      startUpNotify.show();
    }

    // Initialise and run threads for each appid/game 
    std::vector<std::thread> appidThreadVector;

    for (const auto &[appid, game] : config.getAppidMap())
    {
      appidThreadVector.emplace_back(std::thread(std::function(cthread::run), appid, game, config));
    }

    g_main_loop_run(loop);

    // Join thread if joinable
    for (std::thread &thread : appidThreadVector)
    {
      if (thread.joinable())
      {
        thread.join();
      }
    }

    // Uninitialise/destroy daemon and notification
    wrapper::notify::uninit();

    if (config.getDaemonize())
    {
      wrapper::daemon::uninit();
    }
  }

  return 0;
}

