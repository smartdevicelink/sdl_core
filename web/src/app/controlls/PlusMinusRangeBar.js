/**
 * @name MFT.PlusMinusRangeBar
 *
 * @desc Universal button component with indicators bar for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/PlusMinusRangeBar.js
 * @version		1.0
 *
 * @author		Sergey Polishchuk
 */


MFT.PlusMinusRangeBar = Em.ContainerView.extend({

    init: function() {
        this._super();
        this.set('content',
            MFT.RangedValue.create( {
                range: this.range,
                value: this.value,
                cycle: false
            }
        ));
    },

	classNames:	['rangeBarContainer', 'line'],

	/** Label text */
	labelContent: '',

    target:	this.target ? this.target : this,

    /** Content binding */
    content: null,

    /** indicator image icon */
    indIcon: null,

    /** increase button image icon */
    incIcon: null,

    /** decrease button image icon */
    decIcon: null,

    /** indicator active class */
    indActiveClass:	null,

    /** indicator default class */
    indDefaultClass: null,

    /** increase action */
    actionInc: null,

    /** increase action */
    actionDec: null,

    /** range indicator bar */
    range: 3,

    /** start value indicator bar */
    value: 0,

	/** hide component, also in list*/
	hidden: false,

    childViews: [
        'Label',
        'Bar'
    ],

    Label: MFT.Label.extend({
        classNames:			    'indicatorsLabel',
        contentBinding:   		'parentView.labelContent'
    }),

    Bar: Em.ContainerView.extend({
        classNames:	'indicatorsBar',

        targetBinding: Ember.Binding.oneWay('parentView.target'),
        contentBinding: Ember.Binding.oneWay('parentView.content'),
        indIconBinding: Ember.Binding.oneWay('parentView.indIcon'),
        incIconBinding: Ember.Binding.oneWay('parentView.incIcon'),
        decIconBinding: Ember.Binding.oneWay('parentView.decIcon'),
        indActiveClassBinding:	Ember.Binding.oneWay('parentView.indActiveClass'),
        indDefaultClassBinding: Ember.Binding.oneWay('parentView.indDefaultClass'),
        actionIncBinding: Ember.Binding.oneWay('parentView.actionInc'),
        actionDecBinding: Ember.Binding.oneWay('parentView.actionDec'),

        childViews: [
            'Indicator',
            'DecButton',
            'IncButton'
        ],

        Indicator: MFT.IndButton.extend({
            elementId:			    'indicatorLine',
            classNames:			    'indicators',
            contentBinding:		    Ember.Binding.oneWay('parentView.content'),
            iconBinding:			Ember.Binding.oneWay('parentView.indIcon'),
            indActiveClassBinding:	Ember.Binding.oneWay('parentView.indActiveClass'),
            indDefaultClassBinding:	Ember.Binding.oneWay('parentView.indDefaultClass')
        }),

        DecButton: MFT.Button.extend({
            classNames:			    'dec-button',
            targetBinding:			Ember.Binding.oneWay('parentView.target'),
            actionBinding:			Ember.Binding.oneWay('parentView.actionDec'),
            iconBinding:			Ember.Binding.oneWay('parentView.decIcon'),
            timer:				200
        }),

        IncButton: MFT.Button.extend({
            classNames:			    'inc-button',
            targetBinding:			Ember.Binding.oneWay('parentView.target'),
            actionBinding:			Ember.Binding.oneWay('parentView.actionInc'),
            iconBinding:			Ember.Binding.oneWay('parentView.incIcon'),
            timer:				200
        })
    })
});