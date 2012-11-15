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
		'topMenu',
		'mapSettings',
		'zoomInButton',
		'zoomOutButton',
		'view_1',
		'view_2',
		'view_3',
		'HelpmapViewChangeButton',
		'HelpZoomButtons'
	],
	
	maincontent: Em.View.extend({
		
		elementId: 'navigation_maincontent',
		
		template: Em.Handlebars.compile(
			
		'<div id="nav_map_img" {{bindAttr class="MFT.NavigationController.mapType MFT.NavigationController.viewType MFT.NavigationController.zoomMap MFT.helpMode:dark_map"}}>		</div>'+
		'<div id="distance" {{bindAttr class="MFT.helpMode:helpmode_blur_black_text MFT.helpMode:help_blur"}}><span>{{MFT.NavigationController.model.zoomRate.value.value}}{{MFT.NavigationController.model.zoomRate.value.shortLable}}</span></div>'+
		'<div class="time-dest"{{bindAttr class="MFT.helpMode:help_blur "}}>'+
			'<div class=time-dest-cont>'+
			'<span class="distance">{{MFT.NavigationController.model.distance_total.value}}<span class="distance_label">{{MFT.NavigationController.model.distance_total.shortLable}}<span></span>'+
			'<span class="distance_time">0:27<span class="distance_time_label">HRS</span></span>'+
			'</div>'+
		'</div>'
		)
	}),
	
	
	view_1 : Em.View.extend({
		
		elementId: "navigation_view1",
				
		classNameBindings: ['MFT.NavigationController.viewType','MFT.helpMode:help_blur'],
		
		template: Em.Handlebars.compile(
			'<div id="nav_arrow"{{bindAttr class="MFT.helpMode:help_blur"}}></div>'+
			'<div id="mi"{{bindAttr class="MFT.helpMode:help_blur"}}>{{MFT.NavigationController.model.distance_left.value}}{{MFT.NavigationController.model.distance_left.shortLable}}</div>'+
			'<div id="nav_location" {{bindAttr class="MFT.helpMode:help_blur"}}>{{MFT.locale.label.view_navigation_position}}</div>'+
			'<div id="street" {{bindAttr class="MFT.helpMode:help_blur"}}><span {{bindAttr class="MFT.helpMode:helpmode_blur_text"}}>{{MFT.locale.label.view_navigation_street}}</span></div>'
		)
	
	}),
	
	view_2 : Em.View.extend({
		
		elementId: "navigation_view2",
		
		classNameBindings: ['MFT.NavigationController.viewType', 'MFT.helpMode:help_blur'],
		
		template: Em.Handlebars.compile(
			'<div id="view2_list_container">'+
				'<div id="nav_mich_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_left.value}} {{MFT.NavigationController.model.distance_left.shortLable}}</div> <div id="nav_mich" class="nav2_ctrls">Main St</div>'+
				'<div id="nav_outer_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_to_next_turn.value}} {{MFT.NavigationController.model.distance_to_next_turn.shortLable}}</div> <div id="nav_outer" class="nav2_ctrls">Central Parkway</div>'+
				'<div id="nav_main_street_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_left.value}} {{MFT.NavigationController.model.distance_left.shortLable}}</div> <div id="nav_main_street" class="nav2_ctrls">2nd St</div>'+
				'<div id="nav_wyoming_street_mi" class="nav2_ctrls2">{{MFT.NavigationController.model.distance_left.value}} {{MFT.NavigationController.model.distance_left.shortLable}}</div> <div id="nav_wyoming_street" class="nav2_ctrls">Cherry Hill Rd</div>'+
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
		classNameBindings: ['MFT.NavigationController.viewType','MFT.helpMode:help_blur'],
		template: Em.Handlebars.compile(
			'<div id="no_freeway" {{bindAttr class="MFT.helpMode:hidden"}}>{{MFT.locale.label.view_navigation_freeway}}</div>'
		)
	}), 
	
	topMenu: Em.ContainerView.create({
		
		elementId:			'navigation_topmenu',
		
		childViews: [
			'compas',
			'topDelim2',
			'topDelim3',
			'destinationButton',
			'menuButton',
			'menuButtonHelp',
			'repeatButton_helpmode',
			'topMenuArrow',
			'helpYellowBorderRepeatButton',
			'helpYellowBorder'
		],
		
		topDelim2: Em.View.extend({
			classNames:	'delim delim2'
		}),
		
		topDelim3: Em.View.extend({
			classNames:	'delim delim3'
		}),
		
		compas: Em.View.extend({
			elementId:	'nav_compass',
			classNameBindings: ['MFT.helpMode:help_blur']
		}),
		
		topMenuArrow: Em.View.extend({
			elementId:	'nav_top_menu_arrow',
			classNameBindings: ['MFT.helpMode:hidden'],
			classNames: 'disabled',
			template: Em.Handlebars.compile(
				'<img src="images/nav/arrow-inact.png" alt="">'
			)
		}),
		
		helpYellowBorderRepeatButton: Em.View.extend({
			elementId:	'help_yellow_border_repeat_button',
			classNames:	'hidden',
			classNameBindings: ['MFT.helpMode:active_state']
		}),
		
		helpYellowBorder: Em.View.extend({
			elementId:	'help_yellow_border',
			classNames:	'hidden',
			classNameBindings: ['MFT.helpMode:active_state']
		}),
		
		repeatButton_helpmode: MFT.Button.create({
			elementId:			'help_nav_top_menu_repeat_button',
			classNameBindings: ['MFT.helpMode:active_state'],
			classNames: 'hidden',
			target:			   'MFT.NavigationController',
			icon: 				 'images/nav/arrow.png',
			action:			   'turnRepeatHelpVideoOn',
			onDown: false
		}),
		
		menuButton: MFT.Button.create({
			elementId:			 'navigation_topmenu_menu_button',
			classNameBindings: 	['MFT.helpMode:hidden'],
			action:				'turnOnMenu',
			target: 				'MFT.NavigationController',
			templateName: 			'text',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_navi_destination_MyHome_menu'),
		}),
		
		menuButtonHelp: MFT.Button.create({
			elementId:			 'navigation_topmenu_menu_button_help',
			classNameBindings: 	['MFT.helpMode:active_state'],
			classNames: 		'hidden',
			templateName: 			'text',
			text: ''
		}),
		
		
		destinationButton: MFT.Button.create({
			elementId:			'navigation_topmenu_destination_button',
			textBinding:	Ember.Binding.oneWay('MFT.locale.label.view_navigation_dest'),
			target:			   'MFT.NavigationController',
			action:			   'subState',
			onDown: false
		})
	}),
	
	HelpmapViewChangeButton: Em.View.extend({
			elementId:			'help_navigation_mapview_button',
			target:				'MFT.NavigationController',
			contentBinding:	Ember.Binding.oneWay('MFT.NavigationController.model.mapView'),
			classNameBindings: ['this.buttonType','MFT.helpMode:active_state'],
			classNames: 'hidden',
			
			helpMode: function(){
				if(MFT.helpMode){
					switch(this.content.value){
							case 0:{
								this.set('buttonType','button_type1');
								break;
							}
							case 1:{
								this.set('buttonType','button_type2');
								break;
							}
							case 2:{
								this.set('buttonType','button_type3');
								break;
							}
					}
				}
			}.observes('MFT.helpMode')
	}),
	
	mapSettings: Em.ContainerView.create({
		
		elementId:			'navigation_mapsettings',
		
		classNameBindings: ['MFT.helpMode:noboreder'],
		
		
		childViews: [
			'mapViewChangeButton',
			'typeViewChangeButton',
		],
	
		mapViewChangeButton :MFT.ToggleIndicatorsButton.create({
				elementId:			 'navigation_mapview_button',
				action:				'toggleMapView',
				target:				'MFT.NavigationController',
				contentBinding:	Ember.Binding.oneWay('MFT.NavigationController.model.mapView'),
				indActiveClass:		'small_ind_active',
				indDefaultClass:    	'small_ind_def',
				startFrom:			 0,
				classNameBindings: ['MFT.helpMode:hidden'],
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
				
			}),
	}),
	
	HelpZoomButtons: Em.View.extend({
		elementId:			 'help_navigation_zoom_button',
		contentBinding:		Ember.Binding.oneWay('MFT.NavigationController.model.zoomRate.value'),
		classNameBindings: ['this.buttonType','MFT.helpMode:active_state'],
		classNames: 'hidden',
		helpMode: function(){
			if(MFT.helpMode){
				switch(this.content.value){
						case MFT.NavigationController.model.zoomRate.maxZoom.value:{
							this.set('buttonType','button_type1');
							break;
						}
						case MFT.NavigationController.model.zoomRate.minZoom.value:{
							this.set('buttonType','button_type2');
							break;
						}
						default:{
							this.set('buttonType','button_type3');
							break;
						}
				}
			}
		}.observes('MFT.helpMode')
		
		
	}),
	
	zoomInButton:  MFT.Button.extend({
			elementId:			 'navigation_zoomin_button',
			action:				'zoomIn',
			target: 			    'MFT.NavigationController',
			valueBinding:		Ember.Binding.oneWay('MFT.NavigationController.model.zoomRate.value.value'),
			icon:				  'images/nav/zoom_in.png',
			classNameBindings: ['MFT.helpMode:hidden'],
			
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
			classNameBindings: ['MFT.helpMode:hidden'],
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