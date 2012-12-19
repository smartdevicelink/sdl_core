/**
 * @name MFT.InteractionChoisesView
 * 
 * @desc Interaction Choises visual representation
 * 
 * @category	View
 * @filesource	app/view/interactionChoisesView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.InteractionChoisesView = Em.ContainerView.create( MFT.LoadableView, {
	
	elementId: 'perform_interaction_view',
	
	childViews: [
		'backButton',
		'initialText',
		'listOfChoises'
	],
	
	backButton: MFT.Button.extend({
		classNames:	['backButton'],
		icon:		'images/media/ico_back.png',
	}),

    initialText: MFT.Label.extend({
        classNames:	['initialText'],
        content:	'<Initial Text>'
    }),
    
    listOfChoises: MFT.List.extend({
        itemsOnPage:5,
        items:		[]
    })
});
