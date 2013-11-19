#!/usr/bin/env python

import time

import usb.core
import usb.util


# Source: http://android.serverbox.ch/?p=262
# You can install the dependencies with `pip install -r requirements.txt`

def setupAccessory():
    #This crashes?
    #dev.set_configuration()

    #do ctrl transfers to setup accessory mode
    #Read 2 bytes
    msg = dev.ctrl_transfer(0xc0, 51, 0, 0, 2)

    devVersion = msg[1] << 8 | msg[0]
    print("Version Code Device: {}".format(devVersion))
    #sometimes hangs on the next transfer :(
    time.sleep(1)

    manufacturer = "Nexus-Computing GmbH"
    modelName = "osciprime"
    description = "Description"
    version = "antilope"
    uri = "http://nexus-computing.ch"
    serialNumber = "2254711SerialNo."

    # these are based on libusb_control_transfer(handle,0x40,52,0,0,(char*)manufacturer,strlen(manufacturer),0);
    assert dev.ctrl_transfer(0x40, 52, 0, 0, manufacturer) == len(manufacturer)
    assert dev.ctrl_transfer(0x40, 52, 0, 1, modelName) == len(modelName)
    assert dev.ctrl_transfer(0x40, 52, 0, 2, description) == len(description)
    assert dev.ctrl_transfer(0x40, 52, 0, 3, version) == len(version)
    assert dev.ctrl_transfer(0x40, 52, 0, 4, uri) == len(uri)
    assert dev.ctrl_transfer(0x40, 52, 0, 5, serialNumber) == len(serialNumber)

    print("Accessory Identification sent")

    assert dev.ctrl_transfer(0x40, 53, 0, 0, "") == 0

    print("Put device into accessory mode")


#Change these for your particular device!  Use 'lsusb' on linus or USBView on Windows
dev = usb.core.find(idVendor=0x18d1, idProduct=0x4e42)
if dev:
    setupAccessory()

print("Reconnecting to USB accessory")
dev = usb.core.find(idVendor=0x18d1, idProduct=0x2d01)
if dev is None:
    raise ValueError('Device not found')

try:
    IN = 0x81
    OUT = 0x02

    data = dev.read(IN, 256, timeout=60 * 1000)
    print(data)

    bytesWritten = dev.write(OUT, data, timeout=60 * 1000)
    print("{} bytes written".format(bytesWritten))
except usb.USBError:
    print("USB error")
    raise
