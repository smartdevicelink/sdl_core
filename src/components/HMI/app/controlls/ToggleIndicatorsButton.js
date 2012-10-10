/**
 * @name MFT.ToggleIndicatorsButton
 * 
 * @desc Set active only to current seleted indicator
 * 
 * @category	Controlls
 * @filesource	app/Controlls/ToggleIndicatorsButton.js
 * @version	2.0
 *
 * @author	Igor Zhavoronkin
 */             


MFT.ToggleIndicatorsButton = MFT.IndButton.extend({

	// Toggle indicator handeler 
	toggleIndicators: function() {
		
		for (var i=0; i < this.indicators.length; i++ ) {
			if (i != this.content.value) {
				this.indicators[i].set('className', this.indDefaultClass);
			}else{
				this.indicators[i].set('className', this.indActiveClass);
			}
		}
	},
	
	/** Define button template */
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}'+
			'{{#if text}}<p>{{text}}</p>{{/if}}'+
			'{{#if enabled}}'+
				'{{#each indicators}}'+
					'<div {{bindAttr class="className"}}></div>'+
				'{{/each}}'+
			'{{/if}}'+
		'{{/with}}'
	)
});
