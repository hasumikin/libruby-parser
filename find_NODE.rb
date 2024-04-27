#! /usr/bin/env ruby

if ARGV[0].match(/\A\d+\z/)
  num = ARGV[0].to_i
elsif ARGV[0].match(/\A\w+\z/)
  name = ARGV[0].upcase
else
  raise "Argument must be a number or a name of node."
end


h = File.expand_path("./lib/ruby/rubyparser.h", File.dirname(__FILE__))

File.open(h, "r") do |f|
  while line = f.gets
    raise if f.eof?
    break if line.start_with?("enum node_type")
  end

  if num
    num.times { f.gets }
    node_name = f.gets
    node_name_body = node_name.match(/NODE_(\w+)/)[1]
  elsif name
    num = 0
    while line = f.gets
      raise if f.eof?
      break if line.match(/NODE_#{name}/)
      num += 1
    end
    node_name = line
    node_name_body = name
  end
  print num
  puts node_name
  puts

  while line = f.gets
    raise if f.eof?
    break if line.match(/typedef\s+struct\s+RNode_#{node_name_body}/)
  end
  puts line
  while line = f.gets
    raise if f.eof?
    break if line.match(/^}/)
    puts line
  end
  puts line
end
