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
		'buttonControls'
	],

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
			UpBtn: MFT.Button.create({
				elementId:	'TUNEUP',
				classNames:	'UpBtn',
				click:		function(){
					FFW.Buttons.buttonPressed(this.elementId, "SHORT");
				}
			}),
			
			/** Down button */
			DownBtn: MFT.Button.create({
				elementId:	'TUNEDOWN',
				classNames:	'DownBtn',
				click:		function(){
					FFW.Buttons.buttonPressed(this.elementId, "SHORT");
				}
			}),
			
			/** Left button */
			LeftBtn: MFT.Button.create({
				elementId:	'SEEKLEFT',
				classNames:	'LeftBtn',
				click:		function(){
					FFW.Buttons.buttonPressed(this.elementId, "SHORT");
				}
			}),
			
			/** Right button */
			RightBtn: MFT.Button.create({
				elementId:	'SEEKRIGHT',
				classNames:	'RightBtn',
				click:		function(){
					FFW.Buttons.buttonPressed(this.elementId, "SHORT");
				}
			}),
			
			/** Ok button */
			OkBtn: MFT.Button.create({
				elementId:	'OK',
				classNames:	'OkBtn',
				click:		function(){
					FFW.Buttons.buttonPressed(this.elementId, "SHORT");
				}
			})
		}),
		
		/** One button */
		OneBtn: MFT.Button.create({
			elementId:	'PRESET_1',
			classNames:	'OneBtn btnNotPressed',
			text:		'1',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** Two button */
		TwoBtn: MFT.Button.create({
			elementId:	'PRESET_2',
			classNames:	'TwoBtn btnNotPressed',
			text:		'2',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** Three button */
		ThreeBtn: MFT.Button.create({
			elementId:	'PRESET_3',
			classNames:	'ThreeBtn btnNotPressed',
			text:		'3',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** Four button */
		FourBtn: MFT.Button.create({
			elementId:	'PRESET_4',
			classNames:	'FourBtn btnNotPressed',
			text:		'4',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** Five button */
		FiveBtn: MFT.Button.create({
			elementId:	'PRESET_5',
			classNames:	'FiveBtn btnNotPressed',
			text:		'5',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** One button */
		SixBtn: MFT.Button.create({
			elementId:	'PRESET_6',
			classNames:	'SixBtn btnNotPressed',
			text:		'6',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** One button */
		SevenBtn: MFT.Button.create({
			elementId:	'PRESET_7',
			classNames:	'SevenBtn btnNotPressed',
			text:		'7',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** One button */
		EightBtn: MFT.Button.create({
			elementId:	'PRESET_8',
			classNames:	'EightBtn btnNotPressed',
			text:		'8',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** One button */
		NineBtn: MFT.Button.create({
			elementId:	'PRESET_9',
			classNames:	'NineBtn btnNotPressed',
			text:		'9',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		}),
		
		/** One button */
		ZiroBtn: MFT.Button.create({
			elementId:	'PRESET_0',
			classNames:	'ZiroBtn btnNotPressed',
			text:		'0',
			click:		function(){
				FFW.Buttons.buttonPressed( this.elementId, "SHORT");
			}
		})
	})
});