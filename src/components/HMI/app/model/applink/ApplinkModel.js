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
            message.alertText3,
            message.ttsChunks,
			message.duration,
			message.playTone
		);
	},
	
	/** 
	 * Prompt activation
	 */
	onPrompt: function(ttsChunks){
		/*
		ttsChunks = JSON.parse('[{"text":"speak","type":"TEXT"},{"text":"INITIAL_JINGLE","type":"PRE_RECORDED"},{"text":"that was a jingle","type":"TEXT"},{"text":". 1 l ih v 1 .  _ l ay v .  r iy 1 d  . r eh d .","type":"SAPI_PHONEMES"}]');
		*/
		var message = '';
		if(ttsChunks){
			for(var i = 0; i < ttsChunks.length; i++){
				message += ttsChunks[i].text + '\n';
			}
			MFT.TTSPopUp.ActivateTTS(message);
		}
	},
	
	/**
	 * Applink VR AddCommand response handeler
	 * add command to voice recognition window
	 *
	 * @param message:	Object
	 */
	onAddCommandVR: function ( message ) {
		//message = JSON.parse('{"appId":65537,"cmdId":4,"vrCommands":["F you bc hi iv"]}');
		
		MFT.VRPopUp.AddCommand( message.commandId, message.vrCommands, message.appId );
	},
	
	/**
	 * Applink VR DeleteCommand response handeler
	 * delete command from voice recognition window
	 *
	 * @param commandId:	Number
	 */
	onDeleteCommandVR: function ( commandId ) {
		//message = 4;
		
		MFT.VRPopUp.DeleteCommand( commandId );
	}
});
 