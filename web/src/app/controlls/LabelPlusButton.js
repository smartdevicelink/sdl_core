/**
 * @name MFT.LablePlusButton
 * 
 * @desc Universal label component with button for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/LablePlusButton.js
 * @version		2.0
 *
 * @author		Maksym Getashchenko
 */

MFT.LablePlusButton = Em.ContainerView.extend({
	classNames: ['lablePlusButton'],

	/** Label text */
	labelContent: '',

	/** Label ico */
	labelIcon:	null,
	
	/** Class of button **/
	buttonClasses: [ ],

	/** Labels of button */
	buttonText: '',
	
	/** Action of buttno click**/
	buttonAction: null,
	
	/** Target of button action **/
	buttonTarget: null,
	
	/** Button right text **/
	buttonRightText: null,
	
	/** Enable of button arrow**/
	templateName: 'text',
	
	/** Button icon**/
	buttonIcon: null,
	
	/** Button time delay**/
	buttonTimer: null,
	
	/** Touch leave event flag */
	buttonTouchleave: false,

	/** Disable lable */
	labelDisabled: false,
	
	/** Enable of button down click action**/
	buttonOnDown: true,
	
	/** Button help-mode**/
	buttonHelpMode: false,

	/** Disable toggle button */
	buttonDisabled: false,
	
	childViews: [
		'label',
		'button'
	],

	/** Item lable */
	label: MFT.Label.extend({
		classNames: 'lable',
		contentBinding:	  'parentView.labelContent',
		iconBinding:		 'parentView.labelIcon',
		disabledBinding:	 'parentView.labelDisabled',
	}),

	/** Item toggle button */
	button: MFT.Button.extend({
		classNames:	'button',
		classNameBindings:	'parentView.buttonClasses',
		textBinding: 'parentView.buttonText',
		actionBinding: 'parentView.buttonAction',
		targetBinding: 'parentView.buttonTarget',		
		rightTextBinding: 'parentView.buttonRightText',
		templateNameBinding:	'parentView.templateName',
		iconBinding:	'parentView.buttonIcon',	
		timerBinding:	'parentView.buttonTimer',	
		touchleaveBinding:	'parentView.buttonTouchleave',
		onDownBinding:	'parentView.buttonOnDown',	
		helpModeBinding:	'parentView.buttonHelpMode',
		disabledBinding: 	'parentView.buttonDisabled',		
	}),
});