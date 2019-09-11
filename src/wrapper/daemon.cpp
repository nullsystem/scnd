#include "wrapper/daemon.h"

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

void signal_handler(int sig)
{
  switch (sig)
  {
  case SIGHUP:
    syslog(LOG_NOTICE, "Daemon hangup signal catched");
    break;
  case SIGTERM:
    syslog(LOG_NOTICE, "Daemon terminate signal catched");
    exit(0);
    break;
  }
}

void wrapper::daemon::init()
{
  pid_t pid;
  pid = fork();   // Fork off parent process

  // Error occurred
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }

  // Success: Let parent terminate
  if (pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  // On success: Child process becomes session leader
  if (setsid() < 0)
  {
    exit(EXIT_FAILURE);
  }

  // Catch, ignore and handle signals
  signal(SIGCHLD, SIG_IGN);         // Ignore child
  signal(SIGTSTP, SIG_IGN);         // Ignore tty signals
  signal(SIGTTOU, SIG_IGN);         // Ignore terminal output
  signal(SIGTTIN, SIG_IGN);         // Ignore terminal input
  signal(SIGHUP,  signal_handler);
  signal(SIGTERM, signal_handler);

  pid = fork();

  // Error occurred
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }

  // Success: Let parent terminate
  if (pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  umask(0);
  chdir("/");

  // Close all open file descriptors
  int x;
  for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
  {
    close(x);
  }

  // Open logging
  openlog("steamcountsnotifyd", LOG_PID, LOG_DAEMON);
}

void wrapper::daemon::uninit()
{
  closelog();
}

