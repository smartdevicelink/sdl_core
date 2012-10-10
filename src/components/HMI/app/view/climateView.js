/**
 * @name MFT.ClimateView
 * 
 * @desc Climate module visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/ClimateView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateView = Em.ContainerView.create({

	elementId:	'climate',
 	
 	classNames:	'hidden',
 	
 	controller: MFT.ClimateController,
	
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.climate.active:active_state'],
		
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
		'rearControls',
		'rearSwitcher'
	],
	
	/** Store temp popup */
	storeTemp: Em.View.extend({
		elementId:	'climate_storeTemp',
		template: Em.Handlebars.compile('My Temp Stored')
	}),
	
	/** Off mode label */
	offLabel: MFT.Label.extend({
		classNameBindings: ['controller.model.on.value:hide'],
		elementId:	'climate_offLabel',
		content:	'OFF'
	}),
	
	/** Rear mode label */
	rearLabel: MFT.Label.extend({
		classNameBindings: ['controller.model.rearOn.value:visible'],
		elementId:	'climate_rearLabelOn',
		content:	'Rear Climate On'
	}),
	
	/** Help mode front */
	climateHelpFront: Em.View.extend({
		elementId:	'climate_help_blur',
		classNameBindings: ['visible'],
		visible: function() {
			if ( MFT.States.climate.front.active &&  MFT.helpMode) {
				return true;
			} else {
				return false;
			}
		}.property('MFT.States.climate.front.active', 'MFT.helpMode')
	}),
	
	/** Help mode rear */
	climateHelpRear: Em.View.extend({
		elementId:	'climate_rear_help_blur',
		classNameBindings: ['visible'],
		visible: function() {
			if ( MFT.States.climate.rear.active &&  MFT.helpMode) {
				return true;
			} else {
				return false;
			}
		}.property('MFT.States.climate.rear.active', 'MFT.helpMode')
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
			classNameBindings:	['controller.isMyTemp:active'],
			icon:				'images/climate/left_person_control/mytemp_ico.png',
			timer:				2000,
			helpMode:			true,
			
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
			classNameBindings: ['MFT.helpMode:divider'],
			elementId:			'climate_driverControls_heatedSeatButton',
			action:				'onChangeDriverHeatedSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.driverHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			helpMode:			true
		}),
		
		cooledSeatButton: MFT.IndButton.extend({
			classNameBindings: ['MFT.helpMode:divider'],
			elementId:			'climate_driverControls_cooledSeatButton',
			action:				'onChangeDriverCooledSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.driverCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			helpMode:			true
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
		
		childViews: [
			'dualModeButton',
			'heatedSeatButton',
			'cooledSeatButton'
		],
		
		dualModeButton: MFT.IndButton.extend({
			classNameBindings: ['MFT.helpMode:divider'],
			elementId:			'climate_passengerControls_dualModeButton',
			action:				'onToggleDualMode',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.dualMode'),
			icon:				'images/climate/right_person/dual_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			helpMode:			true
		}),
		
		heatedSeatButton: MFT.IndButton.extend({
			classNameBindings: ['MFT.helpMode:divider'],
			elementId:			'climate_passengerControls_heatedSeatButton',
			action:				'onChangePassangerHeatedSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.passangerHeatedSeat'),
			icon:				'images/climate/left_person_control/heatedseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			helpMode:			true
		}),
		
		cooledSeatButton: MFT.IndButton.extend({
			classNameBindings: ['MFT.helpMode:divider'],
			elementId:			'climate_passengerControls_cooledSeatButton',
			action:				'onChangePassangerCooledSeat',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.passangerCooledSeat'),
			icon:				'images/climate/left_person_control/cooledseat_ico.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			helpMode:			true
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
		classNameBindings: ['MFT.States.climate.rear.active:hidden'],
		
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
			action:				'onToggleAuto',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.auto'),
			icon:				'images/climate/gang_buttons/auto_ico.png',
			helpMode:			true
		}),
		
		acButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_acButton',
			action:				'onToggleAc',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.ac'),
			icon:				'images/climate/gang_buttons/ac_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		maxAcButton: MFT.IndButton.extend({
			classNameBindings: ['MFT.helpMode:divider'],
			elementId:			'climate_frontControls_maxAcButton',
			action:				'onToggleMaxAc',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.maxAc'),
			icon:				'images/climate/gang_buttons/maxac_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			helpMode:			true
		}),
		
		recirculationButton: MFT.IndButton.extend({
			elementId:			'climate_frontControls_recirculationButton',
			action:				'onToggleRecirculation',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.recirculation'),
			icon:				'images/climate/gang_buttons/recycle_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			helpMode:			true
		})
	}),
	
	/** Rear controls */
	rearControls: Em.ContainerView.extend({
		classNameBindings: ['MFT.States.climate.front.active:hidden'],
		
		elementId:			'climate_rearControls',
		
		childViews: [
			'topCaption',
			'controlButton',
			'tempControls',
			'powerButton',
			'fanControls'
		],
		
		topCaption: Em.View.extend({
			elementId:	'climate_rearControls_topCaption',	
			template: Em.Handlebars.compile('<img class="rear_ico" src="images/climate/rear_auto/rear_climat_ico.png">')
		}),
		
		/** Rear control button */
		controlButton: MFT.IndButton.extend({
			elementId:			'climate_rearControls_controlButton',
			action:				'onToggleRearControl',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.rearControl'),
			icon:				'images/climate/rear_auto/rear_control_but_ico.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact',
			helpMode:			true
		}),
		
		/** Rear temperature controls */
		tempControls: Em.ContainerView.extend({
			elementId:			'climate_rearControls_tempControls',
			
			childViews: [
				'labels',
				'minusButton',
				'indicator',
				'plusButton'
			],
			
			labels: Em.View.extend({
				elementId:		'climate_rearControls_tempControls_labels',
				classNameBindings: ['MFT.helpMode:pointer'],
				
				text: function() {				
					if ( this.get('controller').model.rearTemp.enabled ) {
						if ( this.get('controller').isRearFullHeated ) {
							return 'Heat';
						}
						if ( this.get('controller').isRearFullCooled ) {
							return 'Cool';
						}
					} else {
						return '';
					}
				}.property(
					'controller.model.rearTemp.enabled',
					'controller.isRearFullHeated',
					'controller.isRearFullCooled'
				),
				
				template: Em.Handlebars.compile(
					'<div {{bindAttr class="controller.model.rearOn.value:hide"}} id="climate_rearOff">OFF</div>'+
					'<span class="full_label" {{bindAttr class="view.text:show"}}>Full</span>'+
					'<span class="mode_label">{{view.text}}</span>'
	            )  
			}),
			
			minusButton: MFT.Button.extend({
				elementId:			'climate_rearControls_tempControls_minusButton',
				target:				'controller',
				action:				'onDecRearTemp',
				icon:				'images/climate/rear_auto/minus_icon.png',
				timer:				200,
				helpMode:			true
			}),
			
			indicator: MFT.IndButton.extend({
				elementId:			'climate_rearControls_tempControls_indicator',
				contentBinding:		Em.Binding.oneWay('controller.model.rearTemp'),
				indActiveClass:		'indt_act',
				indDefaultClass:	'indt_inact',
				target:				'controller',
				action:				'onRearTempAction',
				helpMode:			true,
				
				classNameBindings: ['MFT.helpMode:pointer'],
				
				layout: Em.Handlebars.compile(
					'<div {{bindAttr class="controller.model.rearOn.value:show"}} id="climate_rearControls_tempControls_indicator_dot"></div>'+
					'{{yield}}'
				),
				
				// Override toggle indicator handeler 
				toggleIndicators: function() {
					
					var length = this.indicators.length,
						i;
					
					for ( i=0; i < length; i++ ) {
						// middle 'dot' state
						if ( i == 4 ) {
							this.indicators[i].set('className', 'indt_dot');
						} else {
							// cooled state
							if ( this.content.value <= 5  ) {
								if ( (i >= this.content.value-1) && (i < 5)) {
									if ( (i == 0) || (i == length-1) ) {
										this.indicators[i].set('className', 'mft_indicator ' + this.indActiveClass + '_long')
									} else {
										this.indicators[i].set('className', 'mft_indicator ' + this.indActiveClass);
									}
										
								} else {
									if ( (i == 0) || (i == length-1) ) {
										this.indicators[i].set('className', 'mft_indicator ' + this.indDefaultClass + '_long')
									} else {
										this.indicators[i].set('className', 'mft_indicator ' + this.indDefaultClass);
									}
								}
							}
							// heated state
							if ( this.content.value > 5  ) {
								if ( (i >= this.content.value) || (i < 5) ) {
									if ( (i == 0) || (i == length-1) ) {
										this.indicators[i].set('className', 'mft_indicator ' + this.indDefaultClass + '_long')
									} else {
										this.indicators[i].set('className', 'mft_indicator ' + this.indDefaultClass);
									}
								} else {
									if ( (i == 0) || (i == length-1) ) {
										this.indicators[i].set('className', 'mft_indicator ' + this.indActiveClass + '_long')
									} else {
										this.indicators[i].set('className', 'mft_indicator ' + this.indActiveClass);
									}
								}
							}
						}
					}
				}	
			}),
			
			plusButton: MFT.Button.extend({
				elementId:			'climate_rearControls_tempControls_plusButton',
				target:				'controller',
				action:				'onIncRearTemp',
				icon:				'images/climate/rear_auto/plus_icon.png',
				timer:				200,
				helpMode:			true
			})			
		}),
		
		/** Rear on/off toggle button */
		powerButton: MFT.IndButton.extend({
			elementId:			'climate_rearControls_powerButton',
			action:				'onToggleRearPower',
			target:				'controller',
			contentBinding:		Em.Binding.oneWay('controller.model.rearOn'),
			icon:				'images/climate/power_button/power_icon.png',
			indActiveClass:		'ind_act',
			indDefaultClass:	'ind_inact'
		}),
		
		/** Rear fan speed controls */
		fanControls: Em.ContainerView.extend({
			elementId:			'climate_rearControls_fanControls',
			
			childViews: [
				'minusButton',
				'indicator',
				'plusButton'
			],
	
			indicator: MFT.IndButton.extend({
				elementId:			'climate_rearControls_fanControls_indicator',
				contentBinding:		Em.Binding.oneWay('controller.model.rearFan'),
				icon:				'images/climate/fan_control/fan_ico.png',
				indActiveClass:		'indr_act',
				indDefaultClass:	'indr_inact',
				disable:			true
			}),
			
			minusButton: MFT.Button.extend({
				elementId:			'climate_rearControls_fanControls_minusButton',
				target:				'controller',
				action:				'onDecRearFanSpeed',
				icon:				'images/climate/fan_control/fan_minus_control.png',
				timer:				200
			}),
			
			plusButton: MFT.Button.extend({
				elementId:			'climate_rearControls_fanControls_plusButton',
				target:				'controller',
				action:				'onIncRearFanSpeed',
				icon:				'images/climate/fan_control/fan_plus_comtrol.png',
				timer:				200
			})
		})
	}),
	
	/** Rear controls switch button */
	rearSwitcher: MFT.Button.extend({
		elementId:				'climate_rearSwitcher',
		action:					'onToggleRear',
		target:					'controller',
		icon:					'images/climate/rear_auto/rear_but_icon.png',
		onDown: false,
		
		layout: Em.Handlebars.compile('{{yield}}'+
			'<img class="rear_arrow closed" {{bindAttr class="MFT.States.climate.rear.active:hide"}} src="images/climate/rear_auto/rear_but_closed.png">'+
			'<img class="rear_arrow closed" {{bindAttr class="MFT.States.climate.front.active:hide"}} src="images/climate/rear_auto/rear_but_opened.png">'
		)
	})
});