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
	
	classNames: ['status_bar'],

	classNameBindings: ['MFT.States.navigation.active:selected'],
	
	/** Navigation components */		
	childViews: [
		'statusOn',
		'statusHome'
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.create({
		elementId:		'navigation_status_label',
		classNameBindings: ['MFT.States.home.active:visible'],
		content:		'Navigation'
	}),
	
	/** On status view */
	statusOn: Em.View.create({
		classNameBindings: ['MFT.States.home.active:hidden_display'],
		elementId:		'navigation_status_on',
		template: Em.Handlebars.compile(
			'<div class="nav_distance">3.1 Miles</div>'+
			'<img class="ico" src="images/nav/ico_nav_statusbar.png" />'
		)
	}),
	
	actionUp: function(event){
		MFT.States.goToState(MFT.NavigationController.activeState);
	}
});