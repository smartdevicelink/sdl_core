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

MFT.ApplinkNonMediaModel = MFT.ApplinkAppModel.create({

    /**
     * Info data
     *
     * @type: Em.Object
     */
    appInfo: Em.Object.create({
    	appName:        '<App name>',
        field1:         '<field1>',
        field2:         '<field2>',
        field3:         '<field3>',
        field4:         '<field4>',
        statusBar:      '<statusBar',
        image:			''
    }),
    
    /** Applin UI Show handler */
    onApplinkUIShow: function(params){
        this.appInfo.set('field1',        params.mainField1);
        this.appInfo.set('field2',        params.mainField2);
        this.appInfo.set('field3',        params.mainField3);
        this.appInfo.set('field4',        params.mainField4);
        //this.appInfo.set('image',         params.graphic);
    },
	
	/** Add command to Options list */
    onApplinkOptionsAddCommand: function( commandId, params ){

       MFT.InfoNonMediaOptions.commands.AddCommand( commandId, params );

    },
    
    /** Applink AddCommand handler */
    onApplinkAddCommand: function( params ) {
        if( params.menuParams.parentID ) {
        	this.subMenuCommands.push( params );
            
            if( MFT.States.info.nonMedia.options.subMenu.active ) {
                MFT.InfoNonMediaOptionsSubMenu.SubMenuActivate( this.currentSubMenuId );
            }
        
        } else {
        	this.onApplinkOptionsAddCommand(params.cmdId, params.menuParams);
        }
    },
    
    /** Delete command to Options list */
    onApplinkOptionsDeleteCommand: function(commandId){

        MFT.InfoNonMediaOptions.commands.DeleteCommand( commandId );

        var  count = this.subMenuCommands.length;
        for(var i = count-1; i >= 0; i--){
            if(this.subMenuCommands[i].cmdId == commandId){
               this.subMenuCommands.splice(i, 1);
            }
        }
    },
    
    /** Add subMenu button to Options list */
    onApplinkAddSubMenu: function( menuId, params ){

        MFT.InfoNonMediaOptions.commands.AddSubMenu( menuId, params );

    },
    
    /** Delete subMenu button from Options list */
    onApplinkDeleteSubMenu: function( menuId ){

        if( MFT.NonMediaController.currentApplinkSubMenuid == menuId ){
            MFT.States.back();
        }

        MFT.InfoNonMediaOptions.commands.DeleteSubMenu( menuId );

        return "SUCCESS";
    },
    
    /** Applink Setter for Media Clock Timer */
    applinkSetMediaClockTimer: function(params){
		return;
    },
    
    /** Applink Slider activation */
    onApplinkSlider: function(params){
    	return;
    }
});