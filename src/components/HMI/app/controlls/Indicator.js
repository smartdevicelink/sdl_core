/**
 * @name MFT.Indicator
 * 
 * @desc Universal button component with value indicator for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/Indicator.js
 * @version		2.0
 *
 * @author	Artem Petrosyan
 */

MFT.Indicator = Em.View.extend(Ember.TargetActionSupport, {
	
	/** Content binding */
	content: null,
	
	/** internal indicators array */
	indicators: null,
	
	/** indicator active class */
	indActiveClass: null,
	
	/** indicator default class */
	indDefaultClass: null,
	
	/** Set active indicator from the start*/
	startFrom: null,
	
	/** binding property to enable/disable indicators */
	enabledBinding: 'content.enabled',
	
	/**
	 * Before rendering view handeler
	 * need to generate indicators array
	 * based on indicator range value
	 */
	beforeRender: function() {
		// define variables
		var length = this.content.range,
			view = this,
			i;
				
		// generate indicators
		this.indicators = [];
		
		for (i=0; i < length; i++) {
			this.indicators.push(Em.Object.create({
				index: i,
				className: this.indDefaultClass
			}));
		}
		// apply indicator visualization rule
		view.setRecord();
		
		if(this.startFrom){
			this.indicators[this.startFrom].set('className', this.indActiveClass);
		}
		
		// add observer to content record	
		this.addObserver('content', function() {
			view.setRecord();
		});
		
		// view internal call
		this.applyAttributesToBuffer(this.buffer);
	},
	
	actionDown: function() {
		this.triggerAction();
	},
	
	// change record binding
	setRecord:	function() {
		
		var view = this;
		
		if ( this.content.observersForKey('value').length == 0 ) {
		
			this.content.addObserver('value', function() {
				view.toggleIndicators();
			});
		
		}
		
		this.toggleIndicators();
	},
	
	// Toggle indicator handeler 
	toggleIndicators: function() {
				
		var length = this.indicators.length,
			i;
			
		for ( i=0; i < length; i++ ) {
			if ( i >= this.content.value ) {
				this.indicators[i].set('className', 'mft_indicator ' + this.indDefaultClass);
			} else {
				this.indicators[i].set('className', 'mft_indicator ' + this.indActiveClass);
			}
		}
	},
	
	/** Define indicator template */
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#each indicators}}'+
				'<div {{bindAttr class="className view.enabled:show"}}></div>'+
			'{{/each}}'+
		'{{/with}}'
	)	
	
});

