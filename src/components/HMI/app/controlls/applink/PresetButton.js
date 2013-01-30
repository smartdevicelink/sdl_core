/**
 * @name MFT.Button
 * 
 * @desc Universal button component for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/Button.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.ApplinkPresetButton = MFT.Button.extend({
    actionDown: function(){
        this._super();
        
        FFW.Buttons.buttonEvent( this.presetName, "BUTTONDOWN");
        var self = this;
        this.time = 0;
        setTimeout(function(){ self.time ++; }, 1000);
    },
    
    actionUp: function(){
        this._super();
        
        FFW.Buttons.buttonEvent( this.presetName, "BUTTONUP");
        
        if(this.time > 0){
            FFW.Buttons.buttonPressed( this.presetName, "LONG");
        }else{
            FFW.Buttons.buttonPressed( this.presetName, "SHORT");
        }
        this.time = 0;
    }
});