#ifndef CORE__COUNTERTHREAD_H
#define CORE__COUNTERTHREAD_H

#include <functional>
#include "core/params.h"

namespace cthread
{
  void run(unsigned int appid, param::appidName_s game, const param::config &config);
}

#endif
