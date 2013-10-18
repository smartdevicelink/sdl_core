/**
 * @name MFT.seatsMassageView4_1
 * 
 * @desc MCS massage view component 4.1 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView4_1.js
 * @version		2.0
 *
 * @author	Artem Petrosyan	
 */

MFT.SeatsMassageView5_1 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView5_1',

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
			'caption2',
			'seatLabel',
			'topIcon',
			'bottomIcon',
			'list'
		],

		caption: MFT.Label.extend({
			elementId:	'MultiContourSeat_customMassage_caption_5_1',
			classNames:	'MultiContourSeat_customMassage_caption',
			content:	'MCS Options: Scheduled Massage Setting'
		}),
		
		caption2: MFT.Label.extend({
			elementId:	'MultiContourSeat_customMassage_caption2',
			content:	'Offer Massage to Run when'
		}),
		
		seatLabel: MFT.Label.extend({
			elementId:	'seatsMassageView5_1_seatLabel',
			classNames:	'seatsMassageView5_1_seatLabel',
			content:	function(){
				if( 'driver' == MFT.MCSController.seat){
					return 'Driver';
				}else if( 'passenger' == MFT.MCSController.seat){
					return 'Passenger';
				}
			}.property('MFT.MCSController.seat')
		}),
		
		list: MFT.List.extend({
   
			elementId:			'MCSSettingsList_1_5',
			classNames:			'MCSSettingslist',
			itemsOnPage:		5,
			disableScrollbar:	false,
			setScrollDefault:	function() {
				this.scrollbar.set('currentPage', 0);
			}.observes('MFT.MCSController.seat'),
			items:[{
				type:	MFT.LabelPlusToggleButton,
				params:{
					tButtonsClases:			'btn btn-28 toggles button',
					classContainer:			'seats-message-actions-3',
					tButtonValueBinding:	'MFT.MCSController.activeSeat.7.value',
					tButtonRange:			2,
					labelContent:			'In city',
					tButtonClasses:			'quality',
					tButtonAction:			'onChangeMassageValue',
					tButtonLabels:[
						'On',
						'Off'
					]
				}   
			},
			{
				type:	MFT.LabelPlusToggleButton,
				params:{
					tButtonsClases:			'btn btn-28 toggles button',
					classContainer:			'seats-message-actions-3',
					tButtonValueBinding:	'MFT.MCSController.activeSeat.8.value',
					tButtonRange:			2,
					labelContent:			'Country roads',
					tButtonClasses:			'quality',
					tButtonAction:			'onChangeMassageValue',
					tButtonLabels:[
						'On',
						'Off'
					]
				}   
			},
			{
				type:	MFT.LabelPlusToggleButton,
				params:{
					tButtonsClases:			'btn btn-28 toggles button',
					classContainer:			'seats-message-actions-3',
					tButtonValueBinding:	'MFT.MCSController.activeSeat.9.value',
					tButtonRange:			2,
					labelContent:			'Highway',
					tButtonClasses:			'quality',
					tButtonAction:			'onChangeMassageValue',
					tButtonLabels:[
						'On',
						'Off'
					]
				}   
			},
			{
				type:	MFT.LabelPlusToggleButton,
				params:{
					tButtonsClases:			'btn btn-28 toggles button',
					classContainer:			'seats-message-actions-3',
					tButtonValueBinding:	'MFT.MCSController.activeSeat.10.value',
					tButtonRange:			2,
					labelContent:			'>50 mph',
					tButtonClasses:			'quality',
					tButtonAction:			'onChangeMassageValue',
					tButtonLabels:[
						'On',
						'Off'
					]
				}   
			},
			{
				type:	MFT.LabelPlusToggleButton,
				params:{
					tButtonsClases:			'btn btn-28 toggles button',
					classContainer:			'seats-message-actions-3',
					tButtonValueBinding:	'MFT.MCSController.activeSeat.11.value',
					tButtonRange:			2,
					labelContent:			'Traffic Jams',
					tButtonClasses:			'quality',
					tButtonAction:			'onChangeMassageValue',
					tButtonLabels:[
						'On',
						'Off'
					]
				}   
			},
			{
				type:	MFT.LabelPlusToggleButton,
				params:{
					tButtonsClases:			'btn btn-28 toggles button',
					classContainer:			'seats-message-actions-3',
					tButtonValueBinding:	'MFT.MCSController.activeSeat.12.value',
					tButtonRange:			2,
					labelContent:			'Straight Roads',
					tButtonClasses:			'quality',
					tButtonAction:			'onChangeMassageValue',
					tButtonLabels:[
						'On',
						'Off'
					]
				}   
			},
			{
				type:	MFT.LabelPlusToggleButton,
				params:{
					tButtonsClases:			'btn btn-28 toggles button',
					classContainer:			'seats-message-actions-3',
					tButtonValueBinding:	'MFT.MCSController.activeSeat.13.value',
					tButtonRange:			2,
					labelContent:			'"My Ford"',
					tButtonClasses:			'quality',
					tButtonAction:			'onChangeMassageValue',
					tButtonLabels:[
						'On',
						'Off'
					]
				}   
			}]
		}),

		/** Image of right top seat on Massage view */
		topIcon: Em.View.extend({
			classNames:	'seats-message-3'
		}),

		/** Image of right bottom seat on Massage view */
		bottomIcon: Em.View.extend({
			classNames:	'seats-message-4'
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
		classNames:		'btn btn-48 seatsMassageView_settingsButton ',
		text:			'Settings',
		action:			'onMassageCustomOpen',
		target:			'MFT.MCSController',
		icon:			'images/settings/ico_settings.png'
	})
});