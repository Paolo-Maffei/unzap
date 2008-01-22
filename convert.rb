#!/usr/bin/env ruby

pattern_start = false

puts "type: raw"

$stdin.each_line do |line|

    if line =~ /^\s*(\d+)\s*(\d+)\s*$/
        on, off = Regexp.last_match.to_a[1..-1]
        #on = (on.to_i* 3.125).round
        #off = (off.to_i*3.125).round
        on = on.to_i
        off = off.to_i
        if on > 65535 || off > 65535
            puts "error: %s" % line
            exit 1
        end
        on *= 10
        off *= 10
        if not pattern_start then
            puts "pattern:"
            pattern_start = true
        end

        puts "- [ %5d, %5d ]" % [ on, off ]
    end

    if line =~ /^Carrier Freq\s*=\s*(\d+\.\d+) /
        freq = Regexp.last_match.to_a.last.to_f
        freq = (freq * 1000).to_i
        puts "frequency: %d" % freq
    end

end
