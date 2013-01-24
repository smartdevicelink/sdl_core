/**
 * @name MFT.NonMediaController
 * 
 * @desc Applink NonMedia Controller logic
 * 
 * @category	Controller
 * @filesource	app/controller/applink/NonMediaController.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.NonMediaController = Em.Object.create({
	
	// Model binding
	model: null,
	
    /**
      * Text for label on Perform Interaction screen
      */
    subMenuLabel: '',
	
	// Switching on Applink Sub Mennu
	turnOnApplinkSubMenu: function(el){
		this.set('currentApplinkSubMenuid', el.menuId);
		this.set('subMenuLabel', el.text);
		MFT.States.goToState('info.nonMedia.options.subMenu');
	},

    /**
     * Switching on Application
     *
     */
    turnOnApplink: function( applicationModel ){
        
        this.set('model', applicationModel);
        
        FFW.AppLinkCoreClient.ActivateApp( applicationModel.appId );
        
        // Go to Applink state
        MFT.States.goToState('info.nonMedia');
    }
});