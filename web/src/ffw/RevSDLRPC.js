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

//        if (notification.method == "RevSDL.SDLAccessRequest") {
//            MFT.MediaController.set('sdlAccessStatus', 1);
//        }
    },

    /*
     * identifiers for requests
     */
    sendSDLAccessRequestId: -1,

    /**
     * Sends a request for access to the management of HMI, through SDL interface
     **/
    sendSDLAccessRequest: function(){
        this.sendSDLAccessRequestId = this.client.generateId();

        var JSONMessage = {
            "jsonrpc":	"2.0",
            "id": 		this.sendSDLAccessRequestId,
            "method":	"RevSDL.sendSDLAccessRequest"
        };
        this.client.send(JSONMessage);
    }
});