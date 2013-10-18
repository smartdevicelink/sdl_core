/**
 * @name MFT.btOptionsView
 * 
 * @desc Media Bluetooth Options module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/player/bluetooth/optionsView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.btOptionsView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:			'media_bt_options_view',
	
	classNames: ['bt','media_options_view'],
	
	customInsertElementBinding: 'MFT.MediaView',

	childViews:		   [
							  'backButton',
							  'topTitile',
							  'list'
						  ],
	
	topTitile: MFT.Label.extend({
		elementId:		'media_option_bt_title_header',
		classNames:		  ['block-header-title','media_option_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_bt_options_title')
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_bt_options_list',
			
			classNameBindings: ['FLAGS.locale','MFT.helpMode:disable-click','list'],
			
			itemsOnPage:	5,
			
			disableScrollbar:  true,
			
			items:[
			{
				type:		MFT.Button,
			
				params:{
					textBinding: 'MFT.locale.label.view_media_bt_options_soundSettings',
					templateName:		'arrow',
					action: 'turnOnSoundSettings',
					target: 'MFT.MediaController'
				}							
			},
			
			{
				type:		MFT.Button,
			
				params:{
					textBinding: 'MFT.locale.label.view_media_bt_options_mediaPlayerSettings',
					templateName:		'arrow',
					action: 'goToMediaSettings',
					target: 'MFT.MediaController'
				}							
			},
			
			{
				type:		MFT.Button,
			
				params:{
					textBinding: 'MFT.locale.label.view_media_bt_options_deviceInformation',
					templateName:		'arrow',
					templateName:  'arrow',
					action: 		'turnOnDeviceInformation',
					target: 		'MFT.MediaController'
				}							
			}
		]
 
	}),
	
	backButton: MFT.Button.extend({
		elementId: 			'media_options_bt_back_btn',
		classNames:		  ['backButton','button','media_options_back_btn'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	})
});