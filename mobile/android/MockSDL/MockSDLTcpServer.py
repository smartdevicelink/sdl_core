import socket


TCP_IP = '172.30.138.110'
TCP_PORT = 12345
BUFFER_SIZE = 112  # Normally 1024, but we want fast response
status = ["off", "connected", "singleFrameHeaderReceived"]


def startMobileNavSessionAck():
    """
    >>> startMobileNavSessionAck()
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)

    conn, addr = s.accept()
    print 'Connection address:', addr
    while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print "received data: %r" % data
        if startMobileNavSessionReceived(data):
            conn.send(startMobileNaviSessionACK())
    conn.close()


def startMobileNavSessionNotAck():
    """
    >>> startMobileNavSessionNotAck()
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)

    conn, addr = s.accept()
    print 'Connection address:', addr
    while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print "received data: %r" % data
        if startMobileNavSessionReceived(data):
            conn.send(startSessionNAck())
    conn.close()


def dataEchange():
    """
    >>> dataEchange()
    """
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((TCP_IP, TCP_PORT))
    s.listen(1)

    conn, addr = s.accept()
    print 'Connection address:', addr
    currentData = ""
    currentStatus = "off"
    while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        print "received data: %r" % data
        if startMobileNavSessionReceived(data):
            conn.send(startMobileNaviSessionACK())
            currentStatus = "connected"
        if currentStatus == "connected":
            if singleFrameHeaderReceived(data):
                currentStatus = "singleFrameHeaderReceived"
        if currentStatus == "singleFrameHeaderReceived":
            currentData += data
            if len(currentData) >= 112:
                conn.send(mobileNavAck())
                currentStatus = "connected"
    conn.close()


def singleFrameHeaderReceived(actual):
    expectedSession = singleSendData()[1]
    actualSession = actual[1]
    expectedFrameType = singleSendData()[2]
    actualFrameType = actual[2]
    res = (expectedSession == actualSession) and (actualFrameType == expectedFrameType)
    return res


def startSessionAck():
    return str(bytearray([32, 7, 2, 48, 0, 0, 0, 0, 0, 0, 0, 48]))


def startMobileNavSession():
    return str(bytearray([32, 11, 1, 48, 0, 0, 0, 0, 0, 0, 0, 48]))


def startMobileNaviSessionACK():
    return str(bytearray([32, 11, 2, 48, 0, 0, 0, 0, 0, 0, 0, 48]))


def singleSendData():
    return str(bytearray([33, 11, 0, 48, 0, 0, 0, 100, 0, 0, 0, 48]))


def mobileNavAck():
    return str(bytearray([32, 11, 5, 48, 0, 0, 0, 0, 0, 0, 0, 48]))


def startSessionNAck():
    return str(bytearray([32, 11, 3, 48, 0, 0, 0, 0, 0, 0, 0, 48]))


def startMobileNavSessionReceived(actual):
    """test
    >>> startMobileNavSessionReceived(startMobileNavSession())
    True
    """
    expectedSession = startMobileNavSession()[1]
    actualSession = actual[1]
    expectedFrameType = startMobileNavSession()[2]
    actualFrameType = actual[2]
    res = (expectedSession == actualSession) and (actualFrameType == expectedFrameType)
    return res

