#include "wrapper/notify.h"

wrapper::notify::notify(std::string_view summary, std::string_view body, std::string_view icon)
{
  this->notification = notify_notification_new(summary.data(), body.data(), icon.data());
}

wrapper::notify::~notify()
{
  g_object_unref(G_OBJECT(this->notification));
  notify_uninit();
}

void wrapper::notify::init()
{
  notify_init("neonotifytokyo");
}

void wrapper::notify::show()
{
  notify_notification_show(this->notification, nullptr);
}
