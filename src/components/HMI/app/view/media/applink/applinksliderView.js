
/**
 * @name MFT.ApplinkSliderView
 * 
 * @desc Media Applink Slider View module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/applink/applinksliderView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.ApplinkSliderView = Em.ContainerView.create(MFT.LoadableView, {

    classNames:         ['media_applink_slider_view'],

    elementId:          'media_applink_slider_view',

    childViews:         [
                            'backButton',
                            'initialText',
                            'headerLabel',
                            'footerLabel',
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

        contentBinding:     'MFT.ApplinkMediaController.model.appInfo.appName'
    }),

    headerLabel:    MFT.Label.extend({

        elementId:          'headerLabel',

        classNames:         'headerLabel',

        contentBinding:     'MFT.ApplinkMediaController.model.sliderParams.headerLabel'
    }),

    footerLabel:    MFT.Label.extend({

        elementId:          'footerLabel',

        classNames:         'footerLabel',

        contentBinding:     'MFT.ApplinkMediaController.model.sliderParams.footerLabel'
    }),

    footerText: function(){
        return MFT.ApplinkMediaController.model.sliderParams.footerLabel[MFT.ApplinkMediaController.model.applinkSliderContent.value];
    }.property('MFT.ApplinkMediaController.model.applinkSliderContent.value'),

    afterRender: function(){
        //this.activate();
    },

    activate: function(){
        //MFT.ApplinkController.getApplicationModel(1).

        this.get('childViews').removeObject(this.get('childViews')[4]);

        this.get('childViews').pushObject(

            Em.ContainerView.create( {
                disabled: false,

                classNames:         'control sliderControl',

                elementId:          'sliderControl',

                attributeBindings:  ['style'],

                /** Container components */
                childViews: [
                    'minusBtn',
                    'led',
                    'plusBtn'
                ],

                /** Minus button */
                minusBtn: MFT.Button.extend({
                    classNames: 'minus',
                    icon:       'images/common/minus-ico.png',
                    actionDown: function() {
                        this._super();
                        MFT.ApplinkMediaController.model.applinkSliderContent.decrease();
                        FFW.UI.sendSliderResult("SUCCESS");
                    }
                }),

                /** adjust */
                led: MFT.Indicator.create({
                    classNames:         'ledContainer ico',
                    contentBinding:     'MFT.ApplinkMediaController.model.applinkSliderContent',
                    indActiveClass:     'led',
                    indDefaultClass:    'led-inactive',
                }),

                /** Plus button */
                plusBtn: MFT.Button.extend({
                    classNames: 'plus',
                    icon:       'images/common/plus-ico.png',
                    actionDown: function() {
                        this._super();
                        MFT.ApplinkMediaController.model.applinkSliderContent.increase();
                        FFW.UI.sendSliderResult("SUCCESS");
                    }
                })
            })

        );

        this.get('childViews')[4].set( 'style', 
            'width:' + (MFT.ApplinkMediaController.model.applinkSliderContent.range * 12 + 118) + 'px; left:' + ( 340 - MFT.ApplinkMediaController.model.applinkSliderContent.range * 6) + 'px;' );

    },
/*
    slider: Em.ContainerView.create( {//MFT.SelectedIndex, {
          /*
            index:  this.get('index'),

            classNameBindings:  ['isSelected:active'],
        
            disabled: false,

            classNames:         'control sliderControl',

            elementId:          'sliderControl',

            /** Container components 
            childViews: [
                'minusBtn',
                'led',
                'plusBtn'
            ],

            /** Minus button 
            minusBtn: MFT.Button.extend({
                classNames: 'minus',
                icon:       'images/common/minus-ico.png',
                //action:     'onDecrease',
                //target:     'MFT.MCSController',

                //timer:      FLAGS.MCS_EMULATE_CAN ? 200 : 0,
                /*
                actionUp:   function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        this._super();
                        
                        if (!FLAGS.MCS_EMULATE_CAN) {
                            MFT.MCSController.model.send(this._parentView.index,'off');
                        }
                        MFT.MCSController.set('highlighted', false);
                    }
                },

                actionDown: function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        MFT.MCSController.set('highlighted', true);
                        this._super();
                    }
                },
                
                mouseLeave: function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        if (!FLAGS.MCS_EMULATE_CAN && this.pressed) {
                            MFT.MCSController.model.send(this._parentView.index,'off');
                        }
                        MFT.MCSController.set('highlighted', false);
                        
                        this._super();
                    }
                }
            }),

            /** adjust 
            led: MFT.Indicator.create({
                classNames:         'ledContainer ico',
                //action:             'select',
                //target:             'MFT.MCSController',
                //contentBinding:     'MFT.MCSController.activeSeat.' + (this.get('index') - 1),
                contentBinding:     'MFT.ApplinkMediaController.model.applinkSliderContent',
                indActiveClass:     'led',
                indDefaultClass:    'led-inactive',
/*
                actionDown: function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        MFT.MCSController.set('highlighted', true);
                        this._super();
                    }
                },

                actionUp:   function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        MFT.MCSController.set('highlighted', false);
                    }
                },
                
                mouseLeave: function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        MFT.MCSController.set('highlighted', false);
                    }
                }
            }),

            /** Plus button 
            plusBtn: MFT.Button.extend({
                classNames: 'plus',
                icon:       'images/common/plus-ico.png',
                //action:     'onIncrease',
                //target:     'MFT.MCSController',
                
                timer:      FLAGS.MCS_EMULATE_CAN ? 200 : 0,
/*
                actionUp:   function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        this._super();
                        
                        if (!FLAGS.MCS_EMULATE_CAN) {
                            MFT.MCSController.model.send(this._parentView.index,'off');
                        }
                        MFT.MCSController.set('highlighted', false);
                    }
                },

                actionDown: function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        MFT.MCSController.set('highlighted', true);
                        this._super();
                    }
                },
                
                mouseLeave: function() {
                    if(!this._parentView._parentView.get('disabled')) {
                        if (!FLAGS.MCS_EMULATE_CAN && this.pressed) {
                            MFT.MCSController.model.send(this._parentView.index,'off');
                        }
                        MFT.MCSController.set('highlighted', false);
                        
                        this._super();
                    }
                }
            })
        })*/

});
