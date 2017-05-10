#!/usr/bin/env ruby

FILE_NAME_TEMPLATE = "server_run_%s_._%s_perf_server_delay=%s_loss=%s_bw=%s"

BWS = [5, 10, 100]
LOSSES = [0, 2.5, 5, 7.5, 10]
DELAYS = [12, 50, 250]
PROTOS = ['tcp', 'quic']
RUNS = (1..25)
DATA_POINTS = 29
SCALE = 8.0 / 10**6 # convert from bits to megabyte


BWS.each do |bandwidth|

  table = File.open("datarate-to-loss_bandwidth=%s" % [bandwidth], 'w')
  DELAYS.each do |delay|
    # start new record for delay `delay`
    table << "#[Delay %s]\n" % delay
    table << "loss tcp-average tcp-standard_deviation quic-average quic-standard_deviation\n"
    LOSSES.each do |loss|
      table << "%04.1f " % loss
      PROTOS.each do |proto|
        # start calculation over all runs
        data_points = []
        RUNS.each do |run|
          File.open(FILE_NAME_TEMPLATE % [run, proto, delay, loss, bandwidth]) do |f|
            # declare last_value, so it will be in the outer scope
            last_value = 0
            f.each_with_index do |line, index|
              if index == 0 then
                # skip the first value in each run
                # it usually has a larger delta than the next datapoints
                # due to the way the sampling mechanism is started
                last_value = line.to_i
                next
              end
              # do not use more than DATA_POINTS data points in a run
              # the rest might be garbage if the test did not shut down properly
              break if index > DATA_POINTS
              # if something is not a number an error is occured
              # and we should discard the rest of the test
              break unless line =~ /[0-9]+/
              current_value = line.to_i
              # if no data was transfered since the last sampling point
              # the test client or server did crash and we should discard
              # the rest of the test
              break if current_value == last_value
              # save the delta of the sampeling points which describes
              # the amount of data transferred in one second
              data_points << ((current_value - last_value) * SCALE)
              last_value = current_value
            end
          end
        end
        mean = data_points.reduce(:+) / data_points.size
        standard_deviation = Math.sqrt(data_points.map{|dp| (dp - mean) ** 2 }.reduce(:+) / data_points.size)
        table << "%.3f %.3f " % [mean, standard_deviation]
        # finished calculation over all runs
      end
      # finish calculation for all losses, start new record
      table << "\n"
    end

    # finish record
    table << "\n\n"
  end
  table.close
end
