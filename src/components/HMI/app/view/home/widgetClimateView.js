/**
 * @name MFT.WidgetClimateView
 * 
 * @desc Climate home screen widget visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/WidgetClimateView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.WidgetClimateView = Em.ContainerView.extend({
	
	elementId:	'home_widgetClimate',
	
	childViews: [
		'powerButton',
		'driverTemp',
		'passengerTemp',
		'driverControls',
		'heatedSteerButton',
		'passengerControls'
	],
	
	powerButton: MFT.Button.create({		
		classNameBindings:		[
			'hidden',
			'MFT.ClimateController.isSimple:simple_climate'
		],
		
		hidden: function() {
			if ( MFT.ClimateController.model.on.value ) {
				return true;
			} else if ( MFT.helpMode ) {
				return true;
			} else {
				return false;
			}
		}.property(
			'MFT.ClimateController.model.on.value',
			'MFT.helpMode'
		),
		
		elementId:				'home_widgetClimate_powerButton',
		action:					'onActivate',
		target:					'MFT.ClimateController',
		onDown:					false,
		icon:					'images/climate/power_button/power_icon.png'
	}),
	
	driverTemp: MFT.Label.create({
		classNameBindings:		[
			'MFT.ClimateController.model.on.value:visible',
			'MFT.helpMode:helpmode_blur_text',
			'MFT.ClimateController.isSimple:simple_climate_temp'
		],
		
		elementId:				'home_widgetClimate_driverTemp',
		contentBinding:			Em.Binding.oneWay('MFT.ClimateController.model.driverTemp.valueStr'),		
	}),
	
	passengerTemp: MFT.Label.create({
		classNameBindings:		[
			'MFT.ClimateController.model.on.value:visible',
			'MFT.helpMode:helpmode_blur_text',
			'MFT.ClimateController.isSimple:simple_climate_temp'
		],
		
		elementId:				'home_widgetClimate_passengerTemp',
		contentBinding:			Em.Binding.oneWay('MFT.ClimateController.model.passangerTemp.valueStr'),		
	}),
	
	driverControls: Em.ContainerView.create({
		elementId:		'home_widgetClimate_driverControls',
		
		classNames:		['helpmode_box_shadow'],
		
		classNameBindings: [
			'MFT.ClimateController.isSimple:hidden'
		],
		
		childViews: [
			'heatedSeatButton',
			'cooledSeatButton'
		],
		
		heatedSeatButton: MFT.IndButton.create({
			elementId:			'home_widgetClimate_driverControls_heatedSeatButton',
			classNames:			['helpmode_right_delim'],
			action:				'onChangeDriverHeatedSeat',
			target:				'MFT.ClimateController',
			contentBinding:		Em.Binding.oneWay('MFT.ClimateController.model.driverHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
		}),
		
		cooledSeatButton: MFT.IndButton.create({
			elementId:			'home_widgetClimate_driverControls_cooledSeatButton',
			classNames:			['helpmode_left_delim'],
			action:				'onChangeDriverCooledSeat',
			target:				'MFT.ClimateController',
			contentBinding:		Em.Binding.oneWay('MFT.ClimateController.model.driverCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
		})
	}),
	
	heatedSteerButton: MFT.IndButton.create({
		elementId:			'home_widgetClimate_driverControls_heatedSteerButton',
		classNameBindings: 	[
			'MFT.helpMode: helpmode_blur_cont',
			'MFT.ClimateController.isSimple:hidden'
		],
		action:				'onChangeDriverHeatedSteer',
		target:				'MFT.ClimateController',
		contentBinding:		Em.Binding.oneWay('MFT.ClimateController.model.driverHeatedSteer'),
		icon:				'images/climate/left_person_control/cooledstear_ico.png',
		indActiveClass:		'ind_act',
		indDefaultClass:	'ind_inact',
		disabledBinding:	Em.Binding.oneWay('MFT.helpMode')
	}),
	
	passengerControls: Em.ContainerView.create({
		elementId:			'home_widgetClimate_passengerControls',
		
		classNames:			['helpmode_box_shadow'],
		
		classNameBindings: [
			'MFT.ClimateController.isSimple:hidden'
		],
		
		childViews: [
			'heatedSeatButton',
			'cooledSeatButton',
		],
		
		heatedSeatButton: MFT.IndButton.create({
			elementId:			'home_widgetClimate_passengerControls_heatedSeatButton',
			classNames:			['helpmode_right_delim'],
			action:				'onChangePassangerHeatedSeat',
			target:				'MFT.ClimateController',
			contentBinding:		Em.Binding.oneWay('MFT.ClimateController.model.passangerHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
		}),
		
		cooledSeatButton: MFT.IndButton.create({
			elementId:			'home_widgetClimate_passengerControls_cooledSeatButton',
			classNames:			['helpmode_left_delim'],
			action:				'onChangePassangerCooledSeat',
			target:				'MFT.ClimateController',
			contentBinding:		Em.Binding.oneWay('MFT.ClimateController.model.passangerCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			onDown: false
		})
	})
});