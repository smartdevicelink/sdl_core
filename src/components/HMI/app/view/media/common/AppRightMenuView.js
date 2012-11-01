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
		'MFT.AppModel.active:visible_display'
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
			action:				null,
			target:				null,
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

		Ember.View.views['media_rightmenu_softButton' + commandId].destroy();
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

			childViews: [
				/*'optionsButton',
				'browseButton',
				'hdButton',
				'repeatButton',
				'shuffleButton',
				'replayButton',
				'scanButton',
				'moreInfoButton',
				'directTuneButton',
				'help_yellow_borders',
				'help_yellow_dividers'*/
			],

			/**  Help Mode Yellow Borders*/
			help_yellow_borders:Em.View.extend({
				elementId: 'yellow_borders',
				classNameBindings: 'MFT.helpMode:active_state',
				classNames: 'hidden',
				/** Define button template */
					template: Ember.Handlebars.compile(
						'{{#with view}}'+
							'<div id="media_rightmenu_yellow_amfm_borders" {{bindAttr class="MFT.AmModel.active:visible_display MFT.FmModel.active:visible_display"}}>'+
								'<div id="amfmscan"></div>'+
								'<div id="amfmdirect"></div>'+
							'</div>'+
							'<div id="media_rightmenu_yellow_sir_borders" {{bindAttr class="MFT.SiriusModel.active:visible_display"}}>'+
								'<div id="uplist_borders"></div>'+
								'<div id="bottomlist_borders"></div>'+
							'</div>'+
							'<div id="media_rightmenu_yellow_cd_borders" {{bindAttr class="MFT.CDModel.active:visible_display MFT.SDModel.active:visible_display MFT.USBModel.active:visible_display"}}>'+
								'<div id="triple_borders"></div>'+
								'<div id="single_border"></div>'+
							'</div>'+
						'{{/with}}'
					)
			}),
			/**  Help Mode Yellow Dividers*/
			help_yellow_dividers: Em.View.extend({
					elementId: 'yellow_dividers',
					classNameBindings: 'MFT.helpMode:active_state',
					classNames: 'hidden',
					template: Ember.Handlebars.compile(
						'{{#with view}}'+
							'<div id="media_rightmenu_yellow_sir_dividers" {{bindAttr class="MFT.SiriusModel.active:visible_display"}}>'+
								'<div class="yellow_divider top"></div>'+
								'<div class="yellow_divider bottom"></div>'+
							'</div>'+
							'<div id="media_rightmenu_yellow_cd_dividers" {{bindAttr class="MFT.States.media.player.active:visible_display"}}>'+
								'<div class="yellow_divider cdtop"></div>'+
								'<div class="yellow_divider cdbottom"></div>'+
							'</div>'+
						'{{/with}}'
					)
			}),

			optionsButton: MFT.Button.extend({
				elementId:	'media_rightmenu_optionButton',
				//classNameBindings: [],
				classNames:			['rs-item'],
				icon:				'images/media/active_arrow.png',
				text:				'Options',
				action:				'turnOnOptions',
				target:				'MFT.MediaController',
				
				disabled: function(){
					return ( MFT.SDModel.active || MFT.helpMode )
				}.property('MFT.SDModel.active','MFT.helpMode'),
				
				// Define button template
				template: Ember.Handlebars.compile(
					'{{#with view}}'+
						'{{#unless MFT.helpMode}}'+
							'<img class="ico" {{bindAttr src="icon"}} />'+
							'<span>{{text}}</span>'+
						'{{/unless}}'+
					'{{/with}}'
				)
			}),

			browseButton: MFT.Button.extend({
				//classNameBindings: 	[],
				elementId:			'media_rightmenu_browseButton',
				action:				'turnBrowse',
				target:				'MFT.MediaController',
				classNames:			['rs-item'],
				icon:				'images/media/active_arrow.png',
				text:				'Browse',
				onDown: false,
			}),

			hdButton:  MFT.Button.extend({
				elementId: 'media_am-fm_hdButton',
				//classNameBindings: 	[],
										
				classNames:			['rs-item','hdButton'],
				
				isHDhelpModeActive: function() {
					return (MFT.MediaController.currentModuleData.get('selectedItem').isHd  && MFT.helpMode);
				}.property('MFT.MediaController.currentModuleData.selectedItem.isHd','MFT.helpMode'),
				
				isHDActive: function(){
					if(!MFT.MediaController.directTuneSelected){
						return (MFT.MediaController.currentModuleData.get('selectedItem').isHd);
					}else{
						return (MFT.MediaController.currentModuleData.get('selectedDirectItem').isHd);
					}	
				}.property('MFT.MediaController.currentModuleData.selectedItem', 'MFT.MediaController.directTuneSelected'),
				
				text:				'{SoftButton[0]}',
				
				icon:				'images/media/passiv_horiz_led.png',				
				
				disabled:			true,
				/** Define button template */
				template: Ember.Handlebars.compile(
					'{{#with view}}'+
							'<div class="ico"></div>'+
							'<span>{{text}}</span>'+
					'{{/with}}'
				)
			}),
			repeatButton:  MFT.Button.extend({
				//classNameBindings: 	[],
				elementId: 'media_rightmenu_repeatButton',
				classNames:			['rs-item'],
				icon:				'images/media/passiv_horiz_led.png',
				text:				'{SoftButton[1]}',
				disabled:		true,
				/** Define button template */
				template: Ember.Handlebars.compile(
					'{{#with view}}'+
						'{{#unless MFT.helpMode}}'+
							'<img class="ico" {{bindAttr src="icon"}} />'+
							'<span>{{text}}</span>'+
						'{{/unless}}'+
					'{{/with}}'
				)
			}),
			shuffleButton:  MFT.Button.extend({
				//classNameBindings: 	[],
				elementId:		'media_rightmenu_shuffleButton',
				classNames:		['rs-item'],
				icon:			'images/media/passiv_horiz_led.png',
				text:			'{SoftButton[2]}',
				target:			'MFT.MediaController',
				action:			'turnShuffleHelpVideoOn',
				onDown: false,
				disabled:		true,
				helpModel: function(){
					(MFT.helpMode)? this.set('disabled',false): this.set('disabled',true);
				}.observes('MFT.helpMode')
			}),

			replayButton:  MFT.Button.extend({
				//classNameBindings: 	[],
				elementId:		'media_rightmenu_replayButton',
				classNames:		['rs-item'],
				icon:			'images/media/passiv_horiz_led.png',
				text:			'{SoftButton[3]}',
				target:			'MFT.MediaController',
				action:			'turnReplayHelpVideoOn',
				disabled:		true,
				onDown: false,
				helpModel: function(){
					(MFT.helpMode)? this.set('disabled',false): this.set('disabled',true);
				}.observes('MFT.helpMode')
			}),
			scanButton:  MFT.Button.extend({
				elementId:		'media_rightmenu_scanButton',
				//classNameBindings: [],
				classNames:		['rs-item'],
				icon:			'images/media/passiv_horiz_led.png',
				target:			'MFT.MediaController',
				action:			'turnScanHelpVideoOn',
				disabled:		true,
				onDown:			false,
				text:			'{SoftButton[4]}',
				
				helpModel: function(){
					(MFT.helpMode)? this.set('disabled',false): this.set('disabled',true);
				}.observes('MFT.helpMode')
			}),
			moreInfoButton: MFT.Button.extend({
				//classNameBindings: 	[],
				elementId:			'media_rightmenu_moreinfoButton',
				action:			    'turnMoreInfo',
				target:				'MFT.MediaController',
				classNames:			['rs-item'],
				icon:				'images/media/active_arrow.png',
				text:				'{SoftButton[5]}'
			}),
			directTuneButton: MFT.Button.extend({
				//classNameBindings: 	[],	
				elementId:	'media_rightmenu_directButton',
				classNames:	['rs-item'],
				target:		'MFT.MediaController',
				action:		'onDirectTune',
				onDown:		false,
				text:		'{SoftButton[6]}'
			})
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
	},

	afterRender: function() {
		setTimeout(function () { MFT.AppRightMenuView.loaded(); }, 200);
	}
});