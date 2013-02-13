/**
 * @name MFT.ScrollableMessage
 * 
 * @desc ScrollableMessage module visual representation
 * 
 * @category    View
 * @filesource  app/view/home/scrollableMessage.js
 * @version     1.0
 *
 * @author      Andriy Melnik
 */

MFT.ScrollableMessage = MFT.ApplinkAbstractView.create({

    elementId:          'ScrollableMessage',

    classNames:         'ScrollableMessage',

    classNameBindings:  ['active:active'],

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
     */
    deactivate: function() {
        clearTimeout(this.timer);
        this.set('active',false);
    },

    activate: function( appName, params ){
        if(appName){
            
            var self = this;
            
            this.set('captionText.content',appName);
            this.softButtons.addItems( params.softButtons, params.appId );
            this.set('listOfCommands.items', params.scrollableMessageBody );
            this.set('active',true);
            clearTimeout(this.timer);
            this.timer = setTimeout(function(){self.set('active', false);}, params.timeout);
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
      * List for option on ApplinkOptionsView screen
      */
    listOfCommands: MFT.ScrollableText.extend({

        elementId:      'scrollable_message_list',

        itemsOnPage:    11,

        /** Items array */
        items:          'asdasdasd'
    })
});