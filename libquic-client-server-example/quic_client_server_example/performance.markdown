# Performance evaluation
To determine whether we can use QUIC, we did some simple performance tests.
We used [Mininet](http://mininet.org/) to construct a virtual network in which we can test QUIC and compare it to TCP.
The network simulates only two hosts, connected by a single link.

```
+----+      -----+
|    |  L1  |    |
| H1 <------> H2 |
|    |      |    |
+----+      +----+
```

On host H1 we started the QUIC (or TCP) server and on H2 we started the QUIC (or TCP) client.
The client then sends as much data as possible for 30 seconds.
The server writes the byte count of the received data to stdout each second.

We ran the test for several different values of parameters of the link.
These parameters include the latency of the link, the bandwidth and the packet loss rate.

# Steps to reproduce
Unfortunately, Mininet is only available on Linux, so you need a Linux host with Mininet installed.

1. Compile the QUIC/TCP client and server.

    You need to have [libquic](https://github.com/devsisters/libquic) checked out in the same folder than this project and successfully build.
    Currently we use a patched version of libquic to avoid the SPDY components.
    Therefore you have to apply the patch `libquic.patch` before building libquic.
    Then you should be able to run `make` to build everything.

2. Run `performance_test.py` as root! (Mininet requires to be run as root.)

    The performance_test.py will iterate over several combinations of latency, bandwidth and loss rate.
    By changing a function call, there is the possibility to either execute all test in sequence or some in parallel.
    Take a look at the script to see how to increase the number of tests, executed in parallel.
    Beware that running the test ins sequence can take a long time! Each test run takes at least 30 seconds.
    You can also run the test multiple times by tweaking the NUM_RUNS constant.

3. Run `plot` to plot the graphs for one run.

    For this you will need to have gnuplot installed.
    The `plot` script will plot graphs for one run, just make sure that the possible values for delay, loss and bandwidth are the same in `performance_test.py` and `plot`.
    If you want to determine statistics about the average data-rate, use the `build-datarate-to-loss-table.rb` script in the `vm-run` directory.
    The generated statistics can then be plotted using the `plot-datarate-to-loss.gnuplot` from the same directory.

# Results
In general it seems that QUIC is able to transport more data at a higher packet loss rates, while TCP does perform better at low packet loss rates and high bandwidth.
It is unclear however what influence the software of the virtual network has on the protocol performance.
Furthermore, the default settings for send and receive windows where used. QUIC allows to customize several parameters of the flow control and we have not tested whether changing those would improve throughput.

Our measurements and graphs for a single run can be found in the `results` folder (these are a bit older).
At all bandwidths, TCP outperforms QUIC on low packet loss rates and this becomes more visible at higher bandwidths.

To improve the accuracy of our results, we ran the test for all parameters multiple times and then aggregated (after sorting out failed test runs) all collected data-rates for each parameter combination.
These results are in the `vm-run` folder (as the test was run on a virtual machine).
The mean datarates for each parameter combination can be seen in the following graphs.
The y-axis shows the amount of data transmitted on average, the error bars show the standard deviation.
On the x-axis the different percentages of package loss are plotted.
![Graph: 5 Mbps](vm-run/datarate-to-loss_5Mbps.png)`
![Graph: 10 Mbps](vm-run/datarate-to-loss_10Mbps.png)
At 100 MBps the performance difference between TCP and QUIC becomes visible.
![Graph: 100 Mbps](vm-run/datarate-to-loss_100Mbps.png)

The same data is available in tabular form in `vm-run/datarate-to-loss_bandwidth=(5|10|100)`.

For a 100Mbps link the tables looks like this:

#### RTT 24ms
packet loss [%] | TCP average [Mbps] | TCP standard deviation [Mbps] | QUIC average [Mbps] | QUIC standard deviation [Mbps]
---- | ---- | ---- | ---- | ----
0 | 95.601 | 3.147 | 46.629 | 2.022
2.5 | 79.678 | 16.975 | 21.420 | 4.192
5 | 15.311 | 9.773 | 16.408 | 3.625
7.5 | 6.281 | 4.693 | 13.700 | 3.302
10 | 1.777 | 1.668 | 10.157 | 3.155


#### RTT 100ms
packet loss [%] | TCP average [Mbps] | TCP standard deviation [Mbps] | QUIC average [Mbps] | QUIC standard deviation [Mbps]
---- | ---- | ---- | ---- | ----
0 | 95.609 | 0.098 | 46.782 | 1.716
2.5 | 80.115 | 18.305 | 21.412 | 4.050
5 | 15.026 | 9.743 | 16.085 | 3.398
7.5 | 6.276 | 4.921 | 13.783 | 3.253
10 | 2.152 | 1.846 | 10.232 | 3.298


#### RTT 500ms
packet loss [%] | TCP average [Mbps] | TCP standard deviation [Mbps] | QUIC average [Mbps] | QUIC standard deviation [Mbps]
---- | ---- | ---- | ---- | ----
0 | 95.604 | 0.123 | 46.441 | 1.576
2.5 | 81.561 | 15.078 | 21.385 | 4.325
5 | 15.606 | 9.532 | 14.618 | 3.244
7.5 | 6.548 | 4.859 | 12.240 | 2.840
10 | 1.807 | 1.641 | 9.145 | 2.733
