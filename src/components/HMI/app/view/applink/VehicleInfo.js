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
        'prndlSelect'

    ],

    vehicleInfoLabel:    MFT.Label.extend({

        elementId:      'vehicleInfoLabel',

        classNames:     'vehicleInfoLabel',

        content:        'Vehicle Information'
    }),

    active:           false,

    prndl : MFT.Label.extend({

        elementId:          'prndl',

        classNames:         'prndl',

        content:            'PRNDL'
    }),

    prndlSelect : Ember.Select.extend({

        elementId:          'prndlSelect',

        classNames:         'prndlSelect',

        content:            MFT.ApplinkModel.vehicleInfoPRNDL,

        selected:           function( element ){
            switch(element.selection){
                case "Parking": 
                    MFT.ApplinkModel.set('vehicleData.8.data', "PARK");
                    break;
                case "Reverse gear": 
                    MFT.ApplinkModel.set('vehicleData.8.data', "REVERSE");
                    break;
                case "No gear": 
                    MFT.ApplinkModel.set('vehicleData.8.data', "NEUTRAL");
                    break;
                case "Forward drive": 
                    MFT.ApplinkModel.set('vehicleData.8.data', "FORWARD_DRIVE_2");
                    break;
                case "1st gear hold": 
                    MFT.ApplinkModel.set('vehicleData.8.data', "LOWGEAR");
                    break;
            }
        }.observes('selection')
    }),

    activate: function(){
        if(this.active){
            this.set('active', false);
        }else{
            this.set('active', true);
        }
    }
});