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
     * Application Id
     *
     * @type {Number}
     */
    appId: null,
    
    /**
     * Application name
     *
     * @type {String}
     */
    appName: '',

    /**
     * Chosen device name
     *
     * @type {String}
     */
    deviceName: '',
    
    /**
     * Statusbar text
     *
     * @type {String}
     */
    statusText: '',
    
    /**
     * Info data
     *
     * @type: {Em.Object}
     */
    appInfo: null,
    
    /**
     * Array of Soft Buttons
     *
     * @type {Array}
     */
    softButtons: [],
    
    /**
     * Array of Objects for TBTTurnList
     *
     * @type {Array}
     */
    turnList: [],

    /**
     * URL to application Icon
     *
     * @type {String}
     */
    appIcon: 'images/info/info_leftMenu_apps_ico.png',
    
    /**
     * Application commands list
     *
     * @type {Array}
     */
    commandsList: [],
    
    /**
     * Return current menu commands
     *
     * @return {Array}
     */
    currentCommandsList: function() {
        return this.get('commandsList').filterProperty( 'parent', this.get('currentSubMenuId') );
    }.property('this.commandsList.@each','this.currentSubMenuId'),
    
	/**
	 * Current command submenu identificator
	 *
	 * @type {Number}
	 */
	currentSubMenuId: 0,
	
	/**
	 * Return current submenu name
	 *
	 * @return {String}
	 */	
	currentSubMenuLabel: function() {
	   var submenu = this.get('commandsList').filterProperty( 'menuId', this.get('currentSubMenuId') );
	   return submenu.length ? submenu[0].name : 'Options';
	}.property('this.currentSubMenuId'),
	
	/**
	 * Interaction chooses data
	 *
	 * @type {Object}
	 */	
	interactionChoices: {},
	
	/**
	 * Update Soft Buttons
	 * will handle on command Show
	 *
	 * @param {Array}
	 */
	updateSoftButtons: function( buttons ) {
        // delete existing buttons from array
        this.softButtons.splice(0);
        
        // push new buttons to array
        this.get('softButtons').pushObjects( buttons );
	},
    
    /**
     * Add command to list
     *
     * @param {Object}
     */
    addCommand: function( params ) {

        this.get('commandsList').pushObject({
            commandId:  params.cmdId,
            name:       params.menuParams.menuName,
            parent:     params.menuParams.parentID ? params.menuParams.parentID : 0,
            position:   params.menuParams.position,
            icon:       params.cmdIcon ? params.cmdIcon.value : null
        });
    },
    
    /**
     * Delete command from list
     *
     * @param {Number}
     */
    deleteCommand: function(commandId){

        this.get('commandsList').removeObjects(
            this.get('commandsList').filterProperty('commandId',commandId)
        );
    },
    
    /**
     * Add submenu to commands list
     *
     * @param {Object}
     */
    addSubMenu: function( params ){        
        
        this.get('commandsList').pushObject({
            menuId:     params.menuId,
            name:       params.menuName,
            parent:     0,
            position:   params.position
        });
    },
    
    /**
     * Delete submenu and related commands from list
     *
     * @param {Number}
     */
    deleteSubMenu: function( menuId ){
        
        // remove submenu
        this.get('commandsList').removeObjects(
            this.get('commandsList').filterProperty('menuId',menuId)
        );
        
        // remove commands from deleted submenu
        this.get('commandsList').removeObjects(
            this.get('commandsList').filterProperty('parent',menuId)
        );
        
        // return to root commands list if necessary
        if( this.get('currentSubMenuId') == menuId ){
            MFT.ApplinkAppController.onSubMenu(0);
        }

        return "SUCCESS";
    },

	/**
	 * Applink UI PreformInteraction response handeler
	 * open Perform Interaction screen and show choices
	 *
	 * @param {Object}
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
	 * @param {Object}
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
	 * @param {Object}
	 */
	onDeleteInteraction: function( message ) {		
		delete this.interactionChoices[message.interactionChoiceSetID];
		
		MFT.VRPopUp.DeleteInteractionChoise(message.interactionChoiceSetID);
	},
	
	/**
	 * Applink UI Slider response handeler
	 * open Slider window with received parameters
	 *
	 * @param {Object}
	 */
	onSlider: function( message ) {
		
		MFT.SliderView.loadData( message );
		
		MFT.SliderView.activate( this.appName );
			 
		setTimeout( function() {
			if ( MFT.SliderView.active ) {
				MFT.SliderView.deactivate();
			}
		}, message.timeout );
	}
});
 