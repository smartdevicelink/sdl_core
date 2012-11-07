/**
 * @name MFT.VRPopUp
 * 
 * @desc VRPopUp module visual representation
 * 
 * @category    View
 * @filesource  app/view/home/VRPopUp.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */

MFT.VRPopUp = Em.ContainerView.create({

    elementId:          'VRPopUp',

    classNames:         'VRPopUp hide',

    classNameBindings:      ['received'],

    childViews: [
        'popUp',
        'buttonsWrapper'
    ],

    received:           false,

    AddCommand: function( commandId, vrCommands){

        for(var val in vrCommands){

            button = MFT.Button.create({
                elementId:          'media_rightmenu_softButton' + vrCommands[val],
                click:              function(){
                    FFW.UI.onCommand(this.commandId);
                },
                commandId:          commandId, 
                classNames:         ['rs-item'],
                //icon:             null,//'images/media/active_arrow.png',
                text:               vrCommands[val],
                templateName:       'text'
            });

            MFT.VRPopUp.scrollWrapper.scroller.get('childViews').pushObject(button);
        }
        MFT.VRPopUp.scroll.refresh();
    },

    DeleteCommand: function(commandId){

        if(Ember.View.views['media_rightmenu_softButton' + commandId]){
            Ember.View.views['media_rightmenu_softButton' + commandId].destroy();
        }
        MFT.VRPopUp.scroll.refresh();
    },

    popUp : Em.View.extend({

        elementId:          'popUp',

        classNames:         'popUp',
    }),

    receiveMessage: function(){
        var self = this;
        if(this.received){
            this.set('received', false);
        }else{
            this.set('received', true);
        }
    },

    buttonsWrapper: Em.ContainerView.extend({
        classNames:         'buttonsWrapper',
        elementId:          'buttonsWrapper',
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
            elementId:  'buttonsScroll',
            actionUp:   function(){
                this._parentView.pos = this._parentView.scroll.y - this._parentView.scroll.startY;
            }
        }),

        scrollBar: Em.ContainerView.extend({
            classNames:         'scrollBar',
            elementId:          'scrollBar',
            childViews: [
                'scrollArrowUp',
                'scroller',
                'scrollArrowDown'
            ],

            scrollArrowUp:   MFT.Button.extend({
                classNames: 'scrollArrows scrollArrowUp button',
                click:      function(){
                    MFT.VRPopUp.buttonsWrapper.scroll.scrollTo(0, MFT.VRPopUp.buttonsWrapper.scroll.y + 52, 200);
                    if( MFT.VRPopUp.buttonsWrapper.scroll.y < -52){
                        MFT.VRPopUp.buttonsWrapper.set('scrollPos', MFT.VRPopUp.buttonsWrapper.scroll.y + 52);
                    }else{
                        MFT.VRPopUp.buttonsWrapper.set('scrollPos', 0);
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
                    MFT.VRPopUp.buttonsWrapper.scroll.scrollTo(0, MFT.VRPopUp.buttonsWrapper.scroll.y - 52, 200);
                    if( Math.abs(MFT.VRPopUp.buttonsWrapper.scroll.y) < ((MFT.VRPopUp.buttonsWrapper.scroll.scrollerH - MFT.VRPopUp.buttonsWrapper.scroll.wrapperH) - 52) ){
                        MFT.VRPopUp.buttonsWrapper.set('scrollPos', MFT.VRPopUp.buttonsWrapper.scroll.y - 52);
                    }else{
                        MFT.VRPopUp.buttonsWrapper.set('scrollPos',  (MFT.VRPopUp.buttonsWrapper.scroll.scrollerH - MFT.VRPopUp.buttonsWrapper.scroll.wrapperH));
                    }
                }
            }),
            
            moveScrollBar:  function(){
                
                this.scroller.set( 'style', 'height:' + this._parentView.scrollBarH + 'px; margin-top:' + Math.abs(this._parentView.scrollPos) * this._parentView.coeficient + 'px;' );
            }.observes('this.parentView.scrollPos'),
        }),
        
        scrollEnd:  function(){
            if( MFT.VRPopUp.buttonsWrapper.scroll.y > 0){
                this.set('scrollPos', 0);
            }else if( Math.abs(MFT.VRPopUp.buttonsWrapper.scroll.y) > (MFT.VRPopUp.buttonsWrapper.scroll.scrollerH - MFT.VRPopUp.buttonsWrapper.scroll.wrapperH) ){
                this.set('scrollPos', MFT.VRPopUp.buttonsWrapper.scroll.scrollerH - MFT.VRPopUp.buttonsWrapper.scroll.wrapperH);
            }else{
                this.set('scrollPos', MFT.VRPopUp.buttonsWrapper.scroll.y);
            }
        },
        
        loaded: function() {
            this.set('scroll', new iScroll('buttonsWrapper',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.VRPopUp.buttonsWrapper.scrollEnd();}}));
            this.coeficient = this.scroll.wrapperH / this.scroll.scrollerH;
            this.scrollBarH = (this.scroll.wrapperH - 98) * this.coeficient - 49;
            this.scrollBar.scroller.set( 'style', 'height:' + this.scrollBarH + 'px;' );

            MFT.VRPopUp.buttonsWrapper.scroll.refresh();
        }
    }),

    afterRender: function() {

        setTimeout(function () { MFT.VRPopUp.buttonsWrapper.loaded(); }, 200);
        
    }
});