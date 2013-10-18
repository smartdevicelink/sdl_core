/**
 * @name MFT.SettingsWarningPopupView
 * 
 * @desc Settings Warning Popup  module visual representation
 * 
 * @category	View
 * @filesource	app/view/SettingsWarningPopupView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.SettingsWarningPopupView = Em.ContainerView.create({
	
	elementId: 'settings_warning_popup_view',
	
	controllerBinding: 'MFT.SettingsController',
	
	childViews: [
		'wrapper',
		'content'
	],
	
	wrapper: Em.View.extend({
		elementId: 'settings_warning_popup_view_content_wrapper',
		classNames: ['wrapper']
	}),
	
	content:  Em.ContainerView.extend({
		
		elementId: 'settings_warning_popup_view_content',
		
		classNames: ['content'],
		
		classNameBindings: ['controller.isActiveResetToFactoryWarningContent:reset-verification','controller.isActiveResetToFactoryWarningContent::erase-data'],
		
		childViews: [
			'icon',
			'title',
			'text',
			'okButton',
			'cancelButton',
			'notification'
		],
		
		icon: Em.View.extend({
			elementId:  'settings_warning_popup_view_content_icon',
			classNames: 'icon'
		}),
		
		title: MFT.Label.extend({
			elementId:  'settings_warning_popup_view_content_title',
			classNames: 'title',
			contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_system_warning_popup_title')
		}),
		
		text: Em.View.extend({			
			elementId:	'settings_warning_popup_view_content_message',
			classNames:   'text',
			template: Em.Handlebars.compile(
				'{{#if controller.isActiveResetToFactoryWarningContent}}'+
					'<p>{{MFT.locale.label.view_settings_settings_system_warning_popup_retetToFactory_pr1}}</p>'+
					'<p>{{MFT.locale.label.view_settings_settings_system_warning_popup_retetToFactory_pr2}}</p>'+					
			
					'{{else}}'+					
						'<p>{{MFT.locale.label.view_settings_settings_system_warning_popup_eraseData}}</p>'+
				'{{/if}}'+
				
				'<p>{{MFT.locale.label.view_settings_settings_system_warning_popup_continue}}</p>'
			)
		}),
		
		okButton: MFT.Button.extend({
			elementId: 			 'settings_warning_popup_view_content_okButton',
			classNames:			'rateDialogue_button okButton',
			textBinding:	       Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_system_warning_popup_yes'),
			action:			    'masterResetPositiveAction',
			target:			    'this.controller'
		}),
		
		cancelButton: MFT.Button.extend({
			elementId: 			 'settings_warning_popup_view_content_cancelButton',
			classNames:			'rateDialogue_button okButton',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_system_warning_popup_no'),
			action:			    'masterResetNegativeAction',
			target:			    'this.controller'
		}),
	
		notification: MFT.Label.extend({
			elementId: 			 'settings_warning_popup_view_notification',
			classNames:			'settings-waring-notification',
			contentBinding:	    Ember.Binding.oneWay('MFT.locale.label.view_settings_settings_system_warning_notification'),
		})
	})
})