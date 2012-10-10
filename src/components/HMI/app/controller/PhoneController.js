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
	
	/** Dial call handeler */
	onEndCall: function() {
		this.model.endCall();
		
		// Show popup
		$('#phone_endCallMessage').fadeIn();
		
		// Hide popup after timeout
		setTimeout(function() {
			$('#phone_endCallMessage').fadeOut();
		},2000);
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
			return 'Paul&rsquo;s Phone'
		} else {
			if ( this.model.statusModeText ) {
				return this.model.statusModeText;
			}
			return 'Sarah 3:47&nbsp;&nbsp;&nbsp;' + this.model.get('phoneStatusNumber');
		}
	}.property('this.model.readyForCall', 'this.model.statusModeText', 'this.model.phoneStatusNumber'),
	
	onChildState: function(event){
		MFT.States.goToState(MFT.States.currentState.get('path')+'.'+event.goToState);
	}
});