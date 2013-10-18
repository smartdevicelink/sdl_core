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
		
		classNameBindings: ['MFT.StartUpController.isWarningHidden:inactive_state','FLAGS.locale'],
		
		elementId: 'warning_view',
		
		childViews: [
						'content',
						'button'
					],
						
		content: Em.View.extend({
			
			classNames: 'message',

			template: Ember.Handlebars.compile(
				'<div class="warning_text">{{MFT.locale.label.view_warning}}</div>'+
				'<div class="text">'+
					'<br>'+
					'<p>{{MFT.locale.label.view_warning_paragraph1}}</p><p class="br"></p>'+
					'<p>{{MFT.locale.label.view_warning_paragraph2}}</p><p class="br"></p>'+
					'<p>{{MFT.locale.label.view_warning_paragraph3}}</p><p class="br"></p>'+
				'</div>'
		 	)
		}),
		
		button: MFT.Button.extend({
			elementId: 'warning_ok_button',
			classNameBindings:['isReady: visible_display', 'pressed:pressed'],
			classNames: ['okbut'],
			text: 'OK',
			onDown: false,
			controllerBinding: 'MFT.StartUpController',
			action: 'onWarningSubmit',
			target: 'MFT.StartUpController',
			/** 
			  * this method is called when the web part is fully loaded
			  *
			*/
			appLoaded: function() {
				var self = this;
				/** Show OK Button after 2 second delay*/
				setTimeout(function(){
					self.set('isReady', true);
				},2000);
			}.observes('MFT.appReady')
		})
});