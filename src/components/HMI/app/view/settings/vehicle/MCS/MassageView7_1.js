/**
 * @name MFT.seatsMassageView7_1
 * 
 * @desc MCS massage view component 7.1 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView7_1.js
 * @version		2.0
 *
 * @author	Artem Petrosyan	
 */

MFT.SeatsMassageView7_1 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView7_1',

	childViews: [
		'cushionsDriver',
		'cushionsPassenger',
		'seatsMassageTop',
		'seatsMassageBottom',
		'messageActionsTop',
		'messageActionsBottom',
		'customMySettings',
		'captionMySettings',
		'mySettingsLabel',
		'customSettings'
	],

	/** Driver seat lumbars */
	cushionsDriver: Em.ContainerView.extend({

		classNameBindings: ['MFT.MCSController.seatsPassengerSwitching:not_selected'],

		afterRender: function() {
			var element;

			for ( var i = 5; i < 7; i++ ) {

				element = MFT.Cushion.create( MFT.SelectedIndex, {
					index:		i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		Ember.Binding.oneWay('MFT.MCSController.model.driverSeat.'+i+'.value'),
					rangeBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.range',
					seat: 				'driver',
					elementId: 			'view_settings_vehicle_MultiContourSeat_driverSeat_cushion'+(i+1),
					isActiveBinding: 	'isSelected'
				});

				// Push element to container
				this.get('childViews').pushObject(element);
			}
			
			for ( var i = 8; i < 13; i++ ) {

				element = MFT.Cushion.create( MFT.SelectedIndex, {
					index:				i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.value',
					rangeBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.range',
					seat: 				'driver',
					elementId: 			'view_settings_vehicle_MultiContourSeat_customMassage_driverSeat_cushion'+(i-2),
					isActiveBinding: 	'isSelected',
					popUpHide:			function() {
						if( true == MFT.MCSController.get('mcsAdjustDisableDriver') ){
							this.set('value', 2);
						}else if( false == MFT.MCSController.get('mcsAdjustDisableDriver') ){
							this.set('value', 0);
						}
					}.observes('MFT.MCSController.mcsAdjustDisableDriver')
				});

				// Push element to container
				this.get('childViews').pushObject(element);
			}
			
		}
	}),

	/** Passenger seat lumbars */
	cushionsPassenger: Em.ContainerView.extend({

		classNameBindings: ['MFT.MCSController.seatsDriverSwitching:not_selected'],

		afterRender: function() { 
			var element;

			for ( var i = 5; i < 7; i++ ) {

				element = MFT.Cushion.create( MFT.SelectedIndex, {
					index:		i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		Ember.Binding.oneWay('MFT.MCSController.model.passengerSeat.'+i+'.value'),
					rangeBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.range',
					seat: 				'passenger',
					elementId: 			'view_settings_vehicle_MultiContourSeat_passengerSeat_cushion'+(i+1),
					isActiveBinding: 	'isSelected'
				});

				// Push element to container
				this.get('childViews').pushObject(element);
			}
			
			for ( var i = 8; i < 13; i++ ) {

				element = MFT.Cushion.create( MFT.SelectedIndex, {
					index:				i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.value',
					rangeBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.range',
					seat: 				'driver',
					elementId: 			'view_settings_vehicle_MultiContourSeat_customMassage_passengerSeat_cushion'+(i-2),
					isActiveBinding: 	'isSelected',
					popUpHide:			function() {
						if( true == MFT.MCSController.get('mcsAdjustDisablePassenger') ){
							this.set('value', 2);
						}else if( false == MFT.MCSController.get('mcsAdjustDisablePassenger') ){
							this.set('value', 0);
						}
					}.observes('MFT.MCSController.mcsAdjustDisablePassenger')
				});

				// Push element to container
				this.get('childViews').pushObject(element);
			}
		}
	}),

	/** Image of right top seat on Massage view */
	seatsMassageTop: Em.View.extend({
		classNames:	'seats-message-1'
	}),

	/** Image of right bottom seat on Massage view */
	seatsMassageBottom: Em.View.extend({
		classNames:	'seats-message-2'
	}),

	/** Container for three top buttons on Massage view */
	messageActionsTop: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: 	['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-1',
		valueBinding:		'MFT.MCSController.activeSeat.5.value',
		range:				3,
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		click:				function(){
			MFT.MCSController.activeSeat[7].set('value', 0);
			MFT.MCSController.set('mcsAdjustDisable', false);
			if( MFT.MCSController.get('seat') == 'driver' ){
				MFT.MCSController.set('mcsAdjustDisableDriver', false);
			}else if( MFT.MCSController.get('seat') == 'passenger' ){
				MFT.MCSController.set('mcsAdjustDisablePassenger', false);
			}
		},
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'HI'
		]
	}),

	/** Container for three bottom buttons on Massage view */
	messageActionsBottom: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-2',
		valueBinding:		'MFT.MCSController.activeSeat.6.value',
		range:				3,
		index:				7,
		classes:			'quality',
		action:				'onChangeMassageValue',
		click:				function(){
			MFT.MCSController.activeSeat[7].set('value', 0);
			MFT.MCSController.set('mcsAdjustDisable', false);
			if( MFT.MCSController.get('seat') == 'driver' ){
				MFT.MCSController.set('mcsAdjustDisableDriver', false);
			}else if( MFT.MCSController.get('seat') == 'passenger' ){
				MFT.MCSController.set('mcsAdjustDisablePassenger', false);
			}
		},
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'HI'
		]
	}),

	/** Container for two bottom buttons on Massage view */
	customMySettings: MFT.ButtonsGroup.extend({
		classNames:			'btn btn-48 seatsMassageView_settingsButton',
		valueBinding:		'MFT.MCSController.activeSeat.7.value',
		range:				2,
		index:				null,
		classes:			'quality',
		action:				function(){
			MFT.MCSController.set('selectedIndex', null);
			if( 0 == this.get('value') ){
				
				//MFT.MCSController.activeSeat[7].set('value', 0);
				if( false != MFT.MCSController.get('mcsAdjustDisable') ){
					MFT.MCSController.activeSeat[6].set('value', 0);
					MFT.MCSController.activeSeat[5].set('value', 0);
					MFT.MCSController.set('mcsAdjustDisable', false);
					if( MFT.MCSController.get('seat') == 'driver' ){
						MFT.MCSController.set('mcsAdjustDisableDriver', false);
					}else if( MFT.MCSController.get('seat') == 'passenger' ){
						MFT.MCSController.set('mcsAdjustDisablePassenger', false);
					}
				}
			}else if( 1 == this.get('value') ){
				MFT.MCSController.set('mcsAdjustDisable', true);
				if( MFT.MCSController.get('seat') == 'driver' ){
					MFT.MCSController.set('mcsAdjustDisableDriver', true);
				}else if( MFT.MCSController.get('seat') == 'passenger' ){
					MFT.MCSController.set('mcsAdjustDisablePassenger', true);
				}
				//MFT.MCSController.activeSeat[7].set('value', 1);
				MFT.MCSController.activeSeat[6].set('value', 0);
				MFT.MCSController.activeSeat[5].set('value', 0);
			}
		},
		labels: [
			'OFF',
			'ON'
		]
	}),

	captionMySettings: MFT.Label.extend({
		elementId:	'captionMySettings',
		classNames:	'captionMySettings',
		content:	'MyMassage'
	}),	

	/** Settings button for massage custom adjustment */
	customSettings: MFT.Button.extend({
		classNameBindings:	['disabled'],
		disabled:			function(){
			if( MFT.MCSController.get('seat') == 'driver' &&  true == MFT.MCSController.get('mcsAdjustDisableDriver')){
				return true;
			}else if( MFT.MCSController.get('seat') == 'passenger' &&  true == MFT.MCSController.get('mcsAdjustDisablePassenger')){
				return true;
			}else {
				return false;
			}
		}.property('MFT.MCSController.mcsAdjustDisablePassenger', 'MFT.MCSController.mcsAdjustDisableDriver', 'MFT.MCSController.seat'),
		classNames:			'btn btn-48 seatsMassageView_settingsButton2 ',
		action:				'onStartMCSSettings',
		target:				'MFT.MCSController',
		icon:				'images/settings/ico_settings.png'
	}),

	/** Settings popup for massage custom adjustment */
	mySettingsLabel: Em.ContainerView.extend({

		classNames:			'mySettingsLabel hide',

		classNameBindings:	['hided:show'],
		
		firstStart:					true,

		hided:				function(){
			if(true == this.get('firstStart')){
				this.set('firstStart', false);
			}
			if(MFT.MCSController.activeSeat[7].value == 1 && this.get('firstStart') != true){
				return true;
			}else{
				return false;
			}
		}.property('MFT.MCSController.activeSeat.7.value'),

		childViews: [
			'caption'
		],
		
		
		
		caption: MFT.Label.extend({
			classNameBindings:	['MFT.MCSController.seatsAdjustModeSwitching:hide'],
			elementId:	'mySettingsPopUpTitle',
			classNames:	'mySettingsPopUpTitle',
			content:	'Active'
		})

	}),

	beforeRender: function() {

		MFT.MassageView7_1_appendix.create().appendTo('body');

		// view internal call
		this.applyAttributesToBuffer(this.buffer);
	}
});