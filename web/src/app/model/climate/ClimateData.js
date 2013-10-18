/**
 * @name MFT.ClimateData
 * 
 * @desc Climate data model
 * 
 * @category Model
 * @filesource app/model/ClimateData.js
 * @version  2.0
 *
 * @author  Artem Petrosyan
 */

MFT.ClimateData = Em.Object.create({
    
    // NORMAL CONTROLLS
    on:                     null,
    
    driverTemp:             null,
    myTemp:                 null,
    driverHeatedSeat:       null,
    driverCooledSeat:       null,
    driverHeatedSteer:      null,
    
    passangerTemp:          null,
    passangerHeatedSeat:    null,
    passangerCooledSeat:    null,
    
    dualMode:               null,
    
    defrostAirflow:         null,
    panelAirflow:           null,
    feetAirflow:            null,
    
    defrost:                null, 
    maxDefrost:             null,
    rearDefrost:            null,
    
    ac:                     null,
    maxAc:                  null,
    recirculation:          null,
    
    fan:                    null,
    
    // REAR CONTROLLS
    rearOn:                 null,
    rearControl:            null,
    
    rearTemp:               null,
    rearFan:                null,
    
    // store for previous states
    _beforeMaxAc:           {},
    _beforeMaxDefrost:      {},
    _beforeTemp:            {},
    
    
    /**
     * @constructor
     */
    init: function() {
        this.set('on',                     MFT.RangedValue.create( { range: 1 } ) );
        this.set('driverTemp',             MFT.ClimateTemperature.create() );
        this.set('myTemp',                 MFT.ClimateTemperature.create() );
        this.set('driverHeatedSeat',       MFT.RangedValue.create() );
        this.set('driverCooledSeat',       MFT.RangedValue.create() );
        this.set('driverHeatedSteer',      MFT.RangedValue.create( { range: 1 } ) );
        this.set('passangerTemp',          MFT.ClimateTemperature.create() );
        this.set('passangerHeatedSeat',    MFT.RangedValue.create() );
        this.set('passangerCooledSeat',    MFT.RangedValue.create() );
        this.set('dualMode',               MFT.RangedValue.create( { range: 1 } ) );
        this.set('defrostAirflow',         MFT.RangedValue.create( { range: 1 } ) );
        this.set('panelAirflow',           MFT.RangedValue.create( { range: 1 } ) );
        this.set('feetAirflow',            MFT.RangedValue.create( { range: 1 } ) );
        this.set('defrost',                MFT.RangedValue.create( { range: 1 } ) );
        this.set('maxDefrost',             MFT.RangedValue.create( { range: 1 } ) );
        this.set('rearDefrost',            MFT.RangedValue.create( { range: 1 } ) );
        this.set('ac',                     MFT.RangedValue.create( { range: 1 } ) );
        this.set('maxAc',                  MFT.RangedValue.create( { range: 1 } ) );
        this.set('recirculation',          MFT.RangedValue.create( { range: 1 } ) );
        this.set('fan',                    MFT.RangedValue.create( { range: 7, cycle: false } ) );
        this.set('rearOn',                 MFT.RangedValue.create( { range: 1 } ) );
        this.set('rearControl',            MFT.RangedValue.create( { range: 1 } ) );
        this.set('rearTemp',               MFT.RangedValue.create( { range: 9, cycle: false } ) );
        this.set('rearFan',                MFT.RangedValue.create( { range: 7, cycle: false } ) );
    }
});