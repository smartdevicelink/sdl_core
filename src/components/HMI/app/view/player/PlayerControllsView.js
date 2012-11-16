/**
 * @name MFT.PlayerControllsView
 * 
 * @desc Video player visual representation
 * 
 * @category	View
 * @filesource	app/view/player/PlayerView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.PlayerControllsView = Em.ContainerView.create({
	/** View Id*/
	elementId: 			'video_player_view',
	/** Visual display of player controlls */
	classNameBindings: ["isHidden: hidden", "MFT.VideoPlayerController.disableControlls:hidden_display"],
	/** By default controls are hidden*/
	isHidden: 			true,
	/** Controller binding */
	controllerBinding: Ember.Binding.oneWay('MFT.VideoPlayerController'),
	/** Child Views*/
	childViews: [
		'time',
		'duration',
		'progressbar',
		'play',
		'frw',
		'rew',
		'stop'
	],

	/** Current Time Lable */
	time: MFT.Label.extend({
			elementId: "player_controls_curentTime",
			classNames: "current_time",
			contentBinding: Ember.Binding.oneWay("parentView.controller.currentPlayBackTime"),
	}),
	/** Total Time Lable */
	duration: MFT.Label.extend({
			elementId: "player_controls_totalTime",
			classNames: 'total_time',
			contentBinding: Ember.Binding.oneWay('parentView.controller.totalDuration'),
	}),
	/** Play button */
	play: MFT.Button.extend({
		elementId: "play",
		icon:	"images/player/pause.png",
		actionDown: function(){
			if(MFT.VideoPlayerController.model.isPaused ){ 
				MFT.VideoPlayerController.play();
			}else{
				MFT.VideoPlayerController.pause();
			}
			this._super();
		},
		// toggle play/pause icon
		togglePlayPauseIcon: function(){
			
			if(MFT.VideoPlayerController.model.isPaused) {
				this.set('icon','images/player/play.png');
			}else{
				this.set('icon','images/player/pause.png');
			}
		}.observes('parentView.controller.model.isPaused')
		
	}),
	/** FF button */
	frw: MFT.Button.extend({
		elementId: "frw",
		icon:	"images/player/frd_ico.png",
		target: "MFT.VideoPlayerController",
		action: "FF",
	}),
	/** REW button */
	rew: MFT.Button.extend({
		elementId: "back",
		icon:	"images/player/rewind_ico.png",
		target: "MFT.VideoPlayerController",
		action: "REW",
	}),
	/** STOP button */
	stop: MFT.Button.extend({
		elementId: "stop",
		icon:	"images/player/done_ico.png",
		target: "MFT.VideoPlayerController",
		action: "stop",
		onDown: false
	}),
	/** Progressbar component */
	progressbar: Em.ContainerView.extend({
		
		elementId: 'player_controlls_progressbar',
		
		classNames: 'progressline_holder',
		
		childViews: [
			'dragArea'
		],
		
		layout: Ember.Handlebars.compile(
			'<div id="glide" class="progressline_width_fix">'+
				'{{yield}}'+
			'</div>'
		
		),
		
		dragArea: Em.ContainerView.extend({
			
			elementId: 'player_controlls_progressbar_dragarea',
			
			classNames: 'drag_area',
			
			childViews: [
				'progressbarline',
				'mark'
			],
			
			progressbarline: Em.View.extend({
				elementId: 'player_controlls_progressbarline',
				classNames: 'progressbar',
				attributeBindings: ['style'],
				styleBinding: Ember.Binding.oneWay('parentView.parentView.parentView.controller.progressBarStyle')
			
			}),
			
			mark: Em.View.extend({
				elementId: 'player_controlls_progressbar_mark',
				elementId: 'mark',
				classNameBindings: 'active:active',
				actionDown: function(){
					this.set('active', true);
				},
				actionUp: function(){
					this.set('active', false);
				}
			}),
			
			actionDown: function(){
				MFT.VideoPlayerController.tap();
			}
		})
	})

});
