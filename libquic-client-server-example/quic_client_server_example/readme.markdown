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
