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
        MFT.States.goToState('media.applink.applinkoptions.applinkoptionssubmenu');
    },

    /** Switching on Applink Perform Interaction Choise */
    turnOnApplinkPerform: function(el){
        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.AppPerformInteractionChoise.PerformInteraction(el.interactionChoiceSetIDList);
        }else{
            this.set('currentApplinkPerformInteractionChoiseId', el.interactionChoiceSetIDList);
        }
        MFT.States.goToState('media.applink.applinkperforminteractionchoise');
    },

    /** Applink AddCommand handler */
    applinkAddCommand: function(params){
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

    /** Applink SystemContext switcher */
    onSystemContextSwitcher: function(systemContextValue){
        FFW.UI.OnSystemContext(systemContextValue);
    },

    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){
        if(params.updateMode == "PAUSE"){
            MFT.ApplinkModel.set('pause', true);
        }else if(params.updateMode == "RESUME"){
            MFT.ApplinkModel.set('pause', false);
        }else{
            MFT.ApplinkModel.set('duration', params.startTime.hours*3600 + params.startTime.minutes*60 + params.startTime.seconds );
        }

        if(params.updateMode == "COUNTUP"){
            MFT.ApplinkModel.set('countUp', true);
        }else if(params.updateMode == "COUNTDOWN"){
            MFT.ApplinkModel.set('countUp', false);
        }
        
    },

});