/**
 * @name MFT.Vehicle
 *
 * @desc Vehicle models
 *
 * @category	Model
 * @filesource	app/model/vehicle/vehicle.js
 *
 * @author		Andrii Velychko
 */

MFT.Vehicle = Ember.Object.extend( {
    /** full by default **/
    climateType: "full",

    /** false by default **/
    isClimateDisabled: false,

    /** true by default **/
    isRearClimateDisabled: true
});

MFT.vehicles = Ember.Object.create({
    edge: MFT.Vehicle.create(),

    explorer: MFT.Vehicle.create({
        isRearClimateDisabled: false
    }),

    escape_2013: MFT.Vehicle.create({
        climateType: "simple"
    }),

    f150_2013: MFT.Vehicle.create(),

    flex_2013: MFT.Vehicle.create({
        isRearClimateDisabled: false
    }),

    focus:  MFT.Vehicle.create({
        climateType: "simple"
    }),

    fusion_2013:  MFT.Vehicle.create({
        climateType: "fusion"
    }),

    taurus_2013: MFT.Vehicle.create(),

    fiesta:  MFT.Vehicle.create({
        isClimateDisabled: true
    })
});