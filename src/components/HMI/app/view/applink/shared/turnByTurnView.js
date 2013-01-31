/**
 * @name MFT.TurnByTurnView
 * 
 * @desc TurnByTurnView module visual representation
 * 
 * @category    View
 * @filesource  app/view/home/turnByTurnView.js
 * @version     1.0
 *
 * @author      Andriy Melnik
 */

MFT.TurnByTurnView = MFT.ApplinkAbstractView.create({

    elementId:          'TurnByTurnView',

    classNames:         'TurnByTurnView',

    classNameBindings:  ['active:active'],

    active:             false,

    childViews: [
        'captionText',
        'softButtons',
        'totalDistanceLabel',
        'etaLabel',
        'turnList',
        'homeScreen',
        'navigationText2',
        'turnIconImage'
    ],

    appId:                      -1,
    navigationText2:            null,
    eta:                        null,
    totalDistance:              null,
    turnIcon:                   null,
    distanceToManeuver:         null,
    distanceToManeuverScale:    null,
    maneuverComplete:           null,

    activate: function( params ){
        if( params ){
            this.softButtons.addItems( params.softButtons );
            this.set('captionText.content', params.navigationText1);
            this.set('appId', params.appId);
            this.set('navigationText2', params.navigationText2);
            this.set('eta', params.eta);
            this.set('totalDistance', params.totalDistance);
            this.set('turnIcon', params.turnIcon);
            this.set('distanceToManeuver', params.distanceToManeuver);
            this.set('distanceToManeuverScale', params.distanceToManeuverScale);
            this.set('maneuverComplete', params.maneuverComplete);

            this.set('active',true);
        }
    },

    totalDistanceLabel: MFT.Label.extend({
        classNames:         'totalDistanceLabel',
        contentBinding:     'parentView.totalDistance'
    }),

    etaLabel: MFT.Label.extend({
        classNames:         'etaLabel',
        contentBinding:     'parentView.eta'
    }),

    turnList: MFT.Button.create({
        elementId:          'turnList',
        classNames:         'turnList btn',
        text:               'Turn List',
        action: function(){
            MFT.ApplinkController.tbtTurnList( this._parentView.appId );
        },
        target:             '',
        onDown:             false,
        templateName:       'arrow'
    }),

    turnIconImage: Em.View.create({
        classNames:         'turnIcon btn',
        attributeBindings:  ['style'],
        style: function(){
            if( this._parentView.turnIcon ){
                return 'background-image: URL(' + this._parentView.turnIcon + ');';
            }else{
                return '';
            }
        }.property('this.parentView.turnIcon')
    }),

    navigationText2: MFT.Label.extend({
        classNames:         'navigationText2',
        contentBinding:     'parentView.navigationText2'
    }),

    homeScreen: MFT.Button.create({
        elementId:      'homeScreen',
        classNames:     'homeScreen btn',
        text:           'Home Screen',
        iconBinding:    'MFT.ApplinkAppController.model.appIcon',
        target:         'this.parentView',
        action:         'deactivate',
        onDown:         false
    }),

    softButtons: MFT.MenuList.extend({

        itemsOnPage:    3,

        content: Em.ContainerView.extend({

            classNames: ['content']

        })
    })
});