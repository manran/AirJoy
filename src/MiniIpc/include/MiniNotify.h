/*
 * MiniNotify.h
 *
 * jxfengzi@gmail.com
 *
 *
 */

#ifndef __MINI_NOTIFY_H__
#define __MINI_NOTIFY_H__

#include "MiniNotifyTypedef.h"

#include <string>
#include <list>

namespace miniipc
{

  class MiniNotifyInfo;
 
  class MiniNotify
  {
  public:
    MiniNotify();
    ~MiniNotify();
    bool send(const std::string &name, MiniNotifyResult result);
    bool recv(const std::string &name, MiniNotifyResult *result, int second);

  private:
    MiniNotifyInfo * find(const std::string &name);

  private:
    typedef std::list<MiniNotifyInfo *> MiniNotifyList;
    MiniNotifyList m_list;
  };



}

#endif // __MINI_NOTIFY_H__