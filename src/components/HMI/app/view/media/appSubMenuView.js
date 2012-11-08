/**
 * @name MFT.AppSubMenuView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/AppSubMenuView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.AppSubMenuView = Em.ContainerView.create({
    classNameBindings:  [ 'MFT.States.media.appoptions.appsubmenu.active:active_state',
                          //  'MFT.MediaController.currentOptionsData.view',
                        ],

    classNames:        ['media_app_sub_menu_view', 'hidden'],

    elementId:          'media_app_sub_menu_view',
    childViews:         [
                            'backButton',
                            'buttonsWrapper'
                        ],

    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'back',
        target:            'MFT.States',   
        icon:              'images/media/ico_back.png',   
    }),

    SubMenuActivate: function( menuId ){
        
        MFT.States.goToState('media.appoptions.appsubmenu');

        if(MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('childViews').length > 0){
            for(var i = 0; i < MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('childViews').length; i++ ){
                Ember.View.views[MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('childViews')[i].elementId].destroy();
            }
        }

        for(var id = 0; i < MFT.AppModel.subMenuCommands.length; i++){
            if( menuId == MFT.AppModel.subMenuCommands[id].menuParams.parentID ){
                button = MFT.Button.create({
                    elementId:          'media_app_subMenu_view' + MFT.AppModel.subMenuCommands[id].cmdId,
                    click:              function(){
                        FFW.UI.onCommand(this.commandId);
                    },
                    commandId:          MFT.AppModel.subMenuCommands[id].cmdId, 
                    classNames:         ['rs-item'],
                    //icon:             null,//'images/media/active_arrow.png',
                    text:               MFT.AppModel.subMenuCommands[id].menuParams.menuName 
                });

                MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('childViews').pushObject(button);
            }
        }

        MFT.AppSubMenuView.buttonsWrapper.scroll.refresh();


    },

    buttonsWrapper: Em.ContainerView.extend({
        classNames:         'buttonsWrapper',
        elementId:          'buttonsWrapperSubMenu',
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
            elementId:  'buttonsScrollSubMenu',
            actionUp:   function(){
                this._parentView.pos = this._parentView.scroll.y - this._parentView.scroll.startY;
            }
        }),

        scrollBar: Em.ContainerView.extend({
            classNames:         'scrollBar',
            elementId:          'scrollBarSubMenu',
            childViews: [
                'scrollArrowUp',
                'scroller',
                'scrollArrowDown'
            ],

            scrollArrowUp:   MFT.Button.extend({
                classNames: 'scrollArrows scrollArrowUp button',
                click:      function(){
                    MFT.AppSubMenuView.buttonsWrapper.scroll.scrollTo(0, MFT.AppSubMenuView.buttonsWrapper.scroll.y + 52, 200);
                    if( MFT.AppSubMenuView.buttonsWrapper.scroll.y < -52){
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.y + 52);
                    }else{
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos', 0);
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
                    MFT.AppSubMenuView.buttonsWrapper.scroll.scrollTo(0, MFT.AppSubMenuView.buttonsWrapper.scroll.y - 52, 200);
                    if( Math.abs(MFT.AppSubMenuView.buttonsWrapper.scroll.y) < ((MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH) - 52) ){
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.y - 52);
                    }else{
                        MFT.AppSubMenuView.buttonsWrapper.set('scrollPos',  (MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH));
                    }
                }
            }),
            
            moveScrollBar:  function(){
                
                this.scroller.set( 'style', 'height:' + this._parentView.scrollBarH + 'px; margin-top:' + Math.abs(this._parentView.scrollPos) * this._parentView.coeficient + 'px;' );
            }.observes('this.parentView.scrollPos'),
        }),
        
        scrollEnd:  function(){
            if( MFT.AppSubMenuView.buttonsWrapper.scroll.y > 0){
                this.set('scrollPos', 0);
            }else if( Math.abs(MFT.AppSubMenuView.buttonsWrapper.scroll.y) > (MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH) ){
                this.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.scrollerH - MFT.AppSubMenuView.buttonsWrapper.scroll.wrapperH);
            }else{
                this.set('scrollPos', MFT.AppSubMenuView.buttonsWrapper.scroll.y);
            }
        },
        
        loaded: function() {
            this.set('scroll', new iScroll('buttonsWrapperSubMenu',{scrollbarClass: 'display:none', momentum: false, onBeforeScrollEnd: function(){MFT.AppSubMenuView.buttonsWrapper.scrollEnd();}}));
            this.coeficient = this.scroll.wrapperH / this.scroll.scrollerH;
            this.scrollBarH = (this.scroll.wrapperH - 98) * this.coeficient - 49;
            this.scrollBar.scroller.set( 'style', 'height:' + this.scrollBarH + 'px;' );
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

            MFT.AppSubMenuView.buttonsWrapper.buttonsScroll.get('_childViews').pushObject(butt);
        }
*/
        setTimeout(function () { MFT.AppSubMenuView.buttonsWrapper.loaded(); }, 200);
        
    }
});