/**
 * @name MFT.CdMoreInfoView
 * 
 * @desc MoreInfoView module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/player/cd/moreInfoView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.CdMoreInfoView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 				 'media_cd_moreinfo',
		
	classNames: 		 		['moreInfo_view'],
	
	customInsertElementBinding: 'MFT.MediaView',

	/** Child views*/
	childViews: [
		'backButton',
		'title',
		'poster',
		'list'
	],
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			 'media_cd_moreinfo_backButton',
		classNames:			'moreInfo_backButton',
		icon:				  'images/media/ico_back.png',
		action:			    "back",
		target:			    'MFT.States',
		onDown:      		    false
		}),
	
	/** Upper title*/
	title: MFT.Label.extend({
		elementId: 			 'media_cd_moreinfo_title',
		classNames: 			'moreInfo_title',
		contentBinding: 'MFT.locale.label.view_media_moreInfo_title_cd'
	}),
	
	/** Album image*/
	poster: Em.View.extend({
		elementId: 'media_moreinfo_poster',
		template: Em.Handlebars.compile(
			'<div class="moreInfo_image">'+
				'<div class="cd_poster" {{bindAttr class="MFT.helpMode:helpmode"}}></div>'+
			'</div>'
		)
	}),
	
	list: MFT.List.extend({
		
		elementId: 'media_moreinfo_list',
		classNames:		   'cd-more-info',
		disableScrollbar:true,
		itemsOnPage:	5,
		
		/** Items */
		items: [{type:MFT.Label,params:{templateName: 'icon',icon:'images/media/list/ico_mi1.png',contentBinding:'MFT.MediaController.currentPlpayerModuleData.selectedItem.title'}},
				{type:MFT.Label,params:{templateName: 'icon',icon:'images/media/list/ico_mi2.png',contentBinding:'MFT.MediaController.currentPlpayerModuleData.selectedItem.artist'}},
				{type:MFT.Label,params:{templateName: 'icon',icon:'images/media/list/ico_mi3.png',contentBinding:'MFT.MediaController.currentPlpayerModuleData.selectedItem.disk'}},
				{type:MFT.Label,params:{templateName: 'icon',icon:'images/media/list/ico_mi4.png',contentBinding:'MFT.MediaController.currentPlpayerModuleData.selectedItem.genre',}}
		] 
	})
});