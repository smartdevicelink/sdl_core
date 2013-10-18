/**
 * @name MFT.usbBrowseAllView
 * 
 * @desc Browse All View visual representation
 * 
 * @category	View
 * @filesource	app/view/media/player/usb/browse/BrowseAllView.js
 * @version		2.0
 *
 * @author		Igot Zhavoronkin
 */

MFT.usbBrowseAllView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_usb_browseAll_view',
	
	customInsertElementBinding: 'MFT.MediaView',
	
	/** View class*/
	classNames: ['browse_view','media_browseAll_view'],
		
	/** Child views*/
	childViews: [
		'backButton',
		'heading',
		'list'
	],
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_usb_browseall_back_button',
		classNames:			  'backButton',
		icon:		  			'images/media/ico_back.png',
		action:				  "back",
		target:		          'MFT.States',
		onDown:      			  false
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_usb_browseall_heading_lable',
		
		classNames: ['title','media_browseall_heading_lable'],
		
		contentBinding: 'MFT.locale.label.view_media_usb_browse_browseAvailable'
	}),
	
	/** List menu items of "Browse"*/
	list: MFT.List.extend({
		elementId: 'media_usb_browseAll_list',
		
		classNameBindings:['MFT.helpMode:disable-click'],
		
		classNames: 	 ['browseAlllist','media_browseAll_list'],
		
		disableScrollbar: true,
		
		items: [
				{
					type:MFT.Button,
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_browse_siriusChannel',
                        classNameBindings: ['MFT.SettingsModel.isSpanish:hidden'],
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
	})
	
});