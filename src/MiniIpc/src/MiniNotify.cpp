/*
 * MiniNotify.cpp
 *
 * jxfengzi@gmail.com
 *
 *
 */

#include "MiniNotify.h"
#include "MiniNotifyInfo.h"

#include <iostream>


using namespace miniipc;

MiniNotify::MiniNotify()
{
}

MiniNotify::~MiniNotify()
{
  MiniNotifyInfo *p = NULL;
  MiniNotifyList::iterator iter;
  for (iter = m_list.begin(); iter != m_list.end(); ++iter)
  {
    p = *iter;
    delete p;
  }
}

MiniNotifyInfo * MiniNotify::find(const std::string &name)
{
  MiniNotifyInfo *p = NULL;

  MiniNotifyList::iterator iter;
  for (iter = m_list.begin(); iter != m_list.end(); ++iter)
  {
    p = *iter;
    if (p->name() == name)
      return p;
  }

  if (p == NULL)
  {
    p = new MiniNotifyInfo(name);
    if (p)
      m_list.push_back(p);
  }

  return p;
}

bool MiniNotify::send(const std::string &name, MiniNotifyResult result)
{
  MiniNotifyInfo *p = this->find(name);
  if (p)
    return p->send(result);

  return false;
}

bool MiniNotify::recv(const std::string &name, MiniNotifyResult *result, int second)
{
  MiniNotifyInfo *p = this->find(name);
  if (p)
    return p->recv(result, second);

  return false;
}