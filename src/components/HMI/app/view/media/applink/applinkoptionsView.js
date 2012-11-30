/**
 * @name MFT.ApplinkOptionsView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/ApplinkOptionsView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.ApplinkOptionsView = Em.ContainerView.create(MFT.LoadableView,{

    classNames:        ['media_app_options_view'],

	/** View Id */	
    elementId:          'media_app_options_view',

	/** View Components*/
    childViews:         [
                            'backButton',
                            'listOfOptions',
                            'optionsLabel'
                        ],

    /** Applink Sub Mennu activate handler */
    applinkOptionsActivate: function(){
        this.showOptionsList();
    }.observes('MFT.States.media.applink.applinkoptions.active'),

    /** Button to return to previous view*/
    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'back',
        target:            'MFT.States',  
        icon:              'images/media/ico_back.png',   
    }),

    optionsLabel:    MFT.Label.extend({

        elementId:          'optionsLabel',

        classNames:         'optionsLabel',

        content:            'Options'
    }),


    showOptionsList: function(){
        if( MFT.States.media.applink.applinkoptions.active ){
            this.listOfOptions.items =  MFT.ApplinkMediaModel.optionsCommands.slice();
            this.listOfOptions.list.refresh();
        }
    },

    /**
      * List for option on ApplinkOptionsView screen
      */
    listOfOptions: MFT.List.extend({

        elementId:      'media_app_options_list',

        itemsOnPage:    5,
                
        /** Items array */
        items:          new Array()
    })
});