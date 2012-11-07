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

    classNames:         'VRPopUp',

    classNameBindings:      ['received'],

    childViews: [
        'popUp',
        'message1',
        'message2',
        'buttonsWrapper'
    ],

    content1:           'Title',

    content2:           'Text',

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
                text:               vrCommands[val]
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

    message1 : MFT.Label.extend({

        elementId:          'message1',

        classNames:         'message1',

        contentBinding:     'parentView.content1'
    }),

    message2 : MFT.Label.extend({

        elementId:          'message2',

        classNames:         'message2',

        contentBinding:     'parentView.content2'
    }),

    receiveMessage: function(msg1, msg2, duration, playTone){
        var self = this;

        this.set('content1', msg1);
        this.set('content2', msg2);
        this.set('received', true);
        //setTimeout(function(){self.set('received', false);}, duration);
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

            button = MFT.Button.create({
                elementId:          'media_app_options_view',
                click:              function(){
                    MFT.MediaController.turnOnAppSubMenu(1);
                },
                commandId:          1, 
                classNames:         ['rs-item'],
                //icon:             null,//'images/media/active_arrow.png',
                text:               "menuName" 
            });

            MFT.VRPopUp.buttonsWrapper.buttonsScroll.get('childViews').pushObject(button);
            MFT.VRPopUp.buttonsWrapper.scroll.refresh();
        }
    }),

    afterRender: function() {
/*
        var butt;

        for(var i in MFT.InfoController.get('appsParams')){

            butt = MFT.Button.create({
                goToState:          MFT.InfoController.get('appsParams')[i].goToState,
                classNames:         MFT.InfoController.get('appsParams')[i].classNames,
                icon:               MFT.InfoController.get('appsParams')[i].icon,
                textBinding:        MFT.InfoController.get('appsParams')[i].textBinding,
                arrow:              MFT.InfoController.get('appsParams')[i].arrow,
                action:             MFT.InfoController.get('appsParams')[i].action,
                target:             MFT.InfoController.get('appsParams')[i].target,
                disabledBinding:    MFT.InfoController.get('appsParams')[i].disabledBinding,
                onDown:             MFT.InfoController.get('appsParams')[i].onDown
            });

            MFT.VRPopUp.buttonsWrapper.buttonsScroll.get('_childViews').pushObject(butt);
        }
*/
        setTimeout(function () { MFT.VRPopUp.buttonsWrapper.loaded(); }, 200);
        
    }
});