/**
 * @name MFT.ClimateSimpleView
 * 
 * @desc Climate simple module visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/simpleView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateSimpleView = Em.ContainerView.create(MFT.LoadableView,{

	elementId:	'climate_simple',
 	 	
 	controller: MFT.ClimateController,
 	 	
 	/** Climate components */		
	childViews: [
		'helpMode',
		'offLabel',
		'autoMode',
		'powerButton',
		'driverTemp',
		'passangerTemp',
		'airflowControls',
		'fanControls',
		'controls'
	],
	
	/** Help mode */
	helpMode: Em.View.extend({
		elementId:	'climate_simple_help_blur',
		classNameBindings: ['visible'],
		visible: function() {
			if ( MFT.States.climate.simple.active &&  MFT.helpMode) {
				return true;
			} else {
				return false;
			}
		}.property('MFT.States.climate.simple.active', 'MFT.helpMode')
	}),
	
	/** Off mode label */
	offLabel: MFT.Label.extend({
		classNameBindings: ['controller.model.on.value:hide'],
		elementId:	'climate_simple_offLabel',
		content:	'OFF'
	}),
	
	/** Automode lables and borders */
	autoMode: Em.View.extend({
		elementId:	'climate_simple_automode',
		template: Em.Handlebars.compile(
			'{{#with controller.model}}'+
				'<div class="label" {{bindAttr class="auto.value:auto_full autoAirflow.value:auto_airflow autoFan.value:auto_fan"}}>Auto</div>'+
				'<div class="border" {{bindAttr class="auto.value:border_full autoAirflow.value:border_airflow autoFan.value:border_fan"}}></div>'+
			'{{/with}}'
		)
	}),
	
	/** Power button */
	powerButton: MFT.IndButton.extend({
		elementId:			'climate_simple_powerButton',
		action:				'onActivate',
		target:				'controller',
		contentBinding:		Em.Binding.oneWay('controller.model.on'),
		icon:				'images/climate/power_button/power_icon.png',
		indActiveClass:		'ind_act',
		indDefaultClass:	'ind_inact'
	}),
	
	/** Driver temperature controls */
	driverTemp: Em.ContainerView.extend({
		elementId:			'climate_simple_driverTemp',
				
		childViews: [
			'plusButton',
			'tempLabel',
			'minusButton'
		],
		
		minusButton: MFT.Button.extend({
			elementId:		'climate_simple_driverTemp_minus',
			icon:			'images/climate/minus_ico.png',
			classNames:		'minus',
			action:			'onDecDriverTemp',
			target:			'controller',
			timer:			200
		}),
		
		tempLabel: MFT.Label.extend({
			elementId:		'climate_simple_driverTemp_label',
			classNameBindings: ['MFT.helpMode:helpmode_blur_text'],
			layout: 		Em.Handlebars.compile('<span>{{yield}}</span>'),
			classNames: 	'temp_num',
			contentBinding:	Em.Binding.oneWay('controller.model.driverTemp.valueStr')
		}),
		
		plusButton: MFT.Button.extend({
			elementId:		'climate_simple_driverTemp_plus',
			icon:			'images/climate/plus_ico.png',
			classNames:		'plus',
			action:			'onIncDriverTemp',
			target:			'controller',
			timer:			200
		})
	}),
	
	/** Passanger temperature controls */
	passangerTemp: Em.ContainerView.extend({
		elementId:			'climate_simple_passangerTemp',
				
		childViews: [
			'plusButton',
			'tempLabel',
			'minusButton'
		],
		
		minusButton: MFT.Button.extend({
			elementId:		'climate_simple_passangerTemp_minus',
			icon:			'images/climate/minus_ico.png',
			classNames:		'minus',
			action:			'onDecPassangerTemp',
			target:			'controller',
			timer:			200
		}),
		
		tempLabel: MFT.Label.extend({
			elementId:		'climate_simple_passangerTemp_label',
			classNameBindings: ['MFT.helpMode:helpmode_blur_text'],
			layout: 		Em.Handlebars.compile('<span>{{yield}}</span>'),
			classNames: 	'temp_num',
			contentBinding:	Em.Binding.oneWay('controller.model.passangerTemp.valueStr')
		}),
		
		plusButton: MFT.Button.extend({
			elementId:		'climate_simple_passangerTemp_plus',
			icon:			'images/climate/plus_ico.png',
			classNames:		'plus',
			action:			'onIncPassangerTemp',
			target:			'controller',
			timer:			200
		})
	}),
	
	/** Airflow controls */
	airflowControls: Em.ContainerView.extend({
		elementId:			'climate_simple_airflowControls',
		
		classNameBindings:	['controller.model.autoAirflow.value:automode'],
		
		childViews: [
			'windshieldButton',
			'windshieldMaxButton',
			'panelAirflowButton',
			'feetAirflowButton'
		],
		
		windshieldButton: MFT.IndButton.extend({
			elementId:			'climate_simple_windshieldButton',
			action:				'onToggleWindShield',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.windShield'),
			icon:				'images/climate/gang_buttons/windsheald_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		windshieldMaxButton: MFT.IndButton.extend({
			elementId:			'climate_simple_defrostMaxButton',
			action:				'onToggleMaxDefrost',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.maxDefrost'),
			icon:				'images/climate/gang_buttons/windsheald_max_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		panelAirflowButton: MFT.IndButton.extend({
			elementId:			'climate_simple_panelAirflowButton',
			action:				'onPanelSimpleAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.simpleAirflowPanel'),
			icon:				'images/climate/airflow_panel_wide.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		feetAirflowButton: MFT.IndButton.extend({
			elementId:			'climate_simple_feetAirflowButton',
			action:				'onFeetSimpleAirflow',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.simpleAirflowFeet'),
			icon:				'images/climate/airflow_feet_wide.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		})
	}),
	
	/** Fan controls */
	fanControls: Em.ContainerView.extend({
		elementId:				'climate_simple_fanControls',
		
		classNameBindings:	['controller.model.autoFan.value:automode'],
		
		childViews: [
			'fanPlusButton',
			'fanIndicator',
			'fanMinusButton'
		],

		fanIndicator: MFT.IndButton.extend({
			elementId:			'climate_simple_fanControls_fanIndicator',
			contentBinding:		Em.Binding.oneWay('controller.model.fan'),
			icon:				'images/climate/fan_control/fan_ico.png',
			indActiveClass:		'indf_act',
			indDefaultClass:	'indf_inact',
		}),
		
		fanMinusButton: MFT.Button.extend({
			elementId:			'climate_simple_fanControls_fanMinusButton',
			target:				'controller',
			action:				'onDecFanSpeed',
			icon:				'images/climate/fan_control/fan_minus_control.png',
			timer:				200
		}),
		
		fanPlusButton: MFT.Button.extend({
			elementId:			'climate_simple_fanControls_fanPlusButton',
			target:				'controller',
			action:				'onIncFanSpeed',
			icon:				'images/climate/fan_control/fan_plus_comtrol.png',
			timer:				200
		})
	}),
	
	/** Front bottom controls */
	controls: Em.ContainerView.extend({		
		elementId:			'climate_simple_controls',
		
		childViews: [
			'autoButton',
			'acButton',
			'maxAcButton',
			'recirculationButton',
			'dualButton'
		],

		
		autoButton: MFT.TogButton.extend({
			classNames:			['helpmode_box_shadow'],
			elementId:			'climate_simple_autoButton',
			action:				'onToggleAuto',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_auto'),
			contentBinding:		Em.Binding.oneWay('controller.model.auto')
		}),
		
		acButton: MFT.IndButton.extend({
			elementId:			'climate_simple_acButton',
			action:				'onToggleAc',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_ac'),
			contentBinding:		Em.Binding.oneWay('controller.model.ac'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
					
		maxAcButton: MFT.IndButton.extend({
			elementId:			'climate_simple_maxAcButton',
			classNames:			['helpmode_right_delim','helpmode_box_left_shadow'],
			action:				'onToggleMaxAc',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_maxAc'),
			contentBinding:		Em.Binding.oneWay('controller.model.maxAc'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		recirculationButton: MFT.IndButton.extend({
			elementId:			'climate_simple_recirculationButton',
			classNames:			['helpmode_left_delim','helpmode_right_delim','helpmode_box_shadow'],
			action:				'onToggleRecirculation',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.recirculation'),
			icon:				'images/climate/gang_buttons/recycle_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		dualButton: MFT.IndButton.extend({
			elementId:			'climate_simple_dualButton',
			classNames:			['helpmode_left_delim','helpmode_box_right_shadow'],
			action:				'onToggleDualMode',
			target:				'controller',
			textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_dual'),
			contentBinding:		Em.Binding.oneWay('controller.model.dualMode'),
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		})		
	})
});