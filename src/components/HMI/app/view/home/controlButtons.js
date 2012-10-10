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
				classNames:	'UpBtn'
			}),
			
			/** Down button */
			DownBtn: MFT.Button.create({
				elementId:	'app_controlButtonsDownBtn',
				classNames:	'DownBtn'
			}),
			
			/** Left button */
			LeftBtn: MFT.Button.create({
				elementId:	'app_controlButtonsLeftBtn',
				classNames:	'LeftBtn'
			}),
			
			/** Right button */
			RightBtn: MFT.Button.create({
				elementId:	'app_controlButtonsRightBtn',
				classNames:	'RightBtn'
			}),
			
			/** Ok button */
			OkBtn: MFT.Button.create({
				elementId:	'app_controlButtonsOkBtn',
				classNames:	'OkBtn'
			})
		}),
		
		/** One button */
		OneBtn: MFT.Button.create({
			elementId:	'app_controlButtonsOneBtn',
			classNames:	'OneBtn btnNotPressed',
			text:		'1'
		}),
		
		/** Two button */
		TwoBtn: MFT.Button.create({
			elementId:	'app_controlButtonsTwoBtn',
			classNames:	'TwoBtn btnNotPressed',
			text:		'2'
		}),
		
		/** Three button */
		ThreeBtn: MFT.Button.create({
			elementId:	'app_controlButtonsThreeBtn',
			classNames:	'ThreeBtn btnNotPressed',
			text:		'3'
		}),
		
		/** Four button */
		FourBtn: MFT.Button.create({
			elementId:	'app_controlButtonsFourBtn',
			classNames:	'FourBtn btnNotPressed',
			text:		'4'
		}),
		
		/** Five button */
		FiveBtn: MFT.Button.create({
			elementId:	'app_controlButtonsFiveBtn',
			classNames:	'FiveBtn btnNotPressed',
			text:		'5'
		}),
		
		/** One button */
		SixBtn: MFT.Button.create({
			elementId:	'app_controlButtonsSixBtn',
			classNames:	'SixBtn btnNotPressed',
			text:		'6'
		}),
		
		/** One button */
		SevenBtn: MFT.Button.create({
			elementId:	'app_controlButtonsSevenBtn',
			classNames:	'SevenBtn btnNotPressed',
			text:		'7'
		}),
		
		/** One button */
		EightBtn: MFT.Button.create({
			elementId:	'app_controlButtonsEightBtn',
			classNames:	'EightBtn btnNotPressed',
			text:		'8'
		}),
		
		/** One button */
		NineBtn: MFT.Button.create({
			elementId:	'app_controlButtonsNineBtn',
			classNames:	'NineBtn btnNotPressed',
			text:		'9'
		}),
		
		/** One button */
		ZiroBtn: MFT.Button.create({
			elementId:	'app_controlButtonsZiroBtn',
			classNames:	'ZiroBtn btnNotPressed',
			text:		'0'
		})
	})
});