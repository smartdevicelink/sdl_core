/**
 * @name MFT.LabelPlusToggleButton
 * 
 * @desc Universal label component with toggle buttons for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/LabelPlusToggleButton.js
 * @version		2.0
 *
 * @author		Maksym Getashchenko
 */


MFT.LabelPlusToggleButton = Em.ContainerView.extend({
		
	classNames:			['lableToggleButton'],
	
	classNameBindings:	 ['hidden: hidden_display'],

	/** Label text */
	labelContent:		'',

	/** Label ico */
	labelIcon:			null,
	
	templateName: '',

	/** Index of selected item */
	tButtonValue:		'',

	/** Number of toggle buttons */
	tButtonRange:		'',

	/** Array of labels of toggle buttons */
	tButtonLabels:		[''],

	/** Disable lable */
	labelDisabled:		false,

	/** Disable toggle button */
	tButtonDisabled:	false,
	
	tButtonsClases:		'toggles button',
	
	classContainer:		['toogleButtonContainer'],
	
	/** hide compoment, also in list*/
	hidden: false,
	
	childViews: [
		'label',
		'buttonsGroup'
	],

	/** Item lable */
	label: MFT.Label.extend({
		classNames:			['lableToggle'],
		contentBinding:		'parentView.labelContent',
		iconBinding:		'parentView.labelIcon',
		templateNameBinding: 'parentView.templateName',
		disabledBinding:	'parentView.labelDisabled'
	}),

	/** Item toggle button */
	buttonsGroup: MFT.ButtonsGroup.extend({
		classNameBindings:	['parentView.classContainer'],
		valueBinding:		'parentView.tButtonValue',
		rangeBinding: 		'parentView.tButtonRange',
		labelsBinding:		'parentView.tButtonLabels',
		disabledBinding:	'parentView.tButtonDisabled',
		classesBinding: 	'this._parentView.tButtonsClases',
		controllerBinding:	'MFT.SettingsController'
	})
});