/**
 * @name MFT.WidgetNavigationView
 * 
 * @desc navigation home screen widget visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/WidgetNavigationView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.WidgetNavigationView = Em.View.extend({
	/** View Id */
	elementId:	'widget_navigation',
	
	classNameBindings: [
		'MFT.helpMode:helpmode_blur_text'
	],
	
	/** Define module template */
	template: Ember.Handlebars.compile(
		'{{#unless FFW.Backend.isNavigationEnabled}}'+
			'<div id="home_non_nav" {{bindAttr class="MFT.helpMode:help"}}></div>'+
		'{{/unless}}'+
		'{{#if FFW.Backend.isNavigationEnabled}}'+
			'<div id="home_screen_arrow" {{bindAttr class="MFT.helpMode:help"}}></div>'+
			'<div id="home_main_mi">{{MFT.NavigationController.model.distance_left.value}}{{MFT.NavigationController.model.distance_left.shortLable}}</div>'+
			'<div id="home_main_st">Main St</div>'+
		'{{/if}}'
	)
});