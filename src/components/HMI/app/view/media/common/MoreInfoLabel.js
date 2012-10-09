/**
 * @name MFT.MediaLabel
 * 
 * @desc Universal label component for Media module
 * 
 * @category	vies
 * @filesource	app/view/media/common/MoreInfoLabel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.MoreInfoLabel = Em.View.extend({
	/** Define button template */
	template: Ember.Handlebars.compile('{{#if view.icon}}<img class="ico" {{bindAttr src="view.icon"}} />{{/if}}'+'<span>{{view.content}}</span>')
});