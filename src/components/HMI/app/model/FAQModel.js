/**
 * @name MFT.FAQModel
 * 
 * @desc FAQ data model
 * 
 * @category	Model
 * @filesource	app/model/FAQModel.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.FAQModel = Em.Object.create({
	/** List of items menu*/
	list: [ 
		{index:'0',isDisabled:false, videoName:	'FAQ_Welcome_orientation'},
		{index:'1',isDisabled:false, videoName:   	'FAQ_4_corners'},
		{index:'2',isDisabled:false, videoName:	'FAQ_Customization'},
		{index:'3',isDisabled:false, videoName:	'FAQ_Voice_commands'},
		{index:'4',isDisabled:false, videoName:	'FAQ_Nav_system'},
		{index:'5',isDisabled:false, videoName:    'FAQ_Pair_phones'},
	],
});