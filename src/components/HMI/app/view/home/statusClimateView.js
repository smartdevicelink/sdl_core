/**
 * @name MFT.StatusClimateView
 * 
 * @desc Climate statusbar visual representation
 * 
 * @category	View
 * @filesource	app/view/climate/StatusClimateView.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.StatusClimateView = Em.ContainerView.extend({
	
	elementId:	'status_climate',
	
	classNames: ['status_bar'],
	
	/** Bind class for visual representation */
	classNameBindings: ['MFT.States.climate.active:selected'],
	
	/** Climate components */		
	childViews: [
		'statusOn',
		'statusOff',
		'statusHome'
	],
	
	/** Label for Home screen */
	statusHome: MFT.Label.create({
		elementId:		'status_climate_label',
	
		classNameBindings: ['MFT.States.home.active:visible'],
		
		contentBinding:	    Ember.Binding.oneWay('MFT.locale.label.view_statusClimate_climate')
	}),
	
	/** Off status view */
	statusOff: Em.View.create({
		classNameBindings: ['MFT.States.home.active:hidden_display'],
		
		elementId:		'status_climate_off',
		
		template: Em.Handlebars.compile(
			'{{#if MFT.ClimateController.isOff}}<img class="key" src="images/climate/key.png"">Climate Off{{/if}}'
		)
	}),
	
	/** On status view */
	statusOn: Em.ContainerView.create({
		classNameBindings: ['MFT.States.home.active:hidden_display'],
		
		elementId:		'status_climate_on',
		
		childViews: [
			'driverTemp',
			'passangerTemp',
			'driverSeat',
			'passangerSeat'
		],
		
		/** Driver temperature status */
		driverTemp: MFT.Label.create({
			classNameBindings: ['MFT.ClimateController.isOff:hidden'],
			
			elementId:		'status_climate_driverTemp',
			
			contentBinding:	Em.Binding.oneWay('MFT.ClimateController.model.driverTemp.valueStr')
		}),
		
		/** Passanger temperature status */
		passangerTemp: MFT.Label.create({
			classNameBindings: ['MFT.ClimateController.isOff:hidden'],
			
			elementId:		'status_climate_passangerTemp',
		
			contentBinding:	Em.Binding.oneWay('MFT.ClimateController.model.passangerTemp.valueStr')
		}),
		
		/** Driver seat status */
		driverSeat: Em.View.create({
			
			elementId:			'status_climate_driverSeat',
			
			classNameBindings:	[
				'MFT.ClimateController.isOff:hidden',
				'MFT.ClimateController.isSimple:hidden_display'
			],
						
			heatBinding:		Em.Binding.oneWay('MFT.ClimateController.model.driverHeatedSeat.value'),
			coolBinding:		Em.Binding.oneWay('MFT.ClimateController.model.driverCooledSeat.value'),
			
			isActive: function() {
				( this.cool || this.heat ) ? this.set('active', true) : this.set('active', false);
			}.observes('cool','heat'),
			
			icon: function() {							
				if (this.cool) {
					return 'images/climate/status/cool'+this.cool+'.png';
				}
				if (this.heat) {
					return 'images/climate/status/heat'+this.heat+'.png';
				}
			}.property('cool','heat'),
						
			template: Em.Handlebars.compile(
				'{{#if view.active}}<img {{bindAttr src="view.icon"}}>{{/if}}'
			)
		}),
		
		/** Passanger seat status */
		passangerSeat: Em.View.create({
			
			elementId:			'status_climate_passangerSeat',
			
			classNameBindings:	[
				'MFT.ClimateController.isOff:hidden',
				'MFT.ClimateController.isSimple:hidden_display'
			],
			
			heatBinding:		Em.Binding.oneWay('MFT.ClimateController.model.passangerHeatedSeat.value'),
			coolBinding:		Em.Binding.oneWay('MFT.ClimateController.model.passangerCooledSeat.value'),
			
			isActive: function() {
				( this.cool || this.heat ) ? this.set('active', true) : this.set('active', false);
			}.observes('cool','heat'),
			
			icon: function() {							
				if (this.cool) {
					return 'images/climate/status/cool'+this.cool+'.png';
				}
				if (this.heat) {
					return 'images/climate/status/heat'+this.heat+'.png';
				}
			}.property('cool','heat'),
						
			template: Em.Handlebars.compile(
				'{{#if view.active}}<img {{bindAttr src="view.icon"}}>{{/if}}'
			)
		})
	}),
	
	/**
	 * Select climate state
	 * should be called when user press climate statusbar
	 */
	actionUp: function(event){
		MFT.States.goToState( MFT.ClimateController.get('climateVechicleMode') );
	}
});