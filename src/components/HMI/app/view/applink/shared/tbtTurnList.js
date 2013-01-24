/**
 * @name MFT.TBTTurnList
 * 
 * @desc TBT Turn List visual representation
 * 
 * @category    View
 * @filesource  app/view/applink/shared/tbtTurnList.js
 * @version     1.0
 *
 * @author      Andriy Melnik
 */
 
MFT.TBTTurnList = MFT.ApplinkAbstractView.create({
    
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

        var i=0,
            turnListArray = MFT.ApplinkController.getApplicationModel( 65536 ).turnList;
            length = turnListArray.length;
        for ( i=0; i < length; i++ ) {
            this.get('tbtTurnListList.list.childViews').pushObject(
                MFT.Label.create({
                    icon:                   turnListArray[i].turnIcon,
                    content:                turnListArray[i].navigationText,
                    templateName:           turnListArray[i].turnIcon ? 'icon' : 'text',
                    classNames:             'list-item'
                })
            );
        }
    }
});
