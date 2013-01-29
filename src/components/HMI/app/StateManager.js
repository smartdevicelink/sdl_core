/**
 * @name MFT.States
 * 
 * @desc State manager definition
 * 
 * @category	State Manager
 * @filesource	app/StateManager.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

// Extend base Em.State object
Em.State.reopen({ 
	
	// used for determine display status
	active: false,
	
	// used for determine loading status
	viewLoaded: false,
	
	enter: function() {
		MFT.TransitionIterator.get('stateOnEnter').push(this);
		
		MFT.StateVisitor.visit(this);
	},
	
	exit: function() {
		MFT.TransitionIterator.get('stateOnExit').push(this);
	}
});


// State Manager class
var StateManager = Em.StateManager.extend({
	
	// default state
	initialState: MFT.homeView,
	
	// add verification before state transition
	goToState: function() {
		// return if on current state
		if( arguments[0] === this.currentState.get('path') ) {
			return false;
		}
		// init transition if ready
    	if( MFT.TransitionIterator.get('ready') ) {
    		MFT.TransitionIterator.init( arguments[0] );
    		return this.transitionTo.apply( this, arguments );
    	}
  	},
  	
	/** Go to parent state */
	back: function() {		
		if ( this.currentState.parentState.hasOwnProperty('name') ) {
			this.goToState( this.currentState.parentState.get('path') );
		} else {
			this.goToState('home');
		}
		
		MFT.StateVisitor.visit(this.currentState);
	},
	
	/** Home state */
	home: Em.State.create({
				
	}),
	
	/** FAQ state */
	faq: Em.State.create({
	
		enter: function() {
			this.from = MFT.States.currentState.get('path');
			this._super();
		},
		
		from: null
	}),
	
	/** Climate state */
	climate: Em.State.create({

		full:	Em.State.create({
			
			rear:	Em.State.create({
			
			})
		}),

		simple: Em.State.create({
			
		})
	}),
	
	/** info state */
	info: Em.State.create({
		
		exit: function() {
			MFT.InfoController.set('activeState', MFT.States.currentState.get('path'));
			this._super();
		},
		
		services: Em.State.create({
			
		}),
		
		travelLink: Em.State.create({
			
		}),
		
		alerts: Em.State.create({
			
		}),

		calendar: Em.State.create({
			
		}),
		
		apps: Em.State.create({
			
		}),

		devicelist: Em.State.create({

		}),
		
		nonMedia: Em.State.create({
            enter: function() {
                this._super();
                
                MFT.NonMediaController.restoreCurrentApp();
            }
		})
	}),
	
	/** settings state */
	settings: Em.State.create({	
			 
		exit: function() {
			this._super();
			
			var path = MFT.States.currentState.get('path');
			
			if ( path.split('.').length > 2 ) {
				MFT.SettingsController.set('activeState', path.substring( 0, path.indexOf('.',9) ));
			} else {
				MFT.SettingsController.set('activeState', path);
			}
		},
		
		clock: Em.State.create({
			
		}),
		
		display: Em.State.create({	
			
		}),
		
		sound: Em.State.create({
		
			setSoundBalanceFader: Em.State.create({
				
			})	
		}),

		vehicle: Em.State.create({
		
			ambientLighting: Em.State.create({
				
			}),
			
			healthReport: Em.State.create({
				
				enter: function() {
					if(MFT.States.currentState.get('path') != 'faq'){
						MFT.SettingsController.set('previousState',MFT.States.currentState.get('path'));
					}
					this._super();
				}
			}),
			
			doorKeypadCode: Em.State.create({
				
			}),

			rearViewCamera: Em.State.create({
				
			}),

			enterPIN: Em.State.create({
				
			})/*,
			
			mcs: Em.State.create({
				
			})*/
		}),
		
		settings: Em.State.create({
			
			system: Em.State.create({
				
				keyboardLayout: Em.State.create({
				
				}),
				
				installApplications: Em.State.create({
					
					enter: function() {
						if(MFT.States.currentState.get('path') != 'faq'){
							MFT.SettingsController.set('previousState',MFT.States.currentState.get('path'));
						}
						this._super();
					}
					
				})
			}),
			
			voiceControl: Em.State.create({
			
			}),
			
			mediaPlayer: Em.State.create({
				
				bluetoothDevices: Em.State.create({
					
				})
			}),
			
			snavigation: Em.State.create({
				enter: function() {
						if(MFT.States.currentState.get('path') != 'faq'){
							if(MFT.States.currentState.get('path') == 'navigation.menu'){
								MFT.SettingsController.set('previousState','navigation');
							}else{
								MFT.SettingsController.set('previousState',MFT.States.currentState.get('path'));
							}
						}
						this._super();
				},
				
				mapPreferences: Em.State.create({
					
				}),
				routePreferences: Em.State.create({
				
				}),
				navigationPreferences: Em.State.create({
					
				}),
				trafficPreferences: Em.State.create({
					
				}),
				avoidAreas: Em.State.create({
					
				}),
			}),
			wirelessInternet: Em.State.create({
			
			})			
		}),
		
		help: Em.State.create({
			
			whereAmI: Em.State.create({
							
			}),
			
			systemInformation: Em.State.create({
				
			}),
			
			softwareLicenses: Em.State.create({
								
			}),
			
			drivingRestrictions: Em.State.create({
								
			}),
			
			helpAssist: Em.State.create({
				enter: function() {
						if(MFT.States.currentState.get('path') != 'faq'){
							MFT.SettingsController.set('previousState',MFT.States.currentState.get('path'));
						}
						this._super();
				}
			}),
			
			voiceCommandList: Em.State.create({
				
			})
		})
	}),
		
	/** Media state */
	media: Em.State.create({
		
		exit: function() {
			MFT.MediaController.set('activeState', MFT.States.currentState.get('path'));
			MFT.MediaController.resetDirectTune();
			if(MFT.MediaController.directTuneTimer){
				clearInterval(MFT.MediaController.directTuneTimer);
			}
			this._super();
		},
		

		radio: Em.State.create({
		}),
		
		browse: Em.State.create({
		}),
		
		browseall: Em.State.create({	
		}),
		
		moreinfo:Em.State.create({		
		}),

		player: Em.State.create({
		}),
		
		bluetooth: Em.State.create({
		}),
		
		avin: Em.State.create({
		}),
		
		applink: Em.State.create({

			enter: function() {
                this._super();
                
                MFT.ApplinkMediaController.restoreCurrentApp();
            }	

		}),

		options: Em.State.create({
		})
	}),
	
	/** Navigation state */
	navigation: Em.State.create({

		exit: function() {
			if(MFT.States.currentState.get('path') == 'navigation.menu'){
				MFT.NavigationController.set('activeState', 'navigation');
			}else{
				MFT.NavigationController.set('activeState', MFT.States.currentState.get('path'));
			}
			this._super();
		},
		
		menu: Em.State.create({
		}),
		
		dest: Em.State.create({
			
			myHome: Em.State.create({
				
			}),
			
			favorites: Em.State.create({
				
			}),
			
			previousDestinations: Em.State.create({
				
			}),
			
			POI: Em.State.create({
				
			}),
			
			emergency: Em.State.create({
				
			}),
						
			streetAddress: Em.State.create({
				
			}),
			
			intersection: Em.State.create({
				
			}),
			
			cityCenter: Em.State.create({
				
			}),
			
			map: Em.State.create({
				
			}),
			
			previousStartingsPoint: Em.State.create({
				
			}),
			
			freeWay: Em.State.create({
				
			}),
			
			latitudeLongitude: Em.State.create({
				
			})
		})
	}),
	
	/** Phone state */
	phone: Em.State.create({

		enter: function() {
			MFT.PhoneController.set('hideMenu', false);
			
			this._super();
		},
		
		exit: function() {
			this._super();
			
			var path = MFT.States.currentState.get('path');
			
			if ( path.split('.').length > 2 ) {
				MFT.PhoneController.set('activeState', path.substring( 0, path.indexOf('.',6) ));
			} else {
				MFT.PhoneController.set('activeState', path);
			}
		},
		
		dialpad: Em.State.create({
			
		}),
		quickdial: Em.State.create({
		
		}),
		phonebook: Em.State.create({
		
		}),
		history: Em.State.create({
		
		}),
		messaging: Em.State.create({
		
		}),
		phoneSettings: Em.State.create({
			
			bluetoothDevices: Em.State.create({

			}),
			
			phoneRinger: Em.State.create({

			}),
			
			textMessage: Em.State.create({

			}),
			
			internetDataConnection: Em.State.create({

			}),
			
			managePhoneBook: Em.State.create({

			})
		})
	})
});
