/**
 * @name MFT.seatsMassageView1_1
 * 
 * @desc HMI massage conception view 1.1 for MCS
 * 
 * @category	Controlls
 * @filesource	app/view/settings/MSC/MassageView1_1.js
 * @version		2.0
 *
 * @author	Melnik Andriy
 */

MFT.SeatsMassageView1_1 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView1',

	/** Seats Massage block components */
	childViews: [
		'massageTop1',
		'massageTop2',
		'massageTop3',
		'dividerBlocks',
		'massageBottom1',
		'massageBottom2',
		'massageBottom3',
		'seatsMassageTop',
		'seatsMassageBottom',
		'messageActionsTop1',
		'messageActionsTop2',
		'messageActionsTop3',
		'messageActionsBottom1',
		'messageActionsBottom2',
		'messageActionsBottom3',
		'cushionsDriver',
		'cushionsPassenger'
	],
	
	/** Property switches between two group buttons */
	seatsMassageActiveSeatPassenger5: function(){
		if (0 != MFT.MCSController.model.passengerSeat[5].get('value')){
			return MFT.MCSController.model.passengerSeat[5].get('value');
		}else if (0 != MFT.MCSController.model.passengerSeat[7].get('value')){
			return MFT.MCSController.model.passengerSeat[7].get('value');
		}else if (0 != MFT.MCSController.model.passengerSeat[9].get('value')){
			return MFT.MCSController.model.passengerSeat[9].get('value');
		}
	}.property('MFT.MCSController.model.passengerSeat.5.value', 'MFT.MCSController.model.passengerSeat.7.value', 'MFT.MCSController.model.passengerSeat.9.value'),
	
	/** Property switches between two group buttons */
	seatsMassageActiveSeatPassenger6: function(){
		if (0 != MFT.MCSController.model.passengerSeat[6].get('value')){
			return MFT.MCSController.model.passengerSeat[6].get('value');
		}else if (0 != MFT.MCSController.model.passengerSeat[8].get('value')){
			return MFT.MCSController.model.passengerSeat[8].get('value');
		}else if (0 != MFT.MCSController.model.passengerSeat[10].get('value')){
			return MFT.MCSController.model.passengerSeat[10].get('value');
		}
	}.property('MFT.MCSController.model.passengerSeat.6.value', 'MFT.MCSController.model.passengerSeat.8.value', 'MFT.MCSController.model.passengerSeat.10.value'),
	
	/** Property switches between two group buttons */
	seatsMassageActiveSeatDriver5: function(){
		if (0 != MFT.MCSController.model.driverSeat[5].get('value')){
			return MFT.MCSController.model.driverSeat[5].get('value');
		}else if (0 != MFT.MCSController.model.driverSeat[7].get('value')){
			return MFT.MCSController.model.driverSeat[7].get('value');
		}else if (0 != MFT.MCSController.model.driverSeat[9].get('value')){
			return MFT.MCSController.model.driverSeat[9].get('value');
		}
	}.property('MFT.MCSController.model.driverSeat.5.value', 'MFT.MCSController.model.driverSeat.7.value', 'MFT.MCSController.model.driverSeat.9.value'),
	
	/** Property switches between two group buttons */
	seatsMassageActiveSeatDriver6: function(){
		if (0 != MFT.MCSController.model.driverSeat[6].get('value')){
			return MFT.MCSController.model.driverSeat[6].get('value');
		}else if (0 != MFT.MCSController.model.driverSeat[8].get('value')){
			return MFT.MCSController.model.driverSeat[8].get('value');
		}else if (0 != MFT.MCSController.model.driverSeat[10].get('value')){
			return MFT.MCSController.model.driverSeat[10].get('value');
		}
	}.property('MFT.MCSController.model.driverSeat.6.value', 'MFT.MCSController.model.driverSeat.8.value', 'MFT.MCSController.model.driverSeat.10.value'),

	/** Image of left icon top first group of buttons on Massage view */
	massageTop1: Em.View.extend({
		classNames:	'massageTop1'
	}),

	/** Image of left icon top second group of buttons on Massage view */
	massageTop2: Em.View.extend({
		classNames:	'massageTop2'
	}),

	/** Image of left icon top second group of buttons on Massage view */
	massageTop3: Em.View.extend({
		classNames:	'massageTop3'
	}),

	/** Image of left icon bottom first group of buttons on Massage view */
	massageBottom1: Em.View.extend({
		classNames:	'massageBottom1'
	}),

	/** Image of left icon bottom second group of buttons on Massage view */
	massageBottom2: Em.View.extend({
		classNames:	'massageBottom2'
	}),

	/** Image of left icon bottom second group of buttons on Massage view */
	massageBottom3: Em.View.extend({
		classNames:	'massageBottom3'
	}),

	/** Driver seat lumbars */
	cushionsDriver: Em.ContainerView.extend({
		
		classNameBindings: ['MFT.MCSController.seatsPassengerSwitching:not_selected'],

		afterRender: function() {
			var element;
					
			for ( var i = 5; i < 7; i++ ) {
				
				element = MFT.Cushion.create({
					index:		i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		'this.parentView.parentView.seatsMassageActiveSeatDriver' + i,
					rangeBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.range',
					seat: 				'driver',
					elementId: 			'view_settings_vehicle_MultiContourSeat_driverSeat_cushion'+(i+1),
					isActive: 			function() {
						if ( (MFT.MCSController.selectedIndex == this.index) || (MFT.MCSController.selectedIndex == this.index+2) || (MFT.MCSController.selectedIndex == this.index+4)){
							return MFT.MCSController.highlighted;
						} else {
							return false;
						}
					}.property('MFT.MCSController.selectedIndex', 'MFT.MCSController.highlighted')
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
				
				element = MFT.Cushion.create({
					index:		i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		'this.parentView.parentView.seatsMassageActiveSeatPassenger' + i,
					rangeBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.range',
					seat: 				'passenger',
					elementId: 			'view_settings_vehicle_MultiContourSeat_passengerSeat_cushion'+(i+1),
					isActive: 			function() {
						if ( (MFT.MCSController.selectedIndex == this.index) || (MFT.MCSController.selectedIndex == this.index+2) ){
							return MFT.MCSController.highlighted;
						} else {
							return false;
						}
					}.property('MFT.MCSController.selectedIndex', 'MFT.MCSController.highlighted')
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
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-top1',
		valueBinding:		'MFT.MCSController.activeSeat.5.value',
		range:				4,
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'M',
			'HI'
		]
	}),
	
	/** Container for three top buttons on Massage view */
	messageActionsTop2: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-top2',
		valueBinding:		'MFT.MCSController.activeSeat.7.value',
		range:				4,
		index:				8,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'M',
			'HI'
		]
	}),
	
	/** Container for three top buttons on Massage view */
	messageActionsTop3: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-top3',
		valueBinding:		'MFT.MCSController.activeSeat.9.value',
		range:				4,
		index:				10,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'M',
			'HI'
		]
	}),

	/** Divide two zones by a line  */
	dividerBlocks: Em.View.extend( {
		classNames:			'dividerBlocks'
	}),

	/** Container for three bottom buttons on Massage view */
	messageActionsBottom1: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-bottom1',
		valueBinding:		'MFT.MCSController.activeSeat.6.value',
		range:				4,
		index:				7,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
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
		range:				4,
		index:				9,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'M',
			'HI'
		]
	}),
	
	/** Container for three bottom buttons on Massage view */
	messageActionsBottom3: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-bottom3',
		valueBinding:		'MFT.MCSController.activeSeat.10.value',
		range:				4,
		index:				11,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'M',
			'HI'
		]
	})
});