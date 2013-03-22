MFT.PresetEvents = Ember.Mixin.create({
    actionDown:     function(){
        this._super();
        MFT.SDLController.onSoftButtonActionDown( this);
    },
    actionUp:       function(){
        this._super();
        MFT.SDLController.onSoftButtonActionUp( this);
    }
});