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

/** Override base Em.State object */
Em.State.reopen({ 
	active: false,
	
	enter: function() {
		this.set('active',true);
		if (FLAGS.EXTENDED_LOG) {
			MFT.startTime = Date.now();	
		}
		
		/** Switch off video player if active */
		MFT.VideoPlayerController.close();
		
		// Live rendering view
		if ( this.view ) {
			// Root views
			if ( !this.parentState.view ) {
				this.parentState.view = MFT.views.activeAview;
			}
			
			
						
			// View first open
			if ( !this.parentState.view.get('childViews').contains(this.view) ) {	
				this.parentState.view.get('childViews').pushObject(this.view);
				
				/* Fade transitions for future releases
				this.parentState.view.get('childViews').pushObject(
					this.view.reopen({
						didInsertElement: function() {
							var self = this;
							
							setTimeout(function(){
								document.getElementById(self.elementId).style.opacity = 1;
							},100);
						}					
					})
				);
				*/
			}
			
			/* Fade transitions for future releases
			if( this.view.state == 'inDOM' ) {
				document.getElementById(this.view.elementId).style.opacity = 1;
			}
			*/
		}
	},
	
	exit: function() {
		this.set('active',false);
		
		/* Fade transitions for future releases
		if ( this.view ) {			
			document.getElementById(this.view.elementId).style.opacity = 0;
		}
		*/
	}	
});

var StateManager = Em.StateManager.extend({
	
	enableLogging: FLAGS.EXTENDED_LOG,
	
	/** default state */
	initialState: 'home',
	
	/** Go to parent state */
	back: function() {
		if ( this.currentState.parentState.hasOwnProperty('name') ) {
			this.goToState( this.currentState.parentState.name );
		} else {
			this.goToState('home');
		}
	},
	
	/** Home state */
	home: Em.State.create({
					
	}),
	
	/** FAQ state */
	faq: Em.State.create({
		view:	MFT.FAQView,
	
		enter: function() {
			this.from = MFT.States.currentState.get('path');
			this._super();
		},
		
		from: null
	}),
	
	/** Climate state */
	climate: Em.State.create({
		view:	MFT.ClimateView,
		
		front:	Em.State.create(),
		
		rear:	Em.State.create()
	}),
	
	/** info state */
	info: Em.State.create({
		
		view:	MFT.InfoView,
		
		enter: function() {
			if ( !MFT.views.get('childViews').contains(MFT.InfoView) ) {
				MFT.views.get('childViews').pushObject(MFT.InfoView);
			}
			
			this._super();
		},
		
		exit: function() {
			MFT.InfoController.set('activeState', MFT.States.currentState.get('path'));
			this._super();
		},
		
		services:			  Em.State.create({
			view: MFT.InfoServicesView,	
		}),
		
		travelLink:	  		Em.State.create({
		
			view: MFT.InfoTravelLinkView
		}),
		
		alerts:  	 			Em.State.create({
			
			view: MFT.InfoAlertsView
		}),

		calendar:  	 	      Em.State.create({
			
			view: MFT.InfoCalendarView
		}),
		
		apps:  	 			  Em.State.create({
			
			view: MFT.InfoAppsView
		})		
	}),
	
	/** settings state */
	settings: Em.State.create({
		view:	MFT.SettingsView,
		
		exit: function() {
			var path = MFT.States.currentState.get('path');
						
			if ( path.split('.').length > 2 ) {
				MFT.SettingsController.set('activeState',
					path.substring( 0, path.indexOf('.',9) )
				);
			} else {
				MFT.SettingsController.set('activeState', path);
			}
			
			this._super();
		},
		
		clock: Em.State.create({
			
			view:	MFT.SettingsClockView,
			
			enter: function() {
				this._super();
				MFT.SettingsController.set('time','--:--');
			},
			
			exit: function() {
				this._super();
				MFT.SettingsController.getTime(new Date);
			}
		
		}),
		display: Em.State.create({	
			
			view:	MFT.SettingsDisplayView,

		}),
		sound: Em.State.create({
			
			view:	MFT.SettingsSoundView,
		
			setSoundBalanceFader: Em.State.create({
				view:	MFT.SettingsSoundSetBalanceAndFadeView,
			}),			
		}),
		vehicle: Em.State.create({
		
			view:	MFT.SettingsVehicleView,
			
			ambientLighting:   	   Em.State.create({
				view:	MFT.SettingsVehicleAmbientLightingView
			}),
			
			vehicleHealthReport:   Em.State.create({
				
				enter: function() {
						if(MFT.States.currentState.get('path') != 'faq'){
							MFT.SettingsController.set('previousState',MFT.States.currentState.get('path'));
						}
						this._super();
				},
				
				view:	MFT.SettingsVehicleHealthReportView
			}),
				
			doorKeypadCode:		Em.State.create({
				view:	MFT.SettingsVehicleDoorKeypadCodeView
			}),

			rearViewCamera:		Em.State.create({
				view:	MFT.SettingsVehicleRearCameraView,
			}),

			enterPIN:		Em.State.create({
				
				enter: function(){
					MFT.SettingsVehicleEnableValetModeEnteringPINView.set('popUpHide',true);
					this._super();
				},
				
				view: MFT.SettingsVehicleEnableValetModeEnteringPINView
				
			}),

			mcs:	   Em.State.create({
				/*enter: function(){
					MFT.MultiContourSeat.appendTo('body');
					this._super();
				}*/
				view:	MFT.MultiContourSeat
			})

		}),
		settings: Em.State.create({
			
			view:	MFT.SettingsSettingsView,
			
			system:				Em.State.create({
				
				view:	MFT.SettingsSettingsSystemView,
				
				keyboardLayout:		Em.State.create({
				
					view:	MFT.SettingsSettingsSystemKeyboardLayoutView				
				}),
				
				installApplications:   Em.State.create({
					
					enter: function() {
						if(MFT.States.currentState.get('path') != 'faq'){
							MFT.SettingsController.set('previousState',MFT.States.currentState.get('path'));
						}
						this._super();
					},
					
					view:	MFT.SettingsSettingsSystemInstallApplicationsView,
					
					beginSoftwareInstallation:   	Em.State.create({}),
					
					viewSoftwareLicenses:		 Em.State.create({})
				}),
			}),	
			voiceControl:	  	  Em.State.create({
			
				view:	MFT.SettingsSettingsVoiceControlView
			}),
			mediaPlayer: 	   	   Em.State.create({
				
				view:	MFT.SettingsSettingsMediaPlayerView,
				
				bluetoothDevices:	  Em.State.create({
					
					view:    MFT.SettingsSettingsMediaPlayerBluetoothDevicesView
				})
			}),
			snavigation: 		    Em.State.create({
				
				view:	MFT.SettingsSettingsNavigationView,
				mapPreferences:    Em.State.create({
					
					view: MFT.SettingsSettingsNavigationMapPreferencesView
				}),
				routePreferences:    Em.State.create({
					view: MFT.SettingsSettingsNavigationRoutePreferencesView
				}),
				navigationPreferences:    Em.State.create({
					
					view: MFT.SettingsSettingsNavigationNavigationPreferencesView	
				}),
				trafficPreferences:    Em.State.create({
					
					view: MFT.SettingsSettingsNavigationTrafficPreferencesView
				}),
				avoidAreas:    Em.State.create({
					
					view: MFT.SettingsSettingsNavigationAreasView
				}),
			}),
			phone:			 	 Em.State.create({}),
			wirelessInternet:  	  Em.State.create({
			
				view:	MFT.SettingsSettingsWirelessInternetView
			})			
		}),
		
		help: Em.State.create({
			
			view:	MFT.SettingsHelpView,
			
			whereAmI:				Em.State.create({
							
				view:	MFT.SettingsHelpWhereAmIView
			}),
			
			systemInformation:	   Em.State.create({
				
				view:	MFT.SettingsHelpSystemInformationView
			}),
			
			softwareLicenses:		Em.State.create({
								
				view:	MFT.SettingsHelpSoftwareLicensesView
			}),
			
			drivingRestrictions:	 Em.State.create({
								
				view:	MFT.SettingsHelpDrivingRestrictionsView
			}),
			
			assist:			      Em.State.create({
				enter: function() {
						if(MFT.States.currentState.get('path') != 'faq'){
							MFT.SettingsController.set('previousState',MFT.States.currentState.get('path'));
						}
						this._super();
				},
			
				view:	MFT.SettingsHelp911AssistView
			}),
			
			voiceCommandList:		Em.State.create({
				
				view:	MFT.SettingsHelpVoiceCommandListView
			}),
		})
	}),
		
	/** Media state */
	media: Em.State.create({
	
		view:	MFT.MediaView,
		  
		exit: function() {
			MFT.MediaController.set('activeState', MFT.States.currentState.get('path'));
			MFT.MediaController.resetDirectTune();
			if(MFT.MediaController.directTuneTimer){
				clearInterval(MFT.MediaController.directTuneTimer);
			}
			this._super();
		},
		
		Am: Em.State.create({
			
			view:	MFT.AMFMInfoView,

			enter: function(){
				// Set last saved active view
				MFT.AmModel.band.set('value',MFT.AmModel.band.activeBand);
				// Set currentSelected TAB reference (for non class bindings)
				MFT.MediaController.set('currentSelectedTab' , 'AM');
				MFT.MediaController.set('currentMediaGroup', 'radio');
				this._super();
			},
			exit: function() {
				// save last band when exit from am
				MFT.AmModel.band.set('activeBand',MFT.AmModel.band.value);
				// Disable AM
				MFT.AmModel.band.set('value',-1);
				this._super();
			},
			
			options: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.MediaOptionsView);
					this._super();
			}
			}),
		}),
		Fm: Em.State.create({
			
			enter: function(){
				// Set last saved active view
				MFT.FmModel.band.set('value',MFT.FmModel.band.activeBand);
				// Set currentSelected TAB reference (for non class bindings)
				MFT.MediaController.set('currentSelectedTab' , 'FM');
				MFT.MediaController.set('currentMediaGroup', 'radio');
				this._super();
			},
			
			exit: function() {
				// save last band when exit from fm
				MFT.FmModel.band.set('activeBand',MFT.FmModel.band.value);
				// Disable FM
				MFT.FmModel.band.set('value',-1);
				this._super();
			},
						
			options: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.MediaOptionsView);
					this._super();
			}
			}),
		}),
		Sirius: Em.State.create({
			
			view:	MFT.SiriusView,
			
			enter: function(){
				// Set last saved active view
				MFT.SiriusModel.band.set('value',MFT.SiriusModel.band.activeBand);
				// Set currentSelected TAB reference (for non class bindings)
				MFT.MediaController.set('currentSelectedTab' , 'SIRIUS');
				MFT.MediaController.set('currentMediaGroup', 'radio');
				this._super();
			},
			exit: function() {
			
				// save last band when exit from sirius
				MFT.SiriusModel.band.set('activeBand',MFT.SiriusModel.band.value);
				MFT.SiriusModel.band.set('value',-1);
				MFT.MediaController.currentBrowseData.set('currentPage', MFT.BrowseView.list.currentPage);
				this._super();
			},		
					
			options: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.MediaOptionsView);
					this._super();
			}
			}),
			
			browse: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.BrowseView);
					this._super();
				},
				browseAll: Em.State.create({
					enter: function(){
						MFT.MediaController.insertToParentView(MFT.BrowseAllView);
						this._super();
					}
				})
			})
		}),
		cd: Em.State.create({
			view: MFT.MediaPlayerView,
			
			enter: function() {
				MFT.MediaController.set('currentMediaGroup', 'CDs');
				this._super();
			},	
			exit: function(){
				MFT.MediaController.currentBrowseData.set('currentPage', MFT.BrowseView.list.currentPage);
				this._super();
			},
			moreinfo:Em.State.create({
				view: MFT.MoreInfoView
			}),
			
			options: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.MediaOptionsView);
					this._super();
			}
			}),
			
			browse: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.BrowseView);
					this._super();
				},
				browseAll: Em.State.create({
					enter: function(){
						MFT.MediaController.insertToParentView(MFT.BrowseAllView);
						this._super();
					},
					exit: function(){
						MFT.MediaController.currentBrowseData.set('currentPage', MFT.BrowseView.list.currentPage);
						this._super();
					},
				})
			})
		}),
		usb: Em.State.create({
			view: MFT.MediaPlayerView,
			
			enter: function() {
				MFT.MediaController.set('currentMediaGroup', 'CDs');
				this._super();
			},	
			
			options: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.MediaOptionsView);
					this._super();
				}
			}),
			
			moreinfo:Em.State.create({
				view: MFT.MoreInfoView
			}),
						
			browse: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.USBBrowseView);
					this._super();
				},
				browseAll: Em.State.create({
					enter: function(){
						MFT.MediaController.insertToParentView(MFT.BrowseAllView);
						this._super();
					}
				})
			})
		}),
		sd: Em.State.create({
			view: MFT.MediaPlayerView,
			
			enter: function() {
				MFT.MediaController.set('currentMediaGroup', 'CDs');
				this._super();
			},	
			
			moreinfo:Em.State.create({
				view: MFT.MoreInfoView
			}),
			
			browse: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.USBBrowseView);
					this._super();
				},
				browseAll: Em.State.create({
					enter: function(){
						MFT.MediaController.insertToParentView(MFT.BrowseAllView);
						this._super();
					}
				})
			})
		}),
		
		bt: Em.State.create({
	
			view:	MFT.BlueToothView,
			
			exit: function() {
				MFT.MediaController.set('currentMediaGroup', 'Bluetooth');
				this._super();
			},	
			
			options: Em.State.create({
				enter: function(){
					MFT.MediaController.insertToParentView(MFT.MediaOptionsView);
					this._super();
			}
			}),
		}),
		avin: Em.State.create({
			
			view:	MFT.AvInView,
			
			exit: function() {
				MFT.MediaController.set('currentMediaGroup', 'AVIN');
				this._super();
			},		
		})
	}),
	
	/** Navigation state */
	navigation: Em.State.create({
		
		view:	MFT.NavigationView,
		
		exit: function() {
			MFT.NavigationController.set('activeState', MFT.States.currentState.get('path'));
			this._super();
		},
		
		dest: Em.State.create({
			view:	MFT.NavigationDestinationView,
			
			myHome: Em.State.create({
				view:	MFT.NavigationDestinationMyHomeView,
			}),
			
			favorites: Em.State.create({
				view:	MFT.NavigationDestinationFavorites,
			}),
			
			previousDestinations: Em.State.create({
				view:	MFT.NavigationDestinationPreviousDestinationsView,
			}),
			
			POI: Em.State.create({
				view:	MFT.NavigationDestinationPOIView,
			}),
			
			emergency: Em.State.create({
				view:	MFT.NavigationDestinationEmergencyView,
			}),
						
			streetAddress: Em.State.create({
				view:	MFT.NavigationDestinationStreetAddress,
			}),
			
			intersection: Em.State.create({
				view:	MFT.NavigationDestinationIntersectionView,
			}),
			
			cityCenter: Em.State.create({
				view:	MFT.NavigationDestinationCityCenterView,
			}),
			
			map: Em.State.create({
				view:	MFT.NavigationDestinationMapView,
			}),
			
			previousStartingsPoint: Em.State.create({
				view:	MFT.NavigationDestinationPreviousStartingsPointView,
			}),
			
			freeWay: Em.State.create({
				view:	MFT.NavigationDestinationFreeWayView,
			}),
			
			latitudeLongitude: Em.State.create({
				view:	MFT.NavigationDestinationLatitudeLongitudeView,
			}),
			
			
		})
	}),
	
	/** Phone state */
	phone: Em.State.create({
		
		view:	MFT.PhoneView,
		
		exit: function() {
			MFT.PhoneController.set('activeState', MFT.States.currentState.get('path'));
			this._super();
		},
		phone: Em.State.create({
			view:	MFT.DialpadPhoneView
		}),
		quickdial: Em.State.create({
			view:	MFT.QuickdialPhoneView
		}),
		phonebook: Em.State.create({
			view:	MFT.PhonebookPhoneView
		}),
		history: Em.State.create({
			view:	MFT.HistoryPhoneView
		}),
		messaging: Em.State.create({
			view:	MFT.MessagingPhoneView
		}),
		phoneSettings: Em.State.create({
			view:	MFT.SettingsPhoneView,
			
			bluetoothDevices: Em.State.create({
				enter: function(){
					MFT.PhoneController.insertToParentView(MFT.PhoneSettingsBluetoothDevicesView);
					this._super();
				}
			}),
			
			phoneRinger: Em.State.create({
				enter: function(){
					MFT.PhoneController.insertToParentView(MFT.PhoneSettingsPhoneRingerView);
					this._super();
				}
			}),
			
			textMessage: Em.State.create({
				enter: function(){
					MFT.PhoneController.insertToParentView(MFT.PhoneSettingsTextMessageNotificationView);
					this._super();
				}
			}),
			
			internetDataConnection: Em.State.create({
				enter: function(){
					MFT.PhoneController.insertToParentView(MFT.PhoneSettingsInternetDataConnectionView);
					this._super();
				}
			}),
			
			managePhoneBook: Em.State.create({
				enter: function(){
					MFT.PhoneController.insertToParentView(MFT.PhoneSettingsManagePhoneBookView);
					this._super();
				}
			})
		})
	})
});