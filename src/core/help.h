#ifndef CORE__HELP_H
#define CORE__HELP_H

#define HELP \
  "steamcountsnotifyd help\n"\
  "\n"\
  "-i   Interval: How long in minutes to delay between each player count fetch.\n"\
  "-t   Threshold interval: When the threshold's achieved, what interval gets extended to.\n"\
  "-c   Connection Timeout: How long in seconds the connection stay puts.\n"\
  "-n   Notification Timeout: How long in seconds the notification stays shown.\n"\
  "-a   Action type: Which type of clickable notification should use: 0 - default, 1 - button\n"\
  "-d   Daemon: Defaults to daemonize, 0 to disable daemonization\n"\
  "\n"\
  "-v   Prints version information.\n"\
  "-h   Prints this help information.\n"

#define VERSION \
  "steamcountsnotifyd\n"\
  "The Steam player count notification daemon\n"\
  "\n"\
  "0.0.2 - Alpha Release\n"

#endif
