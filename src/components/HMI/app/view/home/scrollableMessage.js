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

MFT.ScrollableMessage = Em.ContainerView.create({

    elementId:          'ScrollableMessage',

    classNames:         'ScrollableMessage',

    classNameBindings:  ['active:active'],

    active:             false,

    appId:              null,

    childViews: [
        'backButton',
        'title',
        'softButtons',
        'listOfCommands'
    ],

    backButton: MFT.Button.extend({
        classNames: 'backButton',
        
        icon:       'images/common/ico_back.png',
        action: function(el){
            el.get('parentView').deactivate();
        }
    }),

    title:    MFT.Label.extend({

        elementId:          'titleText',

        classNames:         'titleText',

        contentBinding:     'MFT.ApplinkController.getApplicationModel(this.parentView.appId).appInfo.appName'
    }),

    activate: function(appId, buttons){
        if(appId){
            this.set('appId', appId);
            this.softButtons.addItem( buttons );
        }
        this.set('active', true);
    },

    deactivate: function(){
        this.set('active', false);
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
    listOfCommands: MFT.List.extend({

        elementId:      'VR_list',

        itemsOnPage:    5,

        /** Items array */
        items:          new Array()
    })
});