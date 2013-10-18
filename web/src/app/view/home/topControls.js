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
 
MFT.TopControls = Em.ContainerView.extend({
	
	elementId:			'app_top_menu_cont',
	
	classNameBindings:['MFT.StartUpController.isAllStatusHidden:hidden'],
	
	childViews: [
		'controls',
		'toggleFAQButton',
        'toggleAppSettingsButton'
	],
	
	controls: Em.ContainerView.extend({
		elementId:		'top_controls',
		
		
		childViews: [
			'clock',
			'vSeparator',
			'toggleHelpButton'
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
			
			template: Em.Handlebars.compile('<div id="time_num"> {{#if MFT.States.settings.clock.active }} --:-- {{else}}{{MFT.SettingsController.time}}{{/if}} {{MFT.SettingsController.temperature}}</div><div id="time-skew-btn"></div>'),
			
			actionDown: function(event) {
				// Click handeler
				MFT.States.goToState('settings.clock');
				
				if ( MFT.States.settings.clock.viewLoaded) {
					if ( MFT.helpMode ) {
				        MFT.States.settings.clock.view.listClockSettings.beforeHelpmodePage = 0;
					} else {
				        MFT.States.settings.clock.view.listClockSettings.set('currentPage',0);
					}
				} else {
					MFT.States.settings.clock.addObserver('viewLoaded', function(){
						if ( !MFT.helpMode ) {
							this.view.listClockSettings.set('currentPage',0);
						}						
					});
				}
				
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
				
				/** Close FAQ state or App Settings state if they are active */
				if( MFT.States.faq.active || MFT.States.appSettings.active) {
                    if(MFT.States.faq.from){
                        MFT.States.goToState(MFT.States.faq.from);
                    }
					else{
                        MFT.States.goToState(MFT.States.appSettings.from);
                    }
                    /** Reset from property */
                    MFT.States.faq.from = MFT.States.appSettings.from = null;
				}
				
				this._super();
			}
		})
	}),

	toggleFAQButton: MFT.Button.extend({
		elementId:			'faq_btn',
		
		// for helpmode
		classNames:			['faq_show'],
		
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
	}),

    toggleAppSettingsButton: MFT.Button.extend({
        elementId:			'app-settings-button',

        activeBinding:		Em.Binding.oneWay('MFT.States.appSettings.active'),

        classNameBindings: 'MFT.States.appSettings.active:active',

        template: Em.Handlebars.compile(
            '<div class="btn-3ind">' +
                '<div></div><div></div><div></div>' +
            '</div>' +
            '<div id="app-btn-skew"></div>'
        ),

        actionDown: function() {
            if (MFT.States.appSettings.active) {
                MFT.States.goToState(MFT.States.appSettings.from);
            } else {
                MFT.States.goToState(MFT.AppSettingsController.activeState);
            }
            this._super();
        }
    })
});