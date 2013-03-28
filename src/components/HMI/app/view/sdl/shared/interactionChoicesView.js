/**
 * @name MFT.InteractionChoicesView
 * 
 * @desc Interaction Choices visual representation
 * 
 * @category	View
 * @filesource	app/view/sdl/shared/interactionChoicesView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.InteractionChoicesView = MFT.SDLAbstractView.create({
	
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
     * Identifier of current request
     */
    performInteractionRequestId: null,
    
    /**
     * Deactivate window
     */
    deactivate: function() {
        this.set('active',false);
    },

    backButton: MFT.Button.extend({
        classNames: [
            'back-button'
        ],
        target:     'this.parentView',
        action:     'deactivate',
        icon:       'images/media/ico_back.png',
        onDown:     false,
        click: function(){
            MFT.SDLController.interactionChoiseCloseResponse( 'ABORTED', this.get( 'parentView.performInteractionRequestId' ) );
        }
    }),
    
    /**
     * Clean choices caption and list before new proform
     */
    clean: function() {
    	this.set('captionText.content','Interaction Choices');
    	this.listOfChoices.items = [];
    	this.listOfChoices.list.refresh();
    },
    
    /**
     * Update choises list with actual set id
     *
     * @param data: Array
     */
    preformChoices: function( data, performInteractionRequestId ){
		
		if ( !data ) {
			Em.Logger.error('No choices to preform')
			return;
		}

        this.set( 'performInteractionRequestId', performInteractionRequestId );
    	
    	var i=0,
    		length = data.length;
    	
    	// temp for testing
    	for ( i=0; i < length; i++ ) {
    		this.listOfChoices.items.push(
    			{
					type: MFT.Button,
					params:{
						text:			              data[i].menuName,
						choiceId:		              data[i].choiceID,
						action:			              'onChoiceInteraction',
						target:                       'MFT.SDLAppController',
                        performInteractionRequestId:  performInteractionRequestId,
						templateName:	'text'
					}
				} 
    		);
    	}
    	    	
    	this.listOfChoices.list.refresh();
    }
});
