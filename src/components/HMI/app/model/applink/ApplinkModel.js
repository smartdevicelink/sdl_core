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
	deviceName: null,

    /** Applink UI ScrolableMessage handler */
    onApplinkScrolableMessage: function(params){

       MFT.ScrollableMessage.activate( MFT.ApplinkController.getApplicationModel(params.appId).appInfo.appName , params );

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
	},

	/**
	 * Applink UI PreformInteraction response handeler
	 * open Perform Interaction screen
	 *
	 * @param message:Object
	 */
	onPreformInteraction: function( message ) {
		// todo
		MFT.States.goToState('interactionChoises');
	},
	
	/**
	 * Applink UI CreateInteraction response handeler
	 * push set of commands to voice recognition list
	 *
	 * @param message:Object
	 */
	onCreateInteraction: function( message ) {
		// todo
	},
	
	/**
	 * Applink UI DeleteInteraction response handeler
	 * close current interaction set window (if opened)
	 * and delete current set commands from voice recognition list
	 *
	 * @param id: interaction set identificator
	 */
	onDeleteInteraction: function( id ) {
		// todo
	}
});
 