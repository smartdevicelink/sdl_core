/**
 * @name MFT.seatsMassageView6_1
 * 
 * @desc MCS massage view component 6.1 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView6_1.js
 * @version		2.0
 *
 * @author	Andriy Melnik	
 */

MFT.SeatsMassageView6_1 = Em.ContainerView.extend({

	classNameBindings: ['MFT.MCSController.seatsAdjustModeSwitching:hide'],

	classNames:	'seatsMassageContent',

	elementId: 	'seatsMassageView6_1',

	childViews: [
		'cushionsDriver',
		'cushionsPassenger',
		'seatsMassageTop',
		'seatsMassageBottom',
		'messageActionsTop',
		'messageActionsBottom',
		'customMySettings',
		'captionMySettings',
		'mySettingsLabel',
		'customSettings',
		'customAdjust'
	],

	afterRender: function() {
		MFT.MCSController.view = this;
	},
	
	activeState: false,

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
					index:				i+1,
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

	/** Container for three top buttons on Massage view */
	messageActionsTop: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNames:			'seats-message-actions seats-message-actions-1',
		valueBinding:		'MFT.MCSController.activeSeat.17.value',
		range:				3,
		index:				6,
		classes:			'quality',
		action:				'onChangeMassageValue',
		labels: [
			'OFF',
			'LO',
			'HI'
		],
		
		manageValues:		function(){
			MFT.MCSController.set('selectedIndex', null);
			if( false == this._parentView.activeState ){
				MFT.MCSController.activeSeat[5].set('value', this.get('value') * 5);
				MFT.MCSController.activeSeat[6].set('value', this.get('value') * 5);
				MFT.MCSController.activeSeat[7].set('value', this.get('value') * 5);
			}else if( true == this._parentView.activeState ){
			//	MFT.MCSController.activeSeat[5].set('value', 0);
			//	MFT.MCSController.activeSeat[6].set('value', 0);
			//	MFT.MCSController.activeSeat[7].set('value', 0);
			//	MFT.MCSController.activeSeat[17].set('value', 0);
			}
		}.observes('this.parentView.activeState'),

		controller:			MFT.MCSController,

		seatValue: {
			driver:		0,
			passenger:	0
		},

		onChangeMassageValue: function(el) {
			MFT.MCSController.activeSeat[16].set('value', 0);
			MFT.MCSController.set('mcsAdjustDisable', false);
			this._parentView.set('activeState', false);
			this.set('value', el.index);

			// highlight cushions
			this.controller.set( 'selectedIndex', [6,7,8] );

			// save value
			this.seatValue[MFT.MCSController.seat] = el.index;

			// change cushions adjust
			this.controller.activeSeat[5].set('value', el.index * 5);
			this.controller.activeSeat[6].set('value', el.index * 5);
			this.controller.activeSeat[7].set('value', el.index * 5);
		},

		onChangeSeat: function() {		
			this.set('value', this.seatValue[this.controller.seat] );
		}.observes('this.controller.seat')
	}),

	/** Container for three bottom buttons on Massage view */
	messageActionsBottom: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
		classNames:			'seats-message-actions seats-message-actions-2',
		valueBinding:		'MFT.MCSController.activeSeat.18.value',
		range:				3,
		index:				7,
		classes:			'quality',
		action:				'onChangeMassageValue',
		labels: [
			'OFF',
			'LO',
			'HI'
		],

		click:				function(){
			
		},

		manageValues:		function(){
			MFT.MCSController.set('selectedIndex', null);
			if( false == this._parentView.activeState ){
				MFT.MCSController.activeSeat[8].set('value', this.get('value') * 5);
				MFT.MCSController.activeSeat[9].set('value', this.get('value') * 5);
			}else if( true == this._parentView.activeState ){
			//	MFT.MCSController.activeSeat[8].set('value', 0);
			//	MFT.MCSController.activeSeat[9].set('value', 0);
			//	MFT.MCSController.activeSeat[18].set('value', 0);
			}
		}.observes('this.parentView.activeState'),

		controller:			MFT.MCSController,

		seatValue: {
			driver:		0,
			passenger:	0
		},
		
		onChangeMassageValue: function(el) {
			MFT.MCSController.activeSeat[16].set('value', 0);
			MFT.MCSController.set('mcsAdjustDisable', false);
			this._parentView.set('activeState', false);
			this.set('value', el.index);

			// highlight cushions
			this.controller.set( 'selectedIndex', [9,10] );

			// save value
			this.seatValue[this.controller.seat] = el.index;

			// change cushions adjust
			this.controller.activeSeat[8].set('value', el.index * 5);
			this.controller.activeSeat[9].set('value', el.index * 5);
		},

		onChangeSeat: function() {
			this.set('value', this.seatValue[this.controller.seat] );
		}.observes('this.controller.seat'),
		
		onChangeasdadsasdSeat: function() {
			console.log('asdasdasd');
		}.observes('MFT.MCSController.model.driverSeat[9].value')
	}),
	
	/** Container for two bottom buttons on Massage view */
	customMySettings: MFT.ButtonsGroup.extend({
		classNames:			'btn btn-48 seatsMassageView_settingsButton',
		valueBinding:		'MFT.MCSController.activeSeat.16.value',
		range:				2,
		index:				null,
		classes:			'quality',
		clear:				false,
		click:				function(){
			//this._parentView.set('activeState', true);
			MFT.MCSController.set('selectedIndex', null);
			if( 1 == this.get('value') ){
				MFT.MCSController.activeSeat[17].set('value', 0);
				MFT.MCSController.activeSeat[18].set('value', 0);
				this.set('clear', true);
			}else if( 0 == this.get('value') && true == this.get('clear')){
				MFT.MCSController.activeSeat[5].set('value', 0);
				MFT.MCSController.activeSeat[6].set('value', 0);
				MFT.MCSController.activeSeat[7].set('value', 0);
				MFT.MCSController.activeSeat[8].set('value', 0);
				MFT.MCSController.activeSeat[9].set('value', 0);
				this.set('clear', false);
			}
		},
		actionDown:		function(el) {
			this._parentView.set('activeState', true);
		},
		labels: [
			'OFF',
			'ON'
		]
	}),
	
	captionMySettings: MFT.Label.extend({
		elementId:	'captionMySettings',
		classNames:	'captionMySettings',
		content:	'MyMassage'
	}),

	/** Settings popup for massage custom adjustment */
	mySettingsLabel: Em.ContainerView.extend({

		classNames:			'mySettingsLabel hide',

		classNameBindings:	['hided:show'],

		hided:				function(){
			if(MFT.MCSController.activeSeat[16].value == 1){
				return true;
			}else{
				return false;
			}
		}.property('MFT.MCSController.activeSeat.16.value'),

		childViews: [
			'captionDriver',
			'captionPassenger'
		],
		
		firstStart:					true,
		
		intervalDriver:				null,
		
		intervalPassenger:			null,
		
		positionDriver:				-1,
		
		positionPassenger:			-1,
		
		textForCaptionDriver:		null,
		
		textForCaptionPassenger:	null,

		funcForCaptionDriver:		function(self){
			
			var count = 0;
				for(var i in MFT.MCSController.mcsBlockSettingsDriver)
				{

					if( MFT.MCSController.mcsBlockSettingsDriver[i].position - 1 == self.get('positionDriver') ){

						if( MFT.MCSController.mcsBlockSettingsDriver[i].intensity != 3 ){
							MFT.MCSController.model.driverSeat[5].set('value', 3 * (MFT.MCSController.mcsBlockSettingsDriver[i].intensity + 1));
							MFT.MCSController.model.driverSeat[6].set('value', 3 * (MFT.MCSController.mcsBlockSettingsDriver[i].intensity + 1));
							MFT.MCSController.model.driverSeat[7].set('value', 3 * (MFT.MCSController.mcsBlockSettingsDriver[i].intensity + 1));
							MFT.MCSController.model.driverSeat[8].set('value', 3 * (MFT.MCSController.mcsBlockSettingsDriver[i].intensity + 1));
							MFT.MCSController.model.driverSeat[9].set('value', 3 * (MFT.MCSController.mcsBlockSettingsDriver[i].intensity + 1));
						}else {
							MFT.MCSController.model.driverSeat[5].set('value', MFT.MCSController.mcsBlockSettingsDriver[i].intensityCustom[0]);
							MFT.MCSController.model.driverSeat[6].set('value', MFT.MCSController.mcsBlockSettingsDriver[i].intensityCustom[1]);
							MFT.MCSController.model.driverSeat[7].set('value', MFT.MCSController.mcsBlockSettingsDriver[i].intensityCustom[2]);
							MFT.MCSController.model.driverSeat[8].set('value', MFT.MCSController.mcsBlockSettingsDriver[i].intensityCustom[3]);
							MFT.MCSController.model.driverSeat[9].set('value', MFT.MCSController.mcsBlockSettingsDriver[i].intensityCustom[4]);
						}

						self.set('textForCaptionDriver', 'Line ' + i  + ' Active');
						self.set('positionDriver', self.get('positionDriver') + 1);
						return;
					}
					count++;

				}

			if( count - 2 == self.get('positionDriver') ){
				self.set('positionDriver', - 1);
			}

		},
		
		funcForCaptionPassenger:		function(self){
			
			var count = 0;
				for(var i in MFT.MCSController.mcsBlockSettingsPassenger)
				{

					if( MFT.MCSController.mcsBlockSettingsPassenger[i].position - 1 == self.get('positionPassenger') ){
						
						if( MFT.MCSController.mcsBlockSettingsPassenger[i].intensity != 3 ){
							MFT.MCSController.model.passengerSeat[5].set('value', 3 * (MFT.MCSController.mcsBlockSettingsPassenger[i].intensity + 1));
							MFT.MCSController.model.passengerSeat[6].set('value', 3 * (MFT.MCSController.mcsBlockSettingsPassenger[i].intensity + 1));
							MFT.MCSController.model.passengerSeat[7].set('value', 3 * (MFT.MCSController.mcsBlockSettingsPassenger[i].intensity + 1));
							MFT.MCSController.model.passengerSeat[8].set('value', 3 * (MFT.MCSController.mcsBlockSettingsPassenger[i].intensity + 1));
							MFT.MCSController.model.passengerSeat[9].set('value', 3 * (MFT.MCSController.mcsBlockSettingsPassenger[i].intensity + 1));
						}else {
							MFT.MCSController.model.passengerSeat[5].set('value', MFT.MCSController.mcsBlockSettingsPassenger[i].intensityCustom[0]);
							MFT.MCSController.model.passengerSeat[6].set('value', MFT.MCSController.mcsBlockSettingsPassenger[i].intensityCustom[1]);
							MFT.MCSController.model.passengerSeat[7].set('value', MFT.MCSController.mcsBlockSettingsPassenger[i].intensityCustom[2]);
							MFT.MCSController.model.passengerSeat[8].set('value', MFT.MCSController.mcsBlockSettingsPassenger[i].intensityCustom[3]);
							MFT.MCSController.model.passengerSeat[9].set('value', MFT.MCSController.mcsBlockSettingsPassenger[i].intensityCustom[4]);
						}
						
						self.set('textForCaptionPassenger', 'Line ' + i  + ' Active');
						self.set('positionPassenger', self.get('positionPassenger') + 1);
						return;
					}
					count++;

				}

			if( count - 2 == self.get('positionPassenger') ){
				self.set('positionPassenger', - 1);
			}

		},

		refreshCaption:	function(){
			if(true == this.get('firstStart')){
				this.set('firstStart', false);
			}
			var self = this;
			if(MFT.MCSController.activeSeat[16].value == 1){
				MFT.MCSController.set('mcsAdjustDisable', true);
				if( MFT.MCSController.seat == 'driver' ){
					if( null == this.get('intervalDriver') && false == this.get('firstStart') ){
						self.funcForCaptionDriver(self);
						this.intervalDriver = setInterval( function() { self.funcForCaptionDriver(self) } , 1000);
					}else{
						if(this.intervalDriver == null){
							this.intervalDriver = setInterval( function() { self.funcForCaptionDriver(self) } , 1000);
						}
					}
				}else if( MFT.MCSController.seat == 'passenger' ){
					if( null == this.get('intervalPassenger') && false == this.get('firstStart') ){
						self.funcForCaptionPassenger(self);
						this.intervalPassenger = setInterval( function() { self.funcForCaptionPassenger(self) } , 1000);
					}else{
						if(this.intervalPassenger == null){
							this.intervalPassenger = setInterval( function() { self.funcForCaptionPassenger(self) } , 1000);
						}
					}
				}
			}else if(MFT.MCSController.activeSeat[16].value == 0){
				MFT.MCSController.set('mcsAdjustDisable', false);
				if( MFT.MCSController.seat == 'driver' ){
					clearInterval(this.get('intervalDriver'));
					this.set('intervalDriver', null);
					this.set('textForCaptionDriver', null);
					this.set('positionDriver', -1);
				}else if( MFT.MCSController.seat == 'passenger' ){
					clearInterval(this.get('intervalPassenger'));
					this.set('intervalPassenger', null);
					this.set('textForCaptionPassenger', null);
					this.set('positionPassenger', -1);
				}

			}
		}.observes('MFT.MCSController.activeSeat.16.value'),		

		refreshTimer:	function(){

			var self = this;
			if('massage' == MFT.MCSController.mode){
				if( MFT.MCSController.model.driverSeat[16].get('value') == 1 ){
					if( null == this.get('intervalDriver') && false == this.get('firstStart') ){
						self.funcForCaptionDriver(self);
						this.intervalDriver = setInterval( function() { self.funcForCaptionDriver(self) } , 1000);
						MFT.MCSController.set('mcsAdjustDisable', true);
					}
				}
				if( MFT.MCSController.model.passengerSeat[16].get('value') == 1 ){
					if( null == this.get('intervalPassenger') && false == this.get('firstStart') ){
						self.funcForCaptionPassenger(self);
						this.intervalPassenger = setInterval( function() { self.funcForCaptionPassenger(self) } , 1000);
						MFT.MCSController.set('mcsAdjustDisable', true);
					}
				}
			}else if('adjust' == MFT.MCSController.mode){
				MFT.MCSController.set('mcsAdjustDisable', false);
				if( MFT.MCSController.seat == 'driver' ){
					clearInterval(this.get('intervalDriver'));
					this.set('intervalDriver', null);
					this.set('textForCaptionDriver', null);
				}else if( MFT.MCSController.seat == 'passenger' ){
					clearInterval(this.get('intervalPassenger'));
					this.set('intervalPassenger', null);
					this.set('textForCaptionPassenger', null);
				}
			}
		}.observes('MFT.MCSController.mode'),
		
		captionDriver: MFT.Label.extend({
		classNameBindings:	['driverH:hide'],
			driverH:			function(){
				if('driver' == MFT.MCSController.seat && 'massage' == MFT.MCSController.mode ){
					return false;
				}else{
					return true;
				}
			}.property('MFT.MCSController.seat', 'MFT.MCSController.mode'),
			elementId:	'mySettingsPopUpTitlePassenger',
			classNames:	'mySettingsPopUpTitle dri',
			content:	function() {
				return this._parentView.get('textForCaptionDriver');
			}.property('this.parentView.textForCaptionDriver')
		}),
		
		captionPassenger: MFT.Label.extend({
			classNameBindings:	['passengerH:hide'],
			passengerH:		function(){
				if('passenger' == MFT.MCSController.seat && 'massage' == MFT.MCSController.mode ){
					return false;
				}else{
					return true;
				}
			}.property('MFT.MCSController.seat', 'MFT.MCSController.mode'),
			elementId:	'mySettingsPopUpTitleDriver',
			classNames:	'mySettingsPopUpTitle pas',
			content:	function() {
				return this._parentView.get('textForCaptionPassenger');
			}.property('this.parentView.textForCaptionPassenger')
		})

	}),
	
	/** Settings button for massage custom adjustment */
	customSettings: MFT.Button.extend({
		classNameBindings:	['MFT.MCSController.mcsAdjustDisable:disabled'],
		disabledBinding:	'MFT.MCSController.mcsAdjustDisable',
		classNames:			'btn btn-48 seatsMassageView_settingsButton2 ',
		action:			function () {
										MFT.MCSController.onMassageCustomOpen();
									},
		icon:			'images/settings/ico_settings.png'
	}),
	
	/** Custom adjustment massage */
	customAdjust: Em.ContainerView.extend({

		classNames:		'MultiContourSeat_customMassage',

		classNameBindings: ['MFT.MCSController.massageCustom:visible'],

		childViews: [
			'seatLabel',
			'caption',
			'btnUp',
			'btnDown',
			'btnEdit',
			'btnNew',
			'btnTrash',
			'blocks',
			'settingsScreen',
			'settingsScreen2'
		],
		
		seatLabel: MFT.Label.extend({
			elementId:	'seatsMassageView6_1_seatLabel',
			classNames:	'seatsMassageView6_1_seatLabel',
			content:	function(){
				if( 'driver' == MFT.MCSController.seat){
					return 'Driver';
				}else if( 'passenger' == MFT.MCSController.seat){
					return 'Passenger';
				}
			}.property('MFT.MCSController.seat')
		}),
		
		caption: MFT.Label.extend({
			elementId:	'MultiContourSeat_customMassage_caption_6_1',
			classNames:	'MultiContourSeat_customMassage_caption',
			content:	'Individualized Massage Therapy'
		}),
		
		btnUp: MFT.Button.extend( MFT.Dropable, {
			classNameBindings:	['hide:disabled'],
			disabledBinding:		'hide',
			hide:		function(){
				if(MFT.MCSController.mcsActiveBlockButton == null){
					return true;
				}else{
					return false;
				}
			}.property('MFT.MCSController.mcsActiveBlockButton'),
			classNames:		'btn btn-28 seatsMassageView_settingsButtonUp ',
			text:			'move Line Up',
			click: 			function () {
											var blocks 	= $('#SettingsBlocks').find("div[position]"),
												pos 	= parseInt($('#SettingsBlocks').find("div[id='" + MFT.MCSController.mcsActiveBlockButton + "']").attr('position'));
											if(pos >= 1){
												MFT.MCSController.mcsDropId = pos - 1;
												MFT.MCSController.mcsDragId = pos;
												MFT.MCSController.stopDrag();
											}
										}
		}),
		
		btnDown: MFT.Button.extend( MFT.Dropable, {
			classNameBindings:	['hide:disabled'],
			disabledBinding:		'hide',
			hide:		function(){
				if(MFT.MCSController.mcsActiveBlockButton == null){
					return true;
				}else{
					return false;
				}
			}.property('MFT.MCSController.mcsActiveBlockButton'),
			classNames:		'btn btn-28 seatsMassageView_settingsButtonDown ',
			text:			'move Line Down',
			click: 			function () {
											var blocks	= $('#SettingsBlocks').find("div[position]"),
												pos 	= parseInt($('#SettingsBlocks').find("div[id='" + MFT.MCSController.mcsActiveBlockButton + "']").attr('position'));
											if(pos < blocks.length - 1){
												MFT.MCSController.mcsDropId = pos + 1;
												MFT.MCSController.mcsDragId = pos;
												MFT.MCSController.stopDrag();
											}
										}
		}),
		
		btnEdit: MFT.Button.extend( MFT.Dropable, {
			classNameBindings:	['hide:disabled'],
			disabledBinding:		'hide',
			hide:		function(){
				if(MFT.MCSController.mcsActiveBlockButton == null){
					return true;
				}else{
					return false;
				}
			}.property('MFT.MCSController.mcsActiveBlockButton'),
			classNames:		'btn btn-28 seatsMassageView_settingsButtonEdit ',
			text:			'Edit Line',
			click: 			function () {
											if((MFT.MCSController.mcsActiveBlockButton >= 0) && (MFT.MCSController.mcsActiveBlockButton != null)){
												MFT.MCSController.onMassageCustomOpen();
											}
										}
		}),
		
		btnNew: MFT.Button.extend( MFT.Dropable, {
			classNameBindings:	['MFT.MCSController.mcsNewDisable:disabled'],
			disabledBinding:	'MFT.MCSController.mcsNewDisable',
			classNames:		'btn btn-28 seatsMassageView_settingsButtonNew ',
			text:			'Add Line',
			action:			'addNewBlock',
			target:			'MFT.MCSController'
		}),
		
		btnTrash: MFT.Button.extend( MFT.Dropable, {
			classNameBindings:	['MFT.MCSController.mcsDeleteDisable:disabled'],
			disabledBinding:	'MFT.MCSController.mcsDeleteDisable',
			classNames:		'btn btn-28 seatsMassageView_settingsButtonTrash ',
			text:			'Remove Line',
			click: 			function () {
											MFT.MCSController.mcsDragId = parseInt($('#SettingsBlocks').find("div[id='" + MFT.MCSController.mcsActiveBlockButton + "']").attr('position'));
											MFT.MCSController.dropToTrash();
											//Ember.View.views[MFT.MCSController.mcsActiveBlockButton].destroy();
										}
		}),
		
		blocks: Em.ContainerView.extend({

			elementId:		'SettingsBlocks',

			classNames:		'SettingsBlocks',

			childViews: [
				'btn1',
				'btn2',
				'btn3'
			],
			
			btn1: MFT.Button.extend({
				elementId:			'1',
				classNames:			'btn btn-28 seatsMassageView_settingsBlock ',
				classNameBindings:	['isActive:active'],
				isActive:	function () {
										if( this.elementId == MFT.MCSController.mcsActiveBlockButton ){
											return true;
										}else{
											return false;
										}
									}.property('MFT.MCSController.mcsActiveBlockButton'),
				text:				'Line1',
				attributeBindings:	['style','position'],
				position:			0,
				click: 				function () {
										MFT.MCSController.set('mcsActiveBlockButton', parseInt(this.elementId));
										MFT.MCSController.set('mcsDragId', parseInt(this.elementId) - 1);
									},
				style : 			function(){
										if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
											return "left: " + this.position * 105 + "px;";
										}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){
											return "top: " + (this.position * 35 + 50) + "px;";
										}
									}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
			}),

			btn2: MFT.Button.extend({
				elementId:			'2',
				classNames:			'btn btn-28 seatsMassageView_settingsBlock ',
				classNameBindings:	['isActive:active'],
				isActive:	function () {
										if( this.elementId == MFT.MCSController.mcsActiveBlockButton ){
											return true;
										}else{
											return false;
										}
									}.property('MFT.MCSController.mcsActiveBlockButton'),
				text:				'Line2',
				attributeBindings:	['style','position'],
				position:			1,
				click: 				function () {
										MFT.MCSController.set('mcsActiveBlockButton', parseInt(this.elementId));
									},
				style : 			function(){
										if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
											return "left: " + this.position * 105 + "px;";
										}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){
											return "top: " + (this.position * 35 + 50) + "px;";
										}
									}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
			}),
			
			btn3: MFT.Button.extend({
				elementId:			'3',
				classNames:			'btn btn-28 seatsMassageView_settingsBlock ',
				classNameBindings:	['isActive:active'],
				isActive:	function () {
										if( this.elementId == MFT.MCSController.mcsActiveBlockButton ){
											return true;
										}else{
											return false;
										}
									}.property('MFT.MCSController.mcsActiveBlockButton'),
				text:				'Line3',
				attributeBindings:	['style','position'],
				position:			2,
				click: 				function () {
										MFT.MCSController.set('mcsActiveBlockButton', parseInt(this.elementId));
									},
				style : 			function(){
										if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
											return "left: " + this.position * 105 + "px;";
										}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){
											return "top: " + (this.position * 35 + 50) + "px;";
										}
									}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
			})
		}),
		
		settingsScreen: Em.ContainerView.extend({

			elementId:		'settingsScreen',

			classNames:		'settingsScreen',
			
			classNameBindings: ['MFT.MCSController.massageCustomLevel2:visible'],

			childViews: [
				'seatLabel',
				'massage',
				'pattern',
				'intensity',
				'speed',
				'duration',
				'pause',
				'editMenu',
				'massageButtons',
				'patternButtonsTop',
				'patternButtonsBottom',
				'intensityButtons',
				'intensityEditCustom',
				'speedButtons',
				'durationButtons',
				'pauseButtons',
				'preview'
			],
			
			seatLabel: MFT.Label.extend({
				elementId:	'seatsMassageView6_1_seatLabel',
				classNames:	'seatsMassageView6_1_seatLabel',
				content:	function(){
					if( 'driver' == MFT.MCSController.seat){
						return 'Driver';
					}else if( 'passenger' == MFT.MCSController.seat){
						return 'Passenger';
					}
				}.property('MFT.MCSController.seat')
			}),
			
			massage: MFT.Label.extend({
				elementId:	'massageLabel',
				classNames:	'massageLabel label',
				content:	'Which Massage:'
			}),
			
			pattern: MFT.Label.extend({
				elementId:	'pattern',
				classNames:	'pattern label',
				content:	'Pattern:'
			}),
			
			intensity: MFT.Label.extend({
				elementId:	'intensity',
				classNames:	'intensity label',
				content:	'Intensity:'
			}),
			
			speed: MFT.Label.extend({
				elementId:	'speed',
				classNames:	'speed label',
				content:	'Speed:'
			}),
			
			duration: MFT.Label.extend({
				elementId:	'duration',
				classNames:	'duration label',
				content:	'Duration:'
			}),
			
			pause: MFT.Label.extend({
				elementId:	'pause',
				classNames:	'pause label',
				content:	'Pause to next:'
			}),
			
			editMenu: MFT.Label.extend({
				elementId:	'editMenu',
				classNames:	'editMenu label',
				content:	'Edit menu',
				content:	function(){

					return 'Edit menu (Line' + MFT.MCSController.get('mcsActiveBlockButton') + ')';

				}.property('MFT.MCSController.mcsActiveBlockButton')
			}),
			
			massageButtons: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
				classNames:			'seats-message-actions massageButtons',
				valueBinding:		'MFT.MCSController.activeSeat.15.value',
				range:				2,
				index:				8,
				classes:			'btn-28 quality ',
				/*action:				'onChangeMassageValue',
				target:				'MFT.MCSController',*/
				delimeters:			false,
				bindedValue:		null,
				icons:	[
							'images/settings/seats-message-top.png',
							'images/settings/seats-message-bottom.png'
						],
				click:				function(){
					if( MFT.MCSController.get('seat') == 'driver' ){
						MFT.MCSController.set('mcsBlockSettingsDriver.' + MFT.MCSController.get('mcsActiveBlockButton') + '.massage', this.value);
					}else if( MFT.MCSController.seat == 'passenger' ){
						MFT.MCSController.set('mcsBlockSettingsPassenger.' + MFT.MCSController.get('mcsActiveBlockButton') + '.massage', this.value);
					}
				},
				icons:	[
							'images/settings/seats-message-top.png',
							'images/settings/seats-message-bottom.png'
						],
				manageValues:	function(){

					if(MFT.MCSController.get('mcsActiveBlockButton')){
						if( MFT.MCSController.seat == 'driver' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].massage);
						}else if( MFT.MCSController.seat == 'passenger' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].massage);
						}
					}

				}.observes('MFT.MCSController.mcsActiveBlockButton'),
				
				keepValues:	function(){

					if( MFT.MCSController.get('mcsActiveBlockButtonPreview') ){
						if( MFT.MCSController.seat == 'driver' ){
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].massage = this.value;
						}else if( MFT.MCSController.seat == 'passenger' ){
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].massage = this.value;
						}
						
					}
					
				}.observes('MFT.MCSController.mcsActiveBlockButtonPreview')
			}),

			patternButtonsTop: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
				classNames:			'seats-message-actions patternButtons',
				range:				3,
				index:				9,
				classes:			'btn-28 quality ',
				/*action:				'onChangeMassageValue',
				target:				'MFT.MCSController',*/
				delimeters:			false,
				icons:				[
										'images/settings/massageTop.png',
										'images/settings/massageBottom.png',
										'images/settings/massageVertical.png'
									],
				click:				function(){
					if( MFT.MCSController.get('seat') == 'driver' ){
						
						MFT.MCSController.set('mcsBlockSettingsDriver.' + MFT.MCSController.get('mcsActiveBlockButton') + '.patternTop', this.value);
						
					}else if( MFT.MCSController.seat == 'passenger' ){
						
						MFT.MCSController.set('mcsBlockSettingsPassenger.' + MFT.MCSController.get('mcsActiveBlockButton') + '.patternTop', this.value);
						
					}
				},
				managePattern:	function(){
					if( null != MFT.MCSController.get('mcsActiveBlockButton') )
					{

						if( MFT.MCSController.seat == 'driver' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].patternTop);
						}else if( MFT.MCSController.seat == 'passenger' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].patternTop);
						}

					}
				}.observes('MFT.MCSController.activeSeat.15.value', 'MFT.MCSController.mcsActiveBlockButton')
			}),
			
			patternButtonsBottom: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
				classNameBindings:	['hide:hidden'],
				hide:		function(){
					if(MFT.MCSController.activeSeat[15].value == 0){
						return true;
					}else{
						return false;
					}
				}.property('MFT.MCSController.activeSeat.15.value'),
				classNames:			'seats-message-actions patternButtons',
				range:				3,
				index:				9,
				classes:			'btn-28 quality ',
				/*action:				'onChangeMassageValue',
				target:				'MFT.MCSController',*/
				delimeters:			false,
				icons:				[
										'images/settings/massageCircle.png',
										'images/settings/massageRound.png',
										'images/settings/massageHorisontal.png'
									],
				click:				function(){
					if( MFT.MCSController.get('seat') == 'driver' ){
						
						MFT.MCSController.set('mcsBlockSettingsDriver.' + MFT.MCSController.get('mcsActiveBlockButton') + '.patternBottom', this.value);
						
					}else if( MFT.MCSController.seat == 'passenger' ){
						
						MFT.MCSController.set('mcsBlockSettingsPassenger.' + MFT.MCSController.get('mcsActiveBlockButton') + '.patternBottom', this.value);
						
					}
				},
				managePattern:	function(){
					if( null != MFT.MCSController.get('mcsActiveBlockButton') )
					{
						if( MFT.MCSController.seat == 'driver' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].patternBottom);
						}else if( MFT.MCSController.seat == 'passenger' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].patternBottom);
						}
					}
				}.observes('MFT.MCSController.activeSeat.15.value', 'MFT.MCSController.mcsActiveBlockButton')
			}),
			
			intensityButtons: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
				classNames:			'seats-message-actions intensityButtons',
				range:				4,
				index:				10,
				classes:			'btn-28 quality ',
				delimeters:			false,
				labels: [
					'Low',
					'Medium',
					'High',
					'Custom'
				],
				manageValues:	function(){

					if(MFT.MCSController.get('mcsActiveBlockButton')){
						if( MFT.MCSController.seat == 'driver' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensity);
						}else if( MFT.MCSController.seat == 'passenger' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensity);
						}
					}

				}.observes('MFT.MCSController.mcsActiveBlockButton'),
				
				keepValues:	function(){

					if( MFT.MCSController.get('mcsActiveBlockButtonPreview') ){
						if( MFT.MCSController.seat == 'driver' ){
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensity = this.value;
						}else if( MFT.MCSController.seat == 'passenger' ){
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensity = this.value;
						}
						
					}
					
				}.observes('MFT.MCSController.mcsActiveBlockButtonPreview')
			}),
			
			intensityEditCustom: MFT.Button.extend({
				elementId:		'intensityEditCustom',
				classNames:		'btn btn-28 intensityEditCustom',
				text:			'Edit Custom Intensity',
				action:			'onMassageCustomOpen',
				target:			'MFT.MCSController',
				keepValues:	function(){

					if( !MFT.MCSController.get('massageCustomLevel3') ){
						if( MFT.MCSController.seat == 'driver' ){
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom = {};
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[0] = MFT.MCSController.model.driverSeat[10].value;
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[1] = MFT.MCSController.model.driverSeat[11].value;
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[2] = MFT.MCSController.model.driverSeat[12].value;
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[3] = MFT.MCSController.model.driverSeat[13].value;
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[4] = MFT.MCSController.model.driverSeat[14].value;
						}else if( MFT.MCSController.seat == 'passenger' ){
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom = {};
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[0] = MFT.MCSController.model.passengerSeat[10].value;
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[1] = MFT.MCSController.model.passengerSeat[11].value;
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[2] = MFT.MCSController.model.passengerSeat[12].value;
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[3] = MFT.MCSController.model.passengerSeat[13].value;
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[4] = MFT.MCSController.model.passengerSeat[14].value;
						}
					}
					
				}.observes('MFT.MCSController.massageCustomLevel3'),
				setValues:	function(){

					if( MFT.MCSController.get('massageCustomLevel3') ){
						if( MFT.MCSController.seat == 'driver' ){
							MFT.MCSController.model.driverSeat[10].set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[0]);
							MFT.MCSController.model.driverSeat[11].set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[1]);
							MFT.MCSController.model.driverSeat[12].set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[2]);
							MFT.MCSController.model.driverSeat[13].set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[3]);
							MFT.MCSController.model.driverSeat[14].set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[4]);
						}else if( MFT.MCSController.seat == 'passenger' ){
							MFT.MCSController.model.passengerSeat[10].set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[0]);
							MFT.MCSController.model.passengerSeat[11].set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[1]);
							MFT.MCSController.model.passengerSeat[12].set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[2]);
							MFT.MCSController.model.passengerSeat[13].set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[3]);
							MFT.MCSController.model.passengerSeat[14].set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].intensityCustom[4]);
						}
					}
					
				}.observes('MFT.MCSController.massageCustomLevel3')
			}),
			
			speedButtons: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
				classNames:			'seats-message-actions speedButtons',
				range:				3,
				index:				11,
				classes:			'btn-28 quality ',
				/*action:				'onChangeMassageValue',
				target:				'MFT.MCSController',*/
				delimeters:			false,
				labels: [
					'Slow',
					'Medium',
					'Fast'
				],
				manageValues:	function(){

					if(MFT.MCSController.get('mcsActiveBlockButton')){
						if( MFT.MCSController.seat == 'driver' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].speed);
						}else if( MFT.MCSController.seat == 'passenger' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].speed);
						}
					}

				}.observes('MFT.MCSController.mcsActiveBlockButton'),
				
				keepValues:	function(){

					if( MFT.MCSController.get('mcsActiveBlockButtonPreview') ){
						if( MFT.MCSController.seat == 'driver' ){
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].speed = this.value;
						}else if( MFT.MCSController.seat == 'passenger' ){
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].speed = this.value;
						}
						
					}
					
				}.observes('MFT.MCSController.mcsActiveBlockButtonPreview')
			}),
			
			durationButtons: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
				classNames:			'seats-message-actions durationButtons',
				range:				5,
				index:				12,
				classes:			'btn-28 quality ',
				/*action:				'onChangeMassageValue',
				target:				'MFT.MCSController',*/
				delimeters:			false,
				labels: [
					'5',
					'10',
					'15',
					'20',
					'25'
				],
				manageValues:	function(){

					if(MFT.MCSController.get('mcsActiveBlockButton')){
						if( MFT.MCSController.seat == 'driver' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].duration);
						}else if( MFT.MCSController.seat == 'passenger' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].duration);
						}
					}

				}.observes('MFT.MCSController.mcsActiveBlockButton'),
				
				keepValues:	function(){

					if( MFT.MCSController.get('mcsActiveBlockButtonPreview') ){
						if( MFT.MCSController.seat == 'driver' ){
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].duration = this.value;
						}else if( MFT.MCSController.seat == 'passenger' ){
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].duration = this.value;
						}
						
					}
					
				}.observes('MFT.MCSController.mcsActiveBlockButtonPreview')
			}),

			pauseButtons: MFT.ButtonsGroup.extend( MFT.SelectedIndex, {
				classNames:			'seats-message-actions pauseButtons',
				range:				5,
				index:				13,
				classes:			'btn-28 quality ',
				/*action:				'onChangeMassageValue',
				target:				'MFT.MCSController',*/
				delimeters:			false,
				labels: [
					'5',
					'10',
					'15',
					'20',
					'NO'
				],
				manageValues:	function(){

					if(MFT.MCSController.get('mcsActiveBlockButton')){
						if( MFT.MCSController.seat == 'driver' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].pause);
						}else if( MFT.MCSController.seat == 'passenger' ){
							this.set('value', MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].pause);
						}
					}

				}.observes('MFT.MCSController.mcsActiveBlockButton'),
				
				keepValues:	function(){

					if( MFT.MCSController.get('mcsActiveBlockButtonPreview') ){
						if( MFT.MCSController.seat == 'driver' ){
							MFT.MCSController.mcsBlockSettingsDriver[MFT.MCSController.get('mcsActiveBlockButton')].pause = this.value;
						}else if( MFT.MCSController.seat == 'passenger' ){
							MFT.MCSController.mcsBlockSettingsPassenger[MFT.MCSController.get('mcsActiveBlockButton')].pause = this.value;
						}
						
					}
					
				}.observes('MFT.MCSController.mcsActiveBlockButtonPreview')
			}),
			
			preview: MFT.Button.extend({
				elementId:		'preview',
				classNames:		'btn btn-28 preview',
				text:			'Preview'
			})
		}),
		
		settingsScreen2: Em.ContainerView.extend({

			elementId:		'settingsScreen2',

			classNames:		'settingsScreen2',

			classNameBindings: ['MFT.MCSController.massageCustomLevel3:visible'],

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
				'frontLabel'
			],

			caption: MFT.Label.extend({
				classNames:	'MultiContourSeat_customMassage_caption',
				content:	'MCS Options: Custom Intensity Setting'
			}),

			adjusts: MFT.seatsAdjustView.extend({
				elementId:	'seatsMassageView6_1MCSOptions',
				classNameBindings: ['hide:hidden'],

				hide: function(){
					if( true == MFT.MCSController.massageCustomLevel3){
						return false;
					}else if( false == MFT.MCSController.massageCustomLevel3){
						return true;
					}
				}.property('MFT.MCSController.massageCustomLevel3'),
				start:	10,
				end:	15
			}),

			seatLabel: MFT.Label.extend({
				elementId:	'seatsMassageView6_1_seatLabel',
				classNames:	'seatsMassageView6_1_seatLabel',
				content:	function(){
					if( 'driver' == MFT.MCSController.seat){
						return 'Driver';
					}else if( 'passenger' == MFT.MCSController.seat){
						return 'Passenger';
					}
				}.property('MFT.MCSController.seat')
			}),

			upperLabel: MFT.Label.extend({
				elementId:	'seatsMassageView6_1_upperLabel',
				classNames:	'seatsMassageView6_1_upperLabel',
				content:	'Upper'
			}),

			middleLabel: MFT.Label.extend({
				elementId:	'seatsMassageView6_1_middleLabel',
				classNames:	'seatsMassageView6_1_middleLabel',
				content:	'Middle'
			}),

			lowerLabel: MFT.Label.extend({
				elementId:	'seatsMassageView6_1_lowerLabel',
				classNames:	'seatsMassageView6_1_lowerLabel',
				content:	'Lower'
			}),

			rearLabel: MFT.Label.extend({
				elementId:	'seatsMassageView6_1_rearLabel',
				classNames:	'seatsMassageView6_1_rearLabel',
				content:	'Rear'
			}),

			frontLabel: MFT.Label.extend({
				elementId:	'seatsMassageView6_1_frontLabel',
				classNames:	'seatsMassageView6_1_frontLabel',
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
				action:			'onMassageCustomPreview',
				target:			'MFT.MCSController',
				onDown:			false,
				previewId:		'0',

				classNames:		'btn btn-28',
				text:			'Preview'		
			}),

			previewBottom: MFT.Button.extend({
				elementId:		'MultiContourSeat_customMassage_previewButtonBottom',
				action:			'onMassageCustomPreview',
				target:			'MFT.MCSController',
				onDown:			false,
				previewId:		'1',

				classNames:		'btn btn-28',
				text:			'Preview'			
			})
		})
	})
});