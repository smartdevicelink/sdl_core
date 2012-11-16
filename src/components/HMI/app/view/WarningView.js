/**
 * @name MFT.WarningViewg
 * 
 * @desc Warning representation
 * 
 * @category	View
 * @filesource	app/view/WarningView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.warningView = Em.ContainerView.extend({
		
		classNameBindings: ['fade:fadeAnimation:fadeWarning','hide:inactive_state','MFT.localization'],
		
		elementId: 'warning_view',
		
		childViews: [
						'content',
						'button'
					],
						
		content: Em.View.extend({
			
			classNames: 'message',

			template: Ember.Handlebars.compile(
				'<div class="warning_text"> {{MFT.locale.label.view_warning}}</div>'+
				'<div class="text">'+
					'<br>'+
					'<p>{{MFT.locale.label.view_warning_paragraph1}} </p><br>'+
					'<p> {{MFT.locale.label.view_warning_paragraph2}} </p><br>'+
				'</div>'
		 	)
		}),
		
		button: Em.View.create({
		
			elementId: 'warning_ok_button',
			
			classNameBindings:['isReady: visible_display', 'pressed:pressed'],
			
			classNames: ['okbut','ffw-button'],
			/** Check for webkit fillmode animation support 
				Android < 4 version doesnt support webkit animation fillmode 
			*/
			checkForCCS3AnimationSupport: function(){
				if (FFW.isAndroid) {
					return $('body')[0].style.webkitAnimationFillMode === '';
				} else {
					return false;
				}
			},
			
			template: Ember.Handlebars.compile(
				'<span>OK</span>'
			),
	
			/* this method is called when the web part is fully loaded */
			appLoaded: function() {
				var self = this;
				/** Show OK Button after 2 second delay*/
				setTimeout(function(){
					self.set('isReady', true);
				},2000);
			}.observes('MFT.appReady'),
	
			actionDown: function(event) {
				this.set('pressed', true);
			},
			
			actionUp: function(event) {
				this.set('pressed', false);
				
				var self = this;
				
				this._parentView.set('fade',this.checkForCCS3AnimationSupport());
				
				setTimeout(function(){
					self._parentView.set('hide',true);
				},1000);
			}
		})
});