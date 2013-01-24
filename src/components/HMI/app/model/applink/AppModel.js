/**
 * @name MFT.ApplinkAppModel
 * 
 * @desc Abstarct model for Applink applications
 * 
 * @category	Model
 * @filesource	app/model/applink/AppModel.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ApplinkAppModel = Em.Object.extend({


    /**
     * Indicates Applink media application button
     */
    hideApplinkButton: true,

	/**
     * Application Id
     *
     * @type: Number
     */
    appId: null,
    
    /**
     * Application name
     *
     * @type: String
     */
    appName: null,
    
    
    /**
     * Info data
     *
     * @type: Em.Object
     */
    appInfo: null,
    
    /**
     * URL to application Icon
     *
     * @type:	String
     */
    appIcon: 'images/info/info_leftMenu_apps_ico.png',
	
	/**
     * Submenu commands list
     *
     * @type: Array
     */
    subMenuCommands: [],
    
	/**
	 * Current command submenu identificator
	 *
	 * @type:	Number
	 */
	currentSubMenuId:		0,
	
	/**
	 * Current command submenu text label
	 *
	 * @type:	String
	 */	
	currentSubMenuLabel:	'',
	
	/**
	 * Interaction chooses data
	 *
	 * @type:	Object
	 */	
	interactionChoices: {},

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
				this.interactionChoices[ message.interactionChoiceSetIDList[i] ]
			);		
		}
		
		MFT.InteractionChoicesView.activate(message.initialText);
		
		
		// Show Initial prompt
		MFT.ApplinkModel.onPrompt(message.initialPrompt);
		
		// Show Timeout prompt
        setTimeout(function(){
        	if ( MFT.InteractionChoicesView.active ) {
        		MFT.ApplinkModel.onPrompt( message.timeoutPrompt );
        	}
        }, message.timeout);
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
		
		this.interactionChoices[message.interactionChoiceSetID] = message.choiceSet;
		
		MFT.VRPopUp.CreateInteractionChoise(message);
	},
	
	/**
	 * Applink UI DeleteInteraction response handeler
	 * close current interaction set window (if opened)
	 * and delete current set commands from voice recognition list
	 *
	 * @param message:Object
	 */
	onDeleteInteraction: function( message ) {		
		delete this.interactionChoices[message.interactionChoiceSetID];
		
		MFT.VRPopUp.DeleteInteractionChoise(message.interactionChoiceSetID);
	},
	
	/**
	 * Applink UI Slider response handeler
	 * open Slider window with received parameters
	 *
	 * @param message:Object
	 */
	onSlider: function( message ) {
		
		MFT.SliderView.loadData( message );
		
		MFT.SliderView.activate( this.appInfo.appName );
			 
		setTimeout( function() {
			if ( MFT.SliderView.active ) {
				MFT.SliderView.deactivate();
			}
		}, message.timeout );
	}
});
 