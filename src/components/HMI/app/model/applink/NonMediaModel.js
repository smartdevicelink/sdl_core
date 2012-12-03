/**
 * @name MFT.ApplinkNonMediaModel
 * 
 * @desc Data model for Applink NonMedia application
 * 
 * @category	Model
 * @filesource	app/model/applink/ApplinkNonMediaModel.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.ApplinkNonMediaModel = Em.Object.create({
	
	
	deviceName: '<deviceName>',
	
	
	
	/** Add command to Options list */
    onApplinkOptionsAddCommand: function( commandId, params ){

       MFT.InfoNonMediaOptions.AddCommand( commandId, params );

    },
	
	/** Applink UI Show handler */
    onApplinkUIShow: function( params ){
        this.showInfo.set('field1',		params.mainField1);
        this.showInfo.set('field2',		params.mainField2);
        this.showInfo.set('mediaClock',	params.mediaClock);
		this.showInfo.set('field3',		params.mediaTrack);
        this.showInfo.set('statusBar',	params.statusBar);
    },
    
    /** Applin UI Alert handler */
    onApplinkUIAlert: function(params){
        MFT.AlertPopUp.AlertActive(params.AlertText1, params.AlertText2, params.duration, params.playTone);
    },
    
    /** Applink AddCommand handler */
    onApplinkAddCommand: function(params){
        if( params.menuParams.parentID == 0 ){
            this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams);
        }else{
            MFT.ApplinkMediaModel.subMenuCommands.push(params);
            if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
                MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
            }

        }
    },
    
    /** Delete command to Options list */
    onApplinkOptionsDeleteCommand: function(commandId){

        MFT.InfoNonMediaOptions.DeleteCommand( commandId );

        var  count = MFT.ApplinkMediaModel.subMenuCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(this.subMenuCommands[i].cmdId == commandId){
               this.subMenuCommands.splice(i, 1);
            }
        }
    },
    
    /** Add subMenu button to Options list */
    onApplinkAddSubMenu: function( menuId, params ){

        MFT.InfoNonMediaOptions.AddSubMenu( menuId, params );

    },
    
    /** Delete subMenu button from Options list */
    onApplinkDeleteSubMenu: function( menuId ){

        if( MFT.ApplinkMediaController.currentApplinkSubMenuid == menuId ){
            MFT.States.back();
        }

        MFT.InfoNonMediaOptions.DeleteSubMenu( menuId );

        return "SUCCESS";
    },
    
    /** Delete all commands in sub menu from VR */
    onApplinkCreateInteractionChoise: function(params){

        this.interactionChoises.push(params);

        MFT.VRPopUp.CreateInteractionChoise(params);

    },
    
    /** Delete all commands in sub menu from VR */
    onApplinkDeleteInteractionChoise: function(choiseSetID){

        if( (choiseSetID == this.currentApplinkPerformInteractionChoiseId) && MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.States.back();
        }

        for(var val in MFT.ApplinkMediaModel.interactionChoises){
            if(this.interactionChoises[val].interactionChoiceSetID == choiseSetID){
                this.interactionChoises.splice(val, 1);
                break;
            }
        }

        MFT.VRPopUp.DeleteInteractionChoise( choiseSetID );

    },
    
    /** Switching on Applink Perform Interaction Choise */
    turnOnApplinkPerform: function(params){
        this.set('performInteractionInitialText', params.initialText);
        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.AppPerformInteractionChoise.PerformInteraction(params.interactionChoiceSetIDList);
        }else{
            this.set('currentApplinkPerformInteractionChoiseId', params.interactionChoiceSetIDList);
            MFT.States.goToState('media.applink.applinkperforminteractionchoise');
        }
    },
    
    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){

        if(params.updateMode == "COUNTUP"){
            this.set('countUp', true);
        }else if(params.updateMode == "COUNTDOWN"){
            this.set('countUp', false);
        }

        if(params.updateMode == "PAUSE"){
            this.set('pause', true);
        }else if(params.updateMode == "RESUME"){
            this.set('pause', false);
        }else{
            this.set('duration', 0);
            this.set('duration', params.startTime.hours*3600 + params.startTime.minutes*60 + params.startTime.seconds );
            this.set('pause', false);
        }
        
    },
    
    /** Applink Slider activation */
    onApplinkSlider: function(params){
        this.turnOnApplinkSlider();
    }
});