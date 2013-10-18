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
	
	/** Define module template */
	template: Ember.Handlebars.compile(
		'{{#unless FFW.Backend.isNavigationEnabled}}'+
			'<div id="home_non_nav"></div>'+
		'{{/unless}}'+
		'{{#if FFW.Backend.isNavigationEnabled}}'+
			'<div id="home_screen_arrow"></div>'+
			'<div id="home_main_mi">{{MFT.NavigationController.model.distance_left.value}}{{MFT.NavigationController.model.distance_left.shortLable}}</div>'+
			'<div id="home_main_st">{{MFT.locale.label.view_navigation_street}}</div>'+
		'{{/if}}'
	),

    /**
     * Select navigation state
     * should be called when user press home navigation block
     */
    actionUp: function(){
        if (MFT.helpMode) return false;

        if (FFW.Backend.get('isNavigationEnabled')) {
            MFT.States.goToState(MFT.NavigationController.activeState);
        } else {
            MFT.States.goToState(MFT.InfoController.activeState);
        }
    }
});