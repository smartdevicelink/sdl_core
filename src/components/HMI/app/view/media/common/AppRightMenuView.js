/**
 * @name MFT.RightMenuView
 * 
 * @desc Media  Right Menu visual representation
 * 
 * @category	View
 * @filesource	app/view/media/MFT.RightMenuView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.AppRightMenuView = Em.ContainerView.create({
	/** View ID */
	elementId:	'media_appRightMenu',
	/** Class Names bindings for visual representation*/
	classNameBindings: 	[
		'MFT.AppModel.active:visible_display',
		'hideMenu:hidden'
	],
	/** Right menu scrolling object */
	scroll: null,
	/** flag for scroll positions */
	scrollTopPos: false,
	/** flag for scroll positions */
	scrollBottomPos: true,
	/** scroll position */
	position: 0,
	/** Class Names */
	classNames: ['hidden_display'],
	/** View Components*/
	
	childViews: [
		'topScrollButton',
		'scrollWrapper',
		'bottomScrollButton'
	],

	hideMenu: function(){
		if( (MFT.States.media.appsubmenu.active == true) || (MFT.States.media.appoptions.active == true) ){
			return true;
		}else{
			return false;
		}
	}.property('MFT.States.media.appsubmenu.active', 'MFT.States.media.appoptions.active'),

	topScrollButton: MFT.Button.extend({
		classNameBindings: 	[
			'parentView.scrollTopPos:visible_display'
		],
		elementId:	'media_rightmenu_topButton',
		classNames:	['media_rightmenu_topButton', 'hidden_display'],
		action:		function(){
			MFT.AppRightMenuView.scroll.scrollTo(0, MFT.AppRightMenuView.scroll.y + 50, 200);
			MFT.AppRightMenuView.set('position', MFT.AppRightMenuView.scroll.y + 50);
		},
		onDown:		false,
		text:		'top'
	}),
	
	bottomScrollButton: MFT.Button.extend({
		classNameBindings: 	[
			'parentView.scrollBottomPos:visible_display'
		],
		elementId:	'media_rightmenu_bottomButton',
		classNames:	['media_rightmenu_bottomButton', 'hidden_display'],
		action:		function(){
			MFT.AppRightMenuView.scroll.scrollTo(0, MFT.AppRightMenuView.scroll.y - 50, 200);
			MFT.AppRightMenuView.set('position', MFT.AppRightMenuView.scroll.y - 50);
		},
		onDown:		false,
		text:		'bottom'
	}),

	AddCommand: function( commandId, params){

		//parentID
		//position
		//menuName

		button = MFT.Button.create({
			elementId:			'media_rightmenu_softButton' + commandId,
			click:				function(){
				FFW.UI.onCommand(this.commandId);
			},
			commandId:			commandId, 
			classNames:			['rs-item'],
			//icon:				null,//'images/media/active_arrow.png',
			text:				params.menuName	
		});

		MFT.AppRightMenuView.scrollWrapper.scroller.get('childViews').pushObject(button);
		MFT.AppRightMenuView.scroll.refresh();
	},

	DeleteCommand: function(commandId){

		//parentID
		//position
		//menuName

		if(Ember.View.views['media_rightmenu_softButton' + commandId]){
			Ember.View.views['media_rightmenu_softButton' + commandId].destroy();
		}
		MFT.AppRightMenuView.scroll.refresh();
	},

	scrollWrapper: Em.ContainerView.create({

		elementId:	'media_appScrollWrapper',
		
		classNames:	'right-stock',

		childViews: [
			'scroller'
		],

		scroller: Em.ContainerView.create({
			
			elementId:	'appScroller',

		})
	}),
	
	buttonsControl: function(){
		if(this.position >= 0){
			this.set('scrollTopPos', false);
		}else{
			this.set('scrollTopPos', true);
		}
		if(this.position <= (MFT.AppRightMenuView.scroll.wrapperH - MFT.AppRightMenuView.scroll.scrollerH)){
			this.set('scrollBottomPos', false);
		}else{
			this.set('scrollBottomPos', true);
		}
	}.observes('this.position'),

	scrollRefresh: function(){
		setTimeout(function () {
			MFT.AppRightMenuView.scroll.refresh();
		}, 0);
	}.observes('MFT.AppModel.active'),
	
	scrollEnd:	function(){
		this.set('position', MFT.AppRightMenuView.scroll.y);
	},

	loaded: function() {
		this.set('scroll', new iScroll('media_appScrollWrapper',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.AppRightMenuView.scrollEnd();}}));
		button = MFT.Button.create({
			elementId:		'media_rightmenu_optionButton',
			classNames:		['rs-item'],
			icon:			'images/media/active_arrow.png',
			text:			'Options',
			action:			'turnOnAppOptions',
			target:			'MFT.MediaController'
		});

		MFT.AppRightMenuView.scrollWrapper.scroller.get('childViews').pushObject(button);
		MFT.AppRightMenuView.scroll.refresh();
	},

	afterRender: function() {
		setTimeout(function () { MFT.AppRightMenuView.loaded(); }, 200);
	}
});