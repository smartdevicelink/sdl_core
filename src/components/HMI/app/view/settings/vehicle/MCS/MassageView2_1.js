/**
 * @name MFT.seatsMassageView2_1
 * 
 * @desc MCS massage view component 2.1 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView2_1.js
 * @version		2.0
 *
 * @author	Artem Petrosyan	
 */

MFT.SeatsMassageView2_1 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView2_1',
	
	childViews: [
		'cushionsDriver',
		'cushionsPassenger',
		'seatsMassageTop',
		'seatsMassageBottom',
		'massageActionsTop',
		'massageActionsBottom',
		'customSettingsButton',
		'customAdjust'
	],
	
	seatsSwitching2: function() {
				if( false == MFT.MCSController.massageCustom ){
					MFT.MCSController.onMassageCustomPreview(true);
				}
	}.observes('MFT.MCSController.massageCustom'),
	
	/** Driver seat lumbars */
	cushionsDriver: Em.ContainerView.extend({
		
		classNameBindings: ['MFT.MCSController.seatsPassengerSwitching:not_selected'],
		
		afterRender: function() {
			var element;
								
			for ( var i = 5; i < 10; i++ ) {
				
				element = MFT.Cushion.create( MFT.SelectedIndex, {
					index:				i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.value',
					rangeBinding: 		'MFT.MCSController.model.driverSeat.'+i+'.range',
					seat: 				'driver',
					elementId: 			'view_settings_vehicle_MultiContourSeat_customMassage_driverSeat_cushion'+(i+1),
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
								
			for ( var i = 5; i < 10; i++ ) {
				
				element = MFT.Cushion.create( MFT.SelectedIndex, {
					index:		i+1,
					classNameBindings: 	'MFT.MCSController.seatsAdjustModeSwitching: hide',
					valueBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.value',
					rangeBinding: 		'MFT.MCSController.model.passengerSeat.'+i+'.range',
					seat: 				'passenger',
					elementId: 			'view_settings_vehicle_MultiContourSeat_customMassage_passengerSeat_cushion'+(i+1),
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
			'adjusts',
			'topIcon',
			'bottomIcon',
			'seatLabel',
			'upperLabel',
			'middleLabel',
			'lowerLabel',
			'rearLabel',
			'frontLabel',
			'previewTop',
			'previewBottom'
		],
		
		caption: MFT.Label.extend({
			classNames:	'MultiContourSeat_customMassage_caption',
			content:	'MCS Options: Custom Intensity Setting'
		}),

		adjusts: MFT.seatsAdjustView.extend({
			start:	10,
			end:	15
		}),

		seatLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_1_seatLabel',
			classNames:	'seatsMassageView2_1_seatLabel',
			content:	function(){
				if( 'driver' == MFT.MCSController.seat){
					return 'Driver';
				}else if( 'passenger' == MFT.MCSController.seat){
					return 'Passenger';
				}
			}.property('MFT.MCSController.seat')
		}),

		upperLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_1_upperLabel',
			content:	'Upper'
		}),

		middleLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_1_middleLabel',
			content:	'Middle'
		}),

		lowerLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_1_lowerLabel',
			content:	'Lower'
		}),

		rearLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_1_rearLabel',
			content:	'Rear'
		}),

		frontLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_1_frontLabel',
			content:	'Front'
		}),

		/** Image of right top seat on Massage view */
		topIcon: Em.View.extend({
			classNames:	'seats-message-1'
		}),

		/** Image of right bottom seat on Massage view */
		bottomIcon: Em.View.extend({
			classNames:	'seats-message-2'
		}),

		previewTop: MFT.Button.extend({
			elementId:		'MultiContourSeat_customMassage_previewButtonTop',
			onDown:			false,
			previewId:		'0',
			
			classNames:		'btn btn-28 MultiContourSeat_customMassage_previewButtonTop',
			text:			'Preview'		
		}),

		previewBottom: MFT.Button.extend({
			elementId:		'MultiContourSeat_customMassage_previewButtonBottom',
			onDown:			false,
			previewId:		'1',
			
			classNames:		'btn btn-28 MultiContourSeat_customMassage_previewButtonBottom',
			text:			'Preview'			
		})
	}),

	/** Massage mode buttons group for TOP lumbars */
	massageActionsTop: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: 	['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-1',
		valueBinding: 		'MFT.MCSController.activeSeat.15.value',
		range:				4,
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		labels: [
			'OFF',
			'LO',
			'HI',
			'CUSTOM'
		],
		
		controller:			MFT.MCSController,
		
		seatValue: {
			driver:		0,
			passenger:	0
		},
		
		onChangeMassageValue: function(el) {

			this.set('value', el.index);
			
			// highlight cushions
			this.controller.set( 'selectedIndex', [6,7,8] );
			
			// save value
			this.seatValue[MFT.MCSController.seat] = el.index;
			
			// custom massage adjust
			if ( el.index == 3 ) {
				this.controller.activeSeat[5].set('value', this.controller.activeSeat[17].value);
				this.controller.activeSeat[6].set('value', this.controller.activeSeat[18].value);
				this.controller.activeSeat[7].set('value', this.controller.activeSeat[19].value);
				return;
			}
			
			// change cushions adjust
			this.controller.activeSeat[5].set('value', el.index * 4);
			this.controller.activeSeat[6].set('value', el.index * 4);
			this.controller.activeSeat[7].set('value', el.index * 4);
		},
		/*
		onCustomBack: function() {
			if ( this.seatValue['driver'] != 3 ) {
				this.controller.model.driverSeat[5].set('value', this.seatValue['driver'] * 4);
				this.controller.model.driverSeat[6].set('value', this.seatValue['driver'] * 4);
				this.controller.model.driverSeat[7].set('value', this.seatValue['driver'] * 4);
			}
			
			if ( this.seatValue['passenger'] != 3 ) {
				this.controller.model.passengerSeat[5].set('value', this.seatValue['passenger'] * 4);
				this.controller.model.passengerSeat[6].set('value', this.seatValue['passenger'] * 4);
				this.controller.model.passengerSeat[7].set('value', this.seatValue['passenger'] * 4);
			}
		}.observes('this.controller.massageCustom'),
		*/
		onChangeSeat: function() {		
			this.set('value', this.seatValue[this.controller.seat] );
		}.observes('this.controller.seat')
	}),

	/** Massage mode buttons group for BOTTOM lumbars */
	massageActionsBottom: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-2',
		valueBinding: 		'MFT.MCSController.activeSeat.16.value',
		range: 				4,
		index:				9,
		classes:			'quality',
		action:				'onChangeMassageValue',
		labels: [
			'OFF',
			'LO',
			'HI',
			'CUSTOM'
		],
		
		controller:			MFT.MCSController,
		
		seatValue: {
			driver:		0,
			passenger:	0
		},
		
		onChangeMassageValue: function(el) {
			this.set('value', el.index);
			
			// highlight cushions
			this.controller.set( 'selectedIndex', [9,10] );
			
			// save value
			this.seatValue[this.controller.seat] = el.index;

			// custom massage adjust
			if ( el.index == 3 ) {
				this.controller.activeSeat[8].set('value', this.controller.activeSeat[20].value);
				this.controller.activeSeat[9].set('value', this.controller.activeSeat[21].value);
				return;
			}

			// change cushions adjust
			this.controller.activeSeat[8].set('value', el.index * 4);
			this.controller.activeSeat[9].set('value', el.index * 4);
		},
		/*
		onCustomBack: function() {
			if ( this.seatValue['driver'] != 3 ) {
				this.controller.model.driverSeat[8].set('value', this.seatValue['driver'] * 4);
				this.controller.model.driverSeat[9].set('value', this.seatValue['driver'] * 4);
			}
			
			if ( this.seatValue['passenger'] != 3 ) {
				this.controller.model.passengerSeat[8].set('value', this.seatValue['passenger'] * 4);
				this.controller.model.passengerSeat[9].set('value', this.seatValue['passenger'] * 4);
			}
		}.observes('this.controller.massageCustom'),
		*/
		onChangeSeat: function() {		
			this.set('value', this.seatValue[this.controller.seat] );
		}.observes('this.controller.seat'),
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
	})
});