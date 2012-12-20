/**
 * @name MFT.ScrollableMessage
 * 
 * @desc ScrollableMessage module visual representation
 * 
 * @category    View
 * @filesource  app/view/home/scrollableMessage.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */

MFT.ScrollableMessage = MFT.ApplinkAbstractView.create({

    elementId:          'ScrollableMessage',

    classNames:         'ScrollableMessage',

    classNameBindings:  ['active:active'],

    active:             false,

    appId:              null,

    childViews: [
        'backButton',
        'captionText',
        'softButtons',
        'listOfCommands'
    ],

    activate: function( appName, params ){
        if(appName){
            this.set('captionText.content',appName);
            this.softButtons.addItems( params.softButtons );
            this.set('listOfCommands.items', params.scrollableMessageBody );
            this.set('active',true);
        }
    },

    softButtons: MFT.MenuList.extend({

        itemsOnPage:    4,

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