/**
 * @name MFT.IndButton
 * 
 * @desc Universal button component with value indicator for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/IndButton.js
 * @version		2.0
 *
 * @author	Artem Petrosyan
 */

MFT.IndButton = MFT.Button.extend({
		
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
	 * based on button range value
	 */
	beforeRender: function() {
		// define variables
		var length = this.content.range,
			view = this,
			i;
				
		// generate indicators
		this.indicators = [];
		
		for (i=1; i <= length; i++) {
			this.indicators.push(Em.Object.create({
				index: i,
				idName: this.elementId + '_' + i,
				className: this.indDefaultClass // this.content.value >= i ? this.indActiveClass : this.indDefaultClass
			}));
		}
		// apply indicator visualization rule
		view.toggleIndicators();

		if(this.startFrom){
			this.indicators[this.startFrom].set('className', this.indActiveClass);
		}

		// add observer to content record	
		this.addObserver('content', function() {
			view.setRecord();
		});

		// add observer to content record
		this.content.addObserver('value', function() {
			view.toggleIndicators();
		});

		// add observer to reset indicators state in Help mode
		/*
		if ( this.helpMode ) {
			MFT.addObserver('helpMode', function() {
				view.toggleIndicators();
			});
		}
		*/
		
		// view internal call
		this.applyAttributesToBuffer(this.buffer);
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
	
	// component default template
	defaultTemplate: Em.Handlebars.compile(
		'<img class="ico" {{bindAttr src="view.icon"}} />'+
		'<span>{{view.text}}</span>'+
		'{{#each view.indicators}}'+
			'<div id="{{unbound idName}}" {{bindAttr class="className view.enabled:show"}}></div>'+
		'{{/each}}'
	)
	
	/** Define button template */
	/*
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}'+
			'{{#if text}}{{text}}{{/if}}'+
			'{{#each indicators}}'+
				'<div id="{{unbound idName}}" {{bindAttr class="className view.enabled:show"}}></div>'+
			'{{/each}}'+
		'{{/with}}'
	)
	*/
});

