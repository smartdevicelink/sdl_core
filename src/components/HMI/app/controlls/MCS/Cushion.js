/**
 * @name MFT.Indicator
 * 
 * @desc Universal button component with value indicator for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/IndButton.js
 * @version		2.0
 *
 * @author	Artem Petrosyan
 */

MFT.Cushion = Em.ContainerView.extend({
		
	value: 0,
	range: 0,
	
	seat: null,
	
	childViews:[
		'active',
		'body'
	],
					
	active: Em.View.extend({
		
		classNameBindings: [
			'parentView.isActive:active',
		]
		
	}),
	
	body: Em.View.extend({
		
		classNames:			'lumbar',
		
		attributeBindings:	['style','class']

	}),

	beforeRender: function() {
		
		var self = this;
				
		// add observer to content record
		this.addObserver('value', function() {
			self.toggleCushion();
		});
		
		// view internal call
		this.applyAttributesToBuffer(this.buffer);
	},
	
	// Toggle indicator handeler 
	toggleCushion: function() {
		
		if ( FLAGS.MCS_BLADDERS_SYNC ) {
		
			//if ( this.seat == MFT.SettingsController.seat ) {
				this.body.set( 'style', 'opacity:'+this.value/this.range+';' );
			//}	
		}	
	}
});

