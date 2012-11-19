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
		'infoTable'
	],

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

			contentBinding:		'FFW.UI.globalPropertiesDefault.helpPrompt.0.text'
			
			//this.globalProperties.set('timeoutPrompt',
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
				this.time = 0;
			}
		}),

		/** Voice Recognition button */
		VRButton: MFT.Button.create({
			elementId:	'VRButton',
			classNames:	'VRButton',
			actionUp:		function(){
				this._super();
				MFT.VRPopUp.receiveMessage();
			}
		}),
	})
});