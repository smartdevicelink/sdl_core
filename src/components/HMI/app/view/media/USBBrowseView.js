/**
 * @name MFT.USBBrowseView
 * 
 * @desc USB BrowseView visual representation
 * 
 * @category	View
 * @filesource	app/view/media/USBBrowseView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.USBBrowseView = Em.ContainerView.create({
	
	elementId: 'media_usb_browse_view',
	
	classNameBindings: 	['MFT.States.media.usb.browse.active:active_state','MFT.States.media.sd.browse.active:active_state','MFT.helpMode:help_blur'],
	/** View class*/
	classNames: ['browse_view','hidden'],
	
	/** Child views*/
	childViews: [
		'backButton',
		'heading',
		'browesAllSources',
		'list',
	],
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_usb_browse_back_button',
		classNames:			  'backButton',
		icon:		  			'images/media/ico_back.png',
		action:				  "back",
		target:		          'MFT.States',
		onDown:      			  false
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_usb_browse_heading_lable',
		classNameBindings:['MFT.helpMode:helpmode_blur_text'],
		classNames: 'title',
		contentBinding: Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.title')
	}),
	
	/** Button to state "Browes All Sources" */
	browesAllSources: MFT.Button.extend({
		elementId: 'media_usb_browse_browseall_button',
		classNameBindings: ['MFT.helpMode: hidden'],
		classNames:			  'browesAllSources',
		target: 	'MFT.MediaController',
		action:	'turnBrowseAll',
		arrow:					true,
		onDown:      			  false,
		/** Define button template */
		template: Ember.Handlebars.compile(
			'<span>Browse</span><span class="all_sources_span">All Sources</span>'+
			'<img class="arrow-ico" src="images/common/arrow_ico.png" />'
		)
	}),
	
	/** List menu items of "Browse"*/
	list: MFT.List.extend({
		elementId: 'media_usb_browse_list',
		
		classNameBindings: ['MFT.helpMode: hidden'],
		
		classNames: 	 ['guidelist','usbbrowselist'],
		
		disableScrollbar: true,
		
		itemsBinding:  Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.items'),
		
		itemsOnPage:	2
	})
});