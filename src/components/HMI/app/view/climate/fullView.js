/**
 * @name MFT.ClimateFullView
 * 
 * @desc Climate module visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/fullView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateFullView = Em.ContainerView.create(MFT.LoadableView,{

	elementId:	'climate_full',
 	 	
 	controller: MFT.ClimateController,
 			
	/** Climate components */		
	childViews: [
		'storeTemp',
		'offLabel',
		'rearLabel',
		'climateHelpFront',
		'climateHelpRear',
		'autoMode',
		'powerButton',
		'driverTemp',
		'passangerTemp',
		'driverControls',
		'passengerControls',
		'airflowControls',
		'fanControls',
		'frontControls',
		'rearSwitcher'
	],
	
	/** Store temp popup */
	storeTemp: Em.View.extend({
		elementId:	'climate_storeTemp',
		template: Em.Handlebars.compile('{{MFT.locale.label.view_climate_myTempStored}}')
	}),
	
	/** Off mode label */
	offLabel: MFT.Label.extend({
		classNameBindings: ['controller.model.on.value:hide'],
		elementId:	'climate_offLabel',
		contentBinding: Em.Binding.oneWay('MFT.locale.label.view_climate_off'),
	}),
	
	/** Rear mode label */
	rearLabel: MFT.Label.extend({
		classNameBindings: ['controller.model.rearOn.value:visible'],
		elementId:	'climate_rearLabelOn',
		contentBinding: Em.Binding.oneWay('MFT.locale.label.view_climate_rearClimateOn'),
	}),
	
	/** Help mode front */
	climateHelpFront: Em.View.extend({
		elementId:	'climate_help_blur',
		classNameBindings: ['visible'],
		visible: function() {
			if ( MFT.States.climate.full.active &&  MFT.helpMode) {
				return true;
			} else {
				return false;
			}
		}.property('MFT.States.climate.full.active', 'MFT.helpMode')
	}),
	
	/** Help mode rear */
	climateHelpRear: Em.View.extend({
		elementId:	'climate_rear_help_blur',
		classNameBindings: ['visible'],
		visible: function() {
			if ( MFT.States.climate.full.rear.active &&  MFT.helpMode) {
				return true;
			} else {
				return false;
			}
		}.property('MFT.States.climate.full.rear.active', 'MFT.helpMode')
	}),
	
	/** Automode lables and borders */
	autoMode: Em.View.extend({
		elementId:	'climate_automode',
		template: Em.Handlebars.compile(
			'{{#with controller.model}}'+
				'<div class="label" {{bindAttr class="auto.value:auto_full autoAirflow.value:auto_airflow autoFan.value:auto_fan"}}>Auto</div>'+
				'<div class="border" {{bindAttr class="auto.value:border_full autoAirflow.value:border_airflow autoFan.value:border_fan"}}></div>'+
			'{{/with}}'
		)
	}),

	/** Power button */
	powerButton: MFT.IndButton.extend({
		elementId:			'climate_powerButton',
		action:				'onActivate',
		target:				'controller',
		contentBinding:		Em.Binding.oneWay('controller.model.on'),
		icon:				'images/climate/power_button/power_icon.png',
		indActiveClass:		'ind_act',
		indDefaultClass:	'ind_inact'
	}),
	
	/** Driver temperature controls */
	driverTemp: Em.ContainerView.extend({
		elementId:			'climate_driverTemp',
				
		childViews: [
			'minusButton',
			'tempLabel',
			'plusButton'
		],
		
		minusButton: MFT.Button.extend({
			elementId:		'climate_driverTemp_minusButton',
			icon:			'images/climate/minus_ico.png',
			classNames:		'minus',
			action:			'onDecDriverTemp',
			target:			'controller',
			timer:			200
		}),
		
		tempLabel: MFT.Label.extend({
			elementId:		'climate_driverTemp_label',
			
			layout: 		Em.Handlebars.compile('<span>{{yield}}</span>'),
			classNames: 	'temp_num',
			contentBinding:	Em.Binding.oneWay('controller.model.driverTemp.valueStr'),
			
			classNameBindings: ['MFT.helpMode:helpmode_blur_text']
		}),
		
		plusButton: MFT.Button.extend({
			elementId:		'climate_driverTemp_plus',
			icon:			'images/climate/plus_ico.png',
			classNames:		'plus',
			action:			'onIncDriverTemp',
			target:			'controller',
			timer:			200
		})
	}),
	
	/** Passanger temperature controls */
	passangerTemp: Em.ContainerView.extend({
		elementId:			'climate_passangerTemp',
				
		childViews: [
			'minusButton',
			'tempLabel',
			'plusButton'
		],
		
		minusButton: MFT.Button.extend({
			elementId:		'climate_passangerTemp_minus',
			icon:			'images/climate/minus_ico.png',
			classNames:		'minus',
			action:			'onDecPassangerTemp',
			target:			'controller',
			timer:			200
		}),
		
		tempLabel: MFT.Label.extend({
			elementId:		'climate_passangerTemp_label',
			
			layout: 		Em.Handlebars.compile('<span>{{yield}}</span>'),
			classNames: 	'temp_num',
			contentBinding:	Em.Binding.oneWay('controller.model.passangerTemp.valueStr'),
			
			classNameBindings: ['MFT.helpMode:helpmode_blur_text']
		}),
		
		plusButton: MFT.Button.extend({
			elementId:		'climate_passangerTemp_plus',
			icon:			'images/climate/plus_ico.png',
			classNames:		'plus',
			action:			'onIncPassangerTemp',
			target:			'controller',
			timer:			200
		})
	}),
	
	
	/** Driver personal controls */
	driverControls: Em.ContainerView.extend({
		elementId:			'climate_driverControls',
		
		childViews: [
			'heatedSeatButton',
			'myTempButton',
			'cooledSeatButton',
			'heatedSteerButton'
		],
		
		myTempButton: MFT.Button.extend({
			elementId: 			'climate_driverControls_myTempButton',
			classNames:			['helpmode_left_delim','helpmode_box_right_shadow'],
			classNameBindings:	[
				'controller.isMyTemp:active',
				'MFT.States.climate.full.rear.active:climate_helprear:climate_helpfront'
			],
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_myTemp'),
			timer:				2000,
			
			actionDown: function(event) {
				this.set('pressed',true);
				
				if (!MFT.helpMode) {
					this.timerId = setTimeout(function(){
						MFT.ClimateController.onStoreMyTemp(this.timerId);
					},this.timer);
				}else{
					if('ontouchstart' in document.documentElement){
						if(event.originalEvent.touches[0]){
							this.set('targetElement', document.elementFromPoint(event.originalEvent.touches[0].pageX,event.originalEvent.touches[0].pageY));
						}
					}
				}
			},
			
			// Custom event for activate my temperature
			actionUp: function() {
				this.set('pressed',false);
				
				this.get('controller').onActivateMyTemp();				
				
				if (!MFT.helpMode) {
					clearTimeout(this.timerId);
				}				
			}
		}),

		
		heatedSeatButton: MFT.IndButton.extend({
			elementId:			'climate_driverControls_heatedSeatButton',
			classNames:			['helpmode_right_delim','helpmode_bottom_delim','helpmode_box_shadow'],
			action:				'onChangeDriverHeatedSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.driverHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def'
		}),
		
		cooledSeatButton: MFT.IndButton.extend({
			elementId:			'climate_driverControls_cooledSeatButton',
			classNames:			['helpmode_top_delim','helpmode_box_bottom_shadow'],
			action:				'onChangeDriverCooledSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.driverCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def'
		}),
		
		heatedSteerButton: MFT.IndButton.extend({
			elementId:			'climate_driverControls_heatedSteerButton',
			action:				'onChangeDriverHeatedSteer',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.driverHeatedSteer'),
			icon:				'images/climate/left_person_control/cooledstear_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		})
	}),
	
	/** Passenger personal controls */
	passengerControls: Em.ContainerView.extend({
		elementId:			'climate_passengerControls',
		
		classNameBindings:	['MFT.States.climate.full.rear.active:climate_helprear:climate_helpfront'],
		
		classNames:			['helpmode_box_shadow'],
		
		childViews: [
			'dualModeButton',
			'heatedSeatButton',
			'cooledSeatButton'
		],
		
		dualModeButton: MFT.IndButton.extend({
			classNames:			['large','helpmode_right_delim'],
			elementId:			'climate_passengerControls_dualModeButton',
			action:				'onToggleDualMode',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_dual'),
			contentBinding:		Em.Binding.oneWay('controller.model.dualMode'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		heatedSeatButton: MFT.IndButton.extend({
			elementId:			'climate_passengerControls_heatedSeatButton',
			classNames:			['helpmode_left_delim','helpmode_bottom_delim'],
			action:				'onChangePassangerHeatedSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.passangerHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def'
		}),
		
		cooledSeatButton: MFT.IndButton.extend({
			elementId:			'climate_passengerControls_cooledSeatButton',
			classNames:			['helpmode_left_delim','helpmode_top_delim'],
			action:				'onChangePassangerCooledSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.passangerCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def'
		})
	}),
	
	
	/** Airflow controls */
	airflowControls: Em.ContainerView.extend({
		elementId:			'climate_airflowControls',
		
		classNameBindings:	'controller.model.autoAirflow.value:automode',
		
		childViews: [
			'defrostButton',
			'panelButton',
			'feetButton',
			'bothButton'
		],

		itemsBinding: Em.Binding.oneWay('controller.model.airflow.items'),
		
		defrostButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_defrostButton',
			action:				'onDefrostAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('parentView.items.defrost'),
			icon:				'images/climate/modeman/defrost_ico.png'
		}),
		
		panelButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_panelButton',
			action:				'onPanelAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('parentView.items.panel'),
			icon:				'images/climate/modeman/panel_ico.png'
		}),
		
		feetButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_feetButton',
			action:				'onFeetAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('parentView.items.feet'),
			icon:				'images/climate/modeman/feet_ico.png'
		}),
		
		bothButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_bothButton',
			action:				'onBothAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('parentView.items.both'),
			icon:				'images/climate/modeman/panelfeet_ico.png'
		})
	}),
	
	/** Fan controls */
	fanControls: Em.ContainerView.extend({
		elementId:				'climate_fanControls',
		
		classNameBindings:	['controller.model.autoFan.value:automode'],
		
		childViews: [
			'fanIndicator',
			'fanMinusButton',
			'fanPlusButton'
		],

		fanIndicator: MFT.IndButton.extend({
			elementId:			'climate_fanControls_fanIndicator',
			contentBinding:		Em.Binding.oneWay('controller.model.fan'),
			icon:				'images/climate/fan_control/fan_ico.png',
			indActiveClass:		'indf_act',
			indDefaultClass:	'indf_inact',
		}),
		
		fanMinusButton: MFT.Button.extend({
			elementId:			'climate_fanControls_fanMinusButton',
			target:				'controller',
			action:				'onDecFanSpeed',
			icon:				'images/climate/fan_control/fan_minus_control.png',
			timer:				200
		}),
		
		fanPlusButton: MFT.Button.extend({
			elementId:			'climate_fanControls_fanPlusButton',
			target:				'controller',
			action:				'onIncFanSpeed',
			icon:				'images/climate/fan_control/fan_plus_comtrol.png',
			timer:				200
		})
	}),
	
	/** Front bottom controls */
	frontControls: Em.ContainerView.extend({
		classNameBindings: ['MFT.States.climate.full.rear.active:hidden'],
		
		elementId:			'climate_frontControls',
		
		childViews: [
			'windshieldButton',
			'defrostButton',
			'autoButton',
			'acButton',
			'maxAcButton',
			'recirculationButton'
		],
		
		windshieldButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_windshieldButton',
			action:				'onToggleWindShield',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.windShield'),
			icon:				'images/climate/gang_buttons/windsheald_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		defrostButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_defrostButton',
			action:				'onToggleRearDeforost',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.rearDeforost'),
			icon:				'images/climate/gang_buttons/defrost_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		autoButton: MFT.TogButton.extend({
			elementId:			'climate_frontControls_autoButton',
			classNames:			['helpmode_box_shadow'],
			action:				'onToggleAuto',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_auto'),
			contentBinding:		Em.Binding.oneWay('controller.model.auto'),
		}),
		
		acButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_acButton',
			action:				'onToggleAc',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_ac'),
			contentBinding:		Em.Binding.oneWay('controller.model.ac'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		maxAcButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_maxAcButton',
			classNames:			['helpmode_right_delim','helpmode_box_left_shadow'],
			action:				'onToggleMaxAc',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_maxAc'),
			contentBinding:		Em.Binding.oneWay('controller.model.maxAc'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		recirculationButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_recirculationButton',
			classNames:			['helpmode_left_delim','helpmode_box_right_shadow'],
			action:				'onToggleRecirculation',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.recirculation'),
			icon:				'images/climate/gang_buttons/recycle_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		})
	}),
	
	/** Rear controls switch button */
	rearSwitcher: MFT.Button.extend({
		elementId:				'climate_rearSwitcher',
		action:					'onToggleRear',
		classNameBindings:		['MFT.States.climate.full.rear.active::helpmode_box_shadow'],
		target:					'controller',
		iconBinding:			'iconPath',
		textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_rear'),
		onDown: false,
		
		iconPath: function() {
			if ( MFT.States.climate.full.rear.active ) {
				return 'images/climate/rear_auto/rear_but_opened.png';
			} else {
				return 'images/climate/rear_auto/rear_but_closed.png';
			}
		}.property('MFT.States.climate.full.rear.active')
	})
});