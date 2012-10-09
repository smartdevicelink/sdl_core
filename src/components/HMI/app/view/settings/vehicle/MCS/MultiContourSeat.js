/**
 * @name MFT.MultiContourSeat
 * 
 * @desc Multi contour seat view
 * 
 * @category	View
 * @filesource	app/view/MultiContourSeat.js
 * @version		2.0
 *
 * @author	Melnik Andriy		
 */

MFT.MultiContourSeat = Em.ContainerView.create({

	classNameBindings: 	['MFT.States.settings.vehicle.mcs.active:active_state'],
	
	classNames:			'hidden',

	elementId:		'MultiContourSeatContainer',

	/** Multi contour seat content */
	childViews: [
		'MultiContourSeatContent'
	],

	MultiContourSeatContent: Em.ContainerView.extend({

		elementId:			'contentMCS',

		/** Multi contour seat components */
		childViews: [
			'backButton',
			'settingsVehicle'
		],
		/** Button switching back */
		backButton: MFT.Button.extend({	
			elementId:		'back-button',
			classNames:		'btn btn-48',
			action:			'back',
			target:		    'MFT.MCSController',
			onDown:		false
		}),
		/** Container for content and header */
		settingsVehicle: Em.ContainerView.extend({		
			elementId: 			'view_settings_vehicle_MultiContourSeat_Adjust',
			
			/** Container components */
			childViews: [
				'headerTitle',
				'innerContent'
			],

			/** Header block */
			headerTitle: MFT.Label.extend({	
				classNames:		'block-header-title',

				contentBinding: 'MFT.locale.label.view_settings_vehicle_MultiContourSeat_Adjust_header'
			}),

			/** Container for content */
			innerContent: Em.ContainerView.extend({
				elementId: 'inner-content',

				childViews: [
					'seatDriver',
					'seatPassenger',
					'cushionsDriver',
					'cushionsPassenger',
					'btnDriver',
					'btnPassenger',
					'btnAdjust',
					'btnMassage',
					'seatsAdjust'
				],
				
				/** Concept views */
				conceptViews: {
					1: MFT.SeatsMassageView,
					1.1: MFT.SeatsMassageView1_1,
					1.2: MFT.SeatsMassageView1_2,
					1.3: MFT.SeatsMassageView1_3,
					1.4: MFT.SeatsMassageView1_4,
					1.5: MFT.SeatsMassageView1_5,
					2.1: MFT.SeatsMassageView2_1,
					2.2: MFT.SeatsMassageView2_2,
					4.1: MFT.SeatsMassageView4_1,
					5.1: MFT.SeatsMassageView5_1,
					5.2: MFT.SeatsMassageView5_2,
					6.2: MFT.SeatsMassageView6_2,
					6.1: MFT.SeatsMassageView6_1,
					7.1: MFT.SeatsMassageView7_1
				},
				
				afterRender: function() {
					var element = this.conceptViews[FLAGS.MCS_ACTIVE_CONCEPT].create();
					// Insert view in container
					this.get('childViews').pushObject(element);
				},
				
				/** Button Driver */
				btnDriver: MFT.Button.extend({
					classNameBindings:	['MFT.MCSController.seatsDriverSwitching:active'],
					classNames:			'btn-48 btn driver',
					id: 				'driver',
					text: 				'Driver',
					action: 			'onChangeSeat',
					target: 			'MFT.MCSController'
				}),

				/** Button Passenger */
				btnPassenger: MFT.Button.extend({						
					classNameBindings:	['MFT.MCSController.seatsPassengerSwitching:active'],
					classNames:			'btn-48 btn passenger',
					id: 				'passenger',
					text: 				'Passenger',
					action: 			'onChangeSeat',
					target: 			'MFT.MCSController'
				}),

				/** Button Adjust */
				btnAdjust: MFT.Button.extend({
					classNameBindings:	['MFT.MCSController.seatsAdjustModeSwitching:active'],
					classNames:			'btn-48 btn adjust',
					id: 				'adjust',
					text: 				'Adjust',
					action: 			'onChangeSeatsMode',
					target: 			'MFT.MCSController'
				}),

				/** Button Massage */
				btnMassage: MFT.Button.extend({
					classNameBindings:	['MFT.MCSController.seatsMassageModeSwitching:active'],
					classNames:			'btn-48 btn massage',
					id: 				'massage',
					text: 				'Massage',
					action: 			'onChangeSeatsMode',
					target: 			'MFT.MCSController'
				}),

				/** Seats Adjust block */
				seatsAdjust: MFT.seatsAdjustView.extend({
					classNameBindings: ['MFT.MCSController.seatsMassageModeSwitching:hide'],
					
					start:	0,
					end:	5
				}),

				/** Image of left seat */
				seatDriver: Em.View.extend({	
					classNameBindings: [
						'MFT.MCSController.seatsPassengerSwitching:notActiveSeat',
						'MFT.MCSController.seatsAdjustModeSwitching:view_settings_vehicle_MultiContourSeat-left-s',
						'MFT.MCSController.seatsMassageModeSwitching:view_settings_vehicle_MultiContourSeat-Massage-left-s'
					]
				}),

				/** Image of right seat */
				seatPassenger: Em.View.extend({
					classNameBindings: [
						'MFT.MCSController.seatsDriverSwitching:notActiveSeat',
						'MFT.MCSController.seatsAdjustModeSwitching:seat-right',
						'MFT.MCSController.seatsMassageModeSwitching:massage-seat-right'
					]
				}),
				
				/** Driver seat lumbars */
				cushionsDriver: Em.ContainerView.extend({
					
					classNameBindings: ['MFT.MCSController.seatsPassengerSwitching:not_selected'],

					afterRender: function() {
						var element;
											
						for ( var i = 0; i < 5; i++ ) {
							
							element = MFT.Cushion.create( MFT.SelectedIndex, {
								index:		i+1,
								classNameBindings: 	'MFT.MCSController.seatsMassageModeSwitching: hide',
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
											
						for ( var i = 0; i < 5; i++ ) {
							
							element = MFT.Cushion.create( MFT.SelectedIndex, {
								index:		i+1,
								classNameBindings: 	'MFT.MCSController.seatsMassageModeSwitching: hide',
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
				})
			})
		})
	})
});