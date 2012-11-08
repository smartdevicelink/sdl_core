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

        for(var i=0;i<vrCommands.length;i++){

            button = MFT.Button.create({
                elementId:          'vrCommand_' + commandId + "_" + Math.floor((Math.random()*100)+1),
                click:              function() {
                    FFW.UI.onCommand(this.commandId);
		    MFT.VRPopUp.set('received', false);
                },
                commandId:          commandId, 
                classNames:         ['rs-item'],
                //icon:             null,//'images/media/active_arrow.png',
                text:               vrCommands[i],
                templateName:       'text'
            });

            this.buttonsWrapper.buttonsScroll.get('childViews').pushObject(button);
        }

//        this.scroll.refresh();
    },

    DeleteCommand: function(commandId){

	  var i = this.buttonsWrapper.buttonsScroll.get('childViews').length-1;
	  do
	  {
		var button = this.buttonsWrapper.buttonsScroll.get('childViews')[i];
		if (button.commandId == commandId) {
			Em.View.views[button.elementId].destroy();
			this.buttonsWrapper.buttonsScroll.get('childViews').splice(i, 1);
		}
		i--;
	  }
	  while (i>=0);

//        MFT.VRPopUp.scroll.refresh();
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
        elementId:          'buttonsWrapperVR',
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
            elementId:  'buttonsScrollVR',
            actionUp:   function(){
                this._parentView.pos = this._parentView.scroll.y - this._parentView.scroll.startY;
            }
        }),

        scrollBar: Em.ContainerView.extend({
            classNames:         'scrollBar',
            elementId:          'scrollBarVR',
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
            this.set('scroll', new iScroll('buttonsWrapperVR',{scrollbarClass: 'display:none', momentum: true, bounce: false, onBeforeScrollEnd: function(){MFT.VRPopUp.buttonsWrapper.scrollEnd();}}));
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