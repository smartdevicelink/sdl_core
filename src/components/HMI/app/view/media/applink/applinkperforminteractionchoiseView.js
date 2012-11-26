
/**
 * @name MFT.AppPerformInteractionChoise
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/applink/applinkperformInteractionchoiseView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.AppPerformInteractionChoise = Em.ContainerView.create(MFT.LoadableView, {

    classNames:         ['media_app_perform_interaction_view'],

    elementId:          'media_app_perform_interaction_view',

    childViews:         [
                            'backButton',
                            'listOfChoises',
                            'initialText'
                        ],

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],
        action:            'back',
        target:            'MFT.States',
        icon:              'images/media/ico_back.png',
    }),

    initialText:    MFT.Label.extend({

        elementId:          'initialText',

        classNames:         'initialText',

        contentBinding:     'MFT.ApplinkMediaController.performInteractionInitialText'
    }),


    /** Applink Sub Mennu activate handler */
    applinkPerformInteractionChoise: function(){
        if(MFT.States.media.applink.applinkperforminteractionchoise.active){
            MFT.AppPerformInteractionChoise.PerformInteraction(MFT.ApplinkMediaController.currentApplinkPerformInteractionChoiseId);
        }
    }.observes('MFT.States.media.applink.applinkperforminteractionchoise.active'),

    PerformInteraction: function( interactionChoiceSetIDList ){

        var count = this.listOfChoises.items.length;
        if(count > 0){
            for(var i = count-1; i >= 0; i--){
                this.listOfChoises.deleteItem(i);
            }
        }

        for(var IDList = 0; IDList< interactionChoiceSetIDList.length; IDList++){

            for(var ChoisesVal = 0; ChoisesVal < MFT.ApplinkModel.interactionChoises.length; ChoisesVal++){

                if( interactionChoiceSetIDList[IDList] == MFT.ApplinkModel.interactionChoises[ChoisesVal].interactionChoiceSetID ){
                    
                    for(var ChoiseSet = 0; ChoiseSet < MFT.ApplinkModel.interactionChoises[ChoisesVal].choiceSet.length; ChoiseSet++){

                        this.listOfChoises.items.push({
                                type:       MFT.Button,
                                params:     {
                                    action:                 'onPerformInteractionChoosed',
                                    target:                 'MFT.ApplinkMediaController',
                                    commandId:              MFT.ApplinkModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].choiceID,
                                    text:                   MFT.ApplinkModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].menuName,
                                    className:              'rs-item',
                                    templateName:           'text'
                                }                                   
                            });
                    }
                }
            }
        }
        this.listOfChoises.list.refresh();
    },

    /**
      * List of choises on AppPerformInteractionChoise screen
      */
    listOfChoises: MFT.List.extend({

        elementId:      'media_perform_interaction_list',

        itemsOnPage:    5,
                
        /** Items array*/
        items:          new Array()
    })
});