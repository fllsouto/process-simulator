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

x = get_schedulers()

x.each do |key, sch|
  puts "key #{key} -- sch #{sch}"
end