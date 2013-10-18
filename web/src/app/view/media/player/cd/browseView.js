/**
 * @name MFT.cdBrowseView
 * 
 * @desc BrowseView visual representation
 * 
 * @category	View
 * @filesource	app/view/media/player/cd/browseView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.cdBrowseView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_cd_browse_view',
	
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
		elementId: 			   'media_cd_browse_back_button',
		classNames:			  'backButton button',
		icon:		  			'images/media/ico_back.png',
		action:				  "back",
		target:		          'MFT.States',
		onDown:      			  false
	}),

	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_cd_browse_heading_lable',
		classNames: 'title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_media_cd_browse_CDTrack')
	}),
	
	/** Button to state "Browes All Sources" */
	browesAllSources: MFT.Button.extend({
	
		elementId: 'media_cd_browse_browseall_button',
		classNames:			  'browesAllSources',
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
		
		elementId: 'media_cd_browse_list',
		
		
		classNames: 	 'guidelist',
		
		itemsOnPage:	5,
		
		items: [
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.0.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.1.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.2.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.3.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.4.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.5.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.6.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.7.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.8.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.9.title',
						disabled:	true
					}
				}
		]
		
	})
});