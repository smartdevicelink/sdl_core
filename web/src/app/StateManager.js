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
	
	// one previous state
	previousState: null,
	
	// add verification before state transition
	goToState: function() {
		// return if on current state
		if( arguments[0] === this.currentState.get('path') ) {
			return false;
		}
		// init transition if ready
    	if( MFT.TransitionIterator.get('ready') ) {
    		  
			this.set('previousState',this.currentState.get('path'));
    		MFT.TransitionIterator.init( arguments[0] );
    		
    		this.transitionTo.apply( this, arguments );
            
            // call visitor if target state is already active,
            // e. g. when transition triggered from child state
            if ( this.get(arguments[0]).active ) {
                MFT.StateVisitor.visit( this.get(arguments[0]) );
            }
    	}
  	},
	
	/** 
	 * Go to previous state
	 */
	goToPreviousState: function(){
		if(this.previousState){
			this.goToState(this.previousState);
			
			/* Temporary Fix of transition iterator */
			//MFT.StateVisitor.visit(this.currentState);
		}
	},
	
	/** Go to parent state */
	back: function() {		
		if ( this.currentState.parentState.hasOwnProperty('name') ) {
			this.goToState( this.currentState.parentState.get('path') );
		} else {
			this.goToState('home');
		}
		
		//MFT.StateVisitor.visit(this.currentState);
	},
	
	/** Home state */
	home: Em.State.create({
		enter: function(){
			this._super();
			MFT.MediaController.set('isHomeStateActive',true);
		},		
		
		exit: function(){
			this._super();
			MFT.MediaController.set('isHomeStateActive',false);
			MFT.MediaController.updateHomeMediaStatusInfo();
		}
				
	}),
	
	/** FAQ state */
	faq: Em.State.create({
	
		enter: function() {
            var path = MFT.States.currentState.get('path');
            if(path != 'appSettings'){
                this.from = MFT.States.currentState.get('path');
            }
			this._super();
		},
		
		from: null
	}),

    /** App settings state **/
    appSettings: Em.State.create({

        configure: Em.State.create({

        }),

        appRate: Em.State.create({

        }),

        about: Em.State.create({

        }),

        exit: function() {
            this._super();

            /**
             *  Set the active state for AppSettings controller
             */
            var path = MFT.States.currentState.get('path');
            MFT.AppSettingsController.set('activeState', path);
        },

        enter: function() {
            var path = MFT.States.currentState.get('path');
            if(path != 'faq'){
                this.from = path;
            }
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

		}),
		
		fusion: Em.State.create({

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

        whereAmI: Em.State.create({

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
					
				})
			}),
			
			voiceControl: Em.State.create({
			
			}),
			
			mediaPlayer: Em.State.create({
				
				bluetoothDevices: Em.State.create({
					
				})
			}),
			
			snavigation: Em.State.create({

				mapPreferences: Em.State.create({
					
				}),
				routePreferences: Em.State.create({
				
				}),
				navigationPreferences: Em.State.create({
					
				}),
				trafficPreferences: Em.State.create({
					
				}),
				avoidAreas: Em.State.create({
					
				})
			}),
			wirelessInternet: Em.State.create({
			
			})			
		}),
		
		help: Em.State.create({

			systemInformation: Em.State.create({
				
			}),
			
			softwareLicenses: Em.State.create({
								
			}),
			
			drivingRestrictions: Em.State.create({
								
			}),
			
			helpAssist: Em.State.create({

			}),
			
			voiceCommandList: Em.State.create({
                audio: Em.State.create({

                }),
                browse: Em.State.create({

                }),
                climate: Em.State.create({

                }),
                disc: Em.State.create({

                }),
                globalCommands: Em.State.create({

                }),
                media: Em.State.create({

                }),
                navigationCommands: Em.State.create({

                }),
                otherCommands: Em.State.create({

                }),
                phone: Em.State.create({

                }),
                radio: Em.State.create({

                }),
                sirius: Em.State.create({

                }),
                travelLink: Em.State.create({

                }),
                voiceInstructions: Em.State.create({

                }),
				voiceSettings: Em.State.create({

                })
			})
		})
	}),
		
	/** Media state */
	media: Em.State.create({
		
		exit: function() {
			MFT.MediaController.set('activeState', MFT.States.currentState.get('path'));
			// Direct tune is reseted if it was open
			MFT.MediaController.resetDirectTune();
			// clear Direct tune timer
			if(MFT.MediaController.directTuneTimer){
				clearInterval(MFT.MediaController.directTuneTimer);
			}
			// reset Messages
			MFT.MediaController.resetUpdatingMessage();

			// Show HomeMediastatus,Radia labels
			MFT.MediaController.updateMediaHomeStatus();			
			MFT.MediaController.updateHomeMediaStatusInfo();	
			MFT.MediaController.set('isPresetActivated',false);
			this._super();
		},
		

		radio: Em.State.create({
			
			am: Em.State.create({
				enter: function(){
					this._super();
					MFT.MediaController.onRadioEnter(MFT.AmModel);
				},
				exit: function(){
					this._super();
					MFT.MediaController.onRadioExit(MFT.AmModel);
				},

				options: Em.State.create({
					exit: function(){
						this._super();
						MFT.MediaController.resetPopUp('am_refreshPresetPopup');
					}
				})
			}),
			
			fm: Em.State.create({
				enter: function(){
					this._super();
					MFT.MediaController.onRadioEnter(MFT.FmModel);
				},
				exit: function(){
					this._super();
					MFT.MediaController.onRadioExit(MFT.FmModel);
				},

				options: Em.State.create({
					exit: function(){
						this._super();
						MFT.MediaController.resetPopUp('fm_refreshPresetPopup');
					},
					
					PTYCategory: Em.State.create({
						enter: function(){
							this._super();
							MFT.MediaController.resetPopUp('fm_refreshPresetPopup');
						}
					})
				})
			
			}),
			
			sirius: Em.State.create({
				enter: function(){
					this._super();
					MFT.MediaController.onRadioEnter(MFT.SiriusModel);
				},
				exit: function(){
					this._super();
					MFT.MediaController.onRadioExit(MFT.SiriusModel);
				},

				browse: Em.State.create({
					browseall: Em.State.create({
						enter: function(){
							this._super();
							MFT.MediaController.resetDirectTune();
						}
					})
				}),
				
				options: Em.State.create({
					PTYCategory: Em.State.create({
					}),
					changePIN: Em.State.create({
					})
				})
			})
		}),
		
		player: Em.State.create({
			
			cd: Em.State.create({
				
				exit: function(){
					this._super();
					MFT.MediaController.currentSelectedPlayer.pause();
				},
				
				options: Em.State.create({

				}),
				
				browse: Em.State.create({
					browseall: Em.State.create({
						enter: function(){
							this._super();
							MFT.MediaController.resetDirectTune();
						}
					})
				}),
				
				moreinfo:Em.State.create({		
				})
			
			}),
			
			usb: Em.State.create({
				exit: function(){
					this._super();
					MFT.MediaController.currentSelectedPlayer.pause();
                    MFT.MediaController.resetUpdatingMessage();
				},

				options: Em.State.create({
					deviceInformation: Em.State.create({
					})
					
				}),
				
				browse: Em.State.create({
                    enter: function() {
                        this._super();
                        // reset Messages
                        MFT.MediaController.resetUpdatingMessage();
                    },
					browseall: Em.State.create({
						enter: function(){
							this._super();
							MFT.MediaController.resetDirectTune();
						}
					})
				}),
				
				moreinfo:Em.State.create({
                    enter: function() {
                        this._super();
                        // reset Messages
                        MFT.MediaController.resetUpdatingMessage();
                    }
				})
			}),
			
			sd: Em.State.create({
				
				exit: function(){
					this._super();
					MFT.MediaController.currentSelectedPlayer.pause();
                    MFT.MediaController.resetUpdatingMessage();
				},

				options: Em.State.create({
					deviceInformation: Em.State.create({
					})
				}),
				
				browse: Em.State.create({
                    enter: function() {
                        this._super();
                        // reset Messages
                        MFT.MediaController.resetUpdatingMessage();
                    },
					browseall: Em.State.create({
						enter: function(){
							this._super();
							MFT.MediaController.resetDirectTune();
						}
					})
				}),
				
				moreinfo:Em.State.create({
                    enter: function() {
                        this._super();
                        // reset Messages
                        MFT.MediaController.resetUpdatingMessage();
                    }
				})
			
			}),
			
			bluetooth: Em.State.create({
				enter: function(){
					this._super();
					MFT.MediaController.offDirectTune();
					if(MFT.MediaController.directTuneSelected){
						MFT.MediaController.set('directTuneSelected', false);
					}
				    MFT.BTModel.range.set('value',MFT.BTModel.range.activeBand);
				},
				exit: function(){
					this._super();
					MFT.BTModel.range.set('activeBand', MFT.BTModel.range.value);
					MFT.BTModel.range.set('value',-1);
				},

				options: Em.State.create({
					deviceInformation: Em.State.create({
					})
				})
			
			})		
		}),
		
		avin: Em.State.create({
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
