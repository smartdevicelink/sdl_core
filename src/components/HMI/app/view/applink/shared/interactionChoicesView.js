/**
 * @name MFT.InteractionChoicesView
 * 
 * @desc Interaction Choices visual representation
 * 
 * @category	View
 * @filesource	app/view/applink/shared/interactionChoicesView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.InteractionChoicesView = MFT.ApplinkAbstractView.create({
	
	elementId: 'perform_interaction_view',
	
	childViews: [
		'backButton',
		'captionText',
		'listOfChoices'
	],
    
    listOfChoices: MFT.List.extend({
    	elementId:	'perform_interaction_view_list',
        itemsOnPage:5,
        items:		[]
    }),
    
    /**
     * Clean choices caption and list before new proform
     */
    clean: function() {
    	this.set('initialText.content','Interaction Choices');
    	this.listOfChoices.items = [];
    	this.listOfChoices.list.refresh();
    },
    
    /**
     * Update choises list with actual set id
     *
     * @param data: Array
     */
    preformChoices: function( data ){
		
		if ( !data ) {
			Em.Logger.error('No choices to preform')
			return;
		}
    	
    	var i=0,
    		length = data.length;
    	
    	// temp for testing
    	for ( i=0; i < length; i++ ) {
    		this.listOfChoices.items.push(
    			{
					type: MFT.Button,
					params:{
						text:			data[i].menuName,
						choiceId:		data[i].choiceID,
						action:			'onChoiceInteraction',
						target:			'MFT.ApplinkAppController',
						templateName:	'text'
					}
				} 
    		);
    	}
    	    	
    	this.listOfChoices.list.refresh();
    }
});
