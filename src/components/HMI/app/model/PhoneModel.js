/**
 * @name MFT.PhoneModel
 * 
 * @desc Phone model
 * 
 * @category	Model
 * @filesource	app/model/PhoneModel.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.PhoneModel = Em.Object.create({
	
	/** Dial number */
	dialpadNumber:		'',
	
	/** Phone DND mode */
	dnd:				MFT.RangedValue.create({ range: 1 }),
	
	/** Dialpad controls */
	dialpadPrivacy:		MFT.RangedValue.create({ range: 1 }),
	dialpadHold:		MFT.RangedValue.create({ range: 1 }),
	dialpadMute:		MFT.RangedValue.create({ range: 1 }),
	
	readyForCall:		true,
	onCall:				false,
	
	/** Text for phone status bar */
	statusModeText:		null,
	
	/** Start call */
	dialCall: function() {
		if( this.dialpadNumber.length ) {
			this.set('onCall', true);
			this.set('readyForCall', false);
		}
	},
	
	/** End call */
	endCall: function() {
		this.set('onCall', false);
		this.set('readyForCall', true);
		this.set('dialpadNumber', '');
		
		this.dialpadPrivacy.off();
		this.dialpadHold.off();
		this.dialpadMute.off();
		this.set('statusModeText', null);

		if( MFT.SDLModel.phoneCall ){
			MFT.SDLModel.set('phoneCall', false);
			MFT.SDLController.getApplicationModel( MFT.SDLAppController.model.appId ).turnOnSDL();
		}
	},
	
	/** Add symbol to dial pad number */
	setDialpadNumber: function(key) {
		if ( this.dialpadNumber.length < 14 ) {
			key = this.dialpadNumber + key;
			this.set('dialpadNumber', key);
		}
	},
	
	/** Delete last number symbol */
	deleteDialpadNumber: function() {
		if( this.dialpadNumber.length ) {
			this.set('dialpadNumber', this.dialpadNumber.slice(0,-1) );
		}
	},
	
	/** Toggle call privacy mode */
	togglePrivacy: function() {
		this.dialpadPrivacy.increase();
		
		if ( this.dialpadPrivacy.value ) {
			this.set('statusModeText', MFT.locale.label.view_statusPhone_privacyMode);
		} else {
			if ( this.dialpadMute.value ) {
				this.set('statusModeText', MFT.locale.label.view_statusPhone_microphoneMute);
			} else {
				this.set('statusModeText', null);
			}
		}
	},
	
	/** Toggle call hold mode */
	toggleHold: function() {
		this.dialpadHold.increase();
	},
	
	/** Toggle call mute mode */
	toggleMute: function() {
		this.dialpadMute.increase();
		
		if ( this.dialpadMute.value ) {
			this.set('statusModeText', MFT.locale.label.view_statusPhone_microphoneMute);
		} else {
			if ( this.dialpadPrivacy.value ) {
				this.set('statusModeText', MFT.locale.label.view_statusPhone_privacyMode);
			} else {
				this.set('statusModeText', null);
			}
		}
	},
	
	toggleDND: function() {
		this.dnd.increase();
	},
	
	/** Phone status number */
	phoneStatusNumber: function() {
		if ( this.dialpadNumber.length < 6 ) {
			return this.dialpadNumber;
		} else {
			return this.dialpadNumber.substr(0,7) + '…'
		}
	}.property('this.dialpadNumber')
})