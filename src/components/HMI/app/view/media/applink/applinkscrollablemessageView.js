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

MFT.ScrollableMessage = Em.ContainerView.create(MFT.LoadableView,{

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

    /** Applink Sub Mennu activate handler */
    applinkSubMenuActivate: function(){
        if(MFT.States.media.applink.applinkoptions.applinkoptionssubmenu.active){
            MFT.ScrollableMessage.activate( params.appId, params.softButtons );
        }else{
            
        }
    }.observes('MFT.States.media.applink.applinkscrollablemessage.active'),

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],
        action:            'back',
        target:            'MFT.States',   
        icon:              'images/media/ico_back.png',
    }),

    titleText:           '',

    title:    MFT.Label.extend({

        elementId:          'titleText',

        classNames:         'titleText',

        contentBinding:     'this.parentView.titleText'
    }),

    activate: function( appName, params ){
        if(appName){
            this.set('titleText', appName);
            this.softButtons.addItems( params.softButtons );
            this.set('listOfCommands.items', params.scrollableMessageBody );
            MFT.States.goToState('media.applink.applinkscrollablemessage');
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