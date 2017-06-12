// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_ALARM_H_
#define NET_QUIC_QUIC_ALARM_H_

#include "base/memory/scoped_ptr.h"
#include "net/base/net_export.h"
#include "net/quic/quic_time.h"

namespace net {

// Abstract class which represents an alarm which will go off at a
// scheduled time, and execute the |OnAlarm| method of the delegate.
// An alarm may be cancelled, in which case it may or may not be
// removed from the underlying scheduling system, but in either case
// the task will not be executed.

/*
抽象类代表一个报警器，会在预定的时间执行该委托的| onAlarm |方法。
如果报警被取消，在这种情况下，它可能或不可能从底层调度系统中删除，但
在任何情况下，任务将不会被执行。
*/
//注意QuicEpollAlarm(继承QuicAlarm)  EpollAlarmImpl(继承EpollAlarm(继承EpollAlarmCallbackInterface))的关系,他们
//通过QuicEpollAlarm::SetImpl()->EpollServer::RegisterAlarm()衔接起来   //QuicEpollAlarm继承该类   
class NET_EXPORT_PRIVATE QuicAlarm {
 public:
  class NET_EXPORT_PRIVATE Delegate{ 
  /*
     常用的alarm如下，参考QuicConnection::QuicConnection
     该类接口在QuicEpollConnectionHelper::CreateAlarm赋值，实现该类虚拟即可在:
        AckAlarm  RetransmissionAlarm  SendAlarm  SendAlarm  TimeoutAlarm  PingAlarm  FecAlarm
     上面的各种alarm类存储在QuicConnection的以下成员中:
        ack_alarm_  retransmission_alarm_  send_alarm_  resume_writes_alarm_  timeout_alarm_  ping_alarm_  fec_alarm_
     上面的每种alarm都有对应独立的QuicEpollAlarm类，区别仅仅是QuicEpollAlarm::QuicAlarm::Delegate分别由各自的alarm实现
  */
   public:
    virtual ~Delegate() {}

    // Invoked when the alarm fires.  If the return value is not
    // infinite, then the alarm will be rescheduled at the
    // specified time.
    virtual QuicTime OnAlarm() = 0;
  };

  explicit QuicAlarm(Delegate* delegate);
  virtual ~QuicAlarm();

  // Sets the alarm to fire at |deadline|.  Must not be called while
  // the alarm is set.  To reschedule an alarm, call Cancel() first,
  // then Set().
  void Set(QuicTime deadline); //设置deadline_时间，然后执行SetImpl

  // Cancels the alarm.  May be called repeatedly.  Does not
  // guarantee that the underlying scheduling system will remove
  // the alarm's associated task, but guarantees that the
  // delegates OnAlarm method will not be called.
  void Cancel();

  // Cancels and sets the alarm if the |deadline| is farther from the current
  // deadline than |granularity|, and otherwise does nothing.  If |deadline| is
  // not initialized, the alarm is cancelled.
  void Update(QuicTime deadline, QuicTime::Delta granularity); //设置deadline_时间 

  bool IsSet() const;

  QuicTime deadline() const { return deadline_; }

 protected:
  // Subclasses implement this method to perform the platform-specific
  // scheduling of the alarm.  Is called from Set() or Fire(), after the
  // deadline has been updated.
  /*
  子类实现此方法来执行平台特定的报警调度。deadline_设置后，通过set和fire接口调用SetImpl
  */
  //进行epoll注册
  virtual void SetImpl() = 0; //实现见QuicEpollAlarm::SetImpl，注册epoll_server_->RegisterAlarm

  // Subclasses implement this method to perform the platform-specific
  // cancelation of the alarm.
  virtual void CancelImpl() = 0; //实现见QuicEpollAlarm::CancelImpl

  // Called by subclasses when the alarm fires.  Invokes the
  // delegates |OnAlarm| if a delegate is set, and if the deadline
  // has been exceeded.  Implementations which do not remove the
  // alarm from the underlying scheduler on Cancel() may need to handle
  // the situation where the task executes before the deadline has been
  // reached, in which case they need to reschedule the task and must not
  // call invoke this method.
  void Fire();

 private:
 /*
     常用的alarm如下，参考QuicConnection::QuicConnection
     该类接口在QuicEpollConnectionHelper::CreateAlarm赋值，实现该类虚拟即可在:
        AckAlarm  RetransmissionAlarm  SendAlarm  SendAlarm  TimeoutAlarm  PingAlarm  FecAlarm
     上面的每种alarm都有对应独立的QuicEpollAlarm类，区别仅仅是QuicEpollAlarm::QuicAlarm::Delegate分别由各自的alarm实现
  */
  scoped_ptr<Delegate> delegate_; //赋值见QuicEpollConnectionHelper::CreateAlarm()->QuicEpollAlarm()构造函数中调用
  QuicTime deadline_; //Set和Update接口赋值和更新

  DISALLOW_COPY_AND_ASSIGN(QuicAlarm);
};

}  // namespace net

#endif  // NET_QUIC_QUIC_ALARM_H_

