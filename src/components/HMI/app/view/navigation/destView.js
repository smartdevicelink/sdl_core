/**
 * @name MFT.NavigationDestinationView
 * 
 * @desc Destination navigation visual representation
 * 
 * @category	View
 * @filesource	app/view/navigation/NavigationDestinationView.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */

MFT.NavigationDestinationView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'navigation_destination',
	
	classNameBindings: ['MFT.States.navigation.dest.active:active_state','MFT.helpMode'],
	
	classNames: ['hidden'],
	
	stateObj: MFT.States.navigation.dest,
	
	childViews: [
		'backButton',
		'topTitile',
		'pushToTalkButton',
		'editButtons',
		'list'
	],
	
	backButton: MFT.Button.extend({
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',	
	}),
	
	topTitile: MFT.Label.extend({
		classNames:		  ['block-header-title'],
		contentBinding:	 Ember.Binding.oneWay('MFT.locale.label.view_navigation_destination')
	}),	
	
	pushToTalkButton: MFT.Button.extend({
		elementId:		   'navigation_destination_pushToTalk_buttton',
		classNames:		  ['button'],		
		action:	  		  '',
		target:	          'MFT.NavigationController',	
		icon:				'images/nav/ico_pushToTalk.png',
		disabledBinding:	 Ember.Binding.oneWay('MFT.helpMode'),
	}),	
	
	list: MFT.List.extend({
		elementId:		   'navigation_destination_list',
		
		disableScrollbarBinding:  Ember.Binding.oneWay('MFT.helpMode'),
		
		itemsOnPage:		 5,
		
		columnsNumber:	   2,
		
		/** Items */
		items: [
			{
								
				type:		MFT.Button,
								
				params:		{
					goToState:   'myHome',
					icon:		'images/nav/dest/ico_myHome.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_myHome',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode',
					voiceOver: true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'streetAddress',
					icon:		'images/nav/dest/ico_adress.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_streetAddress',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode',
					voiceOver: true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'favorites',
					icon:		'images/nav/dest/ico_favorites.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_favorites',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode',
					voiceOver: true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'intersection',
					icon:		'images/nav/dest/ico_intersection.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_intersection',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode',
					voiceOver: true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'previousDestinations',
					icon:		'images/nav/dest/ico_destination.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_previousDestinations',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode',
					voiceOver: true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'cityCenter',
					icon:		'images/nav/dest/ico_cityCenter.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_cityCenter',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'POI',
					icon:		'images/nav/dest/ico_poi.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_POI',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode',
					voiceOver: true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'map',
					icon:		'images/nav/dest/ico_map.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_map',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'emergency',
					icon:		'images/nav/dest/ico_emergency.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_emergency',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode',
					voiceOver: true,
				}			
					
			},
			{
								
				type:		MFT.Label,
				
				params:		{
					disabled: 	true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'previousStartingsPoint',
					icon:		'images/nav/dest/ico_destination.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_previousStartingsPoint',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'freeWay',
					icon:		'images/nav/dest/ico_freeWay.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_freewayEntranceExit',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'latitudeLongitude',
					icon:		'images/nav/dest/ico_latLong.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_latitudeLongitude',
					arrow:		true,
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					disabledBinding: 'MFT.helpMode'
				}			
					
			}
		]
	}),
	
	editButtons:  Em.View.extend({
		elementId: 'nav_dest_editButton',
		template: Em.Handlebars.compile(
			'<div class="group_button_wraper">'+
				'{{view MFT.Button '+
					'textBinding="MFT.locale.label.view_navigation_destination_editRouteCancelRoute" '+
					'elementId="navigation_destination_editRoute_Button" '+
					'classNames="button" '+
					'action="" '+
					'target="" '+
					'disabled=true '+
				'}}'+
				'<div class="delim"></div>'+
				'{{view MFT.Button '+
					'textBinding="MFT.locale.label.view_navigation_destination_CancelRoute"'+
					'elementId="navigation_destination_cancelRoute_Button" '+
					'classNames="button" '+
					'action="" '+
					'target="" '+
					'disabled=true '+
				'}}'+
			'</div>'
		)
	})
});