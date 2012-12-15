/**
 * @name MFT.ApplinkNonMediaModel
 * 
 * @desc Concrate model for Applink NonMedia application
 * 
 * @category	Model
 * @filesource	app/model/applink/NonMediaModel.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.ApplinkNonMediaModel = MFT.ApplinkAppModel.create({

	/**
     * Submenu commands list
     *
     * @type: Array
     */
    subMenuCommands: [],

    /**
     * Info data
     *
     * @type: Em.Object
     */
    appInfo: Em.Object.create({
    	appName:        '<App name>',
        field1:         '<field1>',
        field2:         '<field2>',
        field3:         '<field3>',
        field4:         '<field4>',
        statusBar:      '<statusBar',
        image:			''
    }),
    
    /** Applin UI Show handler */
    onApplinkUIShow: function(params){
        this.appInfo.set('field1',        params.mainField1);
        this.appInfo.set('field2',        params.mainField2);
        this.appInfo.set('field3',        params.mainField3);
        this.appInfo.set('field4',        params.mainField4);
        //this.appInfo.set('image',         params.graphic);
    },
	
	/** Add command to Options list */
    onApplinkOptionsAddCommand: function( commandId, params ){

       MFT.InfoNonMediaOptions.commands.AddCommand( commandId, params );

    },
    
    /** Applin UI Alert handler */
    onApplinkUIAlert: function(params){
        MFT.AlertPopUp.AlertActive(params.AlertText1, params.AlertText2, params.duration, params.playTone);
    },
    
    /** Applink AddCommand handler */
    onApplinkAddCommand: function( params ) {
        if( params.menuParams.parentID ) {
        	this.subMenuCommands.push( params );
            
            if( MFT.States.info.nonMedia.options.subMenu.active ) {
                MFT.InfoNonMediaOptionsSubMenu.SubMenuActivate( this.currentSubMenuId );
            }
        
        } else {
        	this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams);
        }
    },
    
    /** Delete command to Options list */
    onApplinkOptionsDeleteCommand: function(commandId){

        MFT.InfoNonMediaOptions.commands.DeleteCommand( commandId );

        var  count = this.subMenuCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(this.subMenuCommands[i].cmdId == commandId){
               this.subMenuCommands.splice(i, 1);
            }
        }
    },
    
    /** Add subMenu button to Options list */
    onApplinkAddSubMenu: function( menuId, params ){

        MFT.InfoNonMediaOptions.commands.AddSubMenu( menuId, params );

    },
    
    /** Delete subMenu button from Options list */
    onApplinkDeleteSubMenu: function( menuId ){

        if( MFT.NonMediaController.currentApplinkSubMenuid == menuId ){
            MFT.States.back();
        }

        MFT.InfoNonMediaOptions.commands.DeleteSubMenu( menuId );

        return "SUCCESS";
    },
    
    /** Delete all commands in sub menu from VR */
    onApplinkCreateInteractionChoise: function(params){
		return;
    },
    
    /** Delete all commands in sub menu from VR */
    onApplinkDeleteInteractionChoise: function(choiseSetID){
		return;
    },
    
    /** Switching on Applink Perform Interaction Choise */
    turnOnApplinkPerform: function(params){
        return;
    },
    
    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){
		return;
    },
    
    /** Applink Slider activation */
    onApplinkSlider: function(params){
    	return;
    },

    /** Applin UI Alert handler */
    onApplinkUIAlert: function(params){

        this.alertInfo.set('text1', params.AlertText1);
        this.alertInfo.set('text2', params.AlertText2);
        this.alertInfo.set('text3', params.alertText3);
        this.alertInfo.set('ttsChunks', params.ttsChunks );
        this.alertInfo.set('duration', params.duration );
        this.alertInfo.set('playTone', params.playTone );
        this.alertInfo.set('softButtons', params.softButtons );
        this.alertInfo.set('tryAgainTime', params.tryAgainTime );

        // appId

        MFT.AlertPopUp.AlertActive();

    }
});