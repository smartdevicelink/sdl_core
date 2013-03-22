/**
 * @name MFT.InteractionChoicesView
 * @desc Interaction Choices visual representation
 * @category View
 * @filesource app/view/sdl/shared/interactionChoicesView.js
 * @version 1.0
 * @author Artem Petrosyan
 */

MFT.InteractionChoicesView = MFT.SDLAbstractView.create( {

    elementId: 'perform_interaction_view',

    childViews:
        [
            'backButton',
            'captionText',
            'listOfChoices'
        ],

    listOfChoices: MFT.List.extend( {
        elementId: 'perform_interaction_view_list',
        itemsOnPage: 5,
        items: []
    } ),

    timer: null,

    /**
     * Identifier of current request
     */
    performInteractionRequestId: null,

    /**
     * Deactivate window
     */
    deactivate: function( ABORTED ) {

        clearTimeout( this.timer );
        this.set( 'active', false );

        if( ABORTED ){
            MFT.SDLController.interactionChoiseCloseResponse( 'ABORTED', this.performInteractionRequestId );
        }
    },

    /**
     * Clean choices caption and list before new proform
     */
    clean: function() {
        this.set( 'captionText.content', 'Interaction Choices' );
        this.listOfChoices.items = [];
        this.listOfChoices.list.refresh();
    },

    /**
     * Update choises list with actual set id
     * 
     * @param data: Array
     */
    preformChoices: function( data, performInteractionRequestId, timeout ) {

        if( !data ){
            Em.Logger.error( 'No choices to preform' )
            return;
        }

        this.set( 'performInteractionRequestId', performInteractionRequestId );

        var i = 0, length = data.length, self = this;

        // temp for testing
        for( i = 0; i < length; i++ ){
            this.listOfChoices.items.push( {
                type: MFT.Button,
                params: {
                    text: data[i].menuName,
                    choiceId: data[i].choiceID,
                    action: 'onChoiceInteraction',
                    target: 'MFT.SDLAppController',
                    performInteractionRequestId: performInteractionRequestId,
                    templateName: 'text'
                }
            } );
        }

        this.listOfChoices.list.refresh();

        clearTimeout( this.timer );
        this.timer = setTimeout( function() {
            self.deactivate( true );
        }, timeout );
    }
} );
