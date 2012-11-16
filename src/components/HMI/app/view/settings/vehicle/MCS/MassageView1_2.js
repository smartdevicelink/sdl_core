/**
 * @name MFT.seatsMassageView1_2
 * 
 * @desc MCS massage view component 1.2 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView1_2.js
 * @version		2.0
 *
 * @author	Melnik Andriy	
 */

MFT.SeatsMassageView1_2 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView2',

	/** Seats Massage block components */
	childViews: [
		'seatsMassageTop',
		'seatsMassageBottom',
		'messageActionsTop1',
		'messageActionsTop2',
		'messageActionsBottom1',
		'messageActionsBottom2',
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
					valueBinding: 		'MFT.MCSController.model.driverSeat.'+(i*2-5)+'.value',
					rangeBinding: 		'MFT.MCSController.model.driverSeat.'+(i*2-5)+'.range',
					seat: 				'driver',
					elementId: 			'view_settings_vehicle_MultiContourSeat_driverSeat_cushion'+(i+1),
					isActive: 			function() {
											if ( MFT.MCSController.selectedIndex == this.index*2-6 ) {
												return MFT.MCSController.highlighted;
											} else {
												return false;
											}
										}.property('MFT.MCSController.selectedIndex', 'MFT.MCSController.highlighted'),
					range: 				3
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
					valueBinding: 		'MFT.MCSController.model.passengerSeat.'+(i*2-5)+'.value',
					rangeBinding: 		'MFT.MCSController.model.passengerSeat.'+(i*2-5)+'.range',
					seat: 				'passenger',
					elementId: 			'view_settings_vehicle_MultiContourSeat_passengerSeat_cushion'+(i+1),
					isActive: 			function() {
											if ( MFT.MCSController.selectedIndex == this.index*2-6 ) {
												return MFT.MCSController.highlighted;
											} else {
												return false;
											}
										}.property('MFT.MCSController.selectedIndex', 'MFT.MCSController.highlighted'),
					range: 				3
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
	messageActionsTop1: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
	
		classNameBindings: 	['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-top1',
		valueBinding:		'MFT.MCSController.activeSeat.5.value',
		rangeBinding:		'MFT.MCSController.activeSeat.5.range',
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels:				[
								'OFF',
								'LO',
								'M',
								'HI'
							]
	}),
	
	/** Container for three top buttons on Massage view */
	messageActionsTop2: MFT.ButtonsGroup.extend ( MFT.SelectedIndex, {
	
		classNameBindings: 	['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-top2',
		valueBinding:		'MFT.MCSController.activeSeat.6.value',
		rangeBinding:		'MFT.MCSController.activeSeat.6.range',
		index:				7,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		icons:				[
								'images/settings/massageTop.png',
								'images/settings/massageBottom.png',
								'images/settings/massageVertical.png'
							]
	}),

	/** Container for three bottom buttons on Massage view */
	messageActionsBottom1: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {

		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-bottom1',
		valueBinding:		'MFT.MCSController.activeSeat.7.value',
		rangeBinding:		'MFT.MCSController.activeSeat.7.range',
		index:				8,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels:				[
								'OFF',
								'LO',
								'M',
								'HI'
							]
	}),
	
	/** Container for three bottom buttons on Massage view */
	messageActionsBottom2: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {

		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-bottom2',
		valueBinding:		'MFT.MCSController.activeSeat.8.value',
		rangeBinding:		'MFT.MCSController.activeSeat.8.range',
		index:				9,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		icons:				[
								'images/settings/massageCircle.png',
								'images/settings/massageRound.png',
								'images/settings/massageHorisontal.png'
							]
	})
});