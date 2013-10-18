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
		'offLabel',
		'rearLabel',
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
	
	/** Automode lables and borders */
	autoMode: Em.View.extend({
		elementId:	'climate_automode',
		template: Em.Handlebars.compile(
			'{{#with controller}}'+
                '<div class="border" {{bindAttr class="model.isAuto:border_full model.isAutoAirflow:border_airflow model.isAutoFan:border_fan"}}></div>'+
				'<div class="label" {{bindAttr class="model.isAuto:auto_full model.isAutoAirflow:auto_airflow model.isAutoFan:auto_fan"}}>Auto</div>'+
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
            classNameBindings: ['controller.model.on.value::label_off'],
			contentBinding:	Em.Binding.oneWay('controller.model.driverTemp.valueStr')
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
            classNameBindings: ['controller.model.on.value::label_off'],
			contentBinding:	Em.Binding.oneWay('controller.model.passangerTemp.valueStr')
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
			classNames:			['helpmode_box_shadow'],
			classNameBindings:	[
				'controller.isMyTemp:active',
			],
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_myTemp'),
			action:				'onActivateMyTemp',
			target:				'controller',
			
			onDown: false,
			
			// Custom event for store my temperature
			actionDown: function(event) {
			
				if (!MFT.helpMode) {
					this.customTimerId = setTimeout(function(){
						MFT.ClimateController.onStoreMyTemp( this.customTimerId );
					}, 2000);
				}
				
				this._super();
			},
			
			// Custom event for activate my temperature
			actionUp: function(event) {
				if (!MFT.helpMode) {
					clearTimeout(this.customTimerId);
				}
				
				this._super();
			}
		}),

		
		heatedSeatButton: MFT.IndButton.extend({
			elementId:			'climate_driverControls_heatedSeatButton',
			action:				'onChangeDriverHeatedSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.driverHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
		}),
		
		cooledSeatButton: MFT.IndButton.extend({
			elementId:			'climate_driverControls_cooledSeatButton',
			action:				'onChangeDriverCooledSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.driverCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
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
		
		classNameBindings:	['MFT.States.climate.full.rear.active:climate_helprear'],
				
		childViews: [
			'dualModeButton',
			'heatedSeatButton',
			'cooledSeatButton'
		],
		
		dualModeButton: MFT.IndButton.extend({
			classNames:			['large','helpmode_box_shadow'],
			elementId:			'climate_passengerControls_dualModeButton',
			action:				'onToggleDualMode',
			target:				'controller',
			text: 				  'Dual',
			contentBinding:		Em.Binding.oneWay('controller.model.dualMode'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			onDown: false
		}),
		
		heatedSeatButton: MFT.IndButton.extend({
			elementId:			'climate_passengerControls_heatedSeatButton',
			action:				'onChangePassangerHeatedSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.passangerHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
		}),
		
		cooledSeatButton: MFT.IndButton.extend({
			elementId:			'climate_passengerControls_cooledSeatButton',
			action:				'onChangePassangerCooledSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.passangerCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
		})
	}),
	
	
	/** Airflow controls */
	airflowControls: Em.ContainerView.extend({
		elementId:			'climate_airflowControls',
		
		classNameBindings:	'controller.model.isAutoAirflow:automode',
		
		childViews: [
			'defrostButton',
			'panelButton',
			'feetButton',
			'bothButton'
		],
		
		defrostButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_defrostButton',
			action:				'onDefrostAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.isAirflowDefrost'),
			icon:				'images/climate/modeman/defrost_ico.png'
		}),
		
		panelButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_panelButton',
			action:				'onPanelAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.isAirflowPanel'),
			icon:				'images/climate/modeman/panel_ico.png'
		}),
		
		feetButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_feetButton',
			action:				'onFeetAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.isAirflowFeet'),
			icon:				'images/climate/modeman/feet_ico.png'
		}),
		
		bothButton: MFT.TogButton.extend({
			elementId:			'climate_airflowControls_bothButton',
			action:				'onBothAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.isAirflowBoth'),
			icon:				'images/climate/modeman/panelfeet_ico.png'
		})
	}),
	
	/** Fan controls */
	fanControls: Em.ContainerView.extend({
		elementId:				'climate_fanControls',
		
		classNameBindings:	['controller.model.isAutoFan:automode'],
		
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
			indDefaultClass:	'indf_inact'
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
			'defrostButton',
			'defrostRearButton',
			'autoButton',
			'acButton',
			'maxAcButton',
			'recirculationButton'
		],
		
		defrostButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_defrostButton',
			action:				'onToggleDefrost',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.defrost'),
			icon:				'images/climate/gang_buttons/windsheald_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		defrostRearButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_defrostRearButton',
			action:				'onToggleRearDefrost',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.rearDefrost'),
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
			contentBinding:		Em.Binding.oneWay('controller.model.isAuto'),
			onDown: false
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
			classNames:			['helpmode_box_shadow'],
			action:				'onToggleMaxAc',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_maxAc'),
			contentBinding:		Em.Binding.oneWay('controller.model.maxAc'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			onDown: false
		}),
		
		recirculationButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_recirculationButton',
			classNames:			['helpmode_box_shadow'],
			action:				'onToggleRecirculation',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.recirculation'),
			icon:				'images/climate/gang_buttons/recycle_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			onDown: false
		})
	}),
	
	/** Rear controls switch button */
	rearSwitcher: MFT.Button.extend({
		elementId:				'climate_rearSwitcher',
		action:					'onToggleRear',
        classNames: 'helpmode_box_shadow',
		classNameBindings:		[
            'MFT.ClimateController.isRearDisabled:hidden'
        ],
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