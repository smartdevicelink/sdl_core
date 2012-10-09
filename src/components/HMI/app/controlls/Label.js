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
	/** Define button template */
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}'+
			'<span>{{content}}</span>'+
		'{{/with}}'
	)
});