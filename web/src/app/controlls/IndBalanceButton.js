/**
 * @name MFT.IndBalanceButton
 * 
 * @desc Universal button component with balance value indicator for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/IndBalanceButton.js
 * @version		2.0
 *
 * @author	Sergei Polishchuk
 */

MFT.IndBalanceButton = MFT.Button.extend({
		
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
        this.indicators = {
            negative: [],
            positive: []
        };

        for (i=length; i >= 1; i--) {
            this.indicators.negative.push(Em.Object.create({
                index: -i,
                idName: this.elementId + '_neg' + i,
                className: this.indDefaultClass
            }));
        };

        for (i=1; i <= length; i++) {
            this.indicators.positive.push(Em.Object.create({
                index: i,
                idName: this.elementId + '_pos' + i,
                className: this.indDefaultClass
            }));
        };

        // apply indicator visualization rule
		view.toggleIndicators();

		if(this.startFrom){
            if (this.startFrom > 0) {
			    this.indicators.positive[this.startFrom].set('className', this.indActiveClass);
            } else {
                this.indicators.negative[this.startFrom].set('className', this.indActiveClass);
            }
		}

		// add observer to content record	
		this.addObserver('content', function() {
			view.setRecord();
		});

		// add observer to content record
		this.content.addObserver('value', function() {
			view.toggleIndicators();
		});
		
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

        var length = this.content.range,
            i;

        for ( i=1; i <= length; i++ ) {
            if ( -i >= this.content.value ) {
                this.indicators.negative[length - i].set('className', 'mft_indicator ' + this.indActiveClass);
            } else {
                this.indicators.negative[length - i].set('className', 'mft_indicator ' + this.indDefaultClass);
            }

            if ( i <= this.content.value ) {
                this.indicators.positive[i-1].set('className', 'mft_indicator ' + this.indActiveClass);
            } else {
                this.indicators.positive[i-1].set('className', 'mft_indicator ' + this.indDefaultClass);
            }
        }
    },
	
	// component default template
	defaultTemplate: Em.Handlebars.compile(
        '<span>{{view.text}}</span>'+
            '{{#each view.indicators.negative}}'+
            '<div id="{{unbound idName}}" {{bindAttr class="className view.enabled:show"}}></div>'+
            '{{/each}}' +
            '<span class="counter">{{view.content.value}}</span>' +
            '{{#each view.indicators.positive}}'+
            '<div id="{{unbound idName}}" {{bindAttr class="className view.enabled:show"}}></div>'+
            '{{/each}}' +
            '<img class="ico" {{bindAttr src="view.icon"}} />'
    )
});

