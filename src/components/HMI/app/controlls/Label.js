/**
 * @name MFT.Label
 * 
 * @desc Universal label component for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/Label.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.Label = Em.View.extend({
	classNameBindings: ['disabled'],
	classNames: 'label',
	
	// component default template
	defaultTemplate: Em.Handlebars.compile(
		'<span>{{view.content}}</span>'
	),
	
	templates: {
		icon: Em.Handlebars.compile(
			'<img class="ico" {{bindAttr src="view.icon"}} />'+
			'<span>{{view.content}}</span>'
		)
	}
	
	/** Define button template */
	/*
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}'+
			'<span>{{content}}</span>'+
		'{{/with}}'
	)
	*/
});