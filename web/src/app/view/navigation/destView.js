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
			
	childViews: [
		'backButton',
		'topTitile',
		'editButtons',
		'list'
	],
	
	backButton: MFT.Button.extend({
		elementId: 		   'navigation_destination_backButton',
		classNames:		  ['backButton','button'],		
		action:			  'back',
		target:			  'MFT.States',	
		icon:				'images/media/ico_back.png',
        classNameBindings: [
            'isHide:hide-important'
        ],

        isHide: function () {
            return MFT.States.navigation.dest.map.active
        }.property(
                'MFT.States.navigation.dest.map.active'
            )
	}),
	
	topTitile: MFT.Label.extend({
		elementId:		   'navigation_destination_titile',
		classNames:		  ['block-header-title'],
		contentBinding:	 Ember.Binding.oneWay('MFT.locale.label.view_navigation_destination')
	}),	
	
	list: MFT.List.extend({
		elementId:		   'navigation_destination_list',
		
		disableScrollbar: true  ,
		
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
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'streetAddress',
					icon:		'images/nav/dest/ico_adress.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_streetAddress',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'favorites',
					icon:		'images/nav/dest/ico_favorites.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_favorites',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'intersection',
					icon:		'images/nav/dest/ico_intersection.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_intersection',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'previousDestinations',
					icon:		'images/nav/dest/ico_destination.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_previousDestinations',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'cityCenter',
					icon:		'images/nav/dest/ico_cityCenter.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_cityCenter',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'POI',
					icon:		'images/nav/dest/ico_poi.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_POI',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true,
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'map',
					icon:		'images/nav/dest/ico_map.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_map',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'emergency',
					icon:		'images/nav/dest/ico_emergency.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_emergency',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true,
				}			
					
			},
			{
								
				type:		MFT.Label,
				
				params:		{
					disabled: 	true
				}			
					
			}/*,
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'previousStartingsPoint',
					icon:		'images/nav/dest/ico_destination.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_previousStartingsPoint',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'freeWay',
					icon:		'images/nav/dest/ico_freeWay.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_freewayEntranceExit',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			},
			{
								
				type:		MFT.Button,
				
				params:		{
					goToState:   'latitudeLongitude',
					icon:		'images/nav/dest/ico_latLong.png',
					textBinding: 'MFT.locale.label.view_navigation_destination_latitudeLongitude',
					templateName: 'arrow',
					action:	  'onChildState',
					target:	  	'MFT.NavigationController',
					helpMode: true
				}			
					
			}*/
		]
	}),
	
	editButtons:  Em.View.extend({
		elementId: 'nav_dest_editButton',
		classNames: 'group_button_wraper',
		template: Em.Handlebars.compile(
			'{{view MFT.Button '+
				'textBinding="MFT.locale.label.view_navigation_destination_editRouteCancelRoute" '+
				'elementId="navigation_destination_editRoute_Button" '+
				'classNames="button" '+
				'disabled=true '+
			'}}'+
			'<div class="delim"></div>'+
			'{{view MFT.Button '+
				'textBinding="MFT.locale.label.view_navigation_destination_CancelRoute"'+
				'elementId="navigation_destination_cancelRoute_Button" '+
				'classNames="button" '+
				'disabled=true '+
			'}}'
		)
	})
});