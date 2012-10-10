/**
 * @name MFT.seatsMassageView2
 * 
 * @desc MCS massage view component 1.2 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView2.js
 * @version		2.0
 *
 * @author	Melnik Andriy	
 */

MFT.SeatsMassageView1_3 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView1_3',

	/** Seats Massage block components */
	childViews: [
		'messageActionsTop1',
		'patternTopText',
		'messageActionsTop2',
		'patternBottomText',
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
	
	messageActionsTop1: Em.ContainerView.extend( MFT.SelectedIndex, {

		classNames:	'seats-message-actions-top1',

		/** Seats Massage block components */
		childViews: [
			'indicatorButton',
			'patternTopText'
		],

		index:		6,
		
		patternTopText: MFT.Label.extend({
			classNames:		'patternTextIndicator',

			contentBinding: 'patternTopTextContent',
			
			patternTopTextContent: function(){

				if( 0 == MFT.MCSController.get('activeSeat')[5].value){
					return "OFF";
				}else if( 1 == MFT.MCSController.get('activeSeat')[5].value){
					return "LO";
				}else if( 2 == MFT.MCSController.get('activeSeat')[5].value){
					return "M";
				}else if( 3 == MFT.MCSController.get('activeSeat')[5].value){
					return "HI";
				}

			}.property('MFT.MCSController.activeSeat.5.value')
		}),

		/** Container for three top buttons on Massage view */
		indicatorButton: MFT.IndButton.extend({

			classNameBindings: 	'parentView.isSelected:active',
			classNames:			'indButtonMCS',
			action:				'onIncrease',
			target:				'MFT.MCSController',
			contentBinding:		'MFT.MCSController.activeSeat.5',
			icon:				'images/settings/seats-message-top.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			helpMode:			true,
			actionDown:	function() {
				MFT.MCSController.set('highlighted', true);
				this._super();
			},

			actionUp:	function() {
				this._super();
				
				if (MFT.CAN_SIMULATION_MODE) {
					MFT.MCSController.model.send(this._parentView.index,'off');
				}
				MFT.MCSController.set('highlighted', false);
			},

			mouseLeave:	function() {

				if (MFT.CAN_SIMULATION_MODE && this.pressed) {
					MFT.MCSController.model.send(this._parentView.index,'off');
				};
				MFT.MCSController.set('highlighted', false);
				
				this._super();
			}

		})
	}),
	
	/** Label shows patterns activity */
	patternTopText: MFT.Label.extend({
		classNames:		'patternTextTop',

		contentBinding: 'patternTopTextContent',
		
		patternTopTextContent: function(){

			return 'pattern ' + (MFT.MCSController.get('activeSeat')[6].value + 1) + ' selected';

		}.property('MFT.MCSController.activeSeat.6.value')
	}),

	/** Container for three top buttons with images on Massage view */
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

	/** Label shows patterns activity */
	patternBottomText: MFT.Label.extend({
		classNames:		'patternTextBottom',

		contentBinding: 'patternTopTextContent',
		
		patternTopTextContent: function(){

			return 'pattern ' + (MFT.MCSController.get('activeSeat')[8].value + 1) + ' selected';

		}.property('MFT.MCSController.activeSeat.8.value')
	}),

	/** Container for three bottom buttons on Massage view */
	messageActionsBottom1: Em.ContainerView.extend( MFT.SelectedIndex, {
	
		classNames:	'seats-message-actions-bottom1',

		/** Seats Massage block components */
		childViews: [
			'indicatorButton',
			'patternTopText'
		],

		index:		8,
		
		patternTopText: MFT.Label.extend({
			classNames:		'patternTextIndicator',

			contentBinding: 'patternTopTextContent',
			
			patternTopTextContent: function(){

				if( 0 == MFT.MCSController.get('activeSeat')[7].value){
					return "OFF";
				}else if( 1 == MFT.MCSController.get('activeSeat')[7].value){
					return "LO";
				}else if( 2 == MFT.MCSController.get('activeSeat')[7].value){
					return "M";
				}else if( 3 == MFT.MCSController.get('activeSeat')[7].value){
					return "HI";
				}

			}.property('MFT.MCSController.activeSeat.7.value')
		}),

		/** Container for three top buttons on Massage view */
		indicatorButton: MFT.IndButton.extend({

			classNameBindings: 	'parentView.isSelected:active',
			classNames:			'indButtonMCS',
			action:				'onIncrease',
			target:				'MFT.MCSController',
			contentBinding:		'MFT.MCSController.activeSeat.7',
			icon:				'images/settings/seats-message-bottom.png',
			indActiveClass:		'small_ind_active',
			indDefaultClass:	'small_ind_def',
			helpMode:			true,
			actionDown:	function() {
				MFT.MCSController.set('highlighted', true);
				this._super();
			},

			actionUp:	function() {
				this._super();
				
				if (MFT.CAN_SIMULATION_MODE) {
					MFT.MCSController.model.send(this._parentView.index,'off');
				}
				MFT.MCSController.set('highlighted', false);
			},

			mouseLeave:	function() {

				if (MFT.CAN_SIMULATION_MODE && this.pressed) {
					MFT.MCSController.model.send(this._parentView.index,'off');
				};
				MFT.MCSController.set('highlighted', false);
				
				this._super();
			}

		})
	}),
	
	/** Container for three bottom buttons with images on Massage view */
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