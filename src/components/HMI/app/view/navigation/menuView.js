/**
 * @name MFT.menuView
 * 
 * @desc navigation Menu visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/menuView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.menuView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'nav_menu_view',
	
	childViews: [
		'lable',
		'menuButtons'
	],
	
	lable: MFT.Label.extend({
		elementId: 'nav_menu_lable',
		contentBinding:		 Em.Binding.oneWay('MFT.locale.label.view_navigation_menu_title')
	}),
	
	menuButtons: Em.ContainerView.extend({
		elementId: 'nav_menu_buttons',
		
		childViews: [
			'navset',
			'mute',
			'viewedit',
			'cancelroute',
			'siriuslink',
			'exit'
			
		],
	
		navset: MFT.Button.extend({
			classNames:		  ['button','right_border'],		
			action:			  'turnOnNavSettings',
			target:			  'MFT.NavigationController',
			templateName: 		'arrow',
			textBinding:		 Em.Binding.oneWay('MFT.locale.label.view_navigation_menu_navigationSettings')
		}),
		
		mute: MFT.IndButton.extend({
			elementId: 'guadian_mute',
			classNames:		  ['button'],		
			action:				'toggleMute',
			target:				'MFT.NavigationController',
			contentBinding:		Em.Binding.oneWay('MFT.NavigationController.model.mute'),
			indActiveClass:	  'ind_act',
			indDefaultClass:	 'ind_inact',
			textBinding:		 Em.Binding.oneWay('MFT.locale.label.view_navigation_menu_guidanceMute')
		
		}),
		
		viewedit: MFT.Button.extend({
			classNames:		  ['button','right_border'],		
			templateName: 		'arrow',
			textBinding:		 Em.Binding.oneWay('MFT.locale.label.view_navigation_menu_viewEdit'),
			disabled: true
		}),
		
		siriuslink: MFT.Button.extend({
			classNames:		  ['button','right_border'],
			action:			  'turnOnSiriusLink',
			target:			  'MFT.NavigationController',		
			templateName: 		'arrow',
			textBinding:		 Em.Binding.oneWay('MFT.locale.label.view_navigation_menu_siriusTravelL')
		}),
		
		cancelroute: MFT.Button.extend({
			classNames:		  ['button'],	
			templateName: 		'text',
			textBinding:		 Em.Binding.oneWay('MFT.locale.label.view_navigation_menu_cancelRoute'),
			disabled: true
		}),
		
		exit: MFT.Button.extend({
			classNames:		  ['button'],
			action:			  'back',
			target:			  'MFT.States',		
			templateName: 		'text',
			textBinding:		 Em.Binding.oneWay('MFT.locale.label.view_navigation_menu_close')
		})
	})
})