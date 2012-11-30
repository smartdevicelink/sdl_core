/**
 * @name MFT.ApplinkMediaController
 * 
 * @desc Applink Media Controller logic
 * 
 * @category    Controller
 * @filesource  app/controller/applink/ApplinkMediaController.js
 * @version     1.0
 *
 * @author      Andriy Melnyk

MFT.ApplinkMediaController = Em.Object.create({
    
    // Model binding
    modelBinding: 'MFT.ApplinkMediaModel',

    /** Current applink Sub Menu identificator*/
    currentApplinkSubMenuid: null,

    /** Current applink Perform Interaction Choise identificator*/
    currentApplinkPerformInteractionChoiseId: null,

    /**
     * Indicates Applink media application button
     */
    hideApplinkMediaButton: true,

    /**
      * Text for label on Perform Interaction screen
      */
    performInteractionInitialText: '',

    /**
      * Text for label on Perform Interaction screen
      */
    subMenuLabel: '',

    /*
     *  Driver Distraction State 
     *  may be "DD_OFF" or "DD_ON"
     */
    eDriverDistractionState:{
        on : "DD_ON",
        off : "DD_OFF"
    },

    /*
     * Enumeraction that describes possible contexts
     * and app's HMI might be in.
     */
    eSystemContext:{
        main:           "MAIN",
        VR:             "VRSESSION",
        application:    "MENU"
    },

    /** Call notification OnCommand on UIRPC */
    onCommand: function(element){
       FFW.UI.onCommand(element.commandId);
    },

    /** Create list of lapplications on info view */
    onGetAppList: function(params){
        MFT.InfoAppsView.ShowAppList(params);
    },

    /** Switching on Applink Options */
    turnOnApplinkOptions: function(el){
        MFT.States.goToState('media.applink.applinkoptions');
    },

    /** Switching on Applink Slider */
    turnOnApplinkSlider: function(el){
        MFT.States.goToState('media.applink.applinkslider');
    },

    /** Switching on Applink Sub Mennu */
    turnOnApplinkSubMenu: function(el){
        this.set('currentApplinkSubMenuid', el.menuId);
        this.set('subMenuLabel', el.text);
        MFT.States.goToState('media.applink.applinkoptions.applinkoptionssubmenu');
    },

    /** Switching on Applink Perform Interaction Choise */
    turnOnApplinkPerform: function(el){
        this.set('performInteractionInitialText', el.initialText);
        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.AppPerformInteractionChoise.PerformInteraction(el.interactionChoiceSetIDList);
        }else{
            this.set('currentApplinkPerformInteractionChoiseId', el.interactionChoiceSetIDList);
            MFT.States.goToState('media.applink.applinkperforminteractionchoise');
        }
    },

    /** Add command to Options list */
    onApplinkOptionsAddCommand: function( commandId, params ){

        MFT.ApplinkMediaModel.optionsCommands.push({
            type:       MFT.Button,
            params:     {
                action:                 'onCommand',
                target:                 'MFT.ApplinkMediaController',
                commandId:              commandId,
                text:                   params.menuName,
                className:              'rs-item',
                templateName:           'text'
            }                                   
        });

        MFT.ApplinkOptionsView.showOptionsList();

    },

    /** Delete command to Options list */
    onApplinkOptionsDeleteCommand: function(commandId){

        var deleted = false;
            count = MFT.ApplinkMediaModel.optionsCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(MFT.ApplinkMediaModel.optionsCommands[i].params.commandId == commandId){
                MFT.ApplinkMediaModel.optionsCommands.splice(i, 1);
                deleted = true;
            }
        }
        if(!deleted){
            count = MFT.ApplinkMediaModel.subMenuCommands.length;
            for(var i = count-1; i >= 0; i--){
                if(MFT.ApplinkMediaModel.subMenuCommands[i].cmdId == commandId){
                   MFT.ApplinkMediaModel.subMenuCommands.splice(i, 1);
                }
            }
        }

        MFT.ApplinkOptionsView.showOptionsList();
    },

    /** Add subMenu button to Options list */
    onApplinkAddSubMenu: function( menuId, params ){

        MFT.ApplinkMediaModel.optionsCommands.push({
            type:       MFT.Button,
            params:     {
                action:                 'turnOnApplinkSubMenu',
                target:                 'MFT.ApplinkMediaController',
                menuId:                 menuId,
                text:                   params.menuName,
                className:              'rs-item',
                templateName:           'arrow'
            }                                   
        });

        MFT.ApplinkOptionsView.showOptionsList();
    },

    /** Delete subMenu button from Options list */
    onApplinkDeleteSubMenu: function(menuId){

        if(MFT.ApplinkMediaController.currentApplinkSubMenuid == menuId){
            MFT.States.back();
        }

        var deleted = false;
            count = MFT.ApplinkMediaModel.optionsCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(MFT.ApplinkMediaModel.optionsCommands[i].params.menuId == menuId){
                MFT.ApplinkMediaModel.optionsCommands.splice(i, 1);
                deleted = true;
            }
        }

        MFT.ApplinkOptionsView.showOptionsList();

        if(deleted){
            return "SUCCESS";
        }else{
            return "INVALID_DATA";
        }
    },

    /** Add command to VRPopUp list */
    onApplinkVRAddCommand: function(cmdId, vrCommands){
        for(var j = 0; j < vrCommands.length; j++){
            MFT.ApplinkMediaModel.voiceRecognitionCommands.push({
                type:       MFT.Button,
                params:     {
                    action:                 'onCommand',
                    target:                 'MFT.ApplinkMediaController',
                    commandId:              cmdId,
                    text:                   vrCommands[j],
                    className:              'rs-item',
                    templateName:           'text'
                }                                   
            });
        }

        MFT.VRPopUp.showVRCommands();

    },

    /** Delete command from VRPopUp list */
    onApplinkVRDeleteCommand: function(commandId){
        
        var count = MFT.ApplinkMediaModel.voiceRecognitionCommands.length;
        
        for(var i = count-1; i >= 0; i--){
            if(MFT.ApplinkMediaModel.voiceRecognitionCommands[i].params.commandId == commandId){
                MFT.ApplinkMediaModel.voiceRecognitionCommands.splice(i, 1);
            }
        }

        MFT.VRPopUp.showVRCommands();

    },

    /** Delete all commands in sub menu from VR */
    onApplinkCreateInteractionChoise: function(params){
        MFT.ApplinkMediaModel.interactionChoises.push(params);
        
        for(var i = 0; i<params.choiceSet.length; i++){
            for(var j = 0; j<params.choiceSet[i].vrCommands.length; j++){
                MFT.ApplinkMediaModel.voiceRecognitionCommands.push({
                    type:       MFT.Button,
                    params:     {
                        action:                 'onVRPerformInteractionChoosed',
                        target:                 'MFT.ApplinkMediaController',
                        choiceID:               params.choiceSet[i].choiceID,
                        interactionChoiceSetID: params.interactionChoiceSetID,
                        text:                   params.choiceSet[i].vrCommands[j],
                        className:              'rs-item',
                        templateName:           'text'
                    }                                   
                });
            }
        }

        MFT.VRPopUp.showVRCommands();
    },

    /** Delete all commands in sub menu from VR */
    onApplinkDeleteInteractionChoise: function(choiseSetID){

        if( (choiseSetID == this.currentApplinkPerformInteractionChoiseId) && MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.States.back();
        }

        for(var val in MFT.ApplinkMediaModel.interactionChoises){
            if(MFT.ApplinkMediaModel.interactionChoises[val].interactionChoiceSetID == choiseSetID){
                MFT.ApplinkMediaModel.interactionChoises.splice(val, 1);
                break;
            }
        }

        var count = MFT.ApplinkMediaModel.voiceRecognitionCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(MFT.ApplinkMediaModel.voiceRecognitionCommands[i].params.interactionChoiceSetID == choiseSetID){
                MFT.ApplinkMediaModel.voiceRecognitionCommands.splice(i, 1);
            }
        }

        MFT.VRPopUp.showVRCommands();
    },

    /** Applink AddCommand handler */
    onApplinkAddCommand: function(params){
        if( params.menuParams.parentID == 0 ){
            this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams);
        }else{
        	MFT.ApplinkModel.subMenuCommands.push(params);
            if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
                MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
            }

        }
    },

    /** Applink Driver Distraction ON/OFF switcher */
    selectdDriverDistraction: function(checked){
        if(checked){
            FFW.UI.onDriverDistraction( this.eDriverDistractionState.on );
            MFT.DriverDistraction.activate();
        }else{
            FFW.UI.onDriverDistraction( this.eDriverDistractionState.off );
            MFT.DriverDistraction.deactivate();
        }
    },



    /** Applink perform interaction action from VR */
    onVRPerformInteractionChoosed: function(element){

        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            FFW.VR.onChoise(element.choiceID);
            MFT.States.back();
        }

    },

    /** Applink perform interaction action */
    onPerformInteractionChoosed: function(element){

        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            FFW.UI.onChoosed(element.choiceID);
            MFT.States.back();
        }

    },

    /** Applink SystemContext switcher */
    onSystemContextSwitcher: function(systemContextValue){
        FFW.UI.OnSystemContext(systemContextValue);
    },

    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){

        if(params.updateMode == "COUNTUP"){
            MFT.ApplinkMediaModel.set('countUp', true);
        }else if(params.updateMode == "COUNTDOWN"){
            MFT.ApplinkMediaModel.set('countUp', false);
        }

        if(params.updateMode == "PAUSE"){
            MFT.ApplinkMediaModel.set('pause', true);
        }else if(params.updateMode == "RESUME"){
            MFT.ApplinkMediaModel.set('pause', false);
        }else{
            MFT.ApplinkMediaModel.set('duration', 0);
            MFT.ApplinkMediaModel.set('duration', params.startTime.hours*3600 + params.startTime.minutes*60 + params.startTime.seconds );
            MFT.ApplinkMediaModel.set('pause', false);
        }
        
    },

    /** Applink Slider activation */
    onApplinkSlider: function(params){
        this.turnOnApplinkSlider();
    },

    /** Applink TTS Speak handler */
    onApplinkTTSSpeak: function(params){
        var message = '';
        for(var i = 0; i < params.ttsChunks.length; i++){
            message += params.ttsChunks[i].text + '\n';
        }
        MFT.TTSPopUp.ActivateTTS(message);
    },

    /** Applin UI Alert handler */
    onApplinkUIAlert: function(params){
        MFT.AlertPopUp.AlertActive(params.AlertText1, params.AlertText2, params.duration, params.playTone);
    },

    /** Applin UI Show handler */
    onApplinkUIShow: function(params){
        MFT.ApplinkMediaModel.showInfo.set('field1', params.mainField1);
        MFT.ApplinkMediaModel.showInfo.set('field2', params.mainField2);
        MFT.ApplinkMediaModel.showInfo.set('mediaClock', params.mediaClock);
        MFT.ApplinkMediaModel.showInfo.set('field3', params.mediaTrack);
        MFT.ApplinkMediaModel.showInfo.set('statusBar', params.statusBar);
    }

});