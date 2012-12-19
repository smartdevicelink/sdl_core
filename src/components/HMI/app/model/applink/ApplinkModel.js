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
	 * open Perform Interaction screen and show choices
	 *
	 * @param message:Object
	 */
	onPreformInteraction: function( message ) {
		/* test message
		message = JSON.parse('{"appId":1,"helpPrompt":[{"text":"help me, Im melting","type":"TEXT"}],"initialPrompt":[{"text":"Pick a command","type":"TEXT"}],"initialText":"Pick number:","interactionChoiceSetIDList":[4]}');
		*/
		
		var i = 0,
			length = message.interactionChoiceSetIDList.length;
		
		
		MFT.InteractionChoicesView.clean();
		
		for ( i = 0; i < length; i++ ) {
			MFT.InteractionChoicesView.preformChoices(
				MFT.ApplinkController.getApplicationModel(message.appId).interactionChoises[ message.interactionChoiceSetIDList[i] ]
			);		
		}
		
		MFT.InteractionChoicesView.activate();
	},
	
	/**
	 * Applink UI CreateInteraction response handeler
	 * push set of commands to voice recognition list
	 *
	 * @param message:Object
	 */
	onCreateInteraction: function( message ) {
		
		/* test message
		message = JSON.parse('{"appId":1,"choiceSet":[{"choiceID":10,"menuName":"1st item","vrCommands":["1st item","Command one"]},{"choiceID":11,"menuName":"2nd item","vrCommands":["2nd item","Command two"]},{"choiceID":12,"menuName":"3rd item","vrCommands":["3rd item","Command three"]}],"interactionChoiceSetID":4}');
		*/
		
		MFT.ApplinkController.getApplicationModel(message.appId).interactionChoises[message.interactionChoiceSetID] = message.choiceSet;
	},
	
	/**
	 * Applink UI DeleteInteraction response handeler
	 * close current interaction set window (if opened)
	 * and delete current set commands from voice recognition list
	 *
	 * @param message:Object
	 */
	onDeleteInteraction: function( message ) {		
		delete MFT.ApplinkController.getApplicationModel(message.appId).interactionChoises[message.interactionChoiceSetID];
	}
});
 