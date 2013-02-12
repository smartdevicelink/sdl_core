MFT.PresetEvents = Ember.Mixin.create({
    actionDown:     function(){
        this._super();
        MFT.ApplinkController.onSoftButtonActionDown( this.presetName, this);
    },
    actionUp:       function(){
        this._super();
        MFT.ApplinkController.onSoftButtonActionUp( this.presetName, this);
    }
});