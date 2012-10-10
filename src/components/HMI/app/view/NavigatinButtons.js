/**
 * @name MFT.ClimateView
 * 
 * @desc Climate module visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/ClimateView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.NavigatinButtons = Em.ContainerView.extend({
	
	elementId:			'NavigatinButtons',
	
	childViews: [
		'controls'
	],
	
	controls: Em.ContainerView.extend({

		elementId:		'top_controls',

		childViews: [
			'clock',
			'vSeparator',
			'toggleHelpButton',
		],

		clock: Em.View.extend({
			elementId:			'clock',

			classNameBindings: ['FLAGS.MCS_ENABLED:mcs'],

			afterRender: function() {
				MFT.SettingsController.getTime(new Date);
												
				setInterval(function(){
					MFT.SettingsController.getTime(new Date);
				},60000);
			},
			
			template: Em.Handlebars.compile('<div id="time_num">{{MFT.SettingsController.time}}</div>'),
			
			actionDown: function(event) {
				// Click handeler
				MFT.SettingsClockView.listClockSettings.set('currentPage',1);
				MFT.States.goToState('settings.clock');				
			}
		}),
		
		vSeparator: Em.View.extend({
			elementId:	'top_controls_separator',
			
			classNameBindings: ['FLAGS.MCS_ENABLED:hidden'],
			
			classNames: 'help_dev'
		}),
		
		toggleHelpButton: MFT.Button.extend({
			elementId:			'help',
			
			indClassBinding:	Em.Binding.oneWay('MFT.helpMode'),
			
			hidden:				FLAGS.MCS_ENABLED,
					
			template: Em.Handlebars.compile(
				'<div class="inact" {{bindAttr class="view.indClass:act"}}></div>'+
				'<div id="sing" class="white" {{bindAttr class="view.indClass:yellow"}}></div>'+
				'<div id="help-p"></div>'
			),
			
			/** Toggle Help mode */
			actionDown: function(event) {
				if ( FLAGS.MCS_ENABLED ) {
					return;
				}
			
				MFT.toggleProperty('helpMode');
				
				/** Switch off video player if active */
				if(MFT.VideoPlayerController.model.isReady) MFT.VideoPlayerController.stop(); 
				
				/** Toggle FAQ button animation */
				$('#faq_btn').animate({
					left: MFT.helpMode ? 182: 110,
				},{ 
					duration: 500, 
					queue: false 
				});
				
				/** Close FAQ state if active */
				if( MFT.States.faq.active ) {
					MFT.States.goToState(MFT.States.faq.from);
				}
				
				this._super();
			}
		})
	}),

	toggleFAQButton: MFT.Button.extend({
		elementId:			'faq_btn',
		activeBinding:		Em.Binding.oneWay('MFT.States.faq.active'),
			
		template: Ember.Handlebars.compile(
			'<div id="faq_btn_c"></div>'+
			'<div class="ind_inact" {{bindAttr class="view.active:ind_act"}}></div>'
		),
		
		actionDown: function() {
			if (MFT.States.faq.active) {
				MFT.States.goToState(MFT.States.faq.from);
			} else {
				MFT.States.goToState('faq');
			}
			
			this._super();
		}
	})
});