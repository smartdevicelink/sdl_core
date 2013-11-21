/**
 * @author: Sergei Polishchuk
 * RPC for reverse SDL functionality
 */

FFW.RevSDL = FFW.RPCObserver.create({
    /*
     *	access to basic RPC functionality
     */
    client:		FFW.RPCClient.create({ componentName: "RevSDLClient" }),

    OnControlChangedProperty: "RevSDL.OnControlChanged",
    OnRadioDetailsProperty: "RevSDL.OnRadioDetails",
    OnPresetsChangedProperty: "RevSDL.OnPresetsChanged",

    OnControlChangedSubscribeRequestId: -1,
    OnRadioDetailsSubscribeRequestId: -1,
    OnPresetsChangedSubscribeRequestId: -1,
    OnControlChangedUnsubscribeRequestId: -1,
    OnRadioDetailsUnsubscribeRequestId: -1,
    OnPresetsChangedUnsubscribeRequestId: -1,

    connect: function() {
        this.client.connect(this);
    },

    disconnect: function() {
        this.OnControlChangedUnsubscribeRequestId = this.unsubscribeFromProperty(this.OnControlChangedProperty);
        this.OnRadioDetailsUnsubscribeRequestId = this.unsubscribeFromProperty(this.OnRadioDetailsProperty);
        this.OnPresetsChangedUnsubscribeRequestId = this.unsubscribeFromProperty(this.OnPresetsChangedProperty);

        this.client.disconnect();
    },

    onRPCRegistered: function () {
        this._super();
        Em.Logger.log("FFW.RevSDLClient.onRPCRegistered");

        this.OnControlChangedSubscribeRequestId = this.subscribeToProperty(this.OnControlChangedProperty);
        this.OnRadioDetailsSubscribeRequestId = this.subscribeToProperty(this.OnRadioDetailsProperty);
        this.OnPresetsChangedSubscribeRequestId = this.subscribeToProperty(this.OnPresetsChangedProperty);
    },

    onRPCUnregistered: function () {
        this._super();
        Em.Logger.log("FFW.RevSDLClient.onRPCUnregistered");
    },

    /*
     * Subscribes to property. Returns the request's id.
     */
    subscribeToProperty: function(property) {
        var msgId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		msgId,
            "method":	"MB.subscribeTo",
            "params":	{
                "propertyName": property
            }
        };
        this.client.send(JSONMessage);
        return msgId;
    },

    /*
     * Unsubscribes from property. Returns the request's id.
     */
    unsubscribeFromProperty: function(property) {
        var msgId = this.client.generateId();
        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		msgId,
            "method":	"MB.unsubscribeFrom",
            "params":	{
                "propertyName": property
            }
        };
        this.client.send(JSONMessage);
        return msgId;
    },

    /**
     * when result is received from RPC component this function is called
     * It is the appropriate place to check results of request execution
     * Please use previously store requestID to determine to which request response belongs to
     */
    onRPCResult: function(response) {
        this._super();

        switch (response.id) {
//            case this.GrantAccessRequestId:
//                MFT.MediaController.GrantAccessResult(response.result);
//                break;
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
            case this.OnControlChangedProperty:
                MFT.MediaController.set('sdlAccessStatus', false);
//                MFT.MediaController.set('sdlControlStatusIco', 1);
                break;
            case this.OnRadioDetailsProperty:
                MFT.MediaController.setSDLDirectTuneStation(notification.params);
                break;
            case this.OnPresetsChangedProperty:
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