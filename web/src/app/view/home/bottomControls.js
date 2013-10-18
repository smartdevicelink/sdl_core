/**
 * @name MFT.BottomControlls
 * 
 * @desc BottomControlls module visual representation
 * 
 * @category	View
 * @filesource	app/view/BottomControls.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
MFT.BottomControls = Em.ContainerView.extend({
	
	elementId:			'app_bottom_controlls',
	
	classNameBindings:'MFT.StartUpController.isAllStatusHidden:hidden',
	
	childViews: [
		'information',
		'home',
		'settings'
	],
	
	information : Em.View.extend({
		elementId:			'info_but',
		classNameBindings: [
			'MFT.States.info.active:selected',
			'FFW.Backend.isNavigationEnabled::hidden_display',
            'MFT.AppController.activeVehicleModel.isClimateDisabled:large'
		],
				
		template: Em.Handlebars.compile(
			'<div id="info_but_click"></div>'
		),
		
		actionDown: function(event){
			if ( !MFT.States.info.active ) {
				MFT.States.goToState( MFT.InfoController.activeState );
			}
		}
	}),
	
	home: Em.View.extend({
		elementId:			'home_but',
		classNameBindings: [
			'MFT.States.home.active:selected',
			'FFW.Backend.isNavigationEnabled::large',
            'MFT.AppController.activeVehicleModel.isClimateDisabled:right-large'
		],
		
		template: Em.Handlebars.compile(
			'<div id="home_but_click" {{bindAttr class="FFW.Backend.isNavigationEnabled::large"}}></div>'
		),
		
		actionDown: function(event){
			MFT.States.goToState('home');
		}
	}),
	
	settings: Em.View.extend({
		elementId:			'setting_but',
		
		classNameBindings: [
			'MFT.States.settings.active:selected',
			'FFW.Backend.isNavigationEnabled::large',
            'MFT.AppController.activeVehicleModel.isClimateDisabled:hidden'
		],
		
		template: Em.Handlebars.compile(
			'<div id="setting_but_click" {{bindAttr class="FFW.Backend.isNavigationEnabled::large"}}></div>'
		),
		
		actionDown: function(event){
			if ( !MFT.States.settings.active ) {
				MFT.States.goToState( MFT.SettingsController.activeState );
			}
		}
	})
});