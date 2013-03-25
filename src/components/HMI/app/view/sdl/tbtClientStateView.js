/**
 * @name MFT.TBTClientStateView
 * 
 * @desc TBTClientStateView module visual representation
 * 
 * @category    View
 * @filesource  app/view/sdl/TBTClientStateView.js
 * @version     2.0
 *
 * @author      Andriy Melnyk
 */

MFT.TBTClientStateView = Em.ContainerView.create({

    elementId:          'tbtClientStateView',

    classNames:         'tbtClientStateView',

    classNameBindings:      ['active'],

    childViews: [
        'tbtClientStateLabel',
        'tbtClientState',
        'tbtClientStateSelect'
    ],

    /**
     * Title of VehicleInfo PopUp view
     */ 
    tbtClientStateLabel:    MFT.Label.extend({

        elementId:      'tbtClientStateLabel',

        classNames:     'tbtClientStateLabel',

        content:        'TBT Client State'
    }),


    /**
     * Property indicates the activity state of TBTClientStateView
     */
    active:           false,

    /**
     * Title of tbtClientState group of parameters
     */ 
    tbtClientState : MFT.Label.extend({

        elementId:          'tbtClientState',

        classNames:         'tbtClientState',

        content:            'Client State'
    }),

    /**
     * HMI element Select with parameters of TBTClientStates
     */ 
    tbtClientStateSelect : Em.Select.extend({

        elementId:          'tbtClientStateSelect',

        classNames:         'tbtClientStateSelect',

        contentBinding:     'MFT.SDLModel.tbtClientStates',

        optionValuePath:    'content.id',

        optionLabelPath:    'content.name',

        /**
         * Selected data sent on model for further processing
         */
        click: function(){

            MFT.SDLController.tbtClientStateSeleced( this.selection.name );

        }
    }),

    /**
     * Trigger function that activates and deactivates tbtClientStateView
     */
    toggleActivity: function(){
        this.toggleProperty('active');
        MFT.SDLController.onSystemContextChange();
    }
});