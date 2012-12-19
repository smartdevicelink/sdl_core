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
 
MFT.InteractionChoisesView = Em.ContainerView.create({
	
	elementId: 'perform_interaction_view',
	
	classNameBindings: [
		'active:active_state:inactive_state'
	],
	
	active: false,
	
	childViews: [
		'backButton',
		'initialText',
		'listOfChoises'
	],
	
	backButton: MFT.Button.extend({
		classNames:	['backButton'],
		target:		'this.parentView',
		action:		'deactivate',
		icon:		'images/media/ico_back.png',
	}),

    initialText: MFT.Label.extend({
        classNames:	['initialText'],
        content:	'Interaction Choises'
    }),
    
    listOfChoises: MFT.List.extend({
    	elementId:	'perform_interaction_view_list',
        itemsOnPage:5,
        items:		[]
    }),
    
    /**
     * Activate preform interaction window
     */
    activate: function() {
    	this.set('active',true);
    	this.preformChoises();
    },
    
    /**
     * Activate preform interaction window
     */
    deactivate: function() {
    	this.set('active',false);
    },
    
    /**
     * Update choises list with actual set id
     */
    preformChoises: function(){
    	
    	var i,count = 15;
    	
    	this.listOfChoises.items = [];
    	
    	// temp for testing
    	for ( i=0; i < count; i++ ) {
    		this.listOfChoises.items.push(
    			{
					type: MFT.Button,
					params:{
						text:			'Test Choises ' + Date.now() + i,
						templateName:	'text'
					}
				} 
    		);
    	}
    	    	
    	this.listOfChoises.list.refresh();
    }
});
