/**
 * @name MFT.seatsMassageView
 * 
 * @desc HMI massage conception view 1.0 for MCS
 * 
 * @category	Controlls
 * @filesource	app/view/settings/MSC/MassageView.js
 * @version		2.0
 *
 * @author	Melnik Andriy
 */

MFT.SeatsMassageView = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	/** Seats Massage block components */
	childViews: [
		'seatsMassageTop',
		'seatsMassageBottom',
		'messageActionsTop',
		'messageActionsBottom',
		'cushionsDriver',
		'cushionsPassenger'
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
					valueBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.value',
					rangeBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.range',
					seat: 				'driver',
					elementId: 			'view_settings_vehicle_MultiContourSeat_driverSeat_cushion'+(i+1),
					isActiveBinding: 	'isSelected'
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
					valueBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.value',
					rangeBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.range',
					seat: 				'passenger',
					elementId: 			'view_settings_vehicle_MultiContourSeat_passengerSeat_cushion'+(i+1),
					isActiveBinding: 	'isSelected'
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
		rangeBinding:		'MFT.MCSController.activeSeat.5.range',
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'HI'
		],
	}),

	/** Container for three bottom buttons on Massage view */
	messageActionsBottom: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-2',
		valueBinding:		'MFT.MCSController.activeSeat.6.value',
		rangeBinding:		'MFT.MCSController.activeSeat.6.range',			
		index:				7,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'HI'
		],
	})
});