MFT.PresetEventsCustom = Ember.Mixin.create({
    actionDown:        function(){
        this._super();
        MFT.ApplinkController.onSoftButtonActionDownCustom( this );
    },
    actionUp:        function(){
        this._super();
        MFT.ApplinkController.onSoftButtonActionUpCustom( this );
        if( this.systemAction == 'DEFAULT_ACTION' ){

            MFT.ApplinkController.defaultActionSoftButton( this );

        }else if( this.systemAction == 'STEAL_FOCUS' ){

            MFT.ApplinkController.stealFocusSoftButton( this );

        }else if( this.systemAction == 'KEEP_CONTEXT' ){

            MFT.ApplinkController.keepContextSoftButton( this );

        }
    },
});