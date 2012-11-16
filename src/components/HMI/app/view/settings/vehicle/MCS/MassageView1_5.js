/**
 * @name MFT.seatsMassageView1_5
 * 
 * @desc HMI massage conception view 1.5 for MCS
 * 
 * @category	Controlls
 * @filesource	app/view/settings/MSC/MassageView1_5.js
 * @version		2.0
 *
 * @author	Melnik Andriy
 */

MFT.SeatsMassageView1_5 = Em.ContainerView.extend({

	settingsState: true,

	elementId: 	'seatsMassageView1_5',

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	/** Seats Massage block components */
	childViews: [
		'seatsMassageTop',
		'seatsMassageBottom',
		'messageActionsTop',
		'messageActionsBottom',
		'cushionsDriver',
		'cushionsPassenger',
		'settings',
		'alternatingMassage'
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
				
				/** Push element to container */
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
				
				/** Push element to container */
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
		range:				3,			
		index:				7,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'HI'
		],
	}),

	/** Settings Button */
	settings: MFT.Button.extend({
		classNames:	'btn-48 btn concept1_5Settings',
		id: 		'concept1_5Settings',
		text: 		'Settings',
		action: 	'onMassageCustomOpen',
		target: 	'MFT.MCSController',
		onDown:		false,
		icon:		'images/settings/ico_settings.png'
	}),

	/** Settings button for massage custom adjustment */
	customSettingsButton: MFT.Button.extend({
		elementId:		'seatsMassageView2_1_settingsButton',
		
		classNames:		'btn btn-48',
		text:			'Custom Settings',
		
		action:			'onMassageCustomOpen',
		target:			'MFT.MCSController',
		onDown:			false,
		
		icon:			'images/settings/ico_settings.png'
	}),

	/** Container for Alternating Massage */
	alternatingMassage: Em.ContainerView.extend({

		classNameBindings: ['MFT.MCSController.massageCustom:show'],

		classNames: 'hide concept1_5SettingsScreen',

		childViews: [
			'seatLabel',
			'labelTitle',
			'labelText',
			'massageActionsTop'
		],

		seatLabel: MFT.Label.extend({
			elementId:	'seatsMassageView1_5_seatLabel',
			classNames:	'seatsMassageView1_5_seatLabel',
			content:	function(){
				if( 'driver' == MFT.MCSController.seat){
					return 'Driver';
				}else if( 'passenger' == MFT.MCSController.seat){
					return 'Passenger';
				}
			}.property('MFT.MCSController.seat')
		}),

		/** Label with title text */
		labelTitle: MFT.Label.extend({
			classNames:	'labelTitle',
			content: 		'MCS Options: Alternating Massage Setting'
		}),

		/** Label with question */
		labelText: MFT.Label.extend({
			classNames:	'labelText',
			content: 		'How fast will Lumbar Massage and Active Motion&reg; alternate?'
		}),

		/** Massage mode buttons group for TOP lumbars */
		massageActionsTop: MFT.ButtonsGroup.extend({
			contentBinding:		'MFT.MCSController.activeSeat.7',
			classes:			'concept1_5Massage btn btn-48',
			action:				'onChangeMassageValue',
			delimeters:			false,
			labels: [
				'Slow',
				'Medium',
				'Fast'
			],

			onChangeMassageValue: function(element) {
				MFT.MCSController.set('selectedIndex', 0)
				MFT.MCSController.activeSeat[7].set('value', element.index);
			}
		})
	})
});