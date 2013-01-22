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
    
    /**
      * Model binding
      */
    model:          null,

    /**
      * Text for label on Perform Interaction screen
      */
    subMenuLabel:   '',

    /*
     * Enumeraction that describes possible contexts
     * and app's HMI might be in.
     */
    eSystemContext:{
        main:           "MENU",
        VR:             "VRSESSION",
        application:    "MAIN"
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
        if( MFT.ApplinkController.driverDistractionState ){
            MFT.DriverDistraction.activate();
        }else{
            this.set('currentApplinkSubMenuid', el.menuId);
            this.set('subMenuLabel', el.text);
            MFT.States.goToState('media.applink.applinkoptions.applinkoptionssubmenu');
        }
    },

    /** Switching on Application */
    turnOnApplink: function(element){

        this.set('model', MFT.ApplinkController.getApplicationModel(element.appId) );
        
        FFW.AppLinkCoreClient.ActivateApp(element.appId);

        // Go to Applink state
        MFT.States.goToState('info.nonMedia');




        //this.model.appInfo.set('appName', element.appName);
        //FFW.AppLinkCoreClient.ActivateApp(element.appId);
        //this.model.set('activeAppId', element.appId);

        /* Show Applink application in media left menu */
        this.model.set('hideApplinkButton', false);
        MFT.MediaController.listDown();

        MFT.MediaController.turnOnApplink();
        
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