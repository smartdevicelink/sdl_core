/**
 * @name MFT.InteractionChoicesView
 * 
 * @desc Interaction Choices visual representation
 * 
 * @category	View
 * @filesource	app/view/interactionChoicesView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.InteractionChoicesView = Em.ContainerView.create({
	
	elementId: 'perform_interaction_view',
	
	classNameBindings: [
		'active:active_state:inactive_state'
	],
	
	active: false,
	
	childViews: [
		'backButton',
		'initialText',
		'listOfChoices'
	],
	
	backButton: MFT.Button.extend({
		classNames:	['backButton'],
		target:		'this.parentView',
		action:		'deactivate',
		icon:		'images/media/ico_back.png',
	}),

    initialText: MFT.Label.extend({
        classNames:	['initialText'],
        content:	'Interaction Choices'
    }),
    
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
     * Activate preform interaction window
     * and set caption text
     *
     * @param text: String
     */
    activate: function( text ) {
    	this.set('initialText.content',text);
    	this.set('active',true); 	
    },
    
    /**
     * Activate preform interaction window
     */
    deactivate: function() {
    	this.set('active',false);
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
						action:			'onPerformInteraction',
						target:			'MFT.ApplinkAppController'
						templateName:	'text'
					}
				} 
    		);
    	}
    	    	
    	this.listOfChoices.list.refresh();
    }
});
