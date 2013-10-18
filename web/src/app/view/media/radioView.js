/**
 * @name MFT.AMFMInfoView
 * 
 * @desc Media AM/FM INFO module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/AMFMInfoView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.radioView = Em.ContainerView.create(MFT.LoadableView,{
	/** View Id */	
	elementId: 'radio_view',
    classNameBindings: [
        'MFT.States.media.radio.sirius.browse.active:hide-important',
        'MFT.States.media.radio.sirius.options.active:hide-important'
    ]
});