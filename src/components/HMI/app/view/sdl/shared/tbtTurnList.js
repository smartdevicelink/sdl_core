/**
 * @name MFT.TBTTurnList
 * 
 * @desc TBT Turn List visual representation
 * 
 * @category    View
 * @filesource  app/view/sdl/shared/tbtTurnList.js
 * @version     1.0
 *
 * @author      Andriy Melnik
 */
 
MFT.TBTTurnList = MFT.SDLAbstractView.create({
    
    elementId: 'tbtTurnListView',
    
    childViews: [
        'backButton',
        'captionText',
        'tbtTurnListList'
    ],

    /**
     * Activate window
     */
    activate: function() {
        this.set('active',true);
    },

    /**
     * Title Labes of window
     */
    captionText: MFT.Label.extend({
        classNames: 'caption-text',

        content:    'Turn List'
    }),

    /**
     * Turn List
     */
    tbtTurnListList: MFT.List.extend({
        elementId:  'tbtTurnListList',
        itemsOnPage:5,
        items:      []
    }),

    /**
     * Function updates content of Turn List
     * with data from application's model
     *
     * @type {Number}
     */
    updateList: function( appId ) {
        this.get('tbtTurnListList.list').removeAllChildren();

        this.tbtTurnListList.list.refresh();

        var i=0,
            turnListArray = MFT.SDLController.getApplicationModel( appId ).turnList;
            length = turnListArray.length;
        for ( i=0; i < length; i++ ) {
            this.get('tbtTurnListList.list.childViews').pushObject(
                MFT.Label.create({
                    icon:                   turnListArray[i].turnIcon.value,
                    content:                turnListArray[i].navigationText,
                    templateName:           turnListArray[i].turnIcon.value ? 'icon' : 'text',
                    classNames:             'list-item'
                })
            );
        }
    }
});
