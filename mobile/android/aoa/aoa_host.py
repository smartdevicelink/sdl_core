import usb.core
import usb.util
import sys
import time

# Source: http://android.serverbox.ch/?p=262
# Install the dependencies with `pip install -r aoa_host_requirements.py`

#Change these for your particular device!  Use 'lsusb' on linus or USBView on Windows
dev = usb.core.find(idVendor=0x22b8, idProduct=0x70A9)
if dev is None:
    raise ValueError('Device not found')

#This crashes?
#dev.set_configuration()

cfg = dev.get_active_configuration()

#intf = cfg[(0,0)]
intf = cfg[(1,0)]

ep_in = usb.util.find_descriptor(
    intf, custom_match = lambda e: \
    usb.util.endpoint_direction(e.bEndpointAddress) == \
    usb.util.ENDPOINT_IN
)

ep_out = usb.util.find_descriptor(
    intf, custom_match = lambda e: \
    usb.util.endpoint_direction(e.bEndpointAddress) == \
    usb.util.ENDPOINT_OUT
)

assert ep_out is not None
assert ep_in is not None

#verify endpoint addresses
print "OUT ep addr is ", hex(ep_out.bEndpointAddress)
print "IN  ep addr is ", hex(ep_in.bEndpointAddress)

#do ctrl transfers to setup accessory mode
msg = dev.ctrl_transfer(0xc0, 51, 0, 0, 2)  #Read 2 bytes

devVersion = msg[1] << 8 | msg[0];
print "Verion Code Device:", devVersion
time.sleep(1)	#sometimes hangs on the next transfer :(

manufacturer = "Nexus-Computing GmbH"
modelName = "osciprime"
description = "Description"
version = "antilope"
uri = "http://nexus-computing.ch"
serialNumber = "2254711SerialNo."

# these are based onlibusb_control_transfer(handle,0x40,52,0,0,(char*)manufacturer,strlen(manufacturer),0);
assert dev.ctrl_transfer(0x40, 52, 0, 0, manufacturer) == len(manufacturer)
assert dev.ctrl_transfer(0x40, 52, 0, 1, modelName) == len(modelName)
assert dev.ctrl_transfer(0x40, 52, 0, 2, description) == len(description)
assert dev.ctrl_transfer(0x40, 52, 0, 3, version) == len(version)
assert dev.ctrl_transfer(0x40, 52, 0, 4, uri) == len(uri)
assert dev.ctrl_transfer(0x40, 52, 0, 5, serialNumber) == len(serialNumber)

print "Accessory Identification sent", devVersion

assert dev.ctrl_transfer(0x40, 53, 0, 0, "") == 0

print "Put device into accessory mode", devVersion
try:
    data = ep_in.read(16)
    print data
except usb.USBError:
    print "USB error"
    del dev
    del cfg
    del intf
    del ep_in
    del ep_out
    raise

del dev
del cfg
del intf
del ep_in
del ep_out
