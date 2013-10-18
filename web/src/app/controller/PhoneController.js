/**
 * @name MFT.PhoneController
 * 
 * @desc Phone module logic
 * 
 * @category	Controller
 * @filesource	app/controller/PhoneController.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.PhoneController = Em.Object.create({
	
	/** Model binding */
	modelBinding: "MFT.PhoneModel",
	
	/** Initial substate */
	activeState: 'phone.dialpad',
	
	subState: function(view) {		
		/* Open help video */
		if ( MFT.helpMode && view.videoPath ) {
			MFT.VideoPlayerController.start(view.videoPath, 'phone.' + view.stateName);
		} else {
			MFT.States.goToState( 'phone.' + view.stateName);
		}
	},
	
	// flag for hiding left menu
	hideMenu: false,
	
	/**
	 * End Call popup visibility state
	 *
	 * @type {Boolean}
	 */
	endCallPopupHidden: true,
	
	/** DIALPAD SECTION */
	
	/** Dialpad key press handeler */
	onKeyPress: function(event) {
		this.model.setDialpadNumber(event.keyData);
	},
	
	/** Dialpad delete key press handeler */
	onDelete: function() {
		this.model.deleteDialpadNumber();
	},
	
	/** Dial call handeler */
	onDialCall: function() {
		this.model.dialCall();
	},
	
	/**
	 * End call button handeler
	 * Show End Call popup
	 */
	onEndCall: function() {
		this.model.endCall();
		
		var self = this;
		
		//show message
		$('#phone_endCallMessage').css('display','block');		
		$('#phone_endCallMessage').animate({
			opacity: 1
		},500);
		this.set('endCallPopupHidden',false);
		
		var self = this;
		this.EndCallTimerID = setTimeout(function() {
		    self.hideEndCallMessage();
		},2000);	
	},
	
	/** Hide EndCall Message*/
	hideEndCallMessage: function(){
		// Clean EndCallTimer, if it has already been hidden.
		clearTimeout(this.EndCallTimerID);
		$('#phone_endCallMessage').animate({ opacity: 0 },500);
		var self = this;
		setTimeout(function() {
			$('#phone_endCallMessage').css('display','none');
			self.set('endCallPopupHidden',true);
		},500);
	},
	
	/** Call Privacy toggle handler */
	onPrivacy: function() {
		
		// Help mode video
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('phone_Privacy');
			
			return;
		}
		
		this.model.togglePrivacy();
	},
	
	/** Call Hold toggle handler */
	onHold: function() {
		this.model.toggleHold();
	},
	
	/** Call Mute toggle handler */
	onMute: function() {
		this.model.toggleMute();
	},
	
	/** DND toggle handler */
	onDND: function() {
        // Help mode video
        if ( MFT.helpMode ) {
            MFT.VideoPlayerController.start('phone_Do_not_disturb');
            return;
        }
		this.model.toggleDND();
	},
	
	/** Join toggle handler */
	onJoin: function() {
		// Help mode video
		if ( MFT.helpMode ) {
			MFT.VideoPlayerController.start('phone_Join_call');			
		}
	},
	
	/** Status label text */
	phoneStatus: function() {
		if( this.model.readyForCall ) {
			return MFT.locale.label.view_wigetPhone_phone;
		} else {
			if ( this.model.statusModeText ) {
				return this.model.statusModeText;
			}
			return 'Sarah 3:47&nbsp;&nbsp;&nbsp;' + this.model.get('phoneStatusNumber');
		}
	}.property('this.model.readyForCall', 'this.model.statusModeText', 'this.model.phoneStatusNumber'),
	
	onChildState: function(event){
		//this.set('hideMenu', true);
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	},
	
	onBackState: function(event){
		MFT.States.back();
		
		// show left menu
		this.set('hideMenu', false);
	},
	
	
	// used for settings DND button
	// because ButtonGroup can't work with normal Ranged Values
	// and should be redesigned in future release
    dndValue: function(key, value) {
        
        if (value != undefined) {
            this.model.dnd.set('value',this.invertNumber(value));
        } else {
            return this.invertNumber(this.model.dnd.value);
        }        
    }.property('this.model.dnd.value'),
    
    // helper for invert Number 1 to 0 ( 0 to 1)
    // should be removed with property above
    invertNumber: function( num ) {
        return Number(!num);
    },

    /* Android Phone book*/

    onPhoneBookFilterChange: function(obj){
        FLAGS.ANDROID_PHONE_BOOK ? FFW.PhoneBookRPC.getContacts(obj.range) : FFW.PhoneBookRPC.getContacts_em(obj.range);
    },

    /*Generate Contacts */
    generateContactsList: function(data){
        var items= [];

        if(MFT.PhonebookPhoneView.content.list.items.length > 0){
            MFT.PhonebookPhoneView.content.list.items.clear();
        }

        if(MFT.PhonebookPhoneView.content.list.scrollbar.currentPage != 0){
            MFT.PhonebookPhoneView.content.list.scrollbar.set('currentPage',0);
        }

        if(data.length >0){

            for (var i = 0; i<=data.length-1; i++){
                items.push(
                    {
                        type:MFT.Button,
                        params:{
                            text:data[i].firstName,
                            number: data[i].phoneNumber,
                            action: 'dial',
                            target: 'FFW.PhoneBookRPC'
                        }
                    }
                )
            }
        }else{
            items.push(
                {
                    type:MFT.Label,
                    params:{
                        content:'NO DATA!!!'
                    }
                }
            )


        }

        MFT.PhonebookPhoneView.content.list.items.pushObjects(items);
    }
});