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
 */

MFT.ApplinkMediaController = Em.Object.create({
    
    // Model binding
    modelBinding: 'MFT.ApplinkMediaModel',

    /**
     * Indicates Applink media application button
     */
    hideApplinkMediaButton: true,

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
       FFW.UI.onCommand(element.commandId, element.appId);
    },


    /** Call notification OnCommandSoftButton on UIRPC */
    onCommandSoftButton: function(element){
       FFW.UI.onCommandSoftButton(element.softButtonID, element.appId);
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

    /** Switching on Application */
    turnOnApplink: function(element){

        MFT.ApplinkController.getApplicationModel(1).showInfo.set('appName', element.appName);
        FFW.AppLinkCoreClient.ActivateApp(element.appId);

        /* Show Applink application in media left menu */
        MFT.ApplinkMediaController.set('hideApplinkMediaButton', false);
        MFT.MediaController.listDown();

        MFT.MediaController.turnOnApplink();
        
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
    }

});