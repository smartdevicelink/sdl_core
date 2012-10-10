MFT.Dropable = Ember.Mixin.create({

    dragEnter: function(event)	{
									event.preventDefault();
									return false;
								},
    dragOver: function(event)	{
									event.preventDefault();
									return false;
								},
    drop: function(event) {
        MFT.MCSController.dropToTrash();   
        event.preventDefault();
        return false;
    },
    attributeBindings: 'draggable',
    draggable: 'false',
    dragStart: function(event)	{
									event.preventDefault();
									return false;
								}
});