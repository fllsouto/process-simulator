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
  proc = []
  File.open(input, 'r').readlines.each do |line|
    arr = line.split(" ")
    proc << {
      t0:       arr[0],
      name:     arr[1],
      dt:       arr[2],
      deadline: arr[3],
      p:        arr[4]
    }
  end
end

def count_process(input)
  process_counter = 0
  File.open(input, 'r').readlines.each do |line|
    process_counter = process_counter + 1
  end
  return process_counter
end


def read_output(out, size)
  proc = {}
  counter = 0;
  File.open(out, 'r').readlines.each do |line|
    if(counter < size)
      arr = line.split(" ")
      proc[arr[0].to_sym] = {
        tf:   arr[1],
        tr:   arr[2]
      }
    else
      arr = line.scan(/\d+/)
      proc[:context_swap] = arr[0]
    end
    counter = counter + 1
  end

  return proc
end

def exec_scheduler(exec, i, scheduler, input, output)
  puts ">"*30
  puts "Stating Exec [#{i}]"
  puts "Scheduler : #{scheduler}"
  puts "Input : #{input}"
  puts "Output : #{output}"

  out = %x{ #{exec} '#{scheduler}' '#{input}' '#{output}'}
  puts "Ended Exec"
  puts "<"*30
end

def write_output_data(output_data, output_data_name)
  File.open(output_data_name, 'w') do |f|
    output_data.each do |d|
      f.puts "#{d}"
    end
  end
end

TIMES = 10
# LEVELS = [7,14,21]
LEVELS = [14]

scs = get_schedulers()
inicio = Time.now

puts "Inicio : #{inicio}"
scs.each do |scheduler_num, scheduler|
  puts ">!"*40
  LEVELS.each do |lvl|
    input = "input/input-" + "#{lvl}" + ".txt"
    output = "output/output-" + "#{lvl}-#{scheduler}" + ".txt"
    output_data_name = "statistics/stat-" + "[#{lvl}]-#{scheduler}" + ".txt"
    exec = "./exec/ep1"
    total_process = count_process(input)
    output_data = {}
    for i in 0...TIMES
      exec_scheduler(exec, i, scheduler_num, input, output)
      output_data["[#{lvl}]-#{scheduler}-#{i}".to_sym] = read_output(output, total_process)
    end

    write_output_data(output_data, output_data_name)

  end
  puts "<!"*40
end
fim = Time.now
puts "Fim : #{fim}"

total = fim.to_i - inicio.to_i

puts "Total : #{total}"

