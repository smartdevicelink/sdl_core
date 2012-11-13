
/**
 * @name MFT.AppPerformInteractionChoise
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/AppPerformInteractionChoise.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.AppPerformInteractionChoise = Em.ContainerView.create({
    classNameBindings:  [ 'MFT.States.media.appperforminteraction.active:active_state' ],

    classNames:        ['media_app_perform_interaction_view', 'hidden'],

    elementId:          'media_app_perform_interaction_view',
    childViews:         [
                            'backButton',
                            'buttonsWrapper'
                        ],

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'optionsBack',
        target:            'MFT.MediaController',   
        icon:              'images/media/ico_back.png',   
    }),

    PerformInteraction: function( interactionChoiceSetIDList ){

        var i = this.buttonsWrapper.buttonsScroll.get('childViews').length-1;
        if(i >= 0){
            do{
                var button = this.buttonsWrapper.buttonsScroll.get('childViews')[i];
                Em.View.views[button.elementId].destroy();
                i--;
            }
            while (i>=0);
        }

        for(var IDList = 0; IDList< interactionChoiceSetIDList.length; IDList++){

            for(var ChoisesVal = 0; ChoisesVal < MFT.AppModel.interactionChoises.length; ChoisesVal++){

                if( interactionChoiceSetIDList[IDList] == MFT.AppModel.interactionChoises[ChoisesVal].interactionChoiceSetID ){
                    
                    for(var ChoiseSet = 0; ChoiseSet < MFT.AppModel.interactionChoises[ChoisesVal].choiceSet.length; ChoiseSet++){
                        button = MFT.Button.create({
                            elementId:          'media_app_options_view' + MFT.AppModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].menuName,
                            click:              function(){
                                FFW.UI.onChoosed(this.commandId);
                            },
                            commandId:          MFT.AppModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].choiceID, 
                            classNames:         ['rs-item'],
                            //icon:             null,//'images/media/active_arrow.png',
                            text:               MFT.AppModel.interactionChoises[ChoisesVal].choiceSet[ChoiseSet].menuName,
                            templateName:       'text'
                        });

                        MFT.AppPerformInteractionChoise.buttonsWrapper.buttonsScroll.get('childViews').pushObject(button);
                    }
                }
            }
        }

        MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.refresh();

        MFT.MediaController.turnOnAppPerform();
    },

    buttonsWrapper: Em.ContainerView.extend({
        classNames:         'buttonsWrapper',
        elementId:          'buttonsWrapperPerformInteraction',
        childViews: [
            'buttonsScroll',
            'scrollBar'
        ],

        scroll:             null,

        pos:                0,

        scrollPos:          0,

        coeficient:         0,

        scrollBarH:         0,

        buttonsScroll: Em.ContainerView.extend( Ember.TargetActionSupport, {
            classNames: 'buttonsScroll',
            elementId:  'buttonsScrollPerformInteraction',
            actionUp:   function(){
                this._parentView.pos = this._parentView.scroll.y - this._parentView.scroll.startY;
            }
        }),

        scrollBar: Em.ContainerView.extend({
            classNames:         'scrollBar',
            elementId:          'scrollBarPerformInteraction',
            childViews: [
                'scrollArrowUp',
                'scroller',
                'scrollArrowDown'
            ],

            scrollArrowUp:   MFT.Button.extend({
                classNames: 'scrollArrows scrollArrowUp button',
                click:      function(){
                    MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.scrollTo(0, MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y + 52, 200);
                    if( MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y < -52){
                        MFT.AppPerformInteractionChoise.buttonsWrapper.set('scrollPos', MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y + 52);
                    }else{
                        MFT.AppPerformInteractionChoise.buttonsWrapper.set('scrollPos', 0);
                    }
                }
            }),

            scroller: Em.View.extend({
                classNames: 'scroller',
                attributeBindings: ['style'],
                style: 'margin-top: 0px'
            }),

            scrollArrowDown:   MFT.Button.extend({
                classNames: 'scrollArrows scrollArrowDown button',
                click:      function(){
                    MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.scrollTo(0, MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y - 52, 200);
                    if( Math.abs(MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y) < ((MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.scrollerH - MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.wrapperH) - 52) ){
                        MFT.AppPerformInteractionChoise.buttonsWrapper.set('scrollPos', MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y - 52);
                    }else{
                        MFT.AppPerformInteractionChoise.buttonsWrapper.set('scrollPos',  (MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.scrollerH - MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.wrapperH));
                    }
                }
            }),
            
            moveScrollBar:  function(){
                
                this.scroller.set( 'style', 'height:' + this._parentView.scrollBarH + 'px; margin-top:' + Math.abs(this._parentView.scrollPos) * this._parentView.coeficient + 'px;' );
            }.observes('this.parentView.scrollPos'),
        }),
        
        scrollEnd:  function(){
            if( MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y > 0){
                this.set('scrollPos', 0);
            }else if( Math.abs(MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y) > (MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.scrollerH - MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.wrapperH) ){
                this.set('scrollPos', MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.scrollerH - MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.wrapperH);
            }else{
                this.set('scrollPos', MFT.AppPerformInteractionChoise.buttonsWrapper.scroll.y);
            }
        },
        
        loaded: function() {
            this.set('scroll', new iScroll('buttonsWrapperPerformInteraction',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.AppPerformInteractionChoise.buttonsWrapper.scrollEnd();}}));
            this.coeficient = this.scroll.wrapperH / this.scroll.scrollerH;
            this.scrollBarH = (this.scroll.wrapperH - 98) * this.coeficient - 49;
            this.scrollBar.scroller.set( 'style', 'height:' + this.scrollBarH + 'px;' );
        }
    }),

    afterRender: function() {

        setTimeout(function () { MFT.AppPerformInteractionChoise.buttonsWrapper.loaded(); }, 200);
        
    }
});