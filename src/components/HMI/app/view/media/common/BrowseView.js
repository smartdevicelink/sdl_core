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

MFT.BrowseView = Em.ContainerView.create({
	
	elementId: 'media_browse_view',
	
	classNameBindings: 	['MFT.States.media.Sirius.browse.active:active_state','MFT.States.media.cd.browse.active:active_state','sirBlur:sirhelpblur','cdBlur:cdhelpblur'],
	/** View class*/
	classNames: ['browse_view','hidden'],
	
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
			if(MFT.States.media.Sirius.active ){
				this.set('sirBlur',true);
			}else{
				this.set('cdBlur',true);
			}
		}else{
			this.set('sirBlur',false);
			this.set('cdBlur',false);	
		}
	}.observes('MFT.helpMode'),
	
	/** Button back to previous state*/
	backButton: MFT.Button.extend({
		elementId: 			   'media_browse_back_button',
		classNames:			  'backButton',
		icon:		  			'images/media/ico_back.png',
		action:				  "back",
		target:		          'MFT.States',
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
		classNameBindings: ['MFT.helpMode: hidden'],
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
		
		classNameBindings: ['MFT.helpMode: hidden'],
		
		classNames: 	 'guidelist',
		
		itemsBinding:  Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.items'),
		
		currentPageBinding:   Ember.Binding.oneWay('MFT.MediaController.currentBrowseData.currentPage'),
		
		itemsOnPage:	5
	})
});