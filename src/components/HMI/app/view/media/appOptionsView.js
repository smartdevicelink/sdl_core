/**
 * @name MFT.AppOptionsView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/AppOptionsView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.AppOptionsView = Em.ContainerView.create({
    classNameBindings:  [ 'MFT.States.media.appoptions.active:active_state',
                          //  'MFT.MediaController.currentOptionsData.view',
                        ],

    classNames:        ['media_app_options_view', 'hidden'],

    elementId:          'media_app_options_view',
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

    AddSubMenu: function( menuId, menuName ){

        button = MFT.Button.create({
            elementId:          'media_app_options_view' + menuId,
            click:              function(){
                FFW.UI.onCommand(menuId);
            },
            commandId:          menuId, 
            classNames:         ['rs-item'],
            //icon:             null,//'images/media/active_arrow.png',
            text:               menuName 
        });

        MFT.AppOptionsView.buttonsWrapper.buttonsScroll.get('childViews').pushObject(button);
        MFT.AppOptionsView.buttonsWrapper.scroll.refresh();
    },

    DeleteSubMenu: function(menuId){

        if(Ember.View.views['media_app_options_view' + menuId]){
            Ember.View.views['media_app_options_view' + menuId].destroy();
            MFT.AppOptionsView.buttonsWrapper.scroll.refresh();
            return "SUCCESS";
        }else{
            return "INVALID_DATA";
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
                    MFT.AppOptionsView.buttonsWrapper.scroll.scrollTo(0, MFT.AppOptionsView.buttonsWrapper.scroll.y + 52, 200);
                    if( MFT.AppOptionsView.buttonsWrapper.scroll.y < -52){
                        MFT.AppOptionsView.buttonsWrapper.set('scrollPos', MFT.AppOptionsView.buttonsWrapper.scroll.y + 52);
                    }else{
                        MFT.AppOptionsView.buttonsWrapper.set('scrollPos', 0);
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
                    MFT.AppOptionsView.buttonsWrapper.scroll.scrollTo(0, MFT.AppOptionsView.buttonsWrapper.scroll.y - 52, 200);
                    if( Math.abs(MFT.AppOptionsView.buttonsWrapper.scroll.y) < ((MFT.AppOptionsView.buttonsWrapper.scroll.scrollerH - MFT.AppOptionsView.buttonsWrapper.scroll.wrapperH) - 52) ){
                        MFT.AppOptionsView.buttonsWrapper.set('scrollPos', MFT.AppOptionsView.buttonsWrapper.scroll.y - 52);
                    }else{
                        MFT.AppOptionsView.buttonsWrapper.set('scrollPos',  (MFT.AppOptionsView.buttonsWrapper.scroll.scrollerH - MFT.AppOptionsView.buttonsWrapper.scroll.wrapperH));
                    }
                }
            }),
            
            moveScrollBar:  function(){
                
                this.scroller.set( 'style', 'height:' + this._parentView.scrollBarH + 'px; margin-top:' + Math.abs(this._parentView.scrollPos) * this._parentView.coeficient + 'px;' );
            }.observes('this.parentView.scrollPos'),
        }),
        
        scrollEnd:  function(){
            if( MFT.AppOptionsView.buttonsWrapper.scroll.y > 0){
                this.set('scrollPos', 0);
            }else if( Math.abs(MFT.AppOptionsView.buttonsWrapper.scroll.y) > (MFT.AppOptionsView.buttonsWrapper.scroll.scrollerH - MFT.AppOptionsView.buttonsWrapper.scroll.wrapperH) ){
                this.set('scrollPos', MFT.AppOptionsView.buttonsWrapper.scroll.scrollerH - MFT.AppOptionsView.buttonsWrapper.scroll.wrapperH);
            }else{
                this.set('scrollPos', MFT.AppOptionsView.buttonsWrapper.scroll.y);
            }
        },
        
        loaded: function() {
            this.set('scroll', new iScroll('buttonsWrapper',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.AppOptionsView.buttonsWrapper.scrollEnd();}}));
            this.coeficient = this.scroll.wrapperH / this.scroll.scrollerH;
            this.scrollBarH = (this.scroll.wrapperH - 98) * this.coeficient - 49;
            this.scrollBar.scroller.set( 'style', 'height:' + this.scrollBarH + 'px;' );

            MFT.AppOptionsView.buttonsWrapper.scroll.refresh();
        }
    }),

    afterRender: function() {

        setTimeout(function () { MFT.AppOptionsView.buttonsWrapper.loaded(); }, 200);
        
    }
});