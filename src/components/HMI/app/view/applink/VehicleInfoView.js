/**
 * @name MFT.VehicleInfo
 * 
 * @desc VehicleInfo module visual representation
 * 
 * @category    View
 * @filesource  app/view/home/VehicleInfo.js
 * @version     2.0
 *
 * @author      Andriy Melnyk
 */

MFT.VehicleInfo = Em.ContainerView.create({

    elementId:          'VehicleInfo',

    classNames:         'VehicleInfo',

    classNameBindings:      ['active'],

    childViews: [
        'prndl',
        'vehicleInfoLabel',
        'prndlSelect',
        'ecu1Title',
        'ecu1',
        'ecu2',
        'ecu1Data',
        'ecu2Data'

    ],

    /*
     * Label with stored data in VehicleInfo model
     */ 
    ecu2Data:    MFT.Label.extend({

        elementId:      'ecu2Data',

        classNames:     'ecu2Data',

        contentBinding:        'MFT.ApplinkVehicleInfoModel.ecuDIDData.1.data'
    }),

    /*
     * Label with stored data in VehicleInfo model
     */ 
    ecu1Data:    MFT.Label.extend({

        elementId:      'ecu1Data',

        classNames:     'ecu1Data',

        contentBinding:        'MFT.ApplinkVehicleInfoModel.ecuDIDData.0.data'
    }),

    /*
     * Label with name of some parameter
     * stored data in VehicleInfo model
     */ 
    ecu2:    MFT.Label.extend({

        elementId:      'ecu2',

        classNames:     'ecu2',

        content:        'ECU 2:'
    }),

    /*
     * Label with name of some parameter
     * stored data in VehicleInfo model
     */ 
    ecu1:    MFT.Label.extend({

        elementId:      'ecu1',

        classNames:     'ecu1',

        content:        'ECU 1:'
    }),

    /*
     * Title of ecu group of parameters
     * stored data in VehicleInfo model
     */ 
    ecu1Title:    MFT.Label.extend({

        elementId:      'ecu1Title',

        classNames:     'ecu1Title',

        content:        'ECU'
    }),

    /*
     * Title of VehicleInfo PopUp view
     */ 
    vehicleInfoLabel:    MFT.Label.extend({

        elementId:      'vehicleInfoLabel',

        classNames:     'vehicleInfoLabel',

        content:        'Vehicle Information'
    }),


    /*
     * Property indicates the activity state of VehicleInfo PopUp
     */
    active:           false,

    /*
     * Title of prndl group of parameters
     * stored in VehicleInfo model
     */ 
    prndl : MFT.Label.extend({

        elementId:          'prndl',

        classNames:         'prndl',

        content:            'PRNDL'
    }),

    /*
     * HMI element Select with parameters of transmission state
     * from VehicleInfo Model
     */ 
    prndlSelect : Ember.Select.extend({

        elementId:          'prndlSelect',

        classNames:         'prndlSelect',

        contentBinding:     'MFT.ApplinkVehicleInfoModel.vehicleInfoPRNDL',

        optionValuePath:    'content.id',

        optionLabelPath:    'content.name',

        selected:           function( element ){
            var id = element.selection.id;
            switch(id){
                case 0: 
                    MFT.ApplinkVehicleInfoModel.set('vehicleData.prndl', MFT.ApplinkVehicleInfoModel.vehicleInfoPRNDL[id].name);
                    break;
                case 1: 
                    MFT.ApplinkVehicleInfoModel.set('vehicleData.prndl', MFT.ApplinkVehicleInfoModel.vehicleInfoPRNDL[id].name);
                    break;
                case 2: 
                    MFT.ApplinkVehicleInfoModel.set('vehicleData.prndl', MFT.ApplinkVehicleInfoModel.vehicleInfoPRNDL[id].name);
                    break;
                case 3: 
                    MFT.ApplinkVehicleInfoModel.set('vehicleData.prndl', MFT.ApplinkVehicleInfoModel.vehicleInfoPRNDL[id].name);
                    break;
                case 4: 
                    MFT.ApplinkVehicleInfoModel.set('vehicleData.prndl', MFT.ApplinkVehicleInfoModel.vehicleInfoPRNDL[id].name);
                    break;
            }
        }.observes('selection')
    }),

    /*
     * Trigger function that activates and deactivates VehicleInfo PopUp
     */
    toggleActivity: function(){
        this.set('active', !this.active);
    }
});