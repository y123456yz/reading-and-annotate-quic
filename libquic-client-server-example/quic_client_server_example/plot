#!/usr/bin/env gnuplot
set terminal png size 1024,768
set xlabel "Time (s)"
set ylabel "Mbit transfered"
set key outside center bottom vertical maxrows 3

run = 0
delays = "12 50 250"
bandwidths = "5 10 100"
losses = "0 1 5"

do for [delay in delays] {
do for [bw in bandwidths] {

set title sprintf("QUIC vs. TCP (%dms RTT, %s Mbps)", (delay+0)*2, bw)
set output sprintf("quic_vs_tcp_%dms_%sMbps.png", (delay+0)*2, bw)
plot [1:30] \
     for [loss in losses] sprintf("server_run_%d_._quic_perf_server_delay=%s_loss=%s_bw=%s", run, delay, loss, bw) index 0:30 u ($0+1):($1 * 8/1e6) \
     with linespoints pt 5 title sprintf("QUIC (%s%% loss)", loss), \
     for [loss in losses] sprintf("server_run_%d_._tcp_perf_server_delay=%s_loss=%s_bw=%s", run, delay, loss, bw) index 0:30 u ($0+1):($1 * 8/1e6) \
     with linespoints pt 9 title sprintf("TCP (%s%% loss)", loss), \
     (bw * x) with lines lw 2 lc rgb "#77ac30" title "Target bandwidth"
}
}