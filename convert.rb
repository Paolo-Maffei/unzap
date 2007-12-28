#!/usr/bin/env ruby

$stdin.each_line do |line|

    if line =~ /^\s*(\d+)\s*(\d+)\s*$/
        on, off = Regexp.last_match.to_a[1..-1]
        on = (on.to_i* 3.125).round
        off = (off.to_i*3.125).round
        if on > 65535 || off > 65535
            puts "error: %s" % line
            exit 1
        end
        puts "    { %5d, %5d }, " % [ on, off ]
    end
end
