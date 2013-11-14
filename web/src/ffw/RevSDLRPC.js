/**
 * @author: Sergei Polishchuk
 * RPC for reverse SDL functionality
 */

FFW.RevSDL = FFW.RPCObserver.create({
    /*
     *	access to basic RPC functionality
     */
    client:		FFW.RPCClient.create({ componentName: "RevSDLClient" }),

    connect: function() {
        this.client.connect(this);
    },

    disconnect: function() {
        this.client.disconnect();
    },

    onRPCRegistered: function () {
        this._super();
        Em.Logger.log("FFW.RevSDLClient.onRPCRegistered");
    },

    onRPCUnregistered: function () {
        this._super();
        Em.Logger.log("FFW.RevSDLClient.onRPCUnregistered");
    },

    /**
     * when result is received from RPC component this function is called
     * It is the appropriate place to check results of request execution
     * Please use previously store requestID to determine to which request response belongs to
     */
    onRPCResult: function(response) {
        this._super();
    },

    onRPCError: function(response) {
        this._super();
    },

    /*
     * handle RPC requests here
     */
    onRPCRequest: function(request) {
        Em.Logger.log("FFW.RevSDLClient.onRPCRequest");
        this._super();
    },

    /*
     * handle RPC notifications here
     * for Backend component full screen setting change should be handled here
     */
    onRPCNotification: function(notification) {
        Em.Logger.log("FFW.RevSDLClient.onRPCNotification");
        this._super();

        if (notification.method == "RevSDL.onAccessChanged") {
            MFT.MediaController.set('sdlAccessStatus', notification.status);
        } else if (notification.method == "RevSDL.onRadioDetails") {
            MFT.MediaController.setSDLDirectTuneStation(notification.params);
        }
    },

    /*
     * identifiers for requests
     */
    sendGrantAccessRequestId: -1,
    sendCancelAccessRequestId: -1,
    sendTuneRadioRequestId: -1,

    /**
     * Sends a request for access to the management of HMI, through SDL interface
     **/
    sendGrantAccessRequest: function(){
        this.sendGrantAccessRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.sendGrantAccessRequestId,
            "method":	"RevSDL.sendGrantAccessRequest"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Sends a request for access to the management of HMI, through SDL interface
     **/
    sendCancelAccessRequest: function(){
        this.sendCancelAccessRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.sendCancelAccessRequestId,
            "method":	"RevSDL.sendCancelAccessRequest"
        };
        this.client.send(JSONMessage);
    },

    sendTuneRadioRequest: function (data) {
        var frequency = data.frequency.split('.');

        this.sendTuneRadioRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.sendTuneRadioRequestId,
            "method":	"RevSDL.sendTuneRadioRequest",
            params: {
                RadioStation: {
                    frequency: Number(frequency[0]),
                    fraction: Number(frequency[1]),
                    HD: data.isHd
                }
            }
        };
        this.client.send(JSONMessage);

    }
});