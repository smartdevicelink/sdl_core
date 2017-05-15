#!/usr/bin/env python
import argparse
import asyncore
import socket
import json

import tornado.httpserver
import tornado.websocket
import tornado.ioloop
import tornado.web

parser = argparse.ArgumentParser()
parser.add_argument("--host", type=str, default="127.0.0.1",
                    help="Host to listen HMI")
parser.add_argument("--pack_port", type=int, default="8088",
                    help="Port for packing policy table", required=True)
parser.add_argument("--unpack_port", type=int, default="8089",
                    help="Port for unpacking policy table", required=True)
parser.add_argument("--buffer_size", type=int, default=8192,
                    help="Size of buffer for input data")
parser.add_argument("--add_http_header", action="store_true",
                    help="Enter this parameter to add HTTP heaer on packing policy table")
parser.add_argument("--encryption", action="store_true",
                    help="Encrypt policy table")


def http_header(data):
    header = {}
    header["HTTPRequest"] = {}
    header["HTTPRequest"]["headers"] = {
        "ConnectTimeout": 60,
        "ContentType": "application/json",
        "Content-Length": len(data),
        "DoInput": True,
        "DoOutput": True,
        "InstanceFollowRedirects": False,
        "ReadTimeout": 60,
        "RequestMethod": "POST",
        "UseCaches": False, 
        "charset": "utf-8"}
    header["HTTPRequest"]["body"] = data
    return json.dumps(header)


def crypt(data):
    return data


def decrypt(data):
    return data

def pack(file_path, encryption, add_http_header):

    file = open(file_path, "r+")
    data = file.read()
    file.seek(0)
    file.truncate()    
    
    if encryption:
        data = crypt(data)
    if add_http_header:
        data = http_header(data)

    file.write(data)
    file.close()
    return file_path


def unpack(file_path, encryption):

    file = open(file_path, 'r+')
    read_data = file.read()
    file.seek(0)
    file.truncate()

    json_data = json.loads(read_data)
    policy_data = json.dumps(json_data['data'][0])

    if encryption:
        policy_data = decrypt(policy_data)

    file.write(policy_data)
    file.close()

    return file_path


class WebSocketHandler(tornado.websocket.WebSocketHandler):

    def initialize(self,encryption, add_http_header, handle_func):
        self.handle_func = handle_func
        self.encryption = encryption
        self.add_http_header = add_http_header

    def open(self):
        print ("Socket Connected\n")

    def on_message(self, data):
        self.write_message(self.handle_func(data, self.encryption, self.add_http_header))

    def on_close(self):
        print ("Connection Closed\n")

    def check_origin(self, origin):
        return True


if __name__ == "__main__":

    args = parser.parse_args()

    pack_application = tornado.web.Application([(r'/', WebSocketHandler, dict(encryption=args.encryption, add_http_header=args.add_http_header, 
        handle_func = lambda data: pack(data, args.encryption, args.add_http_header)))])

    unpack_application = tornado.web.Application([(r'/', WebSocketHandler, dict(encryption=args.encryption, add_http_header=None,
        handle_func = lambda data: unpack(data, args.encryption)))])

    pack_server = tornado.httpserver.HTTPServer(pack_application)
    unpack_server = tornado.httpserver.HTTPServer(unpack_application)

    pack_server.listen(args.pack_port, args.host)
    #unpack_server.listen(args.unpack_port, args.host)
    tornado.ioloop.IOLoop.instance().start()