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
		'buttonControls',
		'driverDistractionControl',
		'protocolVersion',
		'infoTable',
		'sendData'
	],

	/** Sending data from HMI for processing in ApplinkCore */
	sendData: MFT.Button.create({
		elementId:	'sendData',
		classNames:	'sendData btnNotPressed',
		action:		'SendData',
		target:		'FFW.AppLinkCoreClient',
		text:		'Send Data'
	}),

	infoTable: Em.ContainerView.extend({
		elementId:	'infoTable',
		
		classNames:	'infoTable',

		childViews: [
			'globalPropertiesLabel',
			'globalPropertiesData'
		],

		globalPropertiesLabel : MFT.Label.extend({

			elementId:			'applinkGPLabel',

			classNames:			'applinkGPLabel',

			content:			'applinkGPLabel'
		}),		

		globalPropertiesData : MFT.Label.extend({

			elementId:			'applinkGPData',

			classNames:			'applinkGPData',

			content:			'Some text for help prompt',
			
			propertiesData: function(){
				var i=0,
					str='HELP_PROMPT: ';
				for(i = 0; i < MFT.ApplinkModel.globalProperties.helpPrompt.length; i++){
					str += MFT.ApplinkModel.globalProperties.helpPrompt[i].text + ' ';
				}
				str += 'TIMEOUT_PROMPT: ';
				for(i = 0; i < MFT.ApplinkModel.globalProperties.timeoutPrompt.length; i++){
					str += MFT.ApplinkModel.globalProperties.timeoutPrompt[i].text + ' ';
				}
				this.set('content', str);
			}
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
				MFT.ApplinkController.selectdDriverDistraction(this.checked);
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
				MFT.ApplinkController.selectProtocolVersion(this.checked);
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
			'ZiroBtn',
			'VRButton'
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
			UpBtn: MFT.Button.create({
				elementId:	'TUNEUP',
				classNames:	'UpBtn',
				time:		0,
				actionDown:		function(){
					this._super();
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
					var self = this;
					this.time = 0;
					setTimeout(function(){ self.time ++; }, 1000);
				},
				actionUp:		function(){
					this._super();
					if(this.time > 0){
						FFW.Buttons.buttonPressed( this.elementId, "LONG");
					}else{
						FFW.Buttons.buttonPressed( this.elementId, "SHORT");
					}
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
					this.time = 0;
				}
			}),
			
			/** Down button */
			DownBtn: MFT.Button.create({
				elementId:	'TUNEDOWN',
				classNames:	'DownBtn',
				time:		0,
				actionDown:		function(){
					this._super();
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
					var self = this;
					this.time = 0;
					setTimeout(function(){ self.time ++; }, 1000);
				},
				actionUp:		function(){
					this._super();
					if(this.time > 0){
						FFW.Buttons.buttonPressed( this.elementId, "LONG");
					}else{
						FFW.Buttons.buttonPressed( this.elementId, "SHORT");
					}
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
					this.time = 0;
				}
			}),
			
			/** Left button */
			LeftBtn: MFT.Button.create({
				elementId:	'SEEKLEFT',
				classNames:	'LeftBtn',
				time:		0,
				actionDown:		function(){
					this._super();
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
					var self = this;
					this.time = 0;
					setTimeout(function(){ self.time ++; }, 1000);
				},
				actionUp:		function(){
					this._super();
					if(this.time > 0){
						FFW.Buttons.buttonPressed( this.elementId, "LONG");
					}else{
						FFW.Buttons.buttonPressed( this.elementId, "SHORT");
					}
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
					this.time = 0;
				}
			}),
			
			/** Right button */
			RightBtn: MFT.Button.create({
				elementId:	'SEEKRIGHT',
				classNames:	'RightBtn',
				time:		0,
				actionDown:		function(){
					this._super();
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
					var self = this;
					this.time = 0;
					setTimeout(function(){ self.time ++; }, 1000);
				},
				actionUp:		function(){
					this._super();
					if(this.time > 0){
						FFW.Buttons.buttonPressed( this.elementId, "LONG");
					}else{
						FFW.Buttons.buttonPressed( this.elementId, "SHORT");
					}
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
					this.time = 0;
				}
			}),
			
			/** Ok button */
			OkBtn: MFT.Button.create({
				elementId:	'OK',
				classNames:	'OkBtn',
				time:		0,
				actionDown:		function(){
					this._super();
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
					var self = this;
					this.time = 0;
					setTimeout(function(){ self.time ++; }, 1000);
				},
				actionUp:		function(){
					this._super();
					if(this.time > 0){
						FFW.Buttons.buttonPressed( this.elementId, "LONG");
					}else{
						FFW.Buttons.buttonPressed( this.elementId, "SHORT");
					}
					FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
					this.time = 0;
				}
			})
		}),
		
		/** One button */
		OneBtn: MFT.Button.create({
			elementId:	'PRESET_1',
			classNames:	'OneBtn btnNotPressed',
			text:		'1',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** Two button */
		TwoBtn: MFT.Button.create({
			elementId:	'PRESET_2',
			classNames:	'TwoBtn btnNotPressed',
			text:		'2',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** Three button */
		ThreeBtn: MFT.Button.create({
			elementId:	'PRESET_3',
			classNames:	'ThreeBtn btnNotPressed',
			text:		'3',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** Four button */
		FourBtn: MFT.Button.create({
			elementId:	'PRESET_4',
			classNames:	'FourBtn btnNotPressed',
			text:		'4',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** Five button */
		FiveBtn: MFT.Button.create({
			elementId:	'PRESET_5',
			classNames:	'FiveBtn btnNotPressed',
			text:		'5',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** One button */
		SixBtn: MFT.Button.create({
			elementId:	'PRESET_6',
			classNames:	'SixBtn btnNotPressed',
			text:		'6',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** One button */
		SevenBtn: MFT.Button.create({
			elementId:	'PRESET_7',
			classNames:	'SevenBtn btnNotPressed',
			text:		'7',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** One button */
		EightBtn: MFT.Button.create({
			elementId:	'PRESET_8',
			classNames:	'EightBtn btnNotPressed',
			text:		'8',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** One button */
		NineBtn: MFT.Button.create({
			elementId:	'PRESET_9',
			classNames:	'NineBtn btnNotPressed',
			text:		'9',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),
		
		/** One button */
		ZiroBtn: MFT.Button.create({
			elementId:	'PRESET_0',
			classNames:	'ZiroBtn btnNotPressed',
			text:		'0',
			time:		0,
			actionDown:		function(){
				this._super();
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONDOWN");
				var self = this;
				this.time = 0;
				setTimeout(function(){ self.time ++; }, 1000);
			},
			actionUp:		function(){
				this._super();
				if(this.time > 0){
					FFW.Buttons.buttonPressed( this.elementId, "LONG");
				}else{
					FFW.Buttons.buttonPressed( this.elementId, "SHORT");
				}
				FFW.Buttons.buttonEvent( this.elementId, "BUTTONUP");
				this.time = 0;
			}
		}),

		/** Voice Recognition button */
		VRButton: MFT.Button.create({
			elementId:	'VRButton',
			classNames:	'VRButton',
			actionUp:		function(){
				this._super();
				MFT.VRPopUp.activateVRPopUp();
			}
		}),
	})
});