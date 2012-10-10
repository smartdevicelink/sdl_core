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
						'MFT.States.media.browse.active:active_state',
						'sirBlur:sirhelpblur',
						'cdBlur:cdhelpblur',
						'usbBlur:usbhelpblur'
					 ],
	/** View class*/
	classNames: ['browse_view','hidden'],
	
	stateObj:MFT.States.media.browse,
	
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
			}	this.set('usbBlur',true);
		}else{
			this.set('sirBlur',false);
			this.set('cdBlur',false);
			this.set('usbBlur',false);	
		}
	}.observes('MFT.helpMode'),
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_browse_back_button',
		classNames:			  'backButton',
		icon:		  			'images/media/ico_back.png',
		action:				  "browseBack",
		target:		          'MFT.MediaController',
		onDown:      			  false
	}),
	
	/** Upper title*/
	heading: MFT.Label.extend({
		elementId: 'media_browse_heading_lable',
		classNameBindings: ['MFT.helpMode: hidden'],
		classNames: 'title',
		contentBinding:  Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.title')
	}),
	
	/** Button to state "Browes All Sources" */
	browesAllSources: MFT.Button.extend({
	
		elementId: 'media_browse_browseall_button',
		classNameBindings: ['MFT.helpMode: hidden' , 
							'MFT.SDModel.active:media_usb_browse_browseall_button',
							'MFT.USBModel.active:media_usb_browse_browseall_button',
						   ],
		classNames:			  'browesAllSources',
		target: 	'MFT.MediaController',
		action:	'turnBrowseAll',
		onDown:      			  false,
		/** Define button template */
		template: Ember.Handlebars.compile(
			'<span>Browse</span><span class="all_sources_span">All Sources</span>'+
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
		
		currentPageBinding:   Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.currentPage'),
		
		itemsOnPage:	5
	})
});