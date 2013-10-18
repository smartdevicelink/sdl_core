/**
 * @name MFT.seatsMassageView2_2
 * 
 * @desc MCS massage view component 2.2 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView2_2.js
 * @version		2.0
 *
 * @author	Artem Petrosyan	
 */

MFT.SeatsMassageView2_2 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView2_2',
	
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
			'seatLabel',
			'caption',
			'adjustsTop',
			'adjustsBottom',
			'topActions',
			'bottomActions',
			'topIcon',
			'bottomIcon',
			'upperLabel',
			'middleLabel',
			'lowerLabel',
			'rearLabel',
			'frontLabel',
			'previewTop',
			'previewBottom'
		],
		
		seatLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_2_seatLabel',
			classNames:	'seatsMassageView2_2_seatLabel',
			content:	function(){
				if( 'driver' == MFT.MCSController.seat){
					return 'Driver';
				}else if( 'passenger' == MFT.MCSController.seat){
					return 'Passenger';
				}
			}.property('MFT.MCSController.seat')
		}),
		
		caption: MFT.Label.extend({
			elementId:	'MultiContourSeat_customMassage_caption',
			content:	'MCS Options: Massage Intensity Setting'
		}),
		
		adjustsTop: MFT.seatsAdjustView.extend({
			start:	10,
			end:	13,
			disabled:	function(){
				if( MFT.MCSController.activeSeat[15].get('value') != 3 ){
					return true;
				}else{
					return false;
				}
			}.property('MFT.MCSController.activeSeat.15.value')
		}),
		
		adjustsBottom: MFT.seatsAdjustView.extend({
			start:	13,
			end:	15,
			disabled:	function(){
				if( MFT.MCSController.activeSeat[16].get('value') != 3 ){
					return true;
				}else{
					return false;
				}
			}.property('MFT.MCSController.activeSeat.16.value')
		}),
		
		topActions: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
			classNameBindings: ['isSelected:active'],
			classNames:			'seats-message-actions seats-message-actions-3',
			valueBinding:				'MFT.MCSController.activeSeat.15.value',
			rangeBinding:				'MFT.MCSController.activeSeat.15.range',
			index:				20,
			classes:			'quality',
			action:				'onChangeMassageValue',
			labels: [
				'LO',
				'M',
				'HI',
				'CUSTOM'
			],
			
			oldValueDriver: 0,
			
			oldValuePassenger: 0,

			controller:			MFT.MCSController,

			seatValue: {
				driver:		0,
				passenger:	0
			},

			onChangeMassageValue: function(el) {
				this.set('value', el.index);
				
				// highlight cushions
				this.controller.set( 'selectedIndex', 20 );
				
				// save value
				this.seatValue[this.controller.seat] = el.index;
				
				// custom massage adjust
				if ( el.index == 3 ) {
					this.controller.activeSeat[10].set('value', this.controller.activeSeat[17].get('value'));
					this.controller.activeSeat[11].set('value', this.controller.activeSeat[18].get('value'));
					this.controller.activeSeat[12].set('value', this.controller.activeSeat[19].get('value'));
					
					if( this.controller.seat == 'driver' ){
						this.set('oldValueDriver', el.index);
					}else if( this.controller.seat == 'passenger' ){
						this.set('oldValuePassenger', el.index);
					}
					return;
				}else if( (this.oldValueDriver == 3 && this.controller.seat == 'driver') || (this.oldValuePassenger == 3 && this.controller.seat == 'passenger') ){
					this.controller.activeSeat[17].set('value', this.controller.activeSeat[10].get('value'));
					this.controller.activeSeat[18].set('value', this.controller.activeSeat[11].get('value'));
					this.controller.activeSeat[19].set('value', this.controller.activeSeat[12].get('value'));
				}

				if( this.controller.seat == 'driver' ){
					this.set('oldValueDriver', el.index);
				}else if( this.controller.seat == 'passenger' ){
					this.set('oldValuePassenger', el.index);
				}
				// change cushions adjust

				this.controller.activeSeat[10].set('value', (el.index+1) * 3 - 1);
				this.controller.activeSeat[11].set('value', (el.index+1) * 3 - 1);
				this.controller.activeSeat[12].set('value', (el.index+1) * 3 - 1);

			},
			
			onChangeValue: function() {
				if ( ( this.controller.selectedIndex >= 10 ) &&  ( this.controller.selectedIndex <= 13 )  ) {
					this.set('value', 3);
					this.seatValue[this.controller.seat] = this.value;
				}
			}.observes('this.controller.selectedIndex'),
			
			onChangeSeat: function() {		
				this.set('value', this.seatValue[this.controller.seat] );
			}.observes('this.controller.seat')
		}),
		
		bottomActions: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
			classNameBindings: ['isSelected:active'],
			classNames:			'seats-message-actions seats-message-actions-4',
			valueBinding:		'MFT.MCSController.activeSeat.16.value',
			rangeBinding:		'MFT.MCSController.activeSeat.16.range',
			index:				21,
			classes:			'quality',
			action:				'onChangeMassageValue',
			labels: [
				'LO',
				'M',
				'HI',
				'CUSTOM'
			],
			
			oldValueDriver: 0,
			
			oldValuePassenger: 0,
			
			controller:			MFT.MCSController,
		
			seatValue: {
				driver:		0,
				passenger:	0
			},
			
			onChangeMassageValue: function(el) {
				this.set('value', el.index);
				
				// highlight cushions
				this.controller.set( 'selectedIndex', 21 );
				
				// save value
				this.seatValue[this.controller.seat] = el.index;
				
				// custom massage adjust
				if ( el.index == 3 ) {
					this.controller.activeSeat[13].set('value', this.controller.activeSeat[20].get('value'));
					this.controller.activeSeat[14].set('value', this.controller.activeSeat[21].get('value'));
					
					if( this.controller.seat == 'driver' ){
						this.set('oldValueDriver', el.index);
					}else if( this.controller.seat == 'passenger' ){
						this.set('oldValuePassenger', el.index);
					}
					return;
				}else if( (this.oldValueDriver == 3 && this.controller.seat == 'driver') || (this.oldValuePassenger == 3 && this.controller.seat == 'passenger') ){
					this.controller.activeSeat[20].set('value', this.controller.activeSeat[13].get('value'));
					this.controller.activeSeat[21].set('value', this.controller.activeSeat[14].get('value'));
				}

				if( this.controller.seat == 'driver' ){
					this.set('oldValueDriver', el.index);
				}else if( this.controller.seat == 'passenger' ){
					this.set('oldValuePassenger', el.index);
				}
				// change cushions adjust
				this.controller.activeSeat[13].set('value', (el.index+1) * 3 - 1);
				this.controller.activeSeat[14].set('value', (el.index+1) * 3 - 1);
			},
			
			onChangeValue: function() {
				if ( ( this.controller.selectedIndex >= 14 ) &&  ( this.controller.selectedIndex <= 15 )  ) {
					this.set('value', 3);
					this.seatValue[this.controller.seat] = this.value;
				}
			}.observes('this.controller.selectedIndex'),
			
			onChangeSeat: function() {		
				this.set('value', this.seatValue[this.controller.seat] );
			}.observes('this.controller.seat'),
		}),

		upperLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_2_upperLabel',
			content:	'Upper'
		}),

		middleLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_2_middleLabel',
			content:	'Middle'
		}),

		lowerLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_2_lowerLabel',
			content:	'Lower'
		}),

		rearLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_2_rearLabel',
			content:	'Rear'
		}),

		frontLabel: MFT.Label.extend({
			elementId:	'seatsMassageView2_2_frontLabel',
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
			classNames:		'btn btn-28 MultiContourSeat_customMassage_previewButtonTop',
			text:			'Preview'		
		}),

		previewBottom: MFT.Button.extend({
			elementId:		'MultiContourSeat_customMassage_previewButtonBottom',
			onDown:			false,
			classNames:		'btn btn-28 MultiContourSeat_customMassage_previewButtonBottom',
			text:			'Preview'			
		})
	}),
	
	/** Massage mode buttons group for TOP lumbars */
	messageActionsTop: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: 	['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-1',
		value:				0,
		range:				2,
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		labels: [
			'OFF',
			'ON'
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
			
			// change cushions adjust
			this.controller.activeSeat[5].set('value', this.controller.activeSeat[10].value * el.index);
			this.controller.activeSeat[6].set('value', this.controller.activeSeat[11].value * el.index);
			this.controller.activeSeat[7].set('value', this.controller.activeSeat[12].value * el.index);
		},

		onCustomBack: function() {
			this.controller.activeSeat[5].set('value', this.controller.activeSeat[10].value * this.seatValue[this.controller.seat]);
			this.controller.activeSeat[6].set('value', this.controller.activeSeat[11].value * this.seatValue[this.controller.seat]);
			this.controller.activeSeat[7].set('value', this.controller.activeSeat[12].value * this.seatValue[this.controller.seat]);
		}.observes('this.controller.massageCustom'),

		onChangeSeat: function() {
			this.set('value', this.seatValue[this.controller.seat] );
		}.observes('this.controller.seat')
	}),
	
	/** Massage mode buttons group for BOTTOM lumbars */
	messageActionsBottom: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNameBindings: ['isSelected:active'],
		classNames:			'seats-message-actions seats-message-actions-2',		
		value:				0,
		range:				2,
		index:				9,
		classes:			'quality',
		action:				'onChangeMassageValue',
		labels: [
			'OFF',
			'ON'
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
			
			// change cushions adjust
			this.controller.activeSeat[8].set('value', this.controller.activeSeat[13].value * el.index);
			this.controller.activeSeat[9].set('value', this.controller.activeSeat[14].value * el.index);
		},

		onCustomBack: function() {
			this.controller.activeSeat[8].set('value', this.controller.activeSeat[13].value * this.seatValue[this.controller.seat]);
			this.controller.activeSeat[9].set('value', this.controller.activeSeat[14].value * this.seatValue[this.controller.seat]);
		}.observes('this.controller.massageCustom'),

		onChangeSeat: function() {		
			this.set('value', this.seatValue[this.controller.seat] );
		}.observes('this.controller.seat'),
	}),
	
	/** Settings button for massage custom adjustment */
	customIntensityButton: MFT.Button.extend({
		elementId:		'seatsMassageView2_2_settingsButton',
		
		classNames:		'btn btn-48',
		text:			'Intensity Settings',
		
		action:			'onMassageCustomOpen',
		target:			'MFT.MCSController',
		onDown:			false,
		
		icon:			'images/settings/ico_settings.png'
	})
});