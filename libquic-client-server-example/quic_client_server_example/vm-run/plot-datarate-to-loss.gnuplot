#!/usr/bin/env gnuplot
set terminal png size 1024,768
set xlabel "Packetloss in %"
set xtics 0,2.5,10
set ylabel "Average Datarate Achieved [Mbps]"
set key outside center bottom vertical maxrows 3
bandwidths = "5 10 100"
delays = "12 50 250"

do for [bandwidth in bandwidths] {

set title sprintf("Maximum Network Datarate of %s Mbps", bandwidth)
set output sprintf("datarate-to-loss_%sMbps.png", bandwidth)
plot for [delay in delays] sprintf("datarate-to-loss_bandwidth=%s", bandwidth) \
     i sprintf("[Delay %s]", delay) u "loss":"tcp-average":"tcp-standard_deviation" \
     with yerrorlines pt 4 lc rgb "#edb120" \
     title sprintf("TCP average %dms RTT", delay * 2), \
     for [delay in delays] sprintf("datarate-to-loss_bandwidth=%s", bandwidth) \
     i sprintf("[Delay %s]", delay) u "loss":"quic-average":"quic-standard_deviation" \
     with yerrorlines pt 5 lc rgb "#2b4ba1" \
     title sprintf("QUIC average %dms RTT", delay * 2), \
}