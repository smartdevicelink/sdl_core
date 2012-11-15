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
		content: 'System Configurations',
	}),
	
	submit: MFT.Button.extend({
		elementId: 'select_system_submit',
		classNames: 'button',
		templateName: 'text',
		text: 'OK',
		action: 'onSettingsSubmit',
		target: 'MFT.AppController',
		onDown: false
	}),
	
	info: MFT.Label.extend({
			elementId: 'select_system_info',
			contentBinding: 'osAdittionalInfo',
			// set Ifor text according to OS
			deviceInfo: function(){
				if( FFW.isAndroid ) 
				{
					this.set('osAdittionalInfo', 'To re-configure in the future go to, Menu ->Settings');
				
				}else if( FFW.isIOS )
				{
					this.set('osAdittionalInfo','To re-configure in the future go to, Application Settings');
					
				}else{
					
					this.set('osAdittionalInfo',"To re-configure in the future, go to the App's Tool Bar");
					
				}
			}.observes('MFT.appReady')
			
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
				content: 'Climate Control Style'
			}),
			
			list: Em.ContainerView.extend({
				elementId: 'select_system_list',
				
				childViews: [
					'edge',
					'escape',
					'explorer',
					'f150',
					'flex',
					'focus',
					'fusion',
					'taurus'
				],
		
			
				edge: MFT.Button.extend({
					elementId: 'edge',
					classNameBindings: ['active'],
					active: true,
					classNames: ['button','left-border'],
					templateName: 'text',
					text: 'Edge',
					action: 'setActiveButton',
					target: 'MFT.AppController',
				}),
				
				escape: MFT.Button.extend({
					elementId: 'escape_2013',
					classNames: ['button'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: "Escape ('13)",
					action: 'setActiveButton',
					target: 'MFT.AppController',
				}),
				
				explorer: MFT.Button.extend({
					elementId: 'explorer',
					classNames: ['button'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: 'Explorer',
					action: 'setActiveButton',
					target: 'MFT.AppController',
				}),
				
				f150: MFT.Button.extend({
					elementId: 'f150_2013',
					classNames: ['button'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: "F150 ('13)",
					action: 'setActiveButton',
					target: 'MFT.AppController',
				}),
				
				flex: MFT.Button.extend({
					elementId: 'flex_2013',
					classNames: ['button','bottom','left-border'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: "Flex ('13)",
					action: 'setActiveButton',
					target: 'MFT.AppController',
				}),
				
				focus: MFT.Button.extend({
					elementId: 'focus',
					classNames: ['button','bottom'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: 'Focus',
					action: 'setActiveButton',
					target: 'MFT.AppController',
				}),
				
				fusion: MFT.Button.extend({
					elementId: 'fusion_2013',
					classNames: ['button','bottom'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: "Fusion ('13)",
					action: 'setActiveButton',
					target: 'MFT.AppController',
				}),
				
				taurus: MFT.Button.extend({
					elementId: 'taurus_2013',
					classNames: ['button','bottom'],
					classNameBindings: ['active'],
					templateName: 'text',
					text: "Taurus ('13)",
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
				content: 'SD Card Navigation'
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
					text: 'Yes',
					action: 'onYespress',
					target: 'MFT.AppController'
				}),
				
				no: MFT.Button.extend({
					elementId: 'navsdcard_no_btn',
					classNameBindings: ['MFT.AppController.navigationEnabled::active'],
					classNames: ['button'],
					templateName: 'text',
					text: 'No',
					action: 'onNopress',
					target: 'MFT.AppController'
				})
			})
		})
	})
});