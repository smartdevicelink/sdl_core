MFT.PresetEvents = Ember.Mixin.create({
    actionDown:     function(){
        this._super();
        MFT.SDLController.onSoftButtonActionDown( this.presetName, this);
    },
    actionUp:       function(){
        this._super();
        MFT.SDLController.onSoftButtonActionUp( this.presetName, this);
    }
});