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

        switch (response.id) {
            case this.GrantAccessRequestId:
                MFT.MediaController.set('sdlAccessStatus', response.result.success);
                break;
//            case this.StartScanRequestId:
//                MFT.MediaController.set('isFrequencyScan', response.result.success);
//                break;
//            case this.StopScanRequestId:
//                MFT.MediaController.set('isFrequencyScan', !response.result.success);
//                break;
            case this.GetRadioDetailsRequestId:
                MFT.MediaController.setSDLDirectTuneStation(response.result);
                this.sendShowRequest();
                break;
        }
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

        switch (notification.method) {
            case "RevSDL.OnControlChanged":
                MFT.MediaController.set('sdlAccessStatus', false);
                break;
            case "RevSDL.OnRadioDetails":
                MFT.MediaController.setSDLDirectTuneStation(notification.params);
                break;
            case "RevSDL.OnPresetsChanged":
                MFT.MediaController.setSDLPresets(notification.params);
                break;
        }
    },

    /*
     * identifiers for requests
     */
    GrantAccessRequestId: -1,
    CancelAccessRequestId: -1,
    TuneRadioRequestId: -1,
    StartScanRequestId: -1,
    StopScanRequestId: -1,
    GetRadioDetailsRequestId: -1,

    /**
     * Sends a request for access to the management of HMI, through SDL interface
     **/
    sendGrantAccessRequest: function(){
        this.GrantAccessRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.GrantAccessRequestId,
            "method":	"RevSDL.GrantAccess"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Sends a request for access to the management of HMI, through SDL interface
     **/
    sendCancelAccessRequest: function(){
        this.CancelAccessRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.CancelAccessRequestId,
            "method":	"RevSDL.CancelAccess"
        };
        this.client.send(JSONMessage);
    },

    sendTuneRadioRequest: function (data) {
        var frequency = data.frequency.split('.');

        this.TuneRadioRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.TuneRadioRequestId,
            "method":	"RevSDL.TuneRadio",
            params: {
                radioStation: {
                    frequency: Number(frequency[0]),
                    fraction: Number(frequency[1])
                }
            }
        };
        this.client.send(JSONMessage);

    },

    /**
     * Start frequency scan on head unit, through SDL interface
     **/
    sendStartScanRequest: function(){
        this.StartScanRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.StartScanRequestId,
            "method":	"RevSDL.StartScan"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Stop frequency scan on head unit, through SDL interface
     **/
    sendStopScanRequest: function(){
        this.StopScanRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.StopScanRequestId,
            "method":	"RevSDL.StopScan"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Stop frequency scan on head unit, through SDL interface
     **/
    sendGetRadioDetailsRequest: function(){
        this.GetRadioDetailsRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.GetRadioDetailsRequestId,
            "method":	"RevSDL.GetRadioDetails"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Stop frequency scan on head unit, through SDL interface
     **/
    sendShowRequest: function(){
        var JSONMessage = {
            "jsonrpc":	"2.0",
            "method":	"RevSDL.Show"
        };
        this.client.send(JSONMessage);
    }
});