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


def encrypt(data):
    return data


def decrypt(data):
    return data


def pack(data, encryption, add_http_header):
    file_path = data['fileName']
    file_ptr = open(file_path, "r+")

    request_type = data['requestType']
    pack_handler = get_handler(request_type, 'pack')

    new_data = pack_handler(data, file_ptr)
    if new_data is not None:
        if encryption:
            new_data = encrypt(new_data)
        if add_http_header:
            new_data = http_header(new_data)

        file_ptr.write(new_data)

    file_ptr.close()
    return file_path


def unpack(data, encryption):
    file_path = data['fileName']
    file_ptr = open(file_path, 'r+')

    request_type = data['requestType']
    unpack_handler = get_handler(request_type, 'unpack')

    new_data = unpack_handler(data, file_ptr)
    if new_data is not None:
        if encryption:
            new_data = decrypt(new_data)

        file_ptr.write(new_data)
    file_ptr.close()

    return file_path


def get_handler(request_type, handler_type):
    handlers_map = {
        "PROPRIETARY": get_proprietary_handler
    }
    if request_type not in handlers_map:
        print('\033[33;1mUnhandled request type: %s. Using default request handler\033[0m' % request_type)
        default_handler = lambda data, file_ptr: None
        return default_handler

    return handlers_map[request_type](handler_type)

# Handler getter template
# def get_<request_type>_handler(handler_type):
#   def pack(data, file_ptr):
#       ...
#   def unpack(data, file_ptr):
#       ...
# return pack if handler_type == 'pack' else unpack

def get_proprietary_handler(handler_type):    
    def pack(data, file_ptr):
        read_data = file_ptr.read()
        file_ptr.seek(0)
        file_ptr.truncate()        
        return read_data

    def unpack(data, file_ptr):
        read_data = file_ptr.read()
        file_ptr.seek(0)
        file_ptr.truncate()

        try:
            json_data = json.loads(read_data)
        except ValueError:
            print('\033[31;1mInvalid JSON data: %s\033[0m' % read_data)
            return None

        policy_data = json.dumps(json_data['data'][0])
        return policy_data
    
    return pack if handler_type == 'pack' else unpack


class WebSocketHandler(tornado.websocket.WebSocketHandler):

    def initialize(self,encryption, add_http_header, handle_func):
        self.handle_func = handle_func
        self.encryption = encryption
        self.add_http_header = add_http_header

    def open(self):
        print ("Socket Connected\n")

    def on_message(self, data):
        try:
            json_data = json.loads(data)
        except ValueError:
            print('\033[31;1mInvalid JSON message: %s\033[0m' % data)
            return
        if 'requestType' not in json_data:
            print('\033[31;1mMissing requestType parameter: %s\033[0m' % str(json_data))
            return
        if 'fileName' not in json_data:
            print('\033[31;1mMissing fileName parameter: %s\033[0m' % str(json_data))
            return

        msg = {
            "requestType": json_data['requestType'],
            "data": self.handle_func(json_data, self.encryption, self.add_http_header) 
        }
        self.write_message(msg)

    def on_close(self):
        print ("Connection Closed\n")

    def check_origin(self, origin):
        return True


if __name__ == "__main__":

    args = parser.parse_args()

    pack_application = tornado.web.Application([(r'/', WebSocketHandler, dict(encryption=args.encryption, add_http_header=args.add_http_header, 
        handle_func = lambda data, encryption, add_http_header: pack(data, encryption, add_http_header)))])

    unpack_application = tornado.web.Application([(r'/', WebSocketHandler, dict(encryption=args.encryption, add_http_header=None,
        handle_func = lambda data, encryption, add_http_header: unpack(data, encryption)))])

    pack_server = tornado.httpserver.HTTPServer(pack_application)
    unpack_server = tornado.httpserver.HTTPServer(unpack_application)

    pack_server.listen(args.pack_port, args.host)
    unpack_server.listen(args.unpack_port, args.host)
    tornado.ioloop.IOLoop.instance().start()