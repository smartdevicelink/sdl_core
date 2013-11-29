import subprocess
from tornado.iostream import PipeIOStream
from tornado.options import parse_command_line
import tornado.web
import tornado.ioloop
import tornado.websocket
from tornado.websocket import WebSocketHandler


class MainHandler(WebSocketHandler):

    @tornado.web.asynchronous
    def open(self):
        print "Socket open"
        self.open_dump_stream()


    @tornado.web.asynchronous
    def on_message(self, message):

        self.p.terminate()
        self.stream.close()

        print "Socket message: " + message
        openxc_command = ""
        if message.startswith("stop_server"):
            return
        if message.startswith("set_preset_"):
            #print ("Set Preset", message[11:])
            openxc_command = 'openxc-control write --name tune --value preset --event ' + message[11:]
        elif message.startswith("set_tuner_band_"):
            #print ("Set tuner band", message[15:])
            openxc_command = 'openxc-control write --name set_tuner_band --value ' + message[15:]
        elif message == "station_up":
            #print "Station Up"
            openxc_command = 'openxc-control write --name tune --value station --event up'
        elif message == "station_down":
            #print "Station Down"
            openxc_command = 'openxc-control write --name tune --value station --event down'
        elif message == "step_up":
            #print "Step Up"
            openxc_command = 'openxc-control write --name tune --value step --event up'
        elif message == "step_down":
            #print "Step Down"
            openxc_command = 'openxc-control write --name tune --value step --event down'
        elif message.startswith("store_preset_"):
            #print "Store Preset"
            openxc_command = 'openxc-control write --name store_preset --value ' + message[13:]
        elif message.startswith("frequency_set_"):
            #print "Store Preset"
            openxc_command = 'openxc-control write --name tune --value direct --event ' + message[14:]

        print openxc_command

        self.p = subprocess.Popen(openxc_command, shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
        self.p.wait()

        self.open_dump_stream()

    def on_close(self, message=None):
        print "Socket close"

    def line_from_net_tail(self, data):
        try:
            self.write_message(data)
            self.stream.read_until("\n", self.line_from_net_tail)
        except Exception:
            self.p.terminate()
            self.stream.close()
            pass

    def open_dump_stream(self):
        self.p = subprocess.Popen(["openxc-dump"], stdout=subprocess.PIPE)
        self.stream = PipeIOStream(self.p.stdout.fileno())
        self.stream.read_until("\n", self.line_from_net_tail)


class Application(tornado.web.Application):
    def __init__(self):
        parse_command_line()
        handlers = (
            [(r'/', MainHandler)]
        )
        tornado.web.Application.__init__(self, handlers)

application = Application()

if __name__ == '__main__':
    application.listen(9092)
    tornado.ioloop.IOLoop.instance().start()