/**
 * @name MFT.ClimateController
 * 
 * @desc Climate module logic
 * 
 * @category	Controller
 * @filesource	app/controller/ClimateController.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ClimateController = Em.Object.create({
    
    init: function() {
        this.setClimateModel();
    },
	
	/** Model binding depends on application mode */
	//modelBinding: FLAGS.EMULATE_CAN_CLIMATE ? 'MFT.ClimateModel' : 'MFT.ClimateModelCAN',
    model: null,
    
    	
	/** Initial substate */
	activeState: 'climate.full',
	
	/** PROPERTY */
	
	/** Flag for front climate controls visibility */
	hideFront: false,
	
	/** Flag for rear climate controls visibility */
	hideRear: true,
	
	/** Flags for rear temperature mode labels */
	isRearFullHeated: false,
	isRearFullCooled: false,
	
	/**
     * Set climate model depends on current climate settings
     */
    setClimateModel: function() {
        switch ( this.get('climateVechicleMode') ) {
            case 'climate.simple':
	 			this.set('model', MFT.FocusClimateModel);
	 			break;
	 		
	 		case 'climate.fusion':
	 		    this.set('model', MFT.FusionClimateModel);
	 			break;
	 			
	 		default:
	 		    this.set('model', MFT.EdgeClimateModel);
        }
        
        //this.model.reset();
    },
	
	/**
	 * Climate mode property
	 * 
	 * Observes vehicle model property
	 * needs to determine climate module visual mode
	 *
	 * @property FFW.Backend.vehicleModel
	 * @return {string} full, simple
	 */
	climateVechicleMode: function() {
    	MFT.VideoPlayerController.close();
        return 'climate.' + MFT.AppController.get('activeVehicleModel').climateType;
	}.property('MFT.AppController.activeVehicleModel'),

	// observer for vechicle mode
	// used to switch climate modes on runtime
	climateVechicleModeSwitcher: function() {
		if (MFT.States){
			if ( MFT.States.climate.active ) {
				MFT.States.goToState( this.get('climateVechicleMode') );				
			}
			
			this.setClimateModel( this.get('climateVechicleMode') );
		}
	}.observes('this.climateVechicleMode'),
	
	/**
	 * Property to determine rear climate button aviability
	 */
	isRearDisabled: function() {
	   return MFT.AppController.get('activeVehicleModel').isRearClimateDisabled;
	}.property('MFT.AppController.activeVehicleModel'),
	
	/**
	 * property to determine climate mode for Ford Focus and Escape 2013
	 */
	isSimple: function() {
		return this.get('climateVechicleMode') === 'climate.simple';
	}.property('this.climateVechicleMode'),
	
	/**
	 * property to determine climate mode for Ford Fusion
	 */
	isFusion: function() {
        return this.get('climateVechicleMode') === 'climate.fusion';
	}.property('this.climateVechicleMode'),
	
	/** EVENT HENDLERS */
	
	/** FRONT CONTROLS */
	
	/** Climate on/off handler */
	onActivate: function() {
		this.model.activate();
	},
	
	/** Show/hide climate rear controls */
	onToggleRear: function() {
        if (MFT.helpMode) {
            MFT.VideoPlayerController.start('climate_rear_Climate');
        } else {
            if (MFT.States.currentState.name === 'full') {
                MFT.States.goToState('climate.full.rear');
            } else {
                MFT.States.back();
            }
        }
	},
	
	/** Driver temperature increase handler */
	onIncDriverTemp: function() {
		this.model.increaseDriverTemp();
	},
	
	/** Driver temperature decrease handler */
	onDecDriverTemp: function() {
		this.model.decreaseDriverTemp();
	},
	
	/** Passanger temperature increase handler */
	onIncPassangerTemp: function() {
		this.model.increasePassangerTemp();
	},
	
	/** Passanger temperature decrease handler */
	onDecPassangerTemp: function() {
		this.model.decreasePassangerTemp();
	},
	
	/** Dual Mode toggle handler */
	onToggleDualMode: function() {
		// Help mode video
		if ( MFT.helpMode ) {
			// if climate in simple mode
			if (this.get('isSimple')) {
				MFT.VideoPlayerController.start('climate_Dual_B');
				return;
			}
			MFT.VideoPlayerController.start('climate_Dual_AC');
			
			return;
		}
		
		this.model.toggleDualMode();
	},
	
	/** My temperature feature activate handler */
	onActivateMyTemp: function() {
		// Help mode video
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('climate_My_temp_AC');
			
			return;
		}
		
		this.model.activateMyTemp();
	},
	
	/** Store my temperature feature handler */
	onStoreMyTemp: function() {		
		this.model.storeMyTemp();
		
		// Show popup
		$('#climate_storeTemp').show().animate({
			opacity: 1
		},500);
				
		// Hide popup after timeout
		setTimeout(function() {
			$('#climate_storeTemp').animate({
				opacity: 0
			},500,function() {
			 	$('#climate_storeTemp').css('display','none');	
			});
		},2000);
	},
	
	/** Driver heated seat button handler */
	onChangeDriverHeatedSeat: function() {
		this.model.changeDriverHeatedSeat();
	},
	
	/** Driver cooled seat button handler */
	onChangeDriverCooledSeat: function() {
		this.model.changeDriverCooledSeat();
	},
	
	/** Driver heated steer button handler */
	onChangeDriverHeatedSteer: function() {
		this.model.changeDriverHeatedSteer();
	},
	
	/** Passanger heated seat change button handler */
	onChangePassangerHeatedSeat: function() {
		this.model.changePassangerHeatedSeat();
	},
	
	/** Passanger cooled seat change button handler */
	onChangePassangerCooledSeat: function() {
		this.model.changePassangerCooledSeat();
	},
 	
 	/** Defrost button handler*/
 	onToggleDefrost: function() {
        this.model.toggleDefrost();	
 	},
	
	/* Max wind shield button handler */
 	onToggleMaxDefrost: function() {
 		this.model.toggleMaxDefrost();
 	},
	
	/** Rear defrost button handler */
 	onToggleRearDefrost: function() {
 		this.model.toggleRearDefrost();
 	},
	
	/** Auto button handler */
 	onToggleAuto: function() {
 		// Help mode video
		if ( MFT.helpMode ) {
			// if climate in simple mode
			if ( this.get('isSimple') || this.get('isFusion') ) {
				MFT.VideoPlayerController.start('climate_Auto_BC');
				return;
			}
			MFT.VideoPlayerController.start('climate_Auto_A');
			
			return;
		}
 		
 		this.model.toggleAuto();
 	},
 	
 	/** AC button handler */
 	onToggleAc: function() {
 		this.model.toggleAc();
 	},
 	
 	/** Max AC button handler */
 	onToggleMaxAc: function() {
 		// Help mode video
		if ( MFT.helpMode ) {
			// if climate in simple mode
			if (this.get('isSimple')) {
				MFT.VideoPlayerController.start('climate_Max_AC_B');
				return;
			}
			
			// if climate in fusion mode
			if (this.get('isFusion')) {
				MFT.VideoPlayerController.start('climate_Max_AC_C');
				return;
			}
			
			MFT.VideoPlayerController.start('climate_Max_AC_A');
			
			return;
		}
 		
 		this.model.toggleMaxAc();
 	},
 	
	/** Recirculation button handler */
 	onToggleRecirculation: function() {
 		// Help mode video
		if ( MFT.helpMode ) {
            
			MFT.VideoPlayerController.start('climate_Recirculation_ABC');
			
			return;
		}
 		
 		this.model.toggleRecirculation();
 	},
 	
 	/** Fan speed decrease handeler */
 	onDecFanSpeed: function() {
 		this.model.increaseFanSpeed();
 	},
 	
 	/** Fan speed increase handeler */
 	onIncFanSpeed: function() {
 		this.model.decreaseFanSpeed();
 	},
 	
 	/**
 	 * Airflow controls block
 	 */
 	onDefrostAirflow: function() {
 		this.model.setDefrostAirflow();
 	},
 	
 	onPanelAirflow: function() {
 		this.model.setPanelAirflow();
 	},
 	
 	onFeetAirflow: function() {
 		this.model.setFeetAirflow();
 	},
 	
 	onBothAirflow: function() {
 		this.model.setBothAirflow();
 	},
 	
 	
 	/** REAR CONTROLS */
 	
 	/** Rear control button handler */
 	onToggleRearControl: function() {
 		this.model.toggleRearControl();
 	},
 	
 	/** Rear power button handler */
 	onToggleRearPower: function() {
 		this.model.toggleRearPower();
 	},
 	
 	/** Rear fan speed decrease handeler */
 	onDecRearFanSpeed: function() {
 		this.model.increaseRearFanSpeed();
 	},
 	
 	/** Rear fan speed increase handeler */
 	onIncRearFanSpeed: function() {
 		this.model.decreaseRearFanSpeed();
 	},
 	
 	/** Rear temperature decrease handeler */
 	onDecRearTemp: function() {
 		this.model.increaseRearTemp();
 	},
 	
 	/** Rear temperatureincrease handeler */
 	onIncRearTemp: function() {
 		this.model.decreaseRearTemp();		
 	},
	
	/** PROPERTY OBSERVERS */
	
	/**
	 * Check temperature value
	 * for visual activation myTemp button 
	 *
	 * @property driverTemp, myTemp
	 * @return {boolean}
	 */
 	isMyTemp: function() {
 		return ( this.model.driverTemp.value === this.model.myTemp.value ) && this.model.on.value;
 	}.property('model.driverTemp.value','model.myTemp.value','model.on.value'),
 	
 	
 	/**  */
 	isOff: function() {
		return !this.model.on.value &&
            !this.model.driverHeatedSeat.value &&
            !this.model.driverCooledSeat.value &&
            !this.model.passangerHeatedSeat.value &&
            !this.model.passangerCooledSeat.value;
 		
 	}.property(
 		'model.on.value',
 		'model.driverHeatedSeat.value',
 		'model.driverCooledSeat.value',
 		'model.passangerHeatedSeat.value',
 		'model.passangerCooledSeat.value'
 	),
 	
 	/** 
 	 * Check rear temperature value
 	 * for hide/show rear temp labels
 	 *
 	 * @observes rearTemp
 	 */
 	onRearTempChange: function() {
 		if ( this.model.rearTemp.value == 1 ) {
 			this.set('isRearFullCooled', true) 
 		} else {
 			this.set('isRearFullCooled', false) 
 		}
 		
 		if ( this.model.rearTemp.value == this.model.rearTemp.range ) {
 			this.set('isRearFullHeated', true) 
 		} else {
 			this.set('isRearFullHeated', false) 
 		}
 	}.observes('model.rearTemp.value')
});
