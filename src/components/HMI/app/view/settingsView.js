/**
 * @name MFT.SettingsView
 * 
 * @desc Settings module visual representation
 * 
 * @category	Settings
 * @filesource	app/view/SettingsView.js
 * @version		2.0
 *
 * @author		Gerashchenko Maksym
 */
 
 MFT.SettingsView = Em.ContainerView.create(MFT.LoadableView, {
 	
 	elementId:	'settings_view',
 	 	
	/** Bind class for visual representation */
	classNameBindings: ['MFT.helpMode'],
			
	/** Settings components */
	childViews: [
		'menu'
	],
	
	/** Left menu */
	menu: Em.ContainerView.extend({
		elementId:	'ctrl_settings_menu',
		
		classNameBindings:	['MFT.SettingsController.hiddenLeftMenu:hidden'],
		
		childViews: [
			'border',
			'items'
		],
		
		border: Em.View.extend({
			classNames:	'ls_border',
			
			template: Ember.Handlebars.compile('<img class="bg" src="images/common/ls_border.png">')
		}),
		
		items: Em.ContainerView.extend({
			classNames:	'ls-items',
			
			childViews: [
				'clockButton',
				'displayButton',
				'soundButton',
				'vehicleButton',
				'settingsButton',
				'helpButton'
			],
			
			clockButton: MFT.Button.extend({
				elementId:			'settings_menu_clockButton',
				
				goToState:   			 'clock',	
				classNames:			'ls-item lsp1_p',
				classNameBindings:	 ['MFT.States.settings.clock.active:settings_active'],
				textBinding:	  	   'MFT.locale.label.view_settings_clock',
				icon:				  'images/settings/ico_clock.png',
				action:				'onState',
				target:				'MFT.SettingsController',
			}),
			
			displayButton: MFT.Button.extend({
				elementId:			'settings_menu_displayButton',
				
				goToState:   			 'display',	
				classNames:			'ls-item lsp1_p',
				classNameBindings:	 ['MFT.States.settings.display.active:settings_active'],
				textBinding:	  	   'MFT.locale.label.view_settings_display',
				icon:				  'images/settings/ico_display.png',
				action:				'onState',
				target:				'MFT.SettingsController',
			}),
			
			soundButton: MFT.Button.extend({
				elementId:			'settings_menu_soundButton',
				
				goToState:   			 'sound',	
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.settings.sound.active:settings_active'],
				textBinding:	  	   'MFT.locale.label.view_settings_sound',
				icon:				  'images/settings/ico_sound.png',
				action:				'onState',
				target:				'MFT.SettingsController',
			}),
			
			vehicleButton: MFT.Button.extend({
				elementId:			'settings_menu_vehicleButton',
				
				goToState:   			 'vehicle',	
				classNames:			'ls-item lsp1_p',
				classNameBindings:	['MFT.States.settings.vehicle.active:settings_active'],
				textBinding:	  	   'MFT.locale.label.view_settings_vehicle',
				icon:				  'images/settings/ico_vehicle.png',
				action:				'onState',
				target:				'MFT.SettingsController',
			}),
			
			settingsButton: MFT.Button.extend({
				elementId:			'settings_menu_settingsButton',
				
				goToState:   			 'settings',	
				classNames:			'ls-item lsp1_p',
				classNameBindings:	 ['MFT.States.settings.settings.active:settings_active'],
				textBinding:	  	   'MFT.locale.label.view_settings_settings',
				icon:				  'images/settings/ico_settings.png',
				action:				'onState',
				target:				'MFT.SettingsController',
			}),
			
			helpButton: MFT.Button.extend({
				elementId:			'settings_menu_helpButton',
				
				goToState:   			 'help',	
				classNames:			'ls-item lsp1_p',
				classNameBindings:	 ['MFT.States.settings.help.active:settings_active'],
				textBinding:	  	   'MFT.locale.label.view_settings_help',
				icon:				  'images/settings/ico_help.png',
				action:				'onState',
				target:				'MFT.SettingsController',
			})
		})
	})
});

