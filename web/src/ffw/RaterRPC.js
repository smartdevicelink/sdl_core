/*
 * Implementation of Rate component.
 * 
 *
 * @author Maksym Gerashchenko
 */

FFW.RaterRPC = FFW.RPCObserver.create({
	
	/** Rate Request Id */
	rateRequestId: -1,
	
	/** Rate MFTGuide Request Id */
	rateMFTGuideRequestId: -1,
	
	/** Subscribe ID*/
	onRateShowErrorSubscribeRequestId: -1,
		
	/**
     *	access to basic RPC functionality
 	 */		
	client:		FFW.RPCClient.create({ componentName: "RateClient" }),
	
	/*
   	 * Connect to RPC
 	 */
	connect: function() {
		if (!FLAGS.VIDEO_PLAYER_JS) {
			this.client.connect(this);
		}
	},

	/*
   	 * Disconnect to RPC
 	 */
	disconnect: function() {
		this.client.disconnect();
	},	
	
	 
	
	onRPCRegistered: function () {
		this._super();
		
		//subscribe to Rate.ShowError event
		this.onRateShowErrorSubscribeRequestId = this.client.generateId();
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.onRateShowErrorSubscribeRequestId,
			"method":	"MB.subscribeTo",
			"params":	{
				"propertyName": "Rate.showError"
			}
		};
		this.client.send(JSONMessage);
	},
	
	/** RPC Request */
	onRPCRequest: function( request ) {
		
//		this._super();
//
//		if ( request.method == "RateClient.rateApp" ) {
//
//			this.rateMFTGuideRequestId = request.id;
//			MFT.RateController.showRateMFTGuideDialogueView();
//
//		}
	},
	
	/**
	 *	RPC Response
	 */
	rateMFTGuideResponse: function( ) {
        this.rateMFTGuideRequestId = this.client.generateId();
		var JSONMessage = {
			"jsonrpc":	"2.0",
			"id": 		this.rateMFTGuideRequestId,
            "method":	"Rate.rateApp"
		};
		
		this.client.send(JSONMessage);
	},
		 
	/**
	 * Notification with code
	 *
	 * code: 40 - RATE_SHOW_REDIRECT_WARNING_CODE
     *       41 - RATE_HIDE_REDIRECT_WARNING_CODE
     *       42 - RATE_NO_NETWORK_ERR_CODE
     *       43 - RATE_GOOGLEPLAY_ERR_CODE
 	 */	
//	onRPCNotification: function(notification) {
//		this._super();
//
//		if (notification.method == "Rate.showError") {
//			switch ( notification.params.code ) {
//				case 40:
//					MFT.RateController.showPopUp('leavingMFTGuideWarningIsVisible');
//					break;
//
//				case 41:
//					MFT.RateController.hidePopUp('leavingMFTGuideWarningIsVisible');
//					break;
//
//				case 42:
//					MFT.RateController.showPopUp('ratingNetworkWarningIsVisible');
//					break;
//
//				case 43:
//					MFT.RateController.showPopUp('ratingNoRespondWarningIsVisible');
//					break;
//
//			}
//		}
//	},

    /*
     * when result is received from RPC component this function is called
     * It is the propriate place to check results of reuqest execution
     * Please use previously store reuqestID to determine to which request repsonse belongs to
     */
    onRPCResult: function(response) {
        Em.Logger.log("FFW.RaterRPC.onRPCResult");
        this._super();
        if (response.id == this.rateMFTGuideRequestId)
        {
            Em.Logger.log("Go to Rate");
        }
    },

    /*
     * handle RPC erros here
     * 42 - RATE_NO_NETWORK_ERR_CODE
     * 43 - RATE_GOOGLEPLAY_ERR_CODE
     */
    onRPCError: function(error) {
        Em.Logger.log("FFW.RaterRPC.onRPCError");
        this._super();

        switch ( error.error.code ) {
            case 42:
                MFT.RateController.showPopUp('ratingNetworkWarningIsVisible');
                break;

            case 43:
                MFT.RateController.showPopUp('ratingNoRespondWarningIsVisible');
                break;

        }
    }
	 	 
});