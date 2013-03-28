MFT.PresetEventsCustom = Ember.Mixin.create({
    actionDown:        function(){
        this._super();
        MFT.SDLController.onSoftButtonActionDownCustom( this );
    },
    actionUp:        function(){
        this._super();
        MFT.SDLController.onSoftButtonActionUpCustom( this );
        if( this.systemAction == 'DEFAULT_ACTION' ){

            MFT.SDLController.defaultActionSoftButton( this );

        }else if( this.systemAction == 'STEAL_FOCUS' ){

            MFT.SDLController.stealFocusSoftButton( this );

        }else if( this.systemAction == 'KEEP_CONTEXT' ){

            MFT.SDLController.keepContextSoftButton( this );

        }
    },
});