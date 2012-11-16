/**
 * @name MFT.MCSController
 * 
 * @desc logic for Seat Comfort
 * 
 * @category	Controller
 * @filesource	app/controller/MCSController.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.MCSController = Em.Object.create({
		
	view: null,
	
	mcsDragId: null,

	mcsDropId: null,
	
	mcsDragAndDropInd: [
		0,

		1,

		2,

		3,

		4,

		5,

		6
	],
	
	mcsActiveBlockButton:		null,
	
	mcsAdjustDisable:			false,
	
	mcsAdjustDisableDriver:		false,
	
	mcsAdjustDisablePassenger:	false,
	
	mcsNewDisable:				false,
	
	mcsDeleteDisable:			false,
	
	mcsActiveBlockButtonPreview: null,
	
	mcsBlockSettingsDriver: {
		1:	{
				massage: 		0,
				patternTop: 	0,
				patternBottom: 	0,
				intensity:		0,
				speed:			0,
				duration:		0,
				pause:			0,
				position:		0,
				intensityCustom:{
					0:		1,
					1:		1,
					2:		1,
					3:		1,
					4:		1
				}
			},
		2:	{
				massage: 		0,
				patternTop: 	0,
				patternBottom: 	0,
				intensity:		0,
				speed:			0,
				duration:		0,
				pause:			0,
				position:		1,
				intensityCustom:{
					0:		1,
					1:		1,
					2:		1,
					3:		1,
					4:		1
				}
			},
		3:	{
				massage: 		0,
				patternTop: 	0,
				patternBottom: 	0,
				intensity:		0,
				speed:			0,
				duration:		0,
				pause:			0,
				position:		2,
				intensityCustom:{
					0:		1,
					1:		1,
					2:		1,
					3:		1,
					4:		1
				}
			},
			
		copy: function(src) {
			Ember.beginPropertyChanges();
			for(var i=1; i <= Object.keys(src).length - 1; i++){
				if(!this[i]){
					this[i] = {};
				}
				this[i].duration = src[i].duration;
				this[i].intensity = src[i].intensity;
				this[i].massage = src[i].massage;
				this[i].pattern = src[i].pattern;
				this[i].pause = src[i].pause;
				this[i].speed = src[i].speed;
				if(src[i].intensity == 3){
					this[i].intensityCustom = {};
						this[i].intensityCustom[0] = src[i].intensityCustom[0];
						this[i].intensityCustom[1] = src[i].intensityCustom[1];
						this[i].intensityCustom[2] = src[i].intensityCustom[2];
						this[i].intensityCustom[3] = src[i].intensityCustom[3];
						this[i].intensityCustom[4] = src[i].intensityCustom[4];
				}
			}
			Ember.endPropertyChanges();
		}
	},

	mcsBlockSettingsPassenger: {
		1:	{
				massage: 		0,
				patternTop: 	0,
				patternBottom: 	0,
				intensity:		0,
				speed:			0,
				duration:		0,
				pause:			0,
				position:		0,
				intensityCustom:{
					0:		1,
					1:		1,
					2:		1,
					3:		1,
					4:		1
				}
			},
		2:	{
				massage: 		0,
				patternTop: 	0,
				patternBottom: 	0,
				intensity:		0,
				speed:			0,
				duration:		0,
				pause:			0,
				position:		1,
				intensityCustom:{
					0:		1,
					1:		1,
					2:		1,
					3:		1,
					4:		1
				}
			},
		3:	{
				massage: 		0,
				patternTop: 	0,
				patternBottom: 	0,
				intensity:		0,
				speed:			0,
				duration:		0,
				pause:			0,
				position:		2,
				intensityCustom:{
					0:		1,
					1:		1,
					2:		1,
					3:		1,
					4:		1
				}
			},
			
		copy: function(src) {
			Ember.beginPropertyChanges();
			for(var i=1; i <= Object.keys(src).length - 1; i++){
				if(!this[i]){
					this[i] = {};
				}
				this[i].duration = src[i].duration;
				this[i].intensity = src[i].intensity;
				this[i].massage = src[i].massage;
				this[i].pattern = src[i].pattern;
				this[i].pause = src[i].pause;
				this[i].speed = src[i].speed;
				if(src[i].intensity == 3){
					this[i].intensityCustom = {};
						this[i].intensityCustom[0] = src[i].intensityCustom[0];
						this[i].intensityCustom[1] = src[i].intensityCustom[1];
						this[i].intensityCustom[2] = src[i].intensityCustom[2];
						this[i].intensityCustom[3] = src[i].intensityCustom[3];
						this[i].intensityCustom[4] = src[i].intensityCustom[4];
				}
			}
			Ember.endPropertyChanges();
		}
	},

	dialog:	{
				0: false,
				1: false,
				2: false,
				3: false,
				4: false,
				5: false
			},

	back: function() {
		if ( true == this.get('massageCustom')){
			if(  this.get('massageCustomLevel3') == true ){
				this.set('massageCustomLevel3', false);
			}else if(  this.get('massageCustomLevel2') == true ){
				this.set('massageCustomLevel2', false);
				MFT.MCSController.set('mcsActiveBlockButtonPreview', true);
			}else{
				this.set('massageCustom', false);
			}
		}else{
			MFT.States.back();
		}
		MFT.MCSController.set('mcsActiveBlockButtonPreview', false);
	},

	/** Active model */
	model: null,

	/** Variable for settings window switch */
	massageCustom: false,

	/** Variable for settings window switch */
	massageCustomLevel2: false,

	/** Variable for settings window switch */
	massageCustomLevel3: false,

	/** Variable for driver/passenger switch */
	seat: 'driver',

	/** Variable for adjust/massage switch */
	mode: 'adjust',

	/** Controller initialization */
	init: function() {		
		/** Model binding */
		this.set('model', FLAGS.MCS_EMULATE_CAN ? MFT.SettingsModel : MFT.SettingsModelCAN );
		
		this.set('activeSeat', this.model.driverSeat);
	},

	/** Active seat data */
	activeSeat: 			null,

	/** Index of selected item */
	selectedIndex: 			0,
	
	/** Flag for highlighting selected element */
	highlighted: 			false,
	
	/** Cushion massage custom mode flag */
	massageCustomShow:		false,
	
	/** Cushion massage value */
	savedValue7_1: 			0,

	/**
	 * Toggle when user select item
	 * @param element: View
	 */
	select: function(element) {
		this.set( 'selectedIndex' , element._parentView.get('index') );
	},

	/**
	 * Function for call increase method of appropriate active seat
	 * handler for minus button that increases indicators value
	 * @param element: View button that trigger action
	 */
	onIncrease: function(element){
		this.select(element);
		this.model.onIncrease(element._parentView.get('index'), this.seat);
	},
	
	/**
	 * Function for call decrease method of appropriate active seat
	 * handler for minus button that decreases indicators value
	 * @param element: View button that trigger action
	 */
	onDecrease: function(element){
		this.select(element);
		this.model.onDecrease(element._parentView.get('index'), this.seat);
	},

	/**
	 * Handler for massage buttons toggle
	 * @param element: View button that trigger action
	 */
	onChangeMassageValue: function(element){

		this.select(element);

		this.model.changeMassageValue(this.selectedIndex, element.get('index'), this.seat);

		if( FLAGS.MCS_ACTIVE_CONCEPT == 1.1 && element.get('index') != 0) 
		{			
			switch( this.selectedIndex )
			{
				case 6 	: this.activeSeat[7].set('value', 0); this.activeSeat[9].set('value', 0); break;
				case 8 	: this.activeSeat[5].set('value', 0); this.activeSeat[9].set('value', 0); break;
				case 10 : this.activeSeat[5].set('value', 0); this.activeSeat[7].set('value', 0); break;
				case 7 	: this.activeSeat[8].set('value', 0); this.activeSeat[10].set('value', 0); break;
				case 9 	: this.activeSeat[6].set('value', 0); this.activeSeat[10].set('value', 0); break;
				case 11 : this.activeSeat[6].set('value', 0); this.activeSeat[8].set('value', 0); break;
			}
		}
	},

	/** 
	 * Function sets seat for driver and passenger
	 * @param element: View button that trigger action
	 */
	onChangeSeat: function(element){
		if('passenger' == element.get('id')){
			this.set('seat', 'passenger');
			
			this.set('activeSeat', this.model.passengerSeat);
			
		}else if('driver' == element.get('id')){
			this.set('seat', 'driver');
			
			this.set('activeSeat', this.model.driverSeat);
		}
	},
	
	/**
	 * Function sets adjust and massage mode 
	 * @param element: View button that trigger action
	 */
	onChangeSeatsMode: function(element){
		if('adjust' == element.get('id')){
			this.set('mode', 'adjust');
		}else if('massage' == element.get('id')){
			this.set('mode', 'massage');
		}
	},	
	
	/** 
	 * Function for detecting seat switching
	 */
	seatsSwitching: function() {
			this.set('mcsActiveBlockButton', null);
	}.observes('this.seat'),

	/** 
	 * Function for detecting drivers seat
	 * @property seat: selected seat
	 */
	seatsDriverSwitching: function() {
	  if ('driver' == this.seat){
			return true;
			this.set('mcsActiveBlockButton', null);
		}
		else{
			return false;
			this.set('mcsActiveBlockButton', null);
		}
	}.property('this.seat'),

	/** 
	 * Function for detecting passengers seat
	 * @property seat: selected seat
	 */
	seatsPassengerSwitching: function(){
		if ('passenger' == this.seat){
			return true;
		}
		else{
			return false;
		}
	}.property('this.seat'),

	/**
	 * Function for detecting adjust mode 
	 * @property mode: selected mode
	 */
	seatsAdjustModeSwitching: function() {
	  if ('adjust' == this.mode){
			return true;
		}
		else{
			return false;
		}
	 }.property('this.mode'),

	/** 
	 * Function for detecting massage mode
	 * @property mode: selected mode
	 */
	seatsMassageModeSwitching: function(){
		if ('massage' == this.mode){
			return true;
		}
		else{
			return false;
		}
	}.property('this.mode'),

	/** Custom massage open handeler */
	onMassageCustomOpen: function() {
		if( (6.1 == FLAGS.MCS_ACTIVE_CONCEPT || 6.2 == FLAGS.MCS_ACTIVE_CONCEPT) && this.get('massageCustom') == true )
		{
			if( this.get('massageCustomLevel2') == true ){
				this.set('massageCustomLevel3', true);
			}else{
				this.set('massageCustomLevel2', true);
			}
		}else{
			this.set('massageCustom', true);
		}
	},

	/** Custom massage close handeler */
	onMassageCustomClose: function() {
		this.set('massageCustom', false);
	},

	/** Custom massage preview handeler */
	onMassageCustomPreview: function(element) {
		if (element == true){
			MFT.MCSController.activeSeat[17].set('value', MFT.MCSController.activeSeat[10].value);
			MFT.MCSController.activeSeat[18].set('value', MFT.MCSController.activeSeat[11].value);
			MFT.MCSController.activeSeat[19].set('value', MFT.MCSController.activeSeat[12].value);
			
			MFT.MCSController.activeSeat[20].set('value', MFT.MCSController.activeSeat[13].value);
			MFT.MCSController.activeSeat[21].set('value', MFT.MCSController.activeSeat[14].value);
			
			if( 3 == MFT.MCSController.activeSeat[16].get('value') ){
				MFT.MCSController.activeSeat[8].set('value', MFT.MCSController.activeSeat[13].value);
				MFT.MCSController.activeSeat[9].set('value', MFT.MCSController.activeSeat[14].value);
			}
			
			if( 3 == MFT.MCSController.activeSeat[15].get('value') ){
				MFT.MCSController.activeSeat[5].set('value', MFT.MCSController.activeSeat[10].value);
				MFT.MCSController.activeSeat[6].set('value', MFT.MCSController.activeSeat[11].value);
				MFT.MCSController.activeSeat[7].set('value', MFT.MCSController.activeSeat[12].value);
			}
		}
		else{
			MFT.MCSController.activeSeat[5].set('value', MFT.MCSController.activeSeat[10].value);
			MFT.MCSController.activeSeat[6].set('value', MFT.MCSController.activeSeat[11].value);
			MFT.MCSController.activeSeat[7].set('value', MFT.MCSController.activeSeat[12].value);
			MFT.MCSController.activeSeat[8].set('value', MFT.MCSController.activeSeat[13].value);
			MFT.MCSController.activeSeat[9].set('value', MFT.MCSController.activeSeat[14].value);
			MFT.MCSController.set('massageCustom', false);
		}
	},

	onStartMCSSettings: function(){
		MFT.MCSController.set('massageCustomLevel2', true);
		MFT.VideoPlayerController.set('disableControlls', true);
		MFT.VideoPlayerController.start('MCS_avatar_prompts');
		MFT.MCSController.set('massageCustom', true);
		popUps = MFT.MCSConcept7_1_VideoHandler.create();
	},

	clearBlocks: function(){
		if(!this.get('massageCustom')){
			if ( (6.2 == FLAGS.MCS_ACTIVE_CONCEPT) || (6.1 == FLAGS.MCS_ACTIVE_CONCEPT) ){
				if(this.seat == 'driver' ){
					for(var i in this.mcsBlockSettingsDriver)
					{
						if(("copy" != i) && (null != i) && (null != Ember.View.views[i])){
							this.mcsBlockSettingsDriver[i].position = parseInt($("#" + i).attr('position'));
							Ember.View.views[i].destroy();
						}
					}
				}else if( this.seat == 'passenger' ){
					for(var i in this.mcsBlockSettingsPassenger)
					{
						if(("copy" != i) && (null != i) && (null != Ember.View.views[i])){
							this.mcsBlockSettingsPassenger[i].position = parseInt($("#" + i).attr('position'));
							Ember.View.views[i].destroy();
						}
					}
				}
			}
		}
	}.observes('this.massageCustom'),
	
	createBlocks: function(){
		if(this.get('massageCustom')){
			if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
			
				var countOfBlocks = 0;
				if(this.seat == 'driver' ){
					for(var i in this.mcsBlockSettingsDriver)
					{
						if(("copy" != i) && (null != i) && (null == Ember.View.views[i]) ){

							button = MFT.Button.create( MFT.Dragable, {
								elementId:			i,
								classNames:			'btn btn-48 seatsMassageView_settingsBlock ',
								text:				'Block' + i,
								attributeBindings:	['style','position'],
								position:			this.mcsBlockSettingsDriver[i].position,
								click: 				function () {
														MFT.MCSController.blockManage(this.elementId);
													},
								style : 			function(){
														return "left: " + this.position * 105 + "px;";
													}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
							});

							MFT.MCSController.view.get('childViews')[10].blocks.get('childViews').pushObject(button);
							countOfBlocks++;

						}
						
					}

					if(countOfBlocks == 0){
						this.set('mcsNewDisable', false);
						countOfBlocks = 3;
					}else if(countOfBlocks < 6){
						this.set('mcsNewDisable', false);
						if(countOfBlocks == 1){
							this.set('mcsDeleteDisable', true);
						}
					}else if(countOfBlocks == 6){
						this.set('mcsNewDisable', true);
						this.set('mcsDeleteDisable', false);
					}
					
				}else if( this.seat == 'passenger' ){
					for(var i in this.mcsBlockSettingsPassenger)
					{
						if(("copy" != i) && (null != i) && (null == Ember.View.views[i]) ){

							button = MFT.Button.create( MFT.Dragable, {
								elementId:			i,
								classNames:			'btn btn-48 seatsMassageView_settingsBlock ',
								text:				'Block' + i,
								attributeBindings:	['style','position'],
								position:			this.mcsBlockSettingsPassenger[i].position,
								click: 				function () {
														MFT.MCSController.blockManage(this.elementId);
													},
								style : 			function(){
														return "left: " + this.position * 105 + "px;";
													}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
							});

							MFT.MCSController.view.get('childViews')[10].blocks.get('childViews').pushObject(button);
							countOfBlocks++;

						}
					}

					if(countOfBlocks == 0){
						this.set('mcsNewDisable', false);
						countOfBlocks = 3;
					}else if(countOfBlocks < 6){
						this.set('mcsNewDisable', false);
						if(countOfBlocks == 1){
							this.set('mcsDeleteDisable', true);
						}
					}else if(countOfBlocks == 6){
						this.set('mcsNewDisable', true);
						this.set('mcsDeleteDisable', false);
					}
				}
				$('#seatsMassageView6_2').find('#SettingsBlocks').css('width', (countOfBlocks) * 105 + 'px' );
			}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){

				var countOfBlocks = 0;
				if(this.seat == 'driver' ){
					for(var i in this.mcsBlockSettingsDriver)
					{
						if(("copy" != i) && (null != i) && (null == Ember.View.views[i]) ){

							button = MFT.Button.create({
								elementId:			i,
								classNames:			'btn btn-28 seatsMassageView_settingsBlock ',
								classNameBindings:	['isActive:active'],
								isActive:			function () {
									if( this.elementId == MFT.MCSController.mcsActiveBlockButton ){
										return true;
									}else{
										return false;
									}
								}.property('MFT.MCSController.mcsActiveBlockButton'),
								text:				'Line' + i,
								attributeBindings:	['style','position'],
								position:			this.mcsBlockSettingsDriver[i].position,
								click: 				function () {
														MFT.MCSController.set('mcsActiveBlockButton', parseInt(this.elementId));
														MFT.MCSController.set('mcsDragId', parseInt(this.elementId) - 1);
													},
								style : 			function(){
														return "top: " + (this.position * 35 + 50) + "px;";
													}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
							});

							MFT.MCSController.view.get('childViews')[10].blocks.get('childViews').pushObject(button);
							countOfBlocks++;

						}
						
					}

					if(countOfBlocks == 0){
						this.set('mcsNewDisable', false);
						countOfBlocks = 3;
					}else if(countOfBlocks < 6){
						this.set('mcsNewDisable', false);
						if(countOfBlocks == 1){
							this.set('mcsDeleteDisable', true);
						}
					}else if(countOfBlocks == 6){
						this.set('mcsNewDisable', true);
						this.set('mcsDeleteDisable', false);
					}
					
				}else if( this.seat == 'passenger' ){
					for(var i in this.mcsBlockSettingsPassenger)
					{
						if(("copy" != i) && (null != i) && (null == Ember.View.views[i]) ){

							
							button = MFT.Button.create({
								elementId:			i,
								classNames:			'btn btn-28 seatsMassageView_settingsBlock ',
								classNameBindings:	['isActive:active'],
								isActive:			function () {
									if( this.elementId == MFT.MCSController.mcsActiveBlockButton ){
										return true;
									}else{
										return false;
									}
								}.property('MFT.MCSController.mcsActiveBlockButton'),
								text:				'Line' + i,
								attributeBindings:	['style','position'],
								position:			this.mcsBlockSettingsPassenger[i].position,
								click: 				function () {
														MFT.MCSController.set('mcsActiveBlockButton', parseInt(this.elementId));
														MFT.MCSController.set('mcsDragId', parseInt(this.elementId) - 1);
													},
								style : 			function(){
														return "top: " + (this.position * 35 + 50) + "px;";
													}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
							});

							MFT.MCSController.view.get('childViews')[10].blocks.get('childViews').pushObject(button);
							countOfBlocks++;

						}
					}

					if(countOfBlocks == 0){
						this.set('mcsNewDisable', false);
						countOfBlocks = 3;
					}else if(countOfBlocks < 6){
						this.set('mcsNewDisable', false);
						if(countOfBlocks == 1){
							this.set('mcsDeleteDisable', true);
						}
					}else if(countOfBlocks == 6){
						this.set('mcsNewDisable', true);
						this.set('mcsDeleteDisable', false);
					}
				}
			}
		}
	}.observes('this.massageCustom'),

	stopDrag: function() {
		var from = this.mcsDragId,
			to   = this.mcsDropId,
			temp = null;
		
		if(this.mcsDragId >= 0){
			if(from < to){

				var blocks = $('#SettingsBlocks').find("div[position]");

				for(var i = 0; i < blocks.length; i++){
					blocks[i] = $('#SettingsBlocks').find('div[position=' + i + ']');
				}

				temp = $(blocks[to]).attr('position');

				for(var i = to; i >= from; i--){
					$(blocks[i]).attr('position', $(blocks[i-1]).attr('position'));
				}

				$(blocks[from]).attr('position', to);

				for(var i = 0; i <= blocks.length; i++){
					
					if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
						$(blocks[i]).css('left', $(blocks[i]).attr('position') * 105 + 'px');
					}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){
						$(blocks[i]).css('top', ($(blocks[i]).attr('position') * 35 + 50) + 'px');
					}
				}
			} else if(from > to){

				var blocks = $('#SettingsBlocks').find("div[position]");

				for(var i = 0; i < blocks.length; i++){
					blocks[i] = $('#SettingsBlocks').find('div[position=' + i + ']');
				}

				temp = $(blocks[from]).attr('position');

				for(var i = to + 1; i <= from; i++){
					$(blocks[i-1]).attr('position', $(blocks[i]).attr('position'));
				}

				$(blocks[from]).attr('position', to);

				for(var i = 0; i <= blocks.length; i++){
					if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
						$(blocks[i]).css('left', $(blocks[i]).attr('position') * 105 + 'px');
					}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){
						$(blocks[i]).css('top', ($(blocks[i]).attr('position') * 35 + 50) + 'px');
					}
				}
			}
		}
	},

	blockManage: function(id) {
		this.set('mcsActiveBlockButton', parseInt(id));
		this.onMassageCustomOpen();
	},

	dropToTrash: function() {
		var from = this.mcsDragId,
			temp = null;

		var blocks = $('#SettingsBlocks').find("div[position]");

		if((blocks.length > 1) && (this.mcsDragId >= 0)){

			this.set('mcsNewDisable', false);

			var deletedId = $('#SettingsBlocks').find("div[position=" + from + "]").attr('id');

			for(var i = 0; i < blocks.length; i++){
				blocks[i] = $('#SettingsBlocks').find('div[position="' + i + '"]');
			}

			for(var i = blocks.length; i > from; i--){
				$(blocks[i]).attr('position', $(blocks[i-1]).attr('position'));
			}

			for(var i = 0; i <= blocks.length; i++){
				if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
					$(blocks[i]).css('left', $(blocks[i]).attr('position') * 105 + 'px');
				}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){
					$(blocks[i]).css('top', ($(blocks[i]).attr('position') * 35 + 50) + 'px');
				}
			}

			if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
				$('#seatsMassageView6_2').find('#SettingsBlocks').css('width', (blocks.length - 1) * 105 + 'px' );
			}
			
			Ember.View.views[deletedId].destroy();
			
			if(this.seat == 'driver' ){
				delete this.mcsBlockSettingsDriver[deletedId];
			}else if( this.seat == 'passenger' ){
				delete this.mcsBlockSettingsPassenger[deletedId];
			}
			
			if(blocks.length == 2){
				
				this.set('mcsDeleteDisable', true);
				
			}
			this.set('mcsActiveBlockButton', null);
			this.set('mcsDragId', null);
		}
	},

	addNewBlock: function(element) {
		var temp = 0,
			button;

		var blocks = $('#SettingsBlocks').find("div[position]");

		if(blocks.length < 6){
		
			this.set('mcsDeleteDisable', false);
		
			for(var i = 0; i < blocks.length; i++){
				if( parseInt($(blocks[i]).attr('id')) > parseInt(temp) ){
					temp = parseInt($(blocks[i]).attr('id'));
				}
			}

			if ( 6.2 == FLAGS.MCS_ACTIVE_CONCEPT ){
				button = MFT.Button.create( MFT.Dragable, {
					elementId:			(parseInt(temp) + 1),
					classNames:			'btn btn-48 seatsMassageView_settingsBlock ',
					text:				'Block' + (parseInt(temp) + 1),
					attributeBindings:	['style','position'],
					position:			blocks.length,
					click: 				function () {
											MFT.MCSController.blockManage(this.elementId);
										},
					style : 			function(){
											return "left: " + this.position * 105 + "px;";
										}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
				});
			}else if ( 6.1 == FLAGS.MCS_ACTIVE_CONCEPT ){
				button = MFT.Button.create({
					elementId:			(parseInt(temp) + 1),
					classNames:			'btn btn-28 seatsMassageView_settingsBlock ',
					classNameBindings:	['isActive:active'],
					isActive:			function () {
											if( this.elementId == MFT.MCSController.mcsActiveBlockButton ){
												return true;
											}else{
												return false;
											}
										}.property('MFT.MCSController.mcsActiveBlockButton'),
					text:				'Line' + (parseInt(temp) + 1),
					attributeBindings:	['style','position'],
					position:			blocks.length,
					click: 				function () {
											MFT.MCSController.set('mcsActiveBlockButton', parseInt(this.elementId));
											MFT.MCSController.set('mcsDragId', parseInt(this.elementId) - 1);
										},
					style : 			function(){
											return "top: " + (this.position * 35 + 50) + "px;";
										}.property('MFT.MCSController.mcsDragAndDropInd.0', 'MFT.MCSController.mcsDragAndDropInd.1', 'MFT.MCSController.mcsDragAndDropInd.2', 'MFT.MCSController.mcsDragAndDropInd.3', 'MFT.MCSController.mcsDragAndDropInd.4', 'MFT.MCSController.mcsDragAndDropInd.5', 'MFT.MCSController.mcsDragAndDropInd.6')
				});
			}

			element._parentView.blocks.get('childViews').pushObject(button);

			$('#seatsMassageView6_2').find('#SettingsBlocks').css('width', (blocks.length + 1) * 105 + 'px' );
			if(this.seat == 'driver' ){
				MFT.MCSController.mcsBlockSettingsDriver[(parseInt(temp) + 1)] = {
					massage: 		0,
					patternTop: 	0,
					patternBottom: 	0,
					intensity:		0,
					speed:			0,
					duration:		0,
					pause:			0,
					intensityCustom:{
						0:		1,
						1:		1,
						2:		1,
						3:		1,
						4:		1
					}
				};
			}else if( this.seat == 'passenger' ){
				MFT.MCSController.mcsBlockSettingsPassenger[(parseInt(temp) + 1)] = {
					massage: 		0,
					patternTop: 	0,
					patternBottom: 	0,
					intensity:		0,
					speed:			0,
					duration:		0,
					pause:			0,
					intensityCustom:{
						0:		1,
						1:		1,
						2:		1,
						3:		1,
						4:		1
					}
				};
			}
			
			if(blocks.length == 5){
				MFT.MCSController.set('mcsNewDisable', true);
			}

		}
	}
});