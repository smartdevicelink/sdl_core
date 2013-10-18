/**
 * @name MFT.ClimateFullRearView
 * 
 * @desc Climate module visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/full/rearView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ClimateFullRearView = Em.ContainerView.create( MFT.LoadableView, {
	
	controller: MFT.ClimateController,
			
	elementId:	'climate_rearControls',
	
	childViews: [
		'topCaption',
		'controlButton',
		'tempControls',
		'powerButton',
		'fanControls'
	],
	
	topCaption: MFT.Label.extend({
		elementId:	'climate_rearControls_topCaption',	
		contentBinding:	Em.Binding.oneWay('MFT.locale.label.view_climate_rearClimate')
	}),

	/** Rear control button */
	controlButton: MFT.IndButton.extend({
		elementId:			'climate_rearControls_controlButton',
		action:				'onToggleRearControl',
		target:				'controller',
		textBinding: Ember.Binding.oneWay('MFT.locale.label.view_climate_rearControl'),
		contentBinding:		Em.Binding.oneWay('controller.model.rearControl'),
		indActiveClass:		'ind_act',
		indDefaultClass:	'ind_inact',
		onDown: false
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
				'<div {{bindAttr class="controller.model.rearOn.value:hide"}} id="climate_rearOff">{{MFT.locale.label.view_climate_off}}</div>'+
				'<span class="full_label" {{bindAttr class="view.text:show"}}>Full</span>'+
				'<span class="mode_label">{{view.text}}</span>'
            )  
		}),
		
		minusButton: MFT.Button.extend({
			elementId:			'climate_rearControls_tempControls_minusButton',
			target:				'controller',
			action:				'onDecRearTemp',
			icon:				'images/climate/rear_auto/minus_icon.png',
			timer:				200
		}),
		
		indicator: MFT.IndButton.extend({
			elementId:			'climate_rearControls_tempControls_indicator',
			contentBinding:		Em.Binding.oneWay('controller.model.rearTemp'),
			indActiveClass:		'indt_act',
			indDefaultClass:	'indt_inact',
						
			onDown: false,
			
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
			timer:				200
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
});