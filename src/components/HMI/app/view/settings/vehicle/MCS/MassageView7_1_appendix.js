/**
 * @name MFT.MassageView7_1_appendix
 * 
 * @desc MCS massage view of component 7.1 for MFT application
 * 
 * @category	Views
 * @filesource	app/view/settings/vehicle/MCS/MassageView7_1_appendix.js
 * @version		2.0
 *
 * @author	Andriy Melnik
 */

MFT.MassageView7_1_appendix = Em.ContainerView.extend({

	elementId:	'seatsMassageView7_1_appendix',

	childViews: [
		'customAdjust',
		'seatsMassageDialogView0',
		'seatsMassageDialogView1',
		'seatsMassageDialogView2',
		'seatsMassageDialogView3',
		'seatsMassageDialogView4',
		'seatsMassageDialogView5'
	],

	/** Observer for hiding popUps when going out from video */
	popUpHide: function() {
		MFT.MCSController.set('massageCustomLevel2', false);
		MFT.MCSController.set('dialog.0', false);
		MFT.MCSController.set('dialog.1', false);
		MFT.MCSController.set('dialog.2', false);
		MFT.MCSController.set('dialog.3', false);
		MFT.MCSController.set('dialog.4', false);
		MFT.MCSController.set('dialog.5', false);
	}.observes('MFT.States.settings.vehicle.mcs.active'),

		/** Observer for hiding popUps when going out from video */
	localBackAction: function() {
		MFT.VideoPlayerController.stop();
		MFT.MCSController.set('dialog.0', false);
		MFT.MCSController.set('dialog.1', false);
		MFT.MCSController.set('dialog.2', false);
		MFT.MCSController.set('dialog.3', false);
		MFT.MCSController.set('dialog.4', false);
		MFT.MCSController.set('dialog.5', false);
	}.observes('MFT.MCSController.massageCustomLevel2'),
	
	
	/** Custom adjustment massage */
	customAdjust: Em.ContainerView.create({

		classNames:		'MultiContourSeat_customMassage',

		classNameBindings: ['MFT.MCSController.massageCustom:visible'],
		
		childViews: [
			'back'
		],

		/** Dublicate of back button*/
		back: MFT.Button.extend({
			classNameBindings:	['MFT.MCSController.massageCustomLevel2:visible'],
			classNames:			'localBack btn btn-48',
			actionUp:			function(){
				if( true == MFT.MCSController.get('massageCustomLevel2') ){
					MFT.MCSController.set('massageCustomLevel2', false);
					MFT.MCSController.back();
				}else {
					MFT.States.back();
				}
				
			}
		})

	}).appendTo("#app"),

	/** Settings button for massage custom adjustment */
	seatsMassageDialogView0: MFT.SeatsMassageDialogView.extend({
		classNameBindings:	['dialog0:show'],
		classNames:			'View1',
		dialog0Binding:		'MFT.MCSController.dialog.0',
		value:		0,
		range:		6,
		currIndex:	0,
		question:	'Which problem do you experience?',
		answers: 	[
						'Lower back pain',
						'Upper back pain',
						'Herniated vertebral disk',
						'Numbness of the buttocks',
						'Stiffness in the lumbar area',
						'No specific problem'
					]
	}),

	/** Settings button for massage custom adjustment */
	seatsMassageDialogView1: MFT.SeatsMassageDialogView.extend({
		classNameBindings: ['dialog1:show'],
		dialog1Binding:		'MFT.MCSController.dialog.1',
		value:		0,
		range:		3,
		currIndex:	1,
		question:	'What is your height?',
		answers: 	[
						'< 5’3” ( 1m60 )',
						'5’3” – 5’11” ( 1m160 – 1m80 )',
						'> 5’11” ( 1m80 )'
					]
	}),

	/** Settings button for massage custom adjustment */
	seatsMassageDialogView2: MFT.SeatsMassageDialogView.extend({
		classNameBindings: ['dialog2:show'],
		dialog2Binding:		'MFT.MCSController.dialog.2',
		value:		0,
		range:		3,
		currIndex:	2,
		question:	'What massage strength do you like?',
		answers: 	[
						'Gentle',
						'Medium',
						'Strong'
					]
	}),

	/** Settings button for massage custom adjustment */
	seatsMassageDialogView3: MFT.SeatsMassageDialogView.extend({
		classNameBindings: ['dialog3:show'],
		dialog3Binding:		'MFT.MCSController.dialog.3',
		value:		0,
		range:		3,
		currIndex:	3,
		question:	'Do you want a fast or slow massage?',
		answers: 	[
						'Slow ',
						'Medium ',
						'Fast'
					]
	}),

	/** Settings button for massage custom adjustment */
	seatsMassageDialogView4:  MFT.Label.extend({
		classNameBindings: 	['dialog4:show'],
		dialog4Binding:		'MFT.MCSController.dialog.4',
		elementId:			'seatsMassageDialogView4',
		classNames:			'seatsMassageDialogView4',
		content:			'Good morning, I am going to help you with optimizing your massage settings'
	}),

	/** Settings button for massage custom adjustment */
	seatsMassageDialogView5:  MFT.Label.extend({
		classNameBindings: 	['dialog5:show'],
		dialog5Binding:		'MFT.MCSController.dialog.5',
		elementId:			'seatsMassageDialogView5',
		classNames:			'seatsMassageDialogView5',
		content:			'Your personalized massage settings have been set. To start the personalized massage press the "MyMassage" button'
	})
});