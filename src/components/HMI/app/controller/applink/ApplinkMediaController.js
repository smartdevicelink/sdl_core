/**
 * @name MFT.ApplinkMediaController
 * 
 * @desc Applink NonMedia Controller logic
 * 
 * @category    Controller
 * @filesource  app/controller/applink/ApplinkMediaController.js
 * @version     1.0
 *
 * @author      Artem Petrosyan
 */

MFT.ApplinkMediaController = Em.Object.create({
    
    // Model binding
    modelBinding: 'MFT.ApplinkModel',

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
        MFT.InfoAppsView.AddApplication(params);
    },

    /** Switching on Applink Options */
    turnOnApplinkOptions: function(el){
        MFT.States.goToState('media.applink.applinkoptions');
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
    
    /** Add sub menu to options view */
    onApplinkAddSubMenu: function(menuId, menuName){

        MFT.ApplinkOptionsView.AddSubMenu(menuId, menuName);

    },


    /** Delete sub menu from options view */
    onApplinkDeleteSubMenu: function(menuId){

        if( (menuId == this.currentApplinkSubMenuid) && MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
            MFT.States.back();
        }

       return MFT.ApplinkOptionsView.DeleteSubMenu(menuId);

    },

    /** Add command to VRPopUp list */
    onApplinkVRAddCommand: function(cmdId, vrCommands){
        for(var j = 0; j < vrCommands.length; j++){
            MFT.ApplinkModel.voiceRecognitionCommands.push({
                type:       MFT.Button,
                params:     {
                    action:                 'onPerformInteractionChoosed',
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
        
        var count = MFT.ApplinkModel.voiceRecognitionCommands.length;
        
        for(var i = count-1; i >= 0; i--){
            if(MFT.ApplinkModel.voiceRecognitionCommands[i].params.commandId == commandId){
                MFT.ApplinkModel.voiceRecognitionCommands.splice(i, 1);
            }
        }

        MFT.VRPopUp.showVRCommands();

    },

    /** Delete all commands in sub menu from VR */
    onApplinkCreateInteractionChoise: function(params){
        MFT.ApplinkModel.interactionChoises.push(params);
        
        for(var i = 0; i<params.choiceSet.length; i++){
            for(var j = 0; j<params.choiceSet[i].vrCommands.length; j++){
                MFT.ApplinkModel.voiceRecognitionCommands.push({
                    type:       MFT.Button,
                    params:     {
                        action:                 'onPerformInteractionChoosed',
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

        for(var val in MFT.ApplinkModel.interactionChoises){
            if(MFT.ApplinkModel.interactionChoises[val].interactionChoiceSetID == choiseSetID){
                MFT.ApplinkModel.interactionChoises.splice(val, 1);
                break;
            }
        }

        var count = MFT.ApplinkModel.voiceRecognitionCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(MFT.ApplinkModel.voiceRecognitionCommands[i].params.interactionChoiceSetID == choiseSetID){
                MFT.ApplinkModel.voiceRecognitionCommands.splice(i, 1);
            }
        }

        MFT.VRPopUp.showVRCommands();
    },

    /** Applink AddCommand handler */
    onApplinkAddCommand: function(params){
        if( params.menuParams.parentID == 0 ){
            MFT.ApplinkOptionsView.AddCommand(params.cmdId, params.menuParams);
        }else{
            if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
                MFT.ApplinkModel.subMenuCommands.push(params);
                MFT.ApplinkOptionsSubMenuView.SubMenuActivate(MFT.MediaController.currentApplinkSubMenuid);
            }else{
                MFT.ApplinkModel.subMenuCommands.push(params);
            }
        }
    },

    /** Applink DeleteCommand handler */
    onApplinkDeleteCommand: function(cmdId){

        MFT.ApplinkOptionsView.DeleteCommand(cmdId);

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

    /** Applink perform interaction action */
    onPerformInteractionChoosed: function(element){
        FFW.UI.onChoosed(element.commandId);
        MFT.States.back();
    },

    /** Applink SystemContext switcher */
    onSystemContextSwitcher: function(systemContextValue){
        FFW.UI.OnSystemContext(systemContextValue);
    },

    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){

        if(params.updateMode == "COUNTUP"){
            MFT.ApplinkModel.set('countUp', true);
        }else if(params.updateMode == "COUNTDOWN"){
            MFT.ApplinkModel.set('countUp', false);
        }

        if(params.updateMode == "PAUSE"){
            MFT.ApplinkModel.set('pause', true);
        }else if(params.updateMode == "RESUME"){
            MFT.ApplinkModel.set('pause', false);
        }else{
            MFT.ApplinkModel.set('duration', 0);
            MFT.ApplinkModel.set('duration', params.startTime.hours*3600 + params.startTime.minutes*60 + params.startTime.seconds );
            MFT.ApplinkModel.set('pause', false);
        }
        
    }

});