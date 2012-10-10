/**
 * @name MFT.TableLabel
 * 
 * @desc Universal TableLabel component for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/TableLabel.js
 * @version		2.0
 *
 * @author		Gerashchenko Maksym
 */

MFT.TableLabel = Em.View.extend({
	classNameBindings: ['disabled'],
	/** Define button template */
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}'+
			'<span class="content">  {{content}} </span>'+
			'{{#if rightIcon}}<img class="right-icon" {{bindAttr src="rightIcon"}} />{{/if}}'+
			'{{#if contentSecond}} <span class="content-second">  {{contentSecond}} </span> {{/if}}'+
			'{{#if contentThird}}  <span class="content-third">   {{contentThird}}  </span> {{/if}}'+
		'{{/with}}'
	)
});