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

    afterRender: function(){
        this.ShowDeviceList();
    },

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

    ShowDeviceList: function( ){

        this.listOfDevices.items = MFT.ApplinkModel.devicesList.slice();

        this.listOfDevices.list.refresh();

    },

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