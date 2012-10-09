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

MFT.SeatsMassageView1_4 = Em.ContainerView.extend({

	settingsState: true,

	elementId: 	'seatsMassageView1_4',

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	/** Seats Massage block components */
	childViews: [
		'seatsMassageTop',
		'seatsMassageBottom',
		'messageActionsTop',
		'patternTopText',
		'patternTopImg',
		'messageActionsBottom',
		'patternBottomText',
		'patternBottomImg',
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
		rangeBinding:		'MFT.MCSController.activeSeat.5.range',
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		target:				'MFT.MCSController',
		labels: [
			'OFF',
			'LO',
			'M',
			'HI'
		],
	}),

	/** Selected pettern text */
	patternTopText: MFT.Label.extend({
		classNames:		'patternText1',
		content: 'selected'
	}),

	/** Selected pettern image */
	patternTopImg: Em.View.extend({
		classNames:		'Image',

		template: Ember.Handlebars.compile('pattern'),
		
		classNameBindings: ['pattern1', 'pattern2', 'pattern3'],

		pattern1:			function(){

			if( MFT.MCSController.activeSeat[7].get('value') == 0 ){
				return true;
			}else{
				return false;
			}

		}.property('MFT.MCSController.activeSeat.7.value'),

		pattern2:			function(){

			if( MFT.MCSController.activeSeat[7].get('value') == 1 ){
				return true;
			}else{
				return false;
			}

		}.property('MFT.MCSController.activeSeat.7.value'),

		pattern3:			function(){

			if( MFT.MCSController.activeSeat[7].get('value') == 2 ){
				return true;
			}else{
				return false;
			}

		}.property('MFT.MCSController.activeSeat.7.value'),
		
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
			'M',
			'HI'
		],
	}),

	/** Selected pettern text */
	patternBottomText: MFT.Label.extend({ 
		classNames:		'patternText2',
		content: 'selected'
	}),

	/** Selected pettern image */
	patternBottomImg: Em.View.extend({
		classNames:				'Image',

		template: 				Ember.Handlebars.compile('pattern'),
		
		classNameBindings: 		['patternbottom1', 'patternbottom2', 'patternbottom3'],

		patternbottom1:			function(){

			if( MFT.MCSController.activeSeat[8].get('value') == 0 ){
				return true;
			}else{
				return false;
			}

		}.property('MFT.MCSController.activeSeat.8.value'),

		patternbottom2:			function(){

			if( MFT.MCSController.activeSeat[8].get('value') == 1 ){
				return true;
			}else{
				return false;
			}

		}.property('MFT.MCSController.activeSeat.8.value'),

		patternbottom3:			function(){

			if( MFT.MCSController.activeSeat[8].get('value') == 2 ){
				return true;
			}else{
				return false;
			}

		}.property('MFT.MCSController.activeSeat.8.value'),
		
	}),

	/** Settings Button */
	settings: MFT.Button.extend({
		classNames:	'btn-48 btn concept1_5Settings',
		id: 		'concept1_5Settings',
		text: 		'Settings',
		action: 	'onMassageCustomOpen',
		target: 	'MFT.MCSController',
		icon:		'images/settings/ico_settings.png'
	}),

	/** Container for Alternating Massage */
	alternatingMassage: Em.ContainerView.extend({

		classNameBindings: ['MFT.MCSController.massageCustom:show'],

		classNames: 'hide concept1_5SettingsScreen',
		
		elementId: 'alternatingMassage',

		childViews: [
			'seatLabel',
			'labelTitle',
			'seatsMassageTop',
			'messageActionsTop',
			'seatsMassageBottom',
			'messageActionsBottom'
		],
		
		seatLabel: MFT.Label.extend({
			elementId:	'seatsMassageView1_4_seatLabel',
			classNames:	'seatsMassageView1_4_seatLabel',
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
			classNames:		'labelTitle',
			content: 		'MCS Options: Massage Pattern Settings'
		}),
		
		/** Image of right top seat on Massage view */
		seatsMassageTop: Em.View.extend({
			classNames:	'seats-message-1'
		}),

		/** Container for three top buttons with images on Massage view */
		messageActionsTop: MFT.ButtonsGroup.extend ( MFT.SelectedIndex, {
		
			classNameBindings: 	['isSelected:active'],
			classNames:			'seats-message-actions seats-message-actions-top',
			valueBinding:		'MFT.MCSController.activeSeat.7.value',
			rangeBinding:		'MFT.MCSController.activeSeat.7.range',
			index:				8,
			classes:			'quality',
			action:				'onChangeMassageValue',
			target:				'MFT.MCSController',
			icons:				[
									'images/settings/massageTop.png',
									'images/settings/massageBottom.png',
									'images/settings/massageVertical.png'
								]
		}),
		
		/** Image of right top seat on Massage view */
		seatsMassageBottom: Em.View.extend({
			classNames:	'seats-message-2'
		}),
		
		/** Container for three top buttons with images on Massage view */
		messageActionsBottom: MFT.ButtonsGroup.extend ( MFT.SelectedIndex, {
		
			classNameBindings: 	['isSelected:active'],
			classNames:			'seats-message-actions seats-message-actions-bottom',
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
	})
});