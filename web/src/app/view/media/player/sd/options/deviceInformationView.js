/**
 * @name MFT.sdDeviceInformationView
 * 
 * @desc Media Options device Information visual representation
 * 
 * @category	View
 * @filesource	app/view/media/player/sd/options/deviceInformationView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.sdDeviceInformationView = Em.ContainerView.create(MFT.LoadableView, {
	
	elementId:			'media_options_sd_deviceInformation_view',
	
	classNames: ['sd','media_options_deviceInformation_view'],
		
	childViews:		   [
							  'backButton',
							  'topTitile',
							  'deviceImage',
							  'list'
						  ],

	backButton: MFT.Button.extend({
		elementId: 			'media_options_sd_deviceInformation_back_btn',
		classNames:		   ['backButton','button','media_options_deviceInformation_back_btn'],		
		action:			   'back',
		target:			   'MFT.States',	
		icon:				 'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'media_options_sd_deviceInformation_title_header',
		classNames:		  ['block-header-title','media_options_deviceInformation_title_header'],
		contentBinding:	  Ember.Binding.oneWay('MFT.locale.label.view_media_usbSD_options_deviceInformation_title')
	}),	
	
	deviceImage: MFT.Label.extend({
		elementId:		   'media_options_sd_deviceInformation_deviceImage',
		classNames:		  ['device-image','media_options_deviceInformation_deviceImage']
	}),				  
	
	list:	MFT.List.extend({
			
			elementId:	  'media_options_sd_deviceInformation_list',
			classNames: ['media_options_deviceInformation_list'],
			classNameBindings: ['FLAGS.locale','MFT.helpMode:disable-click'],
			
			itemsOnPage:	5,
			
			items:[
				
				{
					type:		MFT.Label,
	
					params:{
						contentBinding: 'MFT.locale.label.view_media_usbSD_options_deviceInformation_CJSDRIVE'
					}							
				},
				
				{
					type:		MFT.Label,
	
					params:{
						content: '-1667014053',
					}							
				},
				
				{
					type:		MFT.Label,
	
					params:{
						content: '5'
					}							
				}
					
			]
 
	})
});