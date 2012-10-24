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
				elementId:	'app_controlButtonsUpBtn',
				classNames:	'UpBtn',
				click:		function(){
					FFW.Buttons.buttonPressed("up", "Click");
				}
			}),
			
			/** Down button */
			DownBtn: MFT.Button.create({
				elementId:	'app_controlButtonsDownBtn',
				classNames:	'DownBtn',
				click:		function(){
					FFW.Buttons.buttonPressed("down", "Click");
				}
			}),
			
			/** Left button */
			LeftBtn: MFT.Button.create({
				elementId:	'app_controlButtonsLeftBtn',
				classNames:	'LeftBtn',
				click:		function(){
					FFW.Buttons.buttonPressed("left", "Click");
				}
			}),
			
			/** Right button */
			RightBtn: MFT.Button.create({
				elementId:	'app_controlButtonsRightBtn',
				classNames:	'RightBtn',
				click:		function(){
					FFW.Buttons.buttonPressed("right", "Click");
				}
			}),
			
			/** Ok button */
			OkBtn: MFT.Button.create({
				elementId:	'app_controlButtonsOkBtn',
				classNames:	'OkBtn',
				click:		function(){
					FFW.Buttons.buttonPressed("ok", "Click");
				}
			})
		}),
		
		/** One button */
		OneBtn: MFT.Button.create({
			elementId:	'app_controlButtonsOneBtn',
			classNames:	'OneBtn btnNotPressed',
			text:		'1',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** Two button */
		TwoBtn: MFT.Button.create({
			elementId:	'app_controlButtonsTwoBtn',
			classNames:	'TwoBtn btnNotPressed',
			text:		'2',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** Three button */
		ThreeBtn: MFT.Button.create({
			elementId:	'app_controlButtonsThreeBtn',
			classNames:	'ThreeBtn btnNotPressed',
			text:		'3',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** Four button */
		FourBtn: MFT.Button.create({
			elementId:	'app_controlButtonsFourBtn',
			classNames:	'FourBtn btnNotPressed',
			text:		'4',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** Five button */
		FiveBtn: MFT.Button.create({
			elementId:	'app_controlButtonsFiveBtn',
			classNames:	'FiveBtn btnNotPressed',
			text:		'5',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** One button */
		SixBtn: MFT.Button.create({
			elementId:	'app_controlButtonsSixBtn',
			classNames:	'SixBtn btnNotPressed',
			text:		'6',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** One button */
		SevenBtn: MFT.Button.create({
			elementId:	'app_controlButtonsSevenBtn',
			classNames:	'SevenBtn btnNotPressed',
			text:		'7',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** One button */
		EightBtn: MFT.Button.create({
			elementId:	'app_controlButtonsEightBtn',
			classNames:	'EightBtn btnNotPressed',
			text:		'8',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** One button */
		NineBtn: MFT.Button.create({
			elementId:	'app_controlButtonsNineBtn',
			classNames:	'NineBtn btnNotPressed',
			text:		'9',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		}),
		
		/** One button */
		ZiroBtn: MFT.Button.create({
			elementId:	'app_controlButtonsZiroBtn',
			classNames:	'ZiroBtn btnNotPressed',
			text:		'0',
			click:		function(){
				FFW.Buttons.buttonPressed( this.text, "Click");
			}
		})
	})
});