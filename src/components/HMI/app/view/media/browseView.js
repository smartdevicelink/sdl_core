/**
 * @name MFT.BrowseView
 * 
 * @desc BrowseView visual representation
 * 
 * @category	View
 * @filesource	app/view/media/common/BrowseView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.browseView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId: 'media_browse_view',
	
	classNameBindings:[
						'sirBlur:sirhelpblur',
						'cdBlur:cdhelpblur',
						'usbBlur:usbhelpblur',
						'MFT.localization'
					 ],
	/** View class*/
	classNames: ['browse_view'],
	
	/** Child views*/
	childViews: [
		'backButton',
		'heading',
		'browesAllSources',
		'list',
	],
	/** Toggle help mode blur background */
	omHelpMode: function(){
		if(MFT.helpMode){
			if(MFT.SiriusModel.active){
				this.set('sirBlur',true);
			}else if(MFT.CDModel.active){
				this.set('cdBlur',true);
			}else{
				this.set('usbBlur',true);
			}	
		}else{
			this.set('sirBlur',false);
			this.set('cdBlur',false);
			this.set('usbBlur',false);	
		}
	}.observes('MFT.helpMode'),
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_browse_back_button',
		classNames:			  'backButton button',
		icon:		  			'images/media/ico_back.png',
		action:				  "browseBack",
		target:		          'MFT.MediaController',
		onDown:      			  false
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_browse_heading_lable',
		classNameBindings: ['MFT.helpMode: helpmode_blur_text'],
		classNames: 'title',
		contentBinding:  Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.title')
	}),
	
	/** Button to state "Browes All Sources" */
	browesAllSources: MFT.Button.extend({
	
		elementId: 'media_browse_browseall_button',
		classNameBindings: ['MFT.helpMode: helpmode_blur_text' , 
							'MFT.SDModel.active:media_usb_browse_browseall_button',
							'MFT.USBModel.active:media_usb_browse_browseall_button',
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
		elementId: 'media_browse_list',
		
		classNameBindings: ['MFT.helpMode: hidden', 'MFT.SDModel.active:usbbrowselist','MFT.USBModel.active:usbbrowselist'],
		
		classNames: 	 'guidelist',
		
		disableScrollbarBinding:  Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.disableScrollbar'),
		
		itemsBinding:  Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.items'),
		
		currentPageBinding:   'MFT.MediaController.currentBrowseData.currentPage',
		
		itemsOnPage:	5
	})
});