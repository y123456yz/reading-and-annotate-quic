// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/tools/quic/quic_epoll_connection_helper.h"

#include <errno.h>
#include <sys/socket.h>

#include "base/logging.h"
#include "base/stl_util.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/crypto/quic_random.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "net/tools/quic/quic_socket_utils.h"

namespace net {
namespace tools {

//namespace { //yang add change  

//注意QuicEpollAlarm(继承QuicAlarm)  EpollAlarmImpl(继承EpollAlarm(继承EpollAlarmCallbackInterface))的关系,他们
//通过QuicEpollAlarm::SetImpl()->EpollServer::RegisterAlarm()衔接起来
class QuicEpollAlarm : public QuicAlarm {  //下面的EpollAlarmImpl类中包含QuicEpollAlarm类成员
 public:
 /*
   常用的alarm如下，参考QuicConnection::QuicConnection
   该类接口在QuicEpollConnectionHelper::CreateAlarm赋值，实现该类虚拟即可在:
      AckAlarm  RetransmissionAlarm  SendAlarm  SendAlarm  TimeoutAlarm  PingAlarm  FecAlarm
   上面的每种alarm都有对应独立的QuicEpollAlarm类，区别仅仅是QuicEpollAlarm::QuicAlarm::Delegate分别由各自的alarm实现
*/
  //QuicEpollConnectionHelper::CreateAlarm()中new QuicEpollAlarm类
  QuicEpollAlarm(EpollServer* epoll_server,
                 QuicAlarm::Delegate* delegate)
      : QuicAlarm(delegate),
        epoll_server_(epoll_server),
        epoll_alarm_impl_(this) {}

 protected:
  void SetImpl() override { //EpollServer注册
    DCHECK(deadline().IsInitialized());
    epoll_server_->RegisterAlarm(
        deadline().Subtract(QuicTime::Zero()).ToMicroseconds(),
        &epoll_alarm_impl_);
  }

  void CancelImpl() override {
    DCHECK(!deadline().IsInitialized());
    epoll_alarm_impl_.UnregisterIfRegistered();
  }

 private:
  //EpollAlarmImpl类包含QuicEpollAlarm成员，QuicEpollAlarm也包含EpollAlarmImpl类成员
  class EpollAlarmImpl : public EpollAlarm { //EpollAlarmImpl epoll_alarm_impl_;
   public:
    explicit EpollAlarmImpl(QuicEpollAlarm* alarm) : alarm_(alarm) {}

    int64 OnAlarm() override {//接口函数执行见CallAndReregisterAlarmEvents
      EpollAlarm::OnAlarm();
	  //AckAlarm  RetransmissionAlarm  SendAlarm  SendAlarm  TimeoutAlarm  PingAlarm  FecAlarm
	  //这些类的fire函数接口
      alarm_->Fire();
      // Fire will take care of registering the alarm, if needed.
      return 0;
    }

   private:
   	//AckAlarm  RetransmissionAlarm  SendAlarm  SendAlarm  TimeoutAlarm  PingAlarm  FecAlarm
    QuicEpollAlarm* alarm_;
  };

  //通过该成员把QuicEpollAlarm和EpollAlarm联系起来，见QuicEpollAlarm::SetImpl()->EpollServer::RegisterAlarm()
  EpollServer* epoll_server_; 
  
  EpollAlarmImpl epoll_alarm_impl_;
};

//}  // namespace

QuicEpollConnectionHelper::QuicEpollConnectionHelper(EpollServer* epoll_server)
    : epoll_server_(epoll_server),
      clock_(epoll_server),
      random_generator_(QuicRandom::GetInstance()) {
}

QuicEpollConnectionHelper::~QuicEpollConnectionHelper() {
}

const QuicClock* QuicEpollConnectionHelper::GetClock() const {
  return &clock_;
}

QuicRandom* QuicEpollConnectionHelper::GetRandomGenerator() {
  return random_generator_;
}

/*
   常用的alarm如下，参考QuicConnection::QuicConnection
      ack_alarm_(helper->CreateAlarm(new AckAlarm(this))),
      retransmission_alarm_(helper->CreateAlarm(new RetransmissionAlarm(this))),
      send_alarm_(helper->CreateAlarm(new SendAlarm(this))),
      resume_writes_alarm_(helper->CreateAlarm(new SendAlarm(this))),
      timeout_alarm_(helper->CreateAlarm(new TimeoutAlarm(this))),
      ping_alarm_(helper->CreateAlarm(new PingAlarm(this))),
      fec_alarm_(helper->CreateAlarm(new FecAlarm(&packet_generator_))),
*/
QuicAlarm* QuicEpollConnectionHelper::CreateAlarm(
    QuicAlarm::Delegate* delegate) { 
  return new QuicEpollAlarm(epoll_server_, delegate);
}

}  // namespace tools
}  // namespace net
