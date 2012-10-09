/**
 * @name WarningOkButtonView
 * 
 * @desc Warning View visual representation
 * 
 * @category	View
 * @filesource	app/view/WarningOkButtonView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
	
WarningOkButtonView = Em.View.create({
		
		elementId: 'warning_ok_button',
		
		classNameBindings:['isReady: visible_display', 'pressed:pressed'],
		
		classNames: ['okbut'],

		/* this method is called when the web part is fully loaded */
		appLoaded: function() {
			var self = this;
			/** Show OK Button after 2 seconds delay*/
			setTimeout(function(){
				self.set('isReady', true);
			},2000);
		},

		actionDown: function(event) {
			this.set('pressed', true);
		},
		
		actionUp: function(event) {
			this.set('pressed', false);
			var self = this;
			$('#warning_view').addClass('fadeAnimation');
			setTimeout(function(){
				$('#warning_view').addClass('hidden');
				self.remove();
			},1800);
			
			// Show Seat Comfort screen if MCS flag is true
			if ( FLAGS.MCS_ENABLED ) {
				MFT.States.goToState('settings.vehicle.mcs');
			}
			
		}
});
		
/** Append View to body*/
WarningOkButtonView.appendTo('#warning_view');
