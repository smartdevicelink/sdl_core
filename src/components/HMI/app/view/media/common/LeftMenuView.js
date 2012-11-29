/**
 * @name MFT.LeftMenuView
 * 
 * @desc Media Left Menu module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/SiriusView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.LeftMenuView = Em.ContainerView.extend({
		/** View ID*/
		elementId: 'media_leftmenu',
		
		/** View components*/
		childViews: [
			'border',
			'amButton',
			'fmButton',
			'siriusButton',
			'cdButton',
			'usbButton',
			'scrolldownButton',
			'scrollupButton',
			'sdButton',
			'btButton',
			'avinButton',
			'applinkButton'
		],
		/** Border decoration*/
		border: Em.View.extend({
			elementId: 'media_left_menu_border',
			classNames:['ls_border']
		}),
		
		/** AM Station Button*/
		amButton:  MFT.ToggleIndicatorsButton.create({
			classNameBindings: ['MFT.AmModel.active:active_state', 'MFT.MediaController.isTopListMenu:displayed'],
			elementId:			 'media_amButton',
			classNames:			['media-ls-item'],
			action:				'toggleAmStation',
			icon:				  'images/media/ico_am.png',
			target:				'MFT.MediaController',
			contentBinding:		Ember.Binding.oneWay('MFT.AmModel.band'),
			indActiveClass:		'small_ind_active',
			indDefaultClass:	   'small_ind_def',
			textBinding:		   Ember.Binding.oneWay('MFT.MediaController.amLabel')
		}),
		/** FM Station Button*/
		fmButton:  MFT.ToggleIndicatorsButton.extend({
			classNameBindings: ['MFT.FmModel.active:active_state' , 'MFT.MediaController.isTopListMenu:displayed'],
			elementId:			 'media_fmButton',
			classNames:			['media-ls-item'],
			action:				'toggleFmStation',
			icon:				  'images/media/ico_fm.png',
			target:				'MFT.MediaController',
			contentBinding:		 Ember.Binding.oneWay('MFT.FmModel.band'),
			indActiveClass:		'small_ind_active',
			indDefaultClass:	   'small_ind_def',
			textBinding:		    Ember.Binding.oneWay('MFT.MediaController.fmLabel')
		}),
		/** SIRIUS Station Button*/
		siriusButton:  MFT.ToggleIndicatorsButton.extend({
			classNameBindings: ['MFT.SiriusModel.active:active_state' , 'MFT.MediaController.isTopListMenu:displayed'],
			elementId:			 'media_siriusButton',
			classNames:			['media-ls-item'],
			action:				'toggleSiriusStation',
			icon:				  'images/media/ico_sirius.png',
			target:				'MFT.MediaController',
			contentBinding:		 Ember.Binding.oneWay('MFT.SiriusModel.band'),
			indActiveClass:		'small_ind_active',
			indDefaultClass:	   'small_ind_def',
			textBinding:		    Ember.Binding.oneWay('MFT.MediaController.siriusLabel')
		}),
		/** CD Station Button*/
		cdButton:  MFT.Button.extend({
			classNameBindings: ['MFT.CDModel.active:active_state' , 'MFT.MediaController.isTopListMenu:displayed'],
			elementId:			 'media_cdButton',
			classNames:			['media-ls-item'],
			action:				'turnOnCD',
			icon:				  'images/media/ico_cd.png',
			target:				'MFT.MediaController',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_cd')
		}),
		/** USB Station Button*/
		usbButton:  MFT.Button.extend({
			classNameBindings: ['MFT.USBModel.active:active_state' , 'MFT.MediaController.isTopListMenu:displayed'],
			elementId: 			 'media_usbButton',
			classNames:			['media-ls-item'],
			action:				'turnOnUSB',
			icon:				  'images/media/ico_usb.png',
			target:				'MFT.MediaController',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_usbHard')
		}),
		/** SCROLL DOWN Menu List Button*/
		scrolldownButton:  MFT.Button.extend({
			classNameBindings: ['MFT.MediaController.isTopListMenu:displayed'],
			elementId:			'media_scrolldownButton',
			target:				'MFT.MediaController',
			action:				'listDown',
			classNames:			['media-ls-item'],
			icon:				  'images/media/ico_arrow_down.png'
		}),
		/** SCROLL UP Menu List Button*/
		scrollupButton:  MFT.Button.extend({
			classNameBindings: ['MFT.MediaController.isTopListMenu:displayed'],
			elementId:			 'media_scrollupButton',
			target:				'MFT.MediaController',
			action:				'listUp',
			classNames:			['media-ls-item'],
			icon:				  'images/media/ico_arrow_up.png'
		}),
		/** SD  Button*/
		sdButton:  MFT.Button.extend({
			classNameBindings: ['MFT.SDModel.active:active_state' , 'MFT.MediaController.isTopListMenu:displayed'],
			elementId:			'media_sdButton',
			classNames:			['media-ls-item'],
			action:				'turnOnSD',
			icon:				  'images/media/ico_sd.png',
			target:				'MFT.MediaController',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_usbSD')
		}),
		/** BLUETOOTH Button*/
		btButton:  MFT.Button.extend({
			classNameBindings: ['MFT.States.media.bluetooth.active:active_state' , 'MFT.MediaController.isTopListMenu:displayed'],
			elementId:			'media_btButton',
			classNames:			['media-ls-item'],
			action:				'turnOnBT',
			icon:				'images/media/ico_bt.png',
			target:				'MFT.MediaController',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_bt')
		}),
		/** AVIN Button*/
		avinButton:  MFT.Button.extend({
			classNameBindings: ['MFT.States.media.avin.active:active_state' , 'MFT.MediaController.isTopListMenu:displayed'],
			elementId:			'media_avinButton',
			classNames:			['media-ls-item'],
			action:				'turnOnAVin',
			icon:				'images/media/ico_li.png',
			target:				'MFT.MediaController',
			textBinding:		   Ember.Binding.oneWay('MFT.locale.label.view_media_avi')
		}),
		/** APPLINK Button*/
		applinkButton:  MFT.Button.extend({
			classNameBindings:	[
									'MFT.ApplinkMediaController.hideApplinkMediaButton:hidden',
									'MFT.States.media.applink.active:active_state',
									'MFT.MediaController.isTopListMenu:displayed'
								],
			elementId:			'media_applinkButton',
			classNames:			['media-ls-item'],
			action:				'turnOnApplink',
			icon:				'images/media/ico_li.png',
			target:				'MFT.MediaController',
			textBinding:		'MFT.ApplinkModel.showInfo.appName'
		})
			
	});