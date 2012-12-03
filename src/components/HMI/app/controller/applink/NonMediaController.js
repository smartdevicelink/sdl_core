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
	modelBinding: 'MFT.ApplinkNonMediaModel',
	
	
	// Switching on Applink Sub Mennu
	turnOnApplinkSubMenu: function(el){
		//this.set('currentApplinkSubMenuid', el.menuId);
		//this.set('subMenuLabel', el.text);
		MFT.States.goToState('info.nonMedia.options.subMenu');
	}

});