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
MFT.ApplinkSoftButtons = Em.ContainerView.create({
	/** View ID */
	elementId:	'media_applink_soft_buttons_view',
	/** Class Names bindings for visual representation*/
	classNameBindings: 	[
		'MFT.ApplinkModel.active:visible_display'
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
	classNames: ['media_applink_soft_buttons_view hidden_display'],
	/** View Components*/
	
	childViews: [
		'topScrollButton',
		'scrollWrapper',
		'bottomScrollButton'
	],

	topScrollButton: MFT.Button.extend({
		classNameBindings: 	[
			'parentView.scrollTopPos:visible_display'
		],
		elementId:	'media_rightmenu_topButton_applinkSoftButtons',
		classNames:	['media_rightmenu_topButton', 'hidden_display'],
		action:		function(){
			MFT.ApplinkSoftButtons.scroll.scrollTo(0, MFT.ApplinkSoftButtons.scroll.y + 50, 200);
			MFT.ApplinkSoftButtons.set('position', MFT.ApplinkSoftButtons.scroll.y + 50);
		},
		onDown:		false,
		text:		'top'
	}),
	
	bottomScrollButton: MFT.Button.extend({
		classNameBindings: 	[
			'parentView.scrollBottomPos:visible_display'
		],
		elementId:	'media_rightmenu_bottomButton_applinkSoftButtons',
		classNames:	['media_rightmenu_bottomButton', 'hidden_display'],
		action:		function(){
			MFT.ApplinkSoftButtons.scroll.scrollTo(0, MFT.ApplinkSoftButtons.scroll.y - 50, 200);
			MFT.ApplinkSoftButtons.set('position', MFT.ApplinkSoftButtons.scroll.y - 50);
		},
		onDown:		false,
		text:		'bottom'
	}),

	AddCommand: function( commandId, params){

		button = MFT.Button.create({
			elementId:			'media_rightmenu_softButton' + commandId,
			action:             'onCommand',
            target:             'FFW.UI',
			commandId:			commandId, 
			classNames:			['rs-item'],
			text:				params.menuName,
            templateName:       'text'
		});

		MFT.ApplinkSoftButtons.scrollWrapper.scroller.get('childViews').pushObject(button);
		MFT.ApplinkSoftButtons.scroll.refresh();
	},

	DeleteCommand: function(commandId){

		if(Ember.View.views['media_rightmenu_softButton' + commandId]){
			Ember.View.views['media_rightmenu_softButton' + commandId].destroy();
		}
		MFT.ApplinkSoftButtons.scroll.refresh();
	},

	scrollWrapper: Em.ContainerView.create({

		elementId:	'media_appScrollWrapper_applinkSoftButtons',
		
		classNames:	'right-stock',

		childViews: [
			'scroller'
		],

		scroller: Em.ContainerView.create({
			
			elementId:	'appScroller_applinkSoftButtons',

		})
	}),
	
	buttonsControl: function(){
		if(this.position >= 0){
			this.set('scrollTopPos', false);
		}else{
			this.set('scrollTopPos', true);
		}
		if(this.position <= (MFT.ApplinkSoftButtons.scroll.wrapperH - MFT.ApplinkSoftButtons.scroll.scrollerH)){
			this.set('scrollBottomPos', false);
		}else{
			this.set('scrollBottomPos', true);
		}
	}.observes('this.position'),

	scrollRefresh: function(){
		setTimeout(function () {
			MFT.ApplinkSoftButtons.scroll.refresh();
		}, 0);
	}.observes('MFT.AppModel.active'),
	
	scrollEnd:	function(){
		this.set('position', MFT.ApplinkSoftButtons.scroll.y);
	},

	loaded: function() {
		MFT.ApplinkSoftButtons.set('scroll', new iScroll('media_appScrollWrapper_applinkSoftButtons',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.ApplinkSoftButtons.scrollEnd();}}));
		button = MFT.Button.create({
			elementId:		'media_rightmenu_optionButton_applinkSoftButtons',
			classNames:		['rs-item'],
			icon:			'images/media/active_arrow.png',
			text:			'Options',
			action:			'turnOnApplinkOptions',
			target:			'MFT.MediaController'
		});

		MFT.ApplinkSoftButtons.scrollWrapper.scroller.get('childViews').pushObject(button);
		MFT.ApplinkSoftButtons.scroll.refresh();
	},

	afterRender: function() {
		setTimeout(function () { MFT.ApplinkSoftButtons.loaded(); }, 200);
	}
});