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
                trackIcon:      'images/applink/audio_icon.jpg',
                image:			'',
                customPresets:  []
            })
        );

        //this.set('appIcon', 'images/info/info_leftMenu_apps_ico.png'),
        
        this.set('commandsList',[]);
        this.set('softButtons',[]);
	},

    /**
     * Notification of deactivation of current application model
     * initiated in StateManager
     */
    deactivateApp: function(){

        MFT.ApplinkModel.onDeactivateApp( MFT.TransitionIterator.finalPath, this.appId, this.appName );

    },

    /**
     * Method hides applink activation button
     * @param {Number}
     */
    onDeleteApplication: function( appId ){
        if( MFT.NonMediaController.currentAppId == appId ){
            if( MFT.InfoNonMedia.stateObj.active ){
                MFT.States.goToState('info.apps');
            }
            MFT.NonMediaController.set('currentAppId', 0);
        }
    },

	/**
	 * Activate current application model
	 */
    turnOnApplink: function(){
        MFT.NonMediaController.activateApp( this );
    },

    /**
     * Method to clear App OverLay
     */
    clearAppOverLay: function(){

        clearInterval(this.timer);
        this.appInfo.set('field1',        '');
        this.appInfo.set('field2',        '');
        this.appInfo.set('field3',        '');
        this.appInfo.set('field4',        '');
        this.appInfo.set('alignment',     '');
        this.appInfo.set('trackIcon', 'images/applink/audio_icon.jpg');
        this.updateSoftButtons();
        for(i=0; i< 6; i++){
            this.appInfo.set('customPresets.' + i, '');
        }

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
        if( params.graphic ){
            this.appInfo.set('trackIcon', params.graphic);
        }else{
            this.appInfo.set('trackIcon', 'images/applink/audio_icon.jpg');
        }
        
        if ( params.customPresets ) {
            this.appInfo.set('customPresets', params.customPresets);
        }
        
        // Get soft buttons if exists
        if ( params.softButtons ) {
            this.updateSoftButtons( params.softButtons );
        }
    },
    
	applinkSetMediaClockTimer: function() {
	   return;
	}
});