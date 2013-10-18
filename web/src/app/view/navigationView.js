/**
 * @name MFT.NavigationView
 * 
 * @desc Navigation module visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/NavigationView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.NavigationView = Em.ContainerView.create(MFT.LoadableView,{
	
	elementId:	'navigation',
 			
	childViews: [
		'maincontent',
		'view_1',
		'view_2',
		'view_3',
        'topMenu',
        'mapSettings',
        'zoomInButton',
        'zoomOutButton'
	],
	
	
	onNavigationDisable: function(){
		if(!FFW.Backend.isNavigationEnabled){
			
			 if(MFT.States.currentState.get('path').match('navigation') || (MFT.States.currentState.name == 'faq' && MFT.States.currentState.from == 'navigation')){
				MFT.States.goToState(MFT.InfoController.activeState);
			 }
		}
				
	}.observes('FFW.Backend.isNavigationEnabled'),
	
	maincontent: Em.View.extend({
		
		elementId: 'navigation_maincontent',
		
		template: Em.Handlebars.compile(
			
		'<div id="nav_map_img" {{bindAttr class="MFT.NavigationController.mapType MFT.NavigationController.viewType MFT.NavigationController.zoomMap"}}>		</div>'+
		'<div id="distance"><span>{{MFT.NavigationController.model.zoomRate.value.value}}{{MFT.NavigationController.model.zoomRate.value.shortLable}}</span></div>'+
		'<div class="time-dest">'+
			'<div class=time-dest-cont>'+
			'<span class="distance">{{MFT.NavigationController.model.distance_total.value}}<span class="distance_label">{{MFT.NavigationController.model.distance_total.shortLable}}<span></span>'+
			'<span class="distance_time">0:27<span class="distance_time_label">HRS</span></span>'+
			'</div>'+
		'</div>'
		)
	}),


	view_1 : Em.View.extend({

		elementId: "navigation_view1",

		classNameBindings: ['MFT.NavigationController.viewType'],

		template: Em.Handlebars.compile(
			'<div id="nav_arrow"></div>'+
			'<div id="mi">{{MFT.NavigationController.model.distance_left.value}}{{MFT.NavigationController.model.distance_left.shortLable}}</div>'+
			'<div id="nav_location">{{MFT.locale.label.view_navigation_position}}</div>'+
			'<div id="street"><span>{{MFT.locale.label.view_navigation_street}}</span></div>'
		)

	}),

	view_2 : Em.View.extend({

		elementId: "navigation_view2",

		classNameBindings: ['MFT.NavigationController.viewType'],

		template: Em.Handlebars.compile(
			'<div id="view2_list_container">'+
				'<div id="nav_mich_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_left.value}} {{MFT.NavigationController.model.distance_left.shortLable}}</div> <div id="nav_mich" class="nav2_ctrls">{{MFT.locale.label.view_navigation_street}}</div>'+
				'<div id="nav_outer_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_to_next_turn.value}} {{MFT.NavigationController.model.distance_to_next_turn.shortLable}}</div> <div id="nav_outer" class="nav2_ctrls">{{MFT.locale.label.view_navigation_position}}</div>'+
				'<div id="nav_main_street_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_left.value}} {{MFT.NavigationController.model.distance_left.shortLable}}</div> <div id="nav_main_street" class="nav2_ctrls">{{MFT.locale.label.view_navigation_2ndSt}}</div>'+
				'<div id="nav_wyoming_street_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_left.value}} {{MFT.NavigationController.model.distance_left.shortLable}}</div> <div id="nav_wyoming_street" class="nav2_ctrls">{{MFT.locale.label.view_navigation_cherryHill}}</div>'+
				'{{view MFT.Button '+
					'textBinding="MFT.locale.label.view_navigation_view2_editTurnList" '+
					'classNames="button turn-list" '+
					'templateName="arrow" '+
					'elementId="navigation_destination_editRoute_Button" '+
					'disabled=true '+
				'}}'+
			'</div>'
		)

	}),

	view_3 : Em.View.extend({
		elementId: "navigation_view3",
		classNameBindings: ['MFT.NavigationController.viewType'],
		template: Em.Handlebars.compile(
			'<div id="no_freeway">{{MFT.locale.label.view_navigation_freeway}}</div>'
		)
	}),

	topMenu: Em.ContainerView.create({
		
		elementId:			'navigation_topmenu',

        classNameBindings: [
            'MFT.States.navigation.dest.active:hidden'
        ],
		
		childViews: [
			'compas',
			'topDelim2',
			'topDelim3',
			'destinationButton',
			'menuButton',
			'topMenuArrow'
		],
		
		topDelim2: Em.View.extend({
			classNames:	'delim delim2'
		}),
		
		topDelim3: Em.View.extend({
			classNames:	'delim delim3'
		}),
		
		compas: Em.View.extend({
			elementId:	'nav_compass'
		}),
		
		topMenuArrow: MFT.Button.create({
			elementId:	'nav_top_menu_arrow',
			classNames: ['ffw-button'],
            classNameBindings: ['MFT.helpMode:helpmode_box_shadow:disabled'],
			icon: 'images/nav/arrow.png',
            target:			   'MFT.NavigationController',
            action:			   'turnRepeatHelpVideoOn',
            onDown: false
		}),
		
		menuButton: MFT.Button.create({
			elementId:			 'navigation_topmenu_menu_button',
			action:				'turnOnMenu',
			target: 				'MFT.NavigationController',
			templateName: 			'text',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_navi_destination_MyHome_menu')
		}),
		
		destinationButton: MFT.Button.create({
			elementId:			'navigation_topmenu_destination_button',
			classNames: 'helpmode_box_shadow',
			textBinding:	Ember.Binding.oneWay('MFT.locale.label.view_navigation_dest'),
			target:			   'MFT.NavigationController',
			action:			   'subState',
			onDown: false
		})
	}),
	
	mapSettings: Em.ContainerView.create({
		
		elementId:			'navigation_mapsettings',

        classNameBindings: [
            'MFT.States.navigation.dest.active:hidden'
        ],

		childViews: [
			'mapViewChangeButton',
			'typeViewChangeButton'
			
		],
	
		mapViewChangeButton :MFT.ToggleIndicatorsButton.create({
				elementId:			 'navigation_mapview_button',
				action:				'toggleMapView',
				target:				'MFT.NavigationController',
				contentBinding:	Ember.Binding.oneWay('MFT.NavigationController.model.mapView'),
				indActiveClass:		'small_ind_active',
				indDefaultClass:    	'small_ind_def',
				startFrom:			 0,
				icon: 'images/nav/type_n.png',
				
				changeMap: function(){
						switch(this.content.value){
							case 0:{
								this.set('icon','images/nav/type_n.png');
								MFT.NavigationController.set('mapType','type0');
								break;
							}
							case 1:{
								this.set('icon','images/nav/type_h.png');
								MFT.NavigationController.set('mapType','type1'); 
								break;
							}
							case 2:{
								this.set('icon','images/nav/type_d.png');
								MFT.NavigationController.set('mapType','type2'); 
								break;
							}
						}
				}.observes('this.content.value')
		}),
		
		typeViewChangeButton: MFT.ToggleIndicatorsButton.create({
				elementId:			 'navigation_typeview_button',
				action:				'toggleTypeView',
				target:				'MFT.NavigationController',
				contentBinding:	Ember.Binding.oneWay('MFT.NavigationController.model.typeView'),
				indActiveClass:		'small_ind_active',
				indDefaultClass:       'small_ind_def',
				startFrom:			 0,
				textBinding:	   Ember.Binding.oneWay('MFT.locale.label.view_navigation_view'),
				
				classNames: 'helpmode_box_shadow',
				
				onHelpMode: function(){
					(MFT.helpMode) ? this.set('onDown',false) : this.set('onDown',true); 
				}.observes('MFT.helpMode'),
				
				changeView: function(){
					switch(this.content.value){
						case 0:{
							MFT.NavigationController.set('viewType','typeview1');
							break;
						}
						case 1:{
							MFT.NavigationController.set('viewType','typeview2'); 
							break;
						}
						case 2:{
							MFT.NavigationController.set('viewType','typeview3'); 
							break;
						}
					}
				}.observes('this.content.value')
				
			})
	}),
	
	zoomInButton:  MFT.Button.extend({
			elementId:			 'navigation_zoomin_button',
			action:				'zoomIn',
			target: 			    'MFT.NavigationController',
			valueBinding:		Ember.Binding.oneWay('MFT.NavigationController.model.zoomRate.value.value'),
			icon:				  'images/nav/zoom_in.png',
			
			zoomIn: function(){
				if(this.value == MFT.NavigationController.model.zoomRate.maxZoom.value){
					this.set('disabled', true);	
				}else{
					this.set('disabled', false);		
				}
			}.observes('this.value')
				
	}),
	
	zoomOutButton:  MFT.Button.extend({
			elementId:			'navigation_zoomout_button',
			action:			   'zoomOut',
			target: 			   'MFT.NavigationController',
			valueBinding:	   Ember.Binding.oneWay('MFT.NavigationController.model.zoomRate.value.value'),
			icon:				 'images/nav/zoom_out.png',
			disabled: true,
			
			zoomOut: function(){
				if(this.value == MFT.NavigationController.model.zoomRate.minZoom.value){
					this.set('disabled', true);	
				}else{
					this.set('disabled', false);
				}
					
			}.observes('this.value')
	})

});