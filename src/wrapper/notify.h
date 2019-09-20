#ifndef WRAPPER__NOTIFY_H
#define WRAPPER__NOTIFY_H

#include <libnotify/notify.h>
#include <string_view>

namespace notify_t
{
  using callback = NotifyActionCallback;
  using pointer = gpointer;
  using freeFunc = GFreeFunc;
}

namespace wrapper
{
  class notify
  {
  private:
    NotifyNotification *notification;
  public:
    notify(std::string_view summary = "", std::string_view body = "", std::string_view icon = "dialog-information");
    ~notify();
    void static init(std::string_view threadName);
    void static uninit();
    void show();
    void setTimeout(unsigned int timeout);
    void addAction(std::string_view action, std::string_view label, NotifyActionCallback callback, gpointer user_data = nullptr, GFreeFunc free_func = nullptr);
    void update(std::string_view summary = "", std::string_view body = "", std::string_view icon = "dialog-information");
    void setHint(std::string_view key, GVariant *value);
  };
}

#endif
