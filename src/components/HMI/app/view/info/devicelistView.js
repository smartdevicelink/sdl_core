/**
 * @name MFT.DeviceListView
 * 
 * @desc Media App Options module visual representation
 * 
 * @category    View
 * @filesource  app/view/media/devicelistView.js
 * @version     2.0
 *
 * @author      Andriy Melnik
 */
 
MFT.DeviceListView = Em.ContainerView.create(MFT.LoadableView,{

    classNames:        ['info_apps_deviceList_view'],

	/**
     * View Id
     */	
    elementId:          'info_apps_deviceList_view',

	/**
     * View Components
     */
    childViews:         [
                            'backButton',
                            'listOfDevices',
                            'deviceListLabel'
                        ],

    /** 
     * Button to return to previous view
     */
    backButton: MFT.Button.extend({
        classNames:        ['backButton','button'],     
        action:            'turnChangeDeviceViewBack',
        target:            'MFT.ApplinkController',  
        icon:              'images/media/ico_back.png',   
    }),

    /** 
     * Label in title
     */
    deviceListLabel:    MFT.Label.extend({

        elementId:          'deviceListLabel',

        classNames:         'deviceListLabel',

        content:            'Change Devices'
    }),

    /*
     * Function calls when notification from RPC comes
     * and creates buttons to choose devices
     */
    ShowDeviceList: function( params ){

        var i,
            len = params.deviceList.length;
        for(i = 0; i < len; i++){
            this.get('listOfDevices.list.childViews').pushObject(
                MFT.Button.create({
                    deviceName:             params.deviceList[i],
                    icon:                   params.icon,
                    text:                   params.deviceList[i],
                    classNames:             'ffw-button notpressed list-item',
                    templateName:           params.icon ? 'rightIcon' : 'text',
                    action:                 'onDeviceChoosed',
                    target:                 'MFT.ApplinkController',
                    onDown:                 false
                })
            );
        }
    },

    /*
     * Function calls each time when user enters Change Device menu
     * and clear all old data about devices
     */
    clearDeviceList: function(){
        this.get('listOfDevices.list').removeAllChildren();
        this.listOfDevices.rerender();
    }.observes('this.stateObj.active'),

    /**
      * List for option on DeviceListView screen
      */
    listOfDevices: MFT.List.extend({

        elementId:      'info_apps_deviceList_list',

        itemsOnPage:    5,
                
        /** Items array */
        items:          []
    })
});