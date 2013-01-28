/**
 * @name MFT.ApplinkNonMediaModel
 * 
 * @desc Concrate model for Applink NonMedia application
 * 
 * @category	Model
 * @filesource	app/model/applink/NonMediaModel.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */

MFT.ApplinkNonMediaModel = MFT.ApplinkAppModel.extend({
    
    init: function() {
	   
	   this._super();
        	   
	   // init properties here
	   this.set('appInfo', Em.Object.create({
                field1:         '<field1>',
                field2:         '<field2>',
                field3:         '<field3>',
                field4:         '<field4>',
                image:			''
            })
        );
        
        this.set('commandsList',[]);
	},
	
	
	/**
	 * Activate current application model
	 */
    turnOnApplink: function(){
        MFT.NonMediaController.turnOnApplink( this );
    },
    
    /** 
     * Applin UI Show handler
     * @param {Object}
     */
    onApplinkUIShow: function(params){
        this.appInfo.set('field1',        params.mainField1);
        this.appInfo.set('field2',        params.mainField2);
        this.appInfo.set('field3',        params.mainField3);
        this.appInfo.set('field4',        params.mainField4);
        //this.appInfo.set('image',         params.graphic);
    },
    
    /**
     * Add command to list
     *
     * @param {Object}
     */
    onApplinkAddCommand: function( params ) {
        
        this.get('commandsList').pushObject({
            commandId:  params.cmdId,
            name:       params.menuParams.menuName,
            parent:     params.menuParams.parentID,
            position:   params.menuParams.position
        });
    },
    
    /**
     * Delete command from list
     *
     * @param {Number}
     */
    onApplinkOptionsDeleteCommand: function(commandId){
                
        this.get('commandsList').removeObjects(
            this.get('commandsList').filterProperty('commandId',commandId)
        );
    },
    
    /**
     * Add submenu to commands list
     *
     * @param {Object}
     */
    onApplinkAddSubMenu: function( params ){        
        
        this.get('commandsList').pushObject({
            menuId:     params.menuId,
            name:       params.menuName,
            parent:     0,
            position:   params.position
        });
    },
    
    /**
     * Delete submenu and related commands from list
     *
     * @param {Number}
     */
    onApplinkDeleteSubMenu: function( menuId ){
        
        // remove submenu
        this.get('commandsList').removeObjects(
            this.get('commandsList').filterProperty('menuId',menuId)
        );
        
        // remove commands from deleted submenu
        this.get('commandsList').removeObjects(
            this.get('commandsList').filterProperty('parent',menuId)
        );
        
        // return to root commands list if necessary
        if( this.get('currentSubMenuId') == menuId ){
            MFT.ApplinkAppController.onSubMenu(0);
        }

        return "SUCCESS";
    }
});