/**
 * @name MFT.sdBrowseView
 * 
 * @desc BrowseView visual representation
 * 
 * @category	View
 * @filesource	app/view/media/player/sd/browseView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.sdBrowseView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_sd_browse_view',
	
	classNameBindings:['FLAGS.locale'],
	/** View class*/
	classNames: ['browse_view'],
	
	customInsertElementBinding: 'MFT.MediaView',
	
	/** Child views*/
	childViews: [
		'backButton',
		'heading',
		'browesAllSources',
		'list'
	],
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_sd_browse_back_button',
		classNames:			  'backButton button',
		icon:		  			'images/media/ico_back.png',
		action:				  "back",
		target:		          'MFT.States',
		onDown:      			  false
	}),

	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_sd_browse_heading_lable',
		classNames: 'title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_media_sd_browse_title')
	}),
	
	/** Button to state "Browes All Sources" */
	browesAllSources: MFT.Button.extend({
	
		elementId: 'media_sd_browse_browseall_button',
		classNames:	['browesAllSources','media_usb_browse_browseall_button'],
		target: 	'MFT.MediaController',
		action:	'turnBrowseAll',
		onDown:      			  false,
		/** Define button template */
		template: Ember.Handlebars.compile(
			'<span> {{MFT.locale.label.view_media_usb_browse_browse}} </span><span class="all_sources_span"> {{MFT.locale.label.view_media_usb_browse_allSources}} </span>'+
			'<img class="arrow-ico" src="images/common/arrow_ico.png" />'
		)
	}),
	
	/** List menu items of "Browse"*/
	list: MFT.List.extend({
		
		elementId: 'media_sd_browse_list',
		
		disableScrollbar: true,
		
		classNames: 	 ['guidelist','media_usb_browse_list'],
		
		itemsOnPage:	5,
		
		items:	[
			{
				type:MFT.Button,
				params:{
					textBinding:  'MFT.locale.label.view_media_sd_browse_music',
					disabled:	true
				}
			},
			{
				type:MFT.Button,
				params:{
					textBinding:  'MFT.locale.label.view_media_sd_browse_exploreDevice',
					disabled:	true
				}
			}
		]
		
	})
});