TEMPLATE = subdirs  
SUBDIRS += CAN \
    CAN_Test \
    CANlib

CAN.depends = CANlib
CAN_Test.depends = CANlib
