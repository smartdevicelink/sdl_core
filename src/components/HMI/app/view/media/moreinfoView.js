/**
 * @name MFT.MoreInfoView
 * 
 * @desc MoreInfoView module visual representation
 * 
 * @category	View
 * @filesource	app/view/help/MoreInfoView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.MoreInfoView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_moreinfo',
	
	classNameBindings: 	['MFT.States.media.moreinfo.active:active_state'],
	
	classNames: 		 ['moreInfo_view','hidden'],
	
	stateObj:MFT.States.media.moreinfo,

	/** Child views*/
	childViews: [
		'backButton',
		'title',
		'whatsPlayingButton',
		'poster',
		'list',
		'listBgBlur'
	],
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 'media_moreinfo_backButton',
		classNames:			'moreInfo_backButton',
		icon:				 'images/media/ico_back.png',
		action:			   "backMoreInfo",
		target:			   'MFT.MediaController',
		onDown:      		   false
		}),
	
	/** Upper title*/
	title: MFT.Label.extend({
		elementId: 'media_moreinfo_title',
		classNames: 			'moreInfo_title',
		content: 			  'Song Information'
	}),
	
	/** Button "What is playing" */
	whatsPlayingButton: MFT.Button.extend({
		elementId: 	'media_moreinfo_whatsplayingButton',
		classNameBindings:['MFT.helpMode:helpmode'],
		classNames:	'moreInfo_whatIsPlayingButton',
	}),
	
	/** Album image*/
	poster: Em.View.extend({
		elementId: 'media_moreinfo_poster',
		template: Em.Handlebars.compile(
			'<div class="moreInfo_image">'+
				'<div id="cd_poster" {{bindAttr class="MFT.CDModel.active:visible_display MFT.SDModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
				'<div id="usb_poster" {{bindAttr class="MFT.USBModel.active:visible_display MFT.helpMode:helpmode"}}></div>'+
			'</div>'
		)
	}),
	
	listBgBlur: Em.View.extend({
		elementId: 'media_moreinfo_list_blur',
		classNameBindings: ['MFT.helpMode: active_state'],
		classNames:'hidden'
	}),
	
	list: MFT.List.extend({
		
		elementId: 'media_moreinfo_list',
		classNameBindings:['MFT.helpMode:helpmode_blur_text'],
		classNames:		   'cd-more-info',
		/** Items */
		
		items: [{type:MFT.Label,params:{icon:'images/media/list/ico_mi1.png',contentBinding:'MFT.MediaController.currentModuleData.selectedItem.album'}},
				{type:MFT.Label,params:{icon:'images/media/list/ico_mi2.png',contentBinding:'MFT.MediaController.currentModuleData.selectedItem.title'}},
				{type:MFT.Label,params:{icon:'images/media/list/ico_mi3.png',contentBinding:'MFT.MediaController.currentModuleData.selectedItem.disk'}},
				{type:MFT.Label,params:{icon:'images/media/list/ico_mi4.png',contentBinding:'MFT.MediaController.currentModuleData.selectedItem.genre',}},
				{type:MFT.Label,params:{icon:'images/media/list/ico_mi5.png',contentBinding:'MFT.MediaController.currentModuleData.selectedItem.artist'}}
		],  
		
		itemsOnPage:	5
	})
	
});