#include "wrapper/notify.h"

wrapper::notify::notify(std::string_view summary, std::string_view body, std::string_view icon)
{
  this->notification = notify_notification_new(summary.data(), body.data(), icon.data());
}

wrapper::notify::~notify()
{
}

void wrapper::notify::init(std::string_view threadName)
{
  notify_init(threadName.data());
}

void wrapper::notify::uninit()
{
  notify_uninit();
}

void wrapper::notify::show()
{
  notify_notification_show(this->notification, nullptr);
}

void wrapper::notify::setTimeout(unsigned int timeout)
{
  notify_notification_set_timeout(this->notification, timeout);
}

void wrapper::notify::unref()
{
  g_object_unref(G_OBJECT(this->notification));
}
