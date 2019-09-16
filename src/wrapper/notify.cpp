#include "wrapper/notify.h"

wrapper::notify::notify(std::string_view summary, std::string_view body, std::string_view icon)
{
  this->notification = notify_notification_new(summary.data(), body.data(), icon.data());
}

wrapper::notify::~notify()
{
  notify_notification_clear_actions(this->notification);
  g_object_unref(G_OBJECT(this->notification));
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

void wrapper::notify::addAction(std::string_view action, std::string_view label, NotifyActionCallback callback, gpointer user_data, GFreeFunc free_func)
{
  notify_notification_add_action(this->notification, action.data(), label.data(), NOTIFY_ACTION_CALLBACK(callback), user_data, free_func);
}

void wrapper::notify::update(std::string_view summary, std::string_view body, std::string_view icon)
{
  notify_notification_update(this->notification, summary.data(), body.data(), icon.data());
}
