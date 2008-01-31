#!/usr/bin/env ruby

require 'yaml'

$system_freq = 16000000
$prescaler = 64

def to_timing(us)
    ($system_freq.to_f/$prescaler * us / 1000000).round
end

def convert_raw(code, pwm)
    if code.has_key? 'repeat' then
        repeat_delay = code['repeat_delay']
        # puts "original repeat is %d" % repeat_delay
        codelen = code['pattern'].inject(0) { |mem, obj| mem+obj[0]+obj[1] }
        # puts "codelen is %d" % codelen
        repeat_delay -= codelen
        # puts "repeat_delay is %d" % repeat_delay

        repeat = ($system_freq.to_f/$prescaler * repeat_delay / 1000000).round
    else
        repeat = 0
    end

    puts "    (%d << 8) | %d, %d," % [pwm, code['repeat'], repeat]
    puts "    /* pattern (on, length): */"

    if code['pattern'].last[1] != 0 then
        $stderr.puts "last length not zero!"
        exit 2
    end

    code['pattern'].each do |on, off|
        d_on = $system_freq.to_f / $prescaler * on / 1000000
        d_off = $system_freq.to_f / $prescaler * off / 1000000

        if (d_on+d_off) > 65535 then
            $stderr.puts "ohoh, off duration > 65535: %d %d ..."  % [d_on, d_off]
        end

        # add the on time, if this is not the last on pulse
        d_off += d_on if (d_off > 0)

        puts "    %d, %d," % [d_on, d_off]
    end
end

def convert_nec(code, pwm)
    repeat = ($system_freq.to_f/$prescaler * code['repeat_delay'] / 1000000).round
    puts "    (%d << 8) | %d, %d," % [pwm, code['repeat'], repeat]
    puts "    /* data: */"
    puts "    0x%s, 0x%s," % [ (code['data'][1] * 256 + code['data'][0]).to_s(16),
                          (code['data'][3] * 256 + code['data'][2]).to_s(16)]
end

def convert_pause(code, pwm)
    repeat = to_timing(code['repeat_delay'])
    puts "     (%d << 8) | %d, %d," % [pwm, code['repeat'], repeat]
    puts "    /* parameters */"

    if code.has_key? 'header' then
        puts "    /* header on timing, header length */"
        header_on = to_timing(code['header'][0])
        header_length = to_timing(code['header'][0]+code['header'][1])
        puts "     0x%s, 0x%s," % [header_on.to_s(16), header_length.to_s(16)]
    else
        puts "    /* no header */"
        puts "     0, 0,"
    end

    puts "    /* on length, length one (on+pause), length zero (on+pause) */"
    on = to_timing(code['on_length'])
    one = to_timing(code['on_length']+code['pause_one'])
    zero = to_timing(code['on_length']+code['pause_zero'])

    puts "     0x%s, 0x%s, 0x%s," % [on.to_s(16), one.to_s(16), zero.to_s(16)]

    puts "    /* flags, bitcount */ "

    bits = code['bits']
    bits |= (1<<8) if code['flags'].member? 'repeat_header'
    bits |= (1<<9) if code['flags'].member? 'repeat_subtract_header'

    puts "     0x%s," % bits.to_s(16)

    puts "    /* data: */"

    bits = code['bits']

    if bits == 16 || bits == 12 || bits == 10 then
        puts "     0x%s," % [ (code['data'][1] * 256 + code['data'][0]).to_s(16) ]
    elsif bits == 24 then
        puts "     0x%s, 0x%s," % [(code['data'][1] * 256 + code['data'][0]).to_s(16), \
                                                             code['data'][2].to_s(16)]
    elsif bits == 32 then
        print "     "
        0.upto(1) do |i|
            print "0x%s, " % (code['data'][2*i+1] * 256 + code['data'][2*i]).to_s(16)
        end
        puts
    elsif bits == 42 then
        print "     "
        0.upto(2) do |i|
            print "0x%s, " % (code['data'][2*i+1] * 256 + code['data'][2*i]).to_s(16)
        end
        puts
    elsif bits == 48 then
        print "     "
        0.upto(2) do |i|
            print "0x%s, " % (code['data'][2*i+1] * 256 + code['data'][2*i]).to_s(16)
        end
        puts
    else
        $stderr.puts "unsupported number of bits: %d" % bits
        exit 1
    end
end

code = YAML.load(open(ARGV[0]).read())

# calculate pwm freq with prescalers 1 and 8
pwm1 = $system_freq/(2*1*code['frequency'])
freq1 = $system_freq/(2*1*pwm1)
pwm8 = $system_freq/(2*8*code['frequency'])
freq8 = $system_freq/(2*8*pwm8)

puts "    /***************************************************"
puts "     * code %s" % ARGV[0]
puts "     ***************************************************/"
puts
if code['type'] == 'raw' then
    puts "    (uint16_t)send_raw,"
elsif code['type'] == 'nec' then
    puts "    (uint16_t)send_nec,"
elsif code['type'] == 'pause' then
    puts "    (uint16_t)send_pause,"
else
    $stderr.puts "unknown code type: %s" % code['type']
    exit 2
end

puts "    /* freq %d:    pwm8: %3d (%6d), pwm1: %3d (%6d)" % [code['frequency'], pwm8, freq8, pwm1, freq1]
err1 = (code['frequency']-freq1).abs
err8 = (code['frequency']-freq8).abs
puts "     * error:        %4d,              %4d" % [err8, err1]

pwm = 0

if err8 <= err1 || pwm1 > 255 then
    puts "     * selected freq8 */"
    pwm = pwm8
else
    puts "     * selected freq1 */"
    pwm = pwm1
end

puts "    /* pwm, repeat, repeat_delay */"

if code['type'] == 'raw' then
    convert_raw(code, pwm)
elsif code['type'] == 'nec' then
    convert_nec(code, pwm)
elsif code['type'] == 'pause' then
    convert_pause(code, pwm)
end
