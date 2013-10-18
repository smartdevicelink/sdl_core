/**
 * @name MFT.BrowseView
 * 
 * @desc BrowseView visual representation
 * 
 * @category	View
 * @filesource	app/view/media/radio/sirius/BrowseView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.browseView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_sirius_browse_view',
	
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
		elementId: 			   'media_sirius_browse_back_button',
		classNames:			  'backButton button',
		icon:		  			'images/media/ico_back.png',
		action:				  "back",
		target:		          'MFT.States',
		onDown:      			  false
	}),

	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_sirius_browse_heading_lable',
		classNames: 'title',
		contentBinding:  Ember.Binding.oneWay('MFT.locale.label.view_media_sirius_browse_title')
	}),
	
	/** Button to state "Browes All Sources" */
	browesAllSources: MFT.Button.extend({
	
		elementId: 'media_sirius_browse_browseall_button',
		classNameBindings: [
							'MFT.SDModel.active:media_usb_browse_browseall_button',
							'MFT.USBModel.active:media_usb_browse_browseall_button'
						   ],
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
		
		elementId: 'media_sirius_browse_list',
		
		
		classNames: 	 'guidelist',
		
		itemsOnPage:	5,
		
		items: [
				{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_AllChanels',
						rightText: '158',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_pop',
						rightText: '13',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_rock',
						rightText: '24',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_hipHop',
						rightText: '6',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_dance',
						rightText: '3',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_classic',
						rightText: '235',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_retro',
						rightText: '43',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_trance',
						rightText: '56',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_jazz',
						rightText: '201',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				},{
					type:MFT.Button,
					params:{
						templateName: 'rightText',
						textBinding: 'MFT.locale.label.view_media_sirius_browse_disco',
						rightText: '17',
						icon: 'images/media/ico_plus.png',
						disabled:	true
					}
				}
		]
		
	})
});