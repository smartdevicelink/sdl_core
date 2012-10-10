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

	elementId:			'app_controlButtons',

	childViews: [
		'buttonControls'
	],

	buttonControls: Em.ContainerView.extend({
		elementId:		'buttonControls',
		
		classNames:	'buttonControls',

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
		
		/** Up button */
		DownBtn: MFT.Button.create({
			elementId:	'app_controlButtonsDownBtn',
			classNames:	'DownBtn'
		}),
		
		/** Up button */
		LeftBtn: MFT.Button.create({
			elementId:	'app_controlButtonsLeftBtn',
			classNames:	'LeftBtn'
		}),
		
		/** Up button */
		RightBtn: MFT.Button.create({
			elementId:	'app_controlButtonsRightBtn',
			classNames:	'RightBtn'
		}),
		
		/** Up button */
		OkBtn: MFT.Button.create({
			elementId:	'app_controlButtonsOkBtn',
			classNames:	'OkBtn'
		})
	})
});