# encoding: utf-8 

def get_schedulers
  {
   "1" => "fcfs_core",
   "2" => "sjf_core",
   "3" => "srtf_core",
   "4" => "rr_core",
   "5" => "pr_core",
   "6" => "rtshd_core"
  }
end

def set_args
  {
    times: ARGV.shift,
    level: ARGV.shift,
    scheduler: ARGV.shift
  }
end

def read_input(input)
  input_data = {}
  File.open(input, 'r').readlines.each do |line|
    line = eval(line)
    input_data[line.first.to_sym] = line.last
  end
  return input_data
end

def calculate_square_dif(total_data, mean)
  std_deviation = {}
  total_data.each do |exec, result|
    context_swap = 0
    result.each do |total_result, procs|
      if(std_deviation[exec].nil?) 
        std_deviation[exec] = {}
      end
      procs.each do |proc, times|
        if(std_deviation[exec][proc].nil?)
          std_deviation[exec][proc] = {}
          std_deviation[exec][proc][:tf] = 0
          std_deviation[exec][proc][:tr] = 0
        end
        if(proc != :context_swap)
          std_deviation[exec][proc][:tr] += (times[:tr].to_i - mean[exec][proc][:tr])*(times[:tr].to_i - mean[exec][proc][:tr])
          std_deviation[exec][proc][:tf] += (times[:tf].to_i - mean[exec][proc][:tf])*(times[:tf].to_i - mean[exec][proc][:tf])
        else
          context_swap += (times.to_i - mean[exec][:context_swap])*(times.to_i - mean[exec][:context_swap])
        end
      end
    end
    std_deviation[exec][:context_swap] = context_swap
  end
  return std_deviation
end

def calculate_sqrt_div(std_deviation)
  mean_dev = {}
  std_deviation.each do |k, procs|
    mean_dev[k] = {} if(mean_dev[k].nil?)
    procs.each do |proc|
      key = proc.first
      mean_dev[k][key] = {} if(mean_dev[k][key].nil?)
      time = proc.last
      if(key != :context_swap)
        mean_dev[k][key] = {
          tf: Math.sqrt(time[:tf]/(TIMES*1.0)),
          tr: Math.sqrt(time[:tr]/(TIMES*1.0))
        }
      else
        mean_dev[k][key] = Math.sqrt(time/(TIMES*1.0))
      end
    end
  end
  return mean_dev
end

def calculare_error_rate(total_deviation, mean)
  error = {}
  mean.each do |k, procs|
    error[k] = {} if(error[k].nil?)
    procs.each do |proc|
      key = proc.first
      error[k][key] = {} if(error[k][key].nil?)
      time = proc.last
      if(key != :context_swap)
        error[k][key] = {
          tf: (1.96)*(total_deviation[k][key][:tf]/Math.sqrt(TIMES)),
          tr: (1.96)*(total_deviation[k][key][:tr]/Math.sqrt(TIMES))
        }
      else
        error[k][key] = (1.96)*(total_deviation[k][key]/Math.sqrt(TIMES))
      end
    end
  end
  return error
end

def print_result(mean, error, procs)
  mean.each do |alg, results|
    puts("Algoritmo : #{alg}")
    results.each do |proc, time|
      puts "Process : #{proc}"
      if(proc != :context_swap)
        puts (time[:tr] > procs[proc][:deadline].to_i) ? "Proceso FAILED" : "Proceso EXECUTED" 
        puts "TR : #{time[:tr] - error[alg][proc][:tr]} -- #{time[:tr]} -- #{time[:tr] + error[alg][proc][:tr]}"
        puts "TF : #{time[:tf] - error[alg][proc][:tf]} -- #{time[:tf]} -- #{time[:tf] + error[alg][proc][:tf]}\n\n"
      else
        puts "C Swap : #{time - error[alg][proc]} -- #{time} -- #{time + error[alg][proc]}\n\n"
      end
    end
  end
end

def get_full_results(mean, error, procs)
  procs_results = {}
  mean.each do |alg, results|
    results.each do |proc, time|
      procs_results[proc] = {} if(procs_results[proc].nil?)
      if(proc != :context_swap)
        procs_results[proc][alg] = {} if(procs_results[proc][alg].nil?)
        procs_results[proc][alg] = {
          status: (time[:tr] > procs[proc][:deadline].to_i) ? "FAILED" : "PROCESSED" ,
          tr: ["#{time[:tr] - error[alg][proc][:tr]}", "#{time[:tr]}" , "#{time[:tr] + error[alg][proc][:tr]}"], 
          tf: ["#{time[:tf] - error[alg][proc][:tf]}", "#{time[:tf]}" , "#{time[:tf] + error[alg][proc][:tf]}"]
        }
      else
        procs_results[proc][alg] = {total: ["#{time - error[alg][proc]}" , "#{time}" , "#{time + error[alg][proc]}"] }
      end
    end
  end
  return procs_results
end

def print_full_results(procs_results)
  i = 1
  procs_results.each do |process, schs|
    puts "Process : #{process}"
    schs.each do |k, v|
      if(process != :context_swap)
        # print "#{i}, #{v[:tr][1]}\n"
        print "#{v[:tr][1]}\n"
      else
        print "\npw #{v[:total][1]}"
      end
    end
    i += 1
    puts "\n"
  end

end


def read_proc_input(input)
  proc = {}
  File.open(input, 'r').readlines.each do |line|
    arr = line.split(" ")
    proc[arr[1].to_sym] = {
      t0:       arr[0],
      dt:       arr[2],
      deadline: arr[3],
      p:        arr[4]
    }
  end
  return proc
end


TIMES = 10
# LEVELS = [7,14,21]
LEVELS = [14]

scs = get_schedulers()

total_data = {}
puts "\n\n"
scs.each do |scheduler_num, scheduler|
  LEVELS.each do |lvl|
    input_data_name = "statistics/stat-" + "[#{lvl}]-#{scheduler}" + ".txt"
    total_data["#{scheduler}-#{lvl}"] = read_input(input_data_name)
  end
end


procs_sum = {}
total_data.each do |exec, result|
  context_swap = 0
  result.each do |total_result, procs|
    if(procs_sum[exec].nil?) 
      procs_sum[exec] = {}
    end
    procs.each do |proc, times|
      if(procs_sum[exec][proc].nil?)
        procs_sum[exec][proc] = {}
        procs_sum[exec][proc][:tf] = 0
        procs_sum[exec][proc][:tr] = 0
      end
      if(proc != :context_swap)
        procs_sum[exec][proc][:tf] += times[:tf].to_i 
        procs_sum[exec][proc][:tr] += times[:tr].to_i
      else
        context_swap += times.to_i
      end
    end
  end
  procs_sum[exec][:context_swap] = context_swap
end

mean = {}
procs_sum.each do |k, procs|
  mean[k] = {} if(mean[k].nil?)
  procs.each do |proc|
    key = proc.first
    mean[k][key] = {} if(mean[k][key].nil?)
    time = proc.last
    if(key != :context_swap)
      mean[k][key] = {
        tf: (time[:tf]/(TIMES*1.0)),
        tr: (time[:tr]/(TIMES*1.0))
      }
    else
      mean[k][key] = time/(TIMES*1.0)
    end
  end
end

std_deviation = calculate_square_dif(total_data, mean)
total_deviation = calculate_sqrt_div(std_deviation)
error = calculare_error_rate(total_deviation, mean)
input_procs = read_proc_input(input = "input/input-" + "#{LEVELS.first}" + ".txt")
# print_result(mean, error, input_procs)
procs_results = get_full_results(mean, error, input_procs)
# puts "#{procs_results}"
print_full_results(procs_results)
# puts "#{total_deviation}"
# puts "\n#{error}"
# puts ">>>>>>>>>>>>>>\n"
# puts "#{mean}"
# puts "\n\n\n"
# puts "#{total_data}"