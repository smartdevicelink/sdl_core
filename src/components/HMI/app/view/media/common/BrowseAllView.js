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

MFT.BrowseAllView = Em.ContainerView.create({
	
	elementId: 'media_browseAll_view',
	
	classNameBindings: 	['MFT.States.media.Sirius.browse.browseAll.active:active_state','MFT.States.media.cd.browse.browseAll.active:active_state','MFT.States.media.usb.browse.browseAll.active:active_state','MFT.States.media.sd.browse.browseAll.active:active_state','MFT.helpMode: help_blur'],
	/** View class*/
	classNames: ['browse_view','hidden'],
	
	/** Child views*/
	childViews: [
		'backButton',
		'heading',
		'list',
	],
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_browseall_back_button',
		classNames:			  'backButton',
		icon:		  			'images/media/ico_back.png',
		action:				  "back",
		target:		          'MFT.States',
		onDown:      			  false
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_browseall_heading_lable',
		
		classNameBindings: ['MFT.helpMode: hidden'],
		
		classNames: 'title',
		
		content: 'Browse Available Sources'
	}),
	
	/** List menu items of "Browse"*/
	list: MFT.List.extend({
		elementId: 'media_browseAll_list',
		
		classNameBindings: ['MFT.helpMode: hidden'],
		
		classNames: 	 'browseAlllist',
		
		disableScrollbar: true,
		
		items: [
				{
					type:MFT.Button,
					params:{
						text:'SIRIUS Channel Guide',
						arrow: true,
						icon: 'images/media/ico_sirius.png',
						target: 'MFT.MediaController',
						action: 'turnOnSiriusBrowse',
						onDown: false
					}
				},
				{
					type:MFT.Button,
					params:{
						text:'CD Track List',
						arrow: true,
						icon: 'images/media/ico_cd.png',
						target: 'MFT.MediaController',
						action: 'turnOnCDBrowse',
						onDown: false
					}
				},{
					type:MFT.Button,
					params:{
						text:'USB Hard Disk Drive',
						arrow: true,
						icon: 'images/media/ico_usb.png',
						target: 'MFT.MediaController',
						action: 'turnOnUSBBrowse',
						onDown: false
					}
				},{
					type:MFT.Button,
					params:{
						text:'USB SD Disk Drive',
						arrow: true,
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