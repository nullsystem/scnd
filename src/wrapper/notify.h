#ifndef WRAPPER__NOTIFY_H
#define WRAPPER__NOTIFY_H

#include <libnotify/notify.h>
#include <string_view>

namespace wrapper
{
  class notify
  {
  private:
    NotifyNotification *notification;
  public:
    notify(std::string_view summary, std::string_view body, std::string_view icon = "dialog-information");
    ~notify();
    void static init();
    void show();
  };
}

#endif
