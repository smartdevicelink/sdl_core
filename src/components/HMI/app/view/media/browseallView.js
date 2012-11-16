/**
 * @name MFT.BrowseAllView
 * 
 * @desc Browse All View visual representation
 * 
 * @category	View
 * @filesource	app/view/media/common/BrowseAllView.js
 * @version		2.0
 *
 * @author		Igot Zhavoronkin
 */

MFT.BrowseAllView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_browseAll_view',
	
	classNameBindings:['MFT.helpMode:helpmode_blur_text_light'],

	/** View class*/
	classNames: ['browse_view'],
		
	/** Child views*/
	childViews: [
		'backButton',
		'heading',
		'list',
		'helpblurbg'
	],
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_browseall_back_button',
		classNames:			  'backButton',
		icon:		  			'images/media/ico_back.png',
		action:				  "turnBrowse",
		target:		          'MFT.MediaController',
		onDown:      			  false
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_browseall_heading_lable',
		
		classNames: 'title',
		
		contentBinding: 'MFT.locale.label.view_media_usb_browse_browseAvailable'
	}),
	
	/** List menu items of "Browse"*/
	list: MFT.List.extend({
		elementId: 'media_browseAll_list',
		
		classNames: 	 'browseAlllist',
		
		disableScrollbar: true,
		
		items: [
				{
					type:MFT.Button,
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_browse_siriusChannel',
						templateName: 'arrow',
						icon: 'images/media/ico_sirius.png',
						target: 'MFT.MediaController',
						action: 'turnOnSiriusBrowse',
						onDown: false
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_browse_cdTrack',
						templateName: 'arrow',
						icon: 'images/media/ico_cd.png',
						target: 'MFT.MediaController',
						action: 'turnOnCDBrowse',
						onDown: false
					}
				},{
					type:MFT.Button,
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_browse_usbHard',
						templateName: 'arrow',
						icon: 'images/media/ico_usb.png',
						target: 'MFT.MediaController',
						action: 'turnOnUSBBrowse',
						onDown: false
					}
				},{
					type:MFT.Button,
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_browse_usbSD',
						templateName: 'arrow',
						icon: 'images/media/ico_sd.png',
						target: 'MFT.MediaController',
						action: 'turnOnSDBrowse',
						onDown: false
					}
				}
			],
		
		itemsOnPage:	4
	}),
	
	helpblurbg: Em.View.create({
		elementId: 'media_browseall_blur_bg'
	})
	
});