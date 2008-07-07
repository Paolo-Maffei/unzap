#!/usr/bin/env python

# load modules
import sys
import random
try:
    import optparse
    import usb
except ImportError, e:
    sys.stderr.write("unable to load module: %s\n" % e)
    sys.exit(2)


class Unzap:
    # usb requests
    USBRQ_ECHO                      = 1
    USBRQ_GET_DATAFLASH_INFO        = 2
    USBRQ_GET_DATAFLASH_STATUS      = 3
    USBRQ_READ_WRITE_DATAFLASH      = 4
    USBRQ_DATAFLASH_ERASE           = 5
    USBRQ_DATAFLASH_ERASE_PAGE      = 6

    USBRQ_IR_STATUS                 = 10
    USBRQ_IR_RECORD                 = 11
    USBRQ_IR_PLAY                   = 12
    USBRQ_IR_READ_DATA              = 13

    # dataflash return values
    DF_OK = 0
    DF_PROCESSING = 1
    DF_NOT_FOUND = 2
    DF_BUSY = 3
    DF_ERROR = 4

    # instance variables
    device = None
    handle = None

    pagesize = None
    pages = None
    chipid = None

    def __init__(self, dev):
        self.dev = dev
        try:
            self.handle = dev.open()
        except usb.USBError, e:
            sys.stderr.write("unable to open device: %s\n" % e)

        self.pages, self.pagesize, self.chipid = self.get_pagesize()
        if not self.identify_dataflash(): sys.stderr.write("unknown dataflash!\n")

    # lowlevel functions
    def control_msg_in(self, code, value = 0, index = 0, data = 0, timeout = 100):
        response = self.handle.controlMsg(
                usb.TYPE_VENDOR | usb.RECIP_INTERFACE | usb.ENDPOINT_IN,
                code, data, value = value, index = index, timeout = timeout)
        return response

    def control_msg_out(self, code, value = 0, index = 0, data = 0, timeout = 100):
        response = self.handle.controlMsg(
                usb.TYPE_VENDOR | usb.RECIP_INTERFACE | usb.ENDPOINT_OUT,
                code, data, value = value, index = index, timeout = timeout)
        return response

    # usb api functions
    def get_pagesize(self):
        response = self.control_msg_in(self.USBRQ_GET_DATAFLASH_INFO, data = 5)
        pages = response[0] + response[1] * 256
        pagesize = response[2] + response[3] * 256
        chipid = response[4]
        return (pages, pagesize, chipid)

    def dataflash_busy(self):
        response = self.control_msg_in(self.USBRQ_GET_DATAFLASH_STATUS, data = 1)
        return not response[0]

    def echo(self, num):
        response = self.control_msg_in(self.USBRQ_ECHO, data = 2, value = num)
        num = response[0] + response[1] * 256
        return num

    def erase_chip(self):
        response = self.control_msg_in(self.USBRQ_DATAFLASH_ERASE, data = 1)
        return response[0]

    def erase_page(self, page):
        response = self.control_msg_in(self.USBRQ_DATAFLASH_ERASE_PAGE, value = page, data = 1)
        return response[0]

    def read_page(self, page, offset, length):
        response = self.control_msg_in(self.USBRQ_READ_WRITE_DATAFLASH, value = page, index = offset, data = length)
        return response

    def write_page(self, page, offset, data):
        self.control_msg_out(self.USBRQ_READ_WRITE_DATAFLASH, value = page, index = offset, data = data)

    def record_status(self):
        return self.control_msg_in(self.USBRQ_IR_STATUS, data = 1)[0]

    def record(self):
        self.control_msg_out(self.USBRQ_IR_RECORD)

    def read_recorded(self):
        response = self.control_msg_in(self.USBRQ_IR_READ_DATA, data = 600)
        return response

    # high level functions
    def identify_dataflash(self):
        if self.chipid == 0x0B:
            return 'AT45DB161D'
        return None


def find_devices(vid = None, pid = None, vendor_name = None, product_name = None):
    devices = []

    for bus in usb.busses():
        for dev in bus.devices:
            try:
                if vid and dev.idVendor != vid: continue
                if pid and dev.idProduct != pid: continue

                if vendor_name or product_name:
                    handle = dev.open()
                    vendor_string = handle.getString(dev.iManufacturer, 40)

                    if vendor_name and vendor_name != vendor_string: continue

                    product_string = handle.getString(dev.iProduct, 40)
                    if product_name and product_name != product_string: continue

                devices.append(dev)
            except usb.USBError, e:
                sys.stderr.write("usb error: %s\n" % e)


    return devices


# main function, when called directly
if __name__ == "__main__":

    parser = optparse.OptionParser(usage = 'usage: %prog [options] command')

    group = optparse.OptionGroup(parser, 'usb options')
    group.add_option('-v', '--vid', dest = 'vid',
            type = 'int', default = 0x16c0,
            help = 'unzap usb vendor id (default: %default)')
    group.add_option('-p', '--pid', dest = 'pid',
            type = 'int', default = 0x282e,
            help = 'unzap usb product id (default: %default)')
    group.add_option('-V', '--vendor-name', dest = 'vendor_name',
            type = 'string', default = 'lochraster.org',
            help = 'unzap usb vendor string (default: %default)')
    group.add_option('-P', '--product-name', dest = 'product_name',
            type = 'string', default = 'unzap',
            help = 'unzap usb product string (default: %default)')
    group.add_option('-i', '--index', dest = 'index',
            type = 'int', default = '0',
            help = 'if more than one matching device is found, use this index in the list')

    parser.add_option_group(group)

    (options, args) = parser.parse_args()

    devices = find_devices(options.vid, options.pid, options.vendor_name, options.product_name)

    if len(devices) == 0:
        sys.stderr.write("no unzap found!\n")
        sys.exit(1)

    if len(devices) > 0 and options.index < len(devices)-1:
        dev = devices[options.index]
    else:
        dev = devices[0]

    unzap = Unzap(dev)

    if len(args) == 0:
        print "nothing to do..."
        sys.exit(0)

    if args[0] == "echotest":
        print "trying echotest"
        random.seed()
        for i in range(0, 2000):
            num = random.randint(0, 65535)
            res = unzap.echo(num)
            if res != num:
                print "error: received %d instead of %d" % (res, num)
                sys.exit(3)
            sys.stdout.write('.')
            sys.stdout.flush()
        print "\nechotest passed"
    elif args[0] == "erase_chip":
        print "executing chip erase..."
        res = unzap.erase_chip()
        if res != unzap.DF_PROCESSING:
            print "error, got response %d for erase chip command" % res
            sys.exit(5)

        while unzap.dataflash_busy():
            sys.stdout.write('.')
            sys.stdout.flush()

        print "done"
    elif args[0] == "erase_page":
        try:
            page = int(args[1])
        except:
            page = None

        if len(args) != 2 or page == None:
            sys.stderr.write("usage: erase_page PAGENUM\n")
            sys.exit(2)

        print "erasing page %d" % page
        res = unzap.erase_page(page)

        if res != unzap.DF_PROCESSING:
            print "error, got response %d for erase page command" % res
            sys.exit(5)

        while unzap.dataflash_busy():
            sys.stdout.write('.')
            sys.stdout.flush()

        print "done"

    elif args[0] == "read_page":
        try:
            page = int(args[1])
        except:
            page = None

        if page == None:
            sys.stderr.write("usage: read_page PAGENUM [OFFSET] [LENGTH]\n")
            sys.exit(2)

        try:
            offset = int(args[2])
        except:
            print "using 0 as offset"
            offset = 0

        try:
            length = int(args[3])
        except:
            length = unzap.pagesize-offset
            print "using pagesize-offset as length: %d" % length

        data = list(unzap.read_page(page, offset, length))
        print "got %d byte:" % len(data)
        addr = offset
        while len(data) > 0:
            line = data[0:16]
            sys.stdout.write("%04x   " % addr)
            addr += 16
            for byte in line:
                sys.stdout.write(" %02x" % byte)
            del(data[0:16])
            sys.stdout.write("\n")
            sys.stdout.flush()
    elif args[0] == "write_page":
        try:
            page = int(args[1])
        except:
            page = None

        try:
            file = open(args[2])
        except IOError, e:
            sys.stderr.write("unable to open file: %s\n" % e)
            sys.exit(6)
        except IndexError:
            sys.stderr.write("usage: write_page PAGENUM FILE [OFFSET] [LENGTH]\n")
            sys.exit(1)

        if page == None:
            sys.stderr.write("usage: write_page PAGENUM FILE [OFFSET] [LENGTH]\n")
            sys.exit(2)

        try:
            offset = int(args[3])
        except:
            print "using 0 as offset"
            offset = 0

        try:
            length = int(args[4])
        except:
            length = unzap.pagesize-offset
            print "using pagesize-offset as length: %d" % length

        unzap.write_page(page, offset, file.read(length))

        while unzap.dataflash_busy():
            sys.stdout.write('.')
            sys.stdout.flush()

        print "done"
    elif args[0] == "status":
        print unzap.record_status()
    elif args[0] == "record":
        unzap.record()
    elif args[0] == "read_recorded":
        data = list(unzap.read_recorded())

        while len(data) > 0:
            l = data[0] + data[1] * 256
            l *= 16
            print l
            del(data[0:2])

    else:
        print "nothing to do..."
        sys.exit(0)

