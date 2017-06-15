# QUIC toy
To evaluate QUIC and to get familiar with the implementation in Chromium, we wrote a "simple" QUIC client and server.
This repository contains this client, server as well as a TCP client and server for comparison.
The QUIC client and server depend on [libquic](https://github.com/devsisters/libquic), which is the Chromium QUIC implementation extracted into a library.
To build them, you need to have libquic commit `694a0608ef0b1dde6eafb7c87971e08935092d92` checked out into a `libquic` folder next to this project.
You also have to apply the `libquic.patch` of this repository to libquic.
Furthermore, there is a Python script that executes test cases to compare TCP and QUIC and a Gnuplot script do graph the results.

**This repository is not maintained anymore. If you are looking for a QUIC library, take a look at [proto-quic](https://github.com/google/proto-quic)**

### Documentation
**These documents are currently out of date, as libquic had breaking changes recently.**
The [documentation](documentation.markdown) file contains our findings and understanding of the QUIC implementation.
### How to write a quic client
**These documents are currently out of date, as libquic had breaking changes recently.**
The [howto](howto.markdown) file contains a description on how to implement your own QUIC using application, using the libquic library.
### Performance evaluation
The [performance](performance.markdown) file contains the results of our performance test and instructions on how to reproduce them.




# QUIC toy问题
最近下班业务时间，走读了下代码，toy只能用来做简单的测试，用在生产环境中是不合适的，存在以下几个问题：   
  
网络异常处理考虑不全   
大包传输网络读写事件处理返回EAGAIN没考虑，在极端情况会引起数据包读不全等问题。   
该测试程序只是简单的单线程收发网络模型，无法充分利用现有的CPU多核机制，如果是用quic来做“长连接“（握手协商后一直复用该connect）处理，可以参考nginx的网络模型或者memcached网络模型来实现基于udp的网络模型。   
toy中有份tcp和udp-quic的测试数据对比，实际上这个对比是没有意义的，因为toy中的udp-quic数据收发多了握手协商和加解密处理，因此没有可比性，应该拿测试中的udp-quic于https(带tls认证)来做对比。   
至于你代码中说的出现短错误，这个你可以通过GDB调试或者启用coredump功能。   
  
# 建议：    
在把quic使用到具体项目中前，最好把quic代码读透，否则线上出现问题，你没办法把控。  
  
toy例子只能作为简单的学习quic的工具，不适用于线上生产环境。  
