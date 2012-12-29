/**
 * @name MFT.AlertPopUp
 * 
 * @desc AlertPopUp module visual representation
 * 
 * @category	View
 * @filesource	app/view/home/AlertPopUp.js
 * @version		2.0
 *
 * @author		Andriy Melnik
 */

MFT.AlertPopUp = Em.ContainerView.create({

	elementId:			'AlertPopUp',

	classNames:			'AlertPopUp',

	classNameBindings:		['activate:AlertActive'],

	childViews: [
		'applicationName',
		'message1',
		'message2',
		'message3'
	],

	content1:			'Title',

	content2:			'Text',

	activate: 			false,


	applicationName :	MFT.Label.extend({

		elementId:			'applicationName',

		classNames:			'applicationName',

		contentBinding:		'parentView.appName'
	}),

	message1 : MFT.Label.extend({

		elementId:			'message1',

		classNames:			'message1',

		contentBinding:		'parentView.content1'
	}),

	message2 : MFT.Label.extend({

		elementId:			'message2',

		classNames:			'message2',

		contentBinding:		'parentView.content2'
	}),

	message3 : MFT.Label.extend({

		elementId:			'message3',

		classNames:			'message3',

		contentBinding:		'parentView.content3'
	}),

	/**
	 *
	 * @desc Function creates Soft Buttons on AlertPoUp
	 * 
	 * @param params: SoftButton
	*/
	addSoftButtons: function( params ){
		
		this.get('childViews').removeObjects(
            this.get('childViews').filterProperty( 'buttonType', 'softbutton' )
        );

		if( params ){

	        var softButtonsClass;
	        switch(params.length){
	        	case 1 : softButtonsClass = 'one';
	        		break;
	        	case 2 : softButtonsClass = 'two';
	        		break;
	        	case 3 : softButtonsClass = 'three';
	        		break;
	        	case 4 : softButtonsClass = 'four';
	        		break;
	        }

			for(var i=0; i<params.length; i++){
				this.get('childViews').pushObject(
					MFT.Button.create({
						actionDown:		function(){
							this._super();
							FFW.Buttons.buttonEventCustom( "CUSTOM_BUTTON", "BUTTONDOWN", this.softButtonID);
							var self = this;
							this.time = 0;
							setTimeout(function(){ self.time ++; }, 1000);
						},
						actionUp:		function(){
							this._super();
							FFW.Buttons.buttonEventCustom( "CUSTOM_BUTTON", "BUTTONUP", this.softButtonID);
							if(this.time > 0){
								FFW.Buttons.buttonPressedCustom( "CUSTOM_BUTTON", "LONG", this.softButtonID);
							}else{
								FFW.Buttons.buttonPressedCustom( "CUSTOM_BUTTON", "SHORT", this.softButtonID);
							}
							this.time = 0;
						},
						softButtonID:           params[i].softButtonID,
		                icon:                   params[i].image,
		                buttonType:				'softbutton',
		                text:                   params[i].text,
		                classNames:             'list-item softButton ' + softButtonsClass,
		                elementId:				'softButton' + i,
		                templateName:           params[i].image ? 'rightIcon' : 'text'
		            })
				);
			}
		}
	},

	AlertActive: function( appId, msg1, msg2, msg3, tts, duration, playTone, softButtons){
		var self = this;

		// play audio alert
		if ( playTone ) {
			MFT.Audio.play('audio/alert.wav');
		}

		this.addSoftButtons(softButtons);
		MFT.ApplinkModel.onPrompt(tts.ttsChunks);

		this.set('appName',	MFT.ApplinkController.getApplicationModel(appId).appInfo.appName);

		this.set('content1',	msg1);
		this.set('content2',	msg2);
		this.set('content3',	msg3);
		this.set('activate',	true);
		
		setTimeout(function(){self.set('activate', false);}, duration);
	}
});