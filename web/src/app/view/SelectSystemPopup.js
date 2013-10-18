/**
 * @name MFT.SelectSystempopup
 * 
 * @desc Warning View visual representation
 * 
 * @category	View
 * @filesource	app/view/SelectSystempopup.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.SelectSystempopup = Em.ContainerView.create({

	elementId: 'select_sysytem_view',
	
	classNameBindings: ['MFT.AppController.active:active_state'],
	
	classNames: 'hidden',
	
	childViews: [
		'heading',
		'content',
		'info',
		'submit'
	],
		
	heading:  MFT.Label.extend({
		elementId: 'select_system_heading_lable',
		contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_selectSystempopup_title')
	}),
	
	submit: MFT.Button.extend({
		elementId: 'select_system_submit',
		classNames: 'button',
		templateName: 'text',
		textBinding:	Ember.Binding.oneWay('MFT.locale.label.view_selectSystempopup_ok'),
		action: 'onSettingsSubmit',
		target: 'MFT.AppController',
		onDown: false
	}),
	
	info: MFT.Label.extend({
			elementId: 'select_system_info',
			template: Em.Handlebars.compile(
				'<div>' +
                    '{{MFT.locale.label.view_systemconfig_note_1}}' +
                    '<span class="start-setting-i"></span>' +
                    '{{MFT.locale.label.view_systemconfig_note_2}}' +
                '</div>'
			)
		}),
	
	content: Em.ContainerView.extend({
		
		elementId: 'select_system_container',
		
		childViews: [
			'navsdcard',
			'climate_style'
		],
		
		climate_style: Em.ContainerView.extend({
			
			elementId: 'settings_list',
			
			childViews: [
				'label',
				'list'
	
			],
			
			label: MFT.Label.extend({
				elementId: 'select_system_label',
				contentBinding:	Ember.Binding.oneWay('MFT.locale.label.view_systemconfig_climatecontrollable')
			}),
			
			list: Em.ContainerView.extend({
				elementId: 'select_system_list',
				
				childViews: [
                    'fiesta',
                    'focus',
                    'fusion',
                    'taurus',
                    'escape',
					'edge',
					'explorer',
                    'flex',
					'f150'
				],

                fiesta: MFT.Button.extend({
                    elementId: 'fiesta',
                    classNames: ['button','left-border'],
                    classNameBindings: ['active'],
                    templateName: 'text',
                    text: "Fiesta",
                    action: 'setActiveButton',
                    target: 'MFT.AppController'
                }),

                focus: MFT.Button.extend({
                    elementId: 'focus',
                    classNames: ['button'],
                    classNameBindings: ['active'],
                    templateName: 'text',
                    text: 'Focus',
                    action: 'setActiveButton',
                    target: 'MFT.AppController'
                }),

                fusion: MFT.Button.extend({
                    elementId: 'fusion_2013',
                    classNames: ['button'],
                    classNameBindings: ['active'],
                    templateName: 'text',
                    text: "Fusion",
                    action: 'setActiveButton',
                    target: 'MFT.AppController'
                }),

                taurus: MFT.Button.extend({
                    elementId: 'taurus_2013',
                    classNames: ['button'],
                    classNameBindings: ['active'],
                    templateName: 'text',
                    text: "Taurus",
                    action: 'setActiveButton',
                    target: 'MFT.AppController',
                    disabled: MFT.SettingsModel.isSpanish
                }),

                escape: MFT.Button.extend({
                    elementId: 'escape_2013',
                    classNames: ['button','left-border'],
                    classNameBindings: ['active'],
                    templateName: 'text',
                    text: "Escape",
                    action: 'setActiveButton',
                    target: 'MFT.AppController'
                }),

				edge: MFT.Button.extend({
					elementId: 'edge',
					classNameBindings: ['active'],
					active: true,
					classNames: ['button'],
					templateName: 'text',
					text: 'Edge',
					action: 'setActiveButton',
					target: 'MFT.AppController'
				}),

				explorer: MFT.Button.extend({
					elementId: 'explorer',
					classNames: ['button'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: 'Explorer',
					action: 'setActiveButton',
					target: 'MFT.AppController'
				}),

                flex: MFT.Button.extend({
                    elementId: 'flex_2013',
                    classNames: ['button'],
                    classNameBindings: ['active'],
                    templateName: 'text',
                    text: "Flex",
                    action: 'setActiveButton',
                    target: 'MFT.AppController',
                    disabled: MFT.SettingsModel.isSpanish
                }),

				f150: MFT.Button.extend({
					elementId: 'f150_2013',
					classNames: ['button','left-border'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: MFT.SettingsModel.isSpanish ? "Lobo" : "F150",
					action: 'setActiveButton',
					target: 'MFT.AppController'
				})

			})
		}),
		
		navsdcard: Em.ContainerView.extend({
			elementId: 'navsdcard',
			
			childViews: [
				'label',
				'buttons'
			],
			
			label: MFT.Label.extend({
				elementId: 'sdnav_label',
				contentBinding: 'MFT.locale.label.view_systemconfig_sdlable'
			}),
			
			buttons: Em.ContainerView.extend({
				elementId: 'sdnav_buttons',
				childViews: [
					'yes',
					'no'
				],
					
				yes: MFT.Button.extend({
					elementId: 'navsdcard_yes_btn',
					classNames: ['button'],
					classNameBindings: ['MFT.AppController.navigationEnabled:active'],
					templateName: 'text',
					textBinding: 'MFT.locale.label.view_wo_yes',
					action: 'onYespress',
					target: 'MFT.AppController'
				}),
				
				no: MFT.Button.extend({
					elementId: 'navsdcard_no_btn',
					classNameBindings: ['MFT.AppController.navigationEnabled::active'],
					classNames: ['button'],
					templateName: 'text',
					textBinding: 'MFT.locale.label.view_wo_no',
					action: 'onNopress',
					target: 'MFT.AppController'
				})
			})
		})
	})
});