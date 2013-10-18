/**
 * @name MFT.StatusNavigationView
 * 
 * @desc Component for Navigation Status bar
 * 
 * @category	view
 * @filesource	app/view/navigation/StatusNavigationView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.StatusNavigationView = Em.ContainerView.extend({
	/** View Id */
	elementId:	'status_nav',
	
	classNames: ['status_bar','hidden_nav'],

	classNameBindings: [
		'MFT.StartUpController.isAllStatusHidden:hidden',
		'FFW.Backend.isNavigationEnabled:active_nav',
		'MFT.States.navigation.active:selected'
	],
	
	/** Navigation components */		
	childViews: [
		'statusOn',
		'statusHome'
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.create({
		elementId:		'navigation_status_label',
		classNameBindings: ['MFT.States.home.active:visible'],
		contentBinding:	    Ember.Binding.oneWay('MFT.locale.label.view_statusNavigation_navigation')
	}),
	
	/** On status view */
	statusOn: Em.View.create({
		classNameBindings: ['MFT.States.home.active:hidden_display'],
		elementId:		'navigation_status_on',
		template: Em.Handlebars.compile(
			'<div class="nav_distance">{{MFT.NavigationController.model.distance_left.value}} {{MFT.NavigationController.model.distance_left.longLable}}</div>'+
			'<img class="ico" src="images/nav/ico_nav_statusbar.png" />'
		)
	}),
	
	actionUp: function(event){
		MFT.States.goToState(MFT.NavigationController.activeState);
	}
});