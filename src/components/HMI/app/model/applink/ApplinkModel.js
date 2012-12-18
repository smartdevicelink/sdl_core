/**
 * @name MFT.ApplinkModel
 * 
 * @desc General Applink model
 * 
 * @category	Model
 * @filesource	app/model/applink/ApplinkModel.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ApplinkModel = Em.Object.create({
	
	/**
	 * Chosen device name
	 *
	 * @type:	String
	 */
	deviceName: null

    ,

    /** Applink UI ScrolableMessage handler */
    onApplinkScrolableMessage: function(params){

       

    },
    
    /**
     * Applink UI Alert response handeler
     * show popup window 
     *
     * @param message:Object
     */
	onUIAlert: function( message ) {
		
		MFT.AlertPopUp.AlertActive(
			message.appId,
			message.AlertText1,
			message.AlertText2,
			message.duration,
			message.playTone
		);
	}
});
 