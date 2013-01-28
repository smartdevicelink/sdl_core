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
	 * Add command to Options list
	 *
     * @param {Number} commandId
     * @param {Object} params
	 */
    onApplinkOptionsAddCommand: function( commandId, params ){
        //this.get('commandsList').pushObject({id:commandId, params:params});
        //MFT.InfoNonMediaOptions.commands.AddCommand( commandId, params );

    },
    
    /**
     * Add command to list
     *
     * @param {Object}
     */
    onApplinkAddCommand: function( params ) {
        
        this.get('commandsList').pushObject({
            id:         params.cmdId,
            name:       params.menuParams.menuName,
            parent:     params.menuParams.parentID,
            position:   params.menuParams.position
        });
        
        /*
        if( params.menuParams.parentID ) {
        	this.subMenuCommands.push( params );
            
            if( MFT.States.info.nonMedia.options.subMenu.active ) {
                MFT.InfoNonMediaOptionsSubMenu.SubMenuActivate( this.currentSubMenuId );
            }
        
        } else {
        	this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams);
        }
        */
    },
    
    /**
     * Delete command from list
     *
     * @param {Number}
     */
    onApplinkOptionsDeleteCommand: function(commandId){
                
        this.get('commandsList').removeObjects(
            this.get('commandsList').filterProperty('id',commandId)
        );
        
        /*
        MFT.InfoNonMediaOptions.commands.DeleteCommand( commandId );

        var  count = this.subMenuCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(this.subMenuCommands[i].cmdId == commandId){
               this.subMenuCommands.splice(i, 1);
            }
        }
        */
    },
    
    /** Add subMenu button to Options list */
    onApplinkAddSubMenu: function( menuId, params ){
        
        return;
        
        MFT.InfoNonMediaOptions.commands.AddSubMenu( menuId, params );

    },
    
    /** Delete subMenu button from Options list */
    onApplinkDeleteSubMenu: function( menuId ){
        
        return;
        
        if( MFT.NonMediaController.currentApplinkSubMenuid == menuId ){
            MFT.States.back();
        }

        MFT.InfoNonMediaOptions.commands.DeleteSubMenu( menuId );

        return "SUCCESS";
    }
});