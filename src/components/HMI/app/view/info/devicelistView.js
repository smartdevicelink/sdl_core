/**
 * @name MFT.ApplinkOptionsView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/ApplinkOptionsView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.DeviceLilstView = Em.ContainerView.create(MFT.LoadableView,{

    classNames:        ['info_apps_deviceList_view'],

	/** View Id */	
    elementId:          'info_apps_deviceList_view',

	/** View Components*/
    childViews:         [
                            'backButton',
                            'listOfDevices',
                            'deviceListLabel'
                        ],

    /** Button to return to previous view*/
    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'turnChangeDeviceViewBack',
        target:            'MFT.ApplinkController',  
        icon:              'images/media/ico_back.png',   
    }),

    deviceListLabel:    MFT.Label.extend({

        elementId:          'deviceListLabel',

        classNames:         'deviceListLabel',

        content:            'Change Devices'
    }),

    ShowDeviceList: function( params ){

        this.clearDeviceList();

        if( !(params.deviceList.length == 1 && params.deviceList[0] == "") ){
            for(var i = 0; i < params.deviceList.length; i++){
                this.get('listOfDevices.list.childViews').pushObject(
                    MFT.Button.create({
                        deviceName:             params.deviceList[i],
                        icon:                   params.icon,
                        text:                   params.deviceList[i],
                        classNames:             'ffw-button notpressed list-item',
                        templateName:           params.icon ? 'rightIcon' : 'text',
                        actionUp: function(){
                            MFT.ApplinkController.onDeviceChoosed( this );
                        }
                    })
                );
            }
        }
    },

    clearDeviceList: function(){
        if( this.stateObj.active ){
            var count = this.get('listOfDevices.list.childViews').length - 1;
            for(var i = count; i>=0; i--){
                this.get('listOfDevices.list.childViews').removeObject(
                    this.get('listOfDevices.list.childViews')[0]
                );
            }
        }
    }.observes('this.stateObj.active'),

    /**
      * List for option on ApplinkOptionsView screen
      */
    listOfDevices: MFT.List.extend({

        elementId:      'info_apps_deviceList_list',

        itemsOnPage:    5,
                
        /** Items array */
        items:          new Array()
    })
});