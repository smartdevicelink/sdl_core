/**
 * @name MFT.SeatsMassageView5_2
 * 
 * @desc MCS massage view component 5.2 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView5_2.js
 * @version		2.0
 *
 * @author	Artem Petrosyan	
 */

MFT.SeatsMassageView5_2 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView5_2',

	childViews: [
		'cushionsDriver',
		'cushionsPassenger',
		'seatsMassageTop',
		'seatsMassageBottom',
		'customAdjust',
		'messageActionsTop',
		'messageActionsBottom',
		'customIntensityButton'
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

	/** Custom adjustment massage */
	customAdjust: Em.ContainerView.extend({

		elementId:		'MultiContourSeat_customMassage',

		classNameBindings: ['MFT.MCSController.massageCustom:visible'],

		childViews: [
			'caption',
			'seatLabel',
			'topLabel',
			'topActions',
			'bottomLabel',
			'bottomActions'
		],
		
		caption: MFT.Label.extend({
			elementId:	'MultiContourSeat_customMassage_caption_5_2',
			classNames:	'MultiContourSeat_customMassage_caption',
			content:	'MCS Options: Scheduled Massage Setting'
		}),
		
		seatLabel: MFT.Label.extend({
			elementId:	'seatsMassageView5_2_seatLabel',
			classNames:	'seatsMassageView5_2_seatLabel',
			content:	function(){
				if( 'driver' == MFT.MCSController.seat){
					return 'Driver';
				}else if( 'passenger' == MFT.MCSController.seat){
					return 'Passenger';
				}
			}.property('MFT.MCSController.seat')
		}),
		
		topLabel: MFT.Label.extend({
			elementId:	'MultiContourSeat_customMassage_5_2_topLabel',
			content:	'Duration of Massage (minutes)'
		}),
		
		topActions: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
			classNameBindings: ['isSelected:active'],
			classNames:			'seats-message-actions seats-message-actions-3',		
			value:				2,
			range:				5,
			index:				10,
			classes:			'quality',
			action:				'onChangeMassageValue',
			labels: [
				'5',
				'10',
				'20',
				'30',
				'40'
			],

			controller:			MFT.MCSController,

			seatValue: {
				driver:		2,
				passenger:	2
			},

			onChangeMassageValue: function(el) {
				this.set('value', el.index);

				// highlight cushions
				this.controller.set( 'selectedIndex', 10 );

				// save value
				this.seatValue[MFT.MCSController.seat] = el.index;
			},

			onChangeSeat: function() {
				this.set('value', this.seatValue[this.controller.seat] );
			}.observes('this.controller.seat')
		}),
		
		bottomLabel: MFT.Label.extend({
			elementId:	'MultiContourSeat_customMassage_5_2_bottomLabel',
			content:	'Restart Massage after (minutes)'
		}),

		bottomActions: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
			classNameBindings: ['isSelected:active'],
			classNames:			'seats-message-actions seats-message-actions-4',		
			value:				4,
			range:				5,
			index:				11,
			classes:			'quality',
			action:				'onChangeMassageValue',
			labels: [
				'5',
				'10',
				'20',
				'30',
				'OFF'
			],

			controller:			MFT.MCSController,

			seatValue: {
				driver:		4,
				passenger:	4
			},

			onChangeMassageValue: function(el) {
				this.set('value', el.index);

				// highlight cushions
				this.controller.set( 'selectedIndex', 11 );

				// save value
				this.seatValue[MFT.MCSController.seat] = el.index;
			},

			onChangeSeat: function() {		
				this.set('value', this.seatValue[this.controller.seat] );
			}.observes('this.controller.seat')
		})
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
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'HI'
		]
	}),

	/** Settings button for massage custom adjustment */
	customIntensityButton: MFT.Button.extend({
		elementId:		'seatsMassageView4_1_settingsButton',
		
		classNames:		'btn btn-48',
		text:			'Settings',
		
		action:			'onMassageCustomOpen',
		target:			'MFT.MCSController',
		onDown:			false,
		
		icon:			'images/settings/ico_settings.png'
	})
});