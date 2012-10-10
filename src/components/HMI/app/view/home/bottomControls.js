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
	
	childViews: [
		'information',
		'home',
		'settings'
	],
	
	information : Em.View.extend({
		elementId:			'info_but',
		classNameBindings: ['MFT.States.info.active:selected'],
		
		template: Em.Handlebars.compile(
			'<div id="info_but_click"></div>'
		),
		
		actionDown: function(event){
			MFT.States.goToState( MFT.InfoController.activeState );
		}
	}),
	
	home: Em.View.extend({
		elementId:			'home_but',
		classNameBindings: ['MFT.States.home.active:selected'],
		actionDown: function(event){
			MFT.States.goToState('home');
		}
	}),
	
	settings: Em.View.extend({
		elementId:			'setting_but',
		
		classNameBindings: ['isActive:selected'],
		
		/*Set Active State On Child active state = true*/
		isActive: function(){
			return MFT.States.settings.childStates.filterProperty('active', true).length === 1;
		}.property('MFT.States.settings.childStates.@each.active'),
		
		template: Em.Handlebars.compile(
			'<div id="setting_but_click"></div>'
		),
		
		actionDown: function(event){
			MFT.States.goToState( MFT.SettingsController.activeState );
		}
	})
});