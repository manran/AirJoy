/*
 * MiniNotifyTypedef.h
 *
 * jxfengzi@gmail.com
 *
 *
 */

#ifndef __MINI_NOTIFY_TYPEDEF_H__
#define __MINI_NOTIFY_TYPEDEF_H__

namespace miniipc
{

  typedef enum _MiniNotifyResult
  {
    MiniNotifyResultNull    = 0,
    MiniNotifyResultOk      = 100,
    MiniNotifyResultError   = 200,
  } MiniNotifyResult;

}

#endif // __MINI_NOTIFY_TYPEDEF_H__