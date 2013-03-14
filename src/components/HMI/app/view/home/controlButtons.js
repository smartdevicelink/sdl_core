/**
 * @name MFT.controlButtons
 * 
 * @desc Climate module visual representation
 * 
 * @category	View
 * @filesource	app/view/home/controlButtons.js
 * @version		2.0
 *
 * @author		Andriy Melnyk
 */

MFT.ControlButtons = Em.ContainerView.create({

	elementId:		'app_controlButtons',

	childViews: [
		'VRButton',
		'buttonControls',
		'driverDistractionControl',
		'protocolVersion',
		'infoTable',
		'sendData',
		'sendDataCheckBox',
		'sendDataLabel',
		'vehicleInfo',
		'tbtClientState',
		'UILanguages',
		'TTSVRLanguages',
		'UILanguagesLabel',
		'TTSVRLanguagesLabel',
		'appUILanguagesLabel',
		'appTTSVRLanguagesLabel',
		'appUILang',
		'appTTSVRLang'
	],

	/*
     * Label with name of UILanguages select
     */
     appUILang : MFT.Label.extend({

		elementId:			'appUILang',

		classNames:			'appUILang',

		contentBinding:		'MFT.SDLAppController.model.UILanguage'
	}),

	/*
     * Label with name of TTSVRLanguages select
     */ 
	appTTSVRLang : MFT.Label.extend({

		elementId:			'appTTSVRLang',

		classNames:			'appTTSVRLang',

		contentBinding:		'MFT.SDLAppController.model.TTSVRLanguage'
	}),	

	/*
     * Label with name of UILanguages select
     */ 
	appUILanguagesLabel : MFT.Label.extend({

		elementId:			'appUILanguagesLabel',

		classNames:			'appUILanguagesLabel',

		content:			'application UI Languages'
	}),

	/*
     * Label with name of TTSVRLanguages select
     */ 
	appTTSVRLanguagesLabel : MFT.Label.extend({

		elementId:			'appTTSVRLanguagesLabel',

		classNames:			'appTTSVRLanguagesLabel',

		content:			'application (TTS + VR) Languages'
	}),	

	/*
     * Label with name of UILanguages select
     */ 
	UILanguagesLabel : MFT.Label.extend({

		elementId:			'UILanguagesLabel',

		classNames:			'UILanguagesLabel',

		content:			'UI Languages'
	}),

	/*
     * Label with name of TTSVRLanguages select
     */ 
	TTSVRLanguagesLabel : MFT.Label.extend({

		elementId:			'TTSVRLanguagesLabel',

		classNames:			'TTSVRLanguagesLabel',

		content:			'TTS + VR Languages'
	}),	

	/*
     * HMI element Select with list of supported UI component languages
     */ 
    UILanguages : Em.Select.extend({

        elementId:          'UILanguages',

        classNames:         'languageSelect',

        contentBinding:     'MFT.SDLModel.sdlLanguagesList',

        valeuBinding:		'MFT.SDLModel.hmiUILanguage',

        click: function(){

    		MFT.SDLController.onLanguageChangeUI( this.selection );

        }
    }),

	/*
     * HMI element Select with list of supported TTS and VR component languages
     */ 
    TTSVRLanguages : Em.Select.extend({

        elementId:          'TTSVRLanguages',

        classNames:         'languageSelect',

        contentBinding:     'MFT.SDLModel.sdlLanguagesList',

        valeuBinding:		'MFT.SDLModel.hmiTTSVRLanguage',

        click: function(){

            MFT.SDLController.onLanguageChangeTTSVR( this.selection );

        }
    }),

	/**
	 * Sending data from HMI for processing in SDLCore
	 */
	sendData: MFT.Button.create({
		elementId:	'sendData',
		classNames:	'sendData btnNotPressed',
		action:		function(){
			FFW.BasicCommunication.SendData( null );
		},
		text:		'Send Data'
	}),

	/**
	 * Select for extended param of SendData
	 */
	sendDataCheckBox: Em.Checkbox.extend({

		elementId:			'sendDataCheckBox',

		classNames:			'sendDataCheckBox',

		onCheckBoxSelected:	function(){
			MFT.SDLController.selectSendData(this.checked);
		}.observes('this.checked')

	}),

	/*
     * Label for sendDataCheckBox 
     */
	sendDataLabel: MFT.Label.extend({

		elementId:			'sendDataLabel',

		classNames:			'sendDataLabel',

		content:			'Use URL'
	}),	

	/**
	 * VehicleInfo button
	 */
	vehicleInfo: MFT.Button.create({
		elementId:	'vehicleInfoButton',
		classNames:	'vehicleInfoButton btn',
		text:		'Vehicle Info', 
		action:		function(){
			//this._super();
			MFT.VehicleInfo.toggleActivity();
		}
	}),

	/**
	 * TBT Client State button
	 */
	tbtClientState: MFT.Button.create({
		elementId:	'tbtClientStateButton',
		classNames:	'tbtClientStateButton btn',
		text:		'TBT Client State', 
		action:		function(){
			//this._super();
			MFT.TBTClientStateView.toggleActivity();
		}
	}),

	/**
	 * Voice Recognition button
	 */
	VRButton: MFT.Button.create({
		elementId:	'VRButton',
		classNames:	'VRButton',
		action:		function(){
			//this._super();
			MFT.VRPopUp.activateVRPopUp();
		}
	}),

	infoTable: Em.ContainerView.extend({
		elementId:	'infoTable',
		
		classNames:	'infoTable',

		childViews: [
			'globalPropertiesLabel',
			'gpHelpData',
			'gpTimeoutData'
		],

		globalPropertiesLabel : MFT.Label.extend({

			elementId:			'sdlGPLabel',

			classNames:			'sdlGPLabel',

			content:			'HELP_PROMPT: TIMEOUT_PROMPT:'
		}),		

		gpHelpData : MFT.Label.extend({

			elementId:			'sdlGPHData',

			classNames:			'sdlGPData',

			contentBinding:		'this.propertiesData',
			
			propertiesData: function(){
				var str='';
				if( MFT.SDLModel.globalProperties.helpPrompt ){
					var i=0;
					
					for(i = 0; i < MFT.SDLModel.globalProperties.helpPrompt.length; i++){
						str += MFT.SDLModel.globalProperties.helpPrompt[i].text + ' ';
					}}
				return str;
			}.property( 'MFT.SDLModel.globalProperties.helpPrompt.@each.text' )
		}),

		gpTimeoutData : MFT.Label.extend({

			elementId:			'sdlGPTData',

			classNames:			'sdlGPData',

			contentBinding:		'this.propertiesData',
			
			propertiesData: function(){
				var str='';
				if( MFT.SDLModel.globalProperties.timeoutPrompt ){
					var i=0;
					for(i = 0; i < MFT.SDLModel.globalProperties.timeoutPrompt.length; i++){
						str += MFT.SDLModel.globalProperties.timeoutPrompt[i].text + ' ';
					}
				}
				
				return str;
			}.property( 'MFT.SDLModel.globalProperties.timeoutPrompt.@each.text' )
		})
	}),

	driverDistractionControl: Em.ContainerView.extend({
		elementId:	'driverDistractionControl',
		
		classNames:	'driverDistractionControl',

		childViews: [
			'driverDistractionLabel',
			'driverDistractionCheckBox'
		],

		driverDistractionLabel : MFT.Label.extend({

			elementId:			'driverDistractionControlLabel',

			classNames:			'driverDistractionControlLabel',

			content:			'Driver Distraction'
		}),		

		driverDistractionCheckBox : Em.Checkbox.extend({

			elementId:			'driverDistractionControlCheckBox',

			classNames:			'driverDistractionControlCheckBox',

			onCheckBoxSelected:	function(){
				MFT.SDLController.selectDriverDistraction(this.checked);
			}.observes('this.checked')

		})
	}),


	protocolVersion: Em.ContainerView.extend({
		elementId:	'protocolVersion',
		
		classNames:	'protocolVersion',

		childViews: [
			'protocolVersionLabel',
			'protocolVersionCheckBox'
		],

		protocolVersionLabel : MFT.Label.extend({

			elementId:			'protocolVersionLabel',

			classNames:			'protocolVersionLabel',

			content:			'Protocol version V2'
		}),		

		protocolVersionCheckBox : Em.Checkbox.extend({

			elementId:			'protocolVersionCheckBox',

			classNames:			'protocolVersionCheckBox',

			onCheckBoxSelected:	function(){
				MFT.SDLController.selectProtocolVersion(this.checked);
			}.observes('this.checked')

		})
	}),

	buttonControls: Em.ContainerView.extend({
		elementId:	'buttonControls',
		
		classNames:	'buttonControls',

		childViews: [
			'ContainerControlls',
			'OneBtn',
			'TwoBtn',
			'ThreeBtn',
			'FourBtn',
			'FiveBtn',
			'SixBtn',
			'SevenBtn',
			'EightBtn',
			'NineBtn',
			'ZiroBtn'
		],
		
		ContainerControlls: Em.ContainerView.extend({
			elementId:	'ContainerControlls',
			
			classNames:	'ContainerControlls',

			childViews: [
				'UpBtn',
				'DownBtn',
				'LeftBtn',
				'RightBtn',
				'OkBtn'
			],

			/** Up button */
			UpBtn: MFT.Button.create(MFT.PresetEvents, {
				elementId:	'TUNEUP',
				classNames:	'UpBtn',
				time:		0,
				presetName: 'TUNEUP'
			}),
			
			/** Down button */
			DownBtn: MFT.Button.create(MFT.PresetEvents, {
				elementId:	'TUNEDOWN',
				classNames:	'DownBtn',
				time:		0,
				presetName: 'TUNEDOWN'
			}),
			
			/** Left button */
			LeftBtn: MFT.Button.create(MFT.PresetEvents, {
				elementId:	'SEEKLEFT',
				classNames:	'LeftBtn',
				time:		0,
				presetName: 'SEEKLEFT'
			}),
			
			/** Right button */
			RightBtn: MFT.Button.create(MFT.PresetEvents, {
				elementId:	'SEEKRIGHT',
				classNames:	'RightBtn',
				time:		0,
				presetName: 'SEEKRIGHT'
			}),
			
			/** Ok button */
			OkBtn: MFT.Button.create(MFT.PresetEvents, {
				elementId:	'OK',
				classNames:	'OkBtn',
				time:		0,
				presetName: 'OK'
			})
		}),
		
		/** One button */
		OneBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_1',
			classNames:	'OneBtn btnNotPressed',
			text:		'1',
			time:		0,
			presetName: 'PRESET_1'
		}),
		
		/** Two button */
		TwoBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_2',
			classNames:	'TwoBtn btnNotPressed',
			text:		'2',
			time:		0,
			presetName: 'PRESET_2'
		}),
		
		/** Three button */
		ThreeBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_3',
			classNames:	'ThreeBtn btnNotPressed',
			text:		'3',
			time:		0,
			presetName: 'PRESET_3'
		}),
		
		/** Four button */
		FourBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_4',
			classNames:	'FourBtn btnNotPressed',
			text:		'4',
			time:		0,
			presetName: 'PRESET_4'
		}),
		
		/** Five button */
		FiveBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_5',
			classNames:	'FiveBtn btnNotPressed',
			text:		'5',
			time:		0,
			presetName: 'PRESET_5'
		}),
		
		/** One button */
		SixBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_6',
			classNames:	'SixBtn btnNotPressed',
			text:		'6',
			time:		0,
			presetName: 'PRESET_6'
		}),
		
		/** One button */
		SevenBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_7',
			classNames:	'SevenBtn btnNotPressed',
			text:		'7',
			time:		0,
			presetName: 'PRESET_7'
		}),
		
		/** One button */
		EightBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_8',
			classNames:	'EightBtn btnNotPressed',
			text:		'8',
			time:		0,
			presetName: 'PRESET_8'
		}),
		
		/** One button */
		NineBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_9',
			classNames:	'NineBtn btnNotPressed',
			text:		'9',
			time:		0,
			presetName: 'PRESET_9'
		}),
		
		/** One button */
		ZiroBtn: MFT.Button.create(MFT.PresetEvents, {
			elementId:	'PRESET_0',
			classNames:	'ZiroBtn btnNotPressed',
			text:		'0',
			time:		0,
			presetName: 'PRESET_0'
		})
	})
});