/**
 * @name MFT.ScrollableMessage
 * 
 * @desc ScrollableMessage module visual representation
 * 
 * @category    View
 * @filesource  app/view/sdl/shared/scrollableMessage.js
 * @version     1.0
 *
 * @author      Andriy Melnik
 */

MFT.ScrollableMessage = MFT.SDLAbstractView.create({

    elementId:          'ScrollableMessage',

    classNames:         'ScrollableMessage',

    classNameBindings:  ['active:active'],

    /**
     * Id of current request
     * @type {Number}
     */
    messageRequestId:     null,

    active:             false,

    appId:              null,

    timer:              null,

    childViews: [
        'backButton',
        'captionText',
        'softButtons',
        'listOfCommands'
    ],

    /**
     * Deactivate View
     * @param {Object} ABORTED Parameter to indicate status for UI.ScrollableMessageResponse
     */
    deactivate: function( ABORTED ) {
        clearTimeout(this.timer);
        this.set('active',false);

        MFT.SDLController.scrollableMessageResponse( ABORTED ? 'ABORTED' : 'SUCCESS', this.messageRequestId );
    },

    activate: function( appName, params, messageRequestId ){
        if(appName){
            
            var self = this;
            
            this.set( 'messageRequestId', messageRequestId );
            this.set('captionText.content',appName);
            this.softButtons.addItems( params.softButtons, params.appId );
            this.set('listOfCommands.items', params.scrollableMessageBody );
            this.set('active',true);
            clearTimeout(this.timer);
            this.timer = setTimeout(
                function(){
                    self.deactivate();
                }, params.timeout);
        }
    },

    softButtons: MFT.MenuList.extend({

        itemsOnPage:    4,

        groupName:      "ScrollableMessage",

        content: Em.ContainerView.extend({

            classNames: ['content'],

            attributeBindings: ['parentView.contentPositon:style'],

        })
    }),

     /**
      * List for option on SDLOptionsView screen
      */
    listOfCommands: MFT.ScrollableText.extend({

        elementId:      'scrollable_message_list',

        itemsOnPage:    11,

        /** Items array */
        items:          'asdasdasd'
    })
});