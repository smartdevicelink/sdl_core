MFT.Dragable = Ember.Mixin.create({

    dragEnter: function(event)	{
									event.preventDefault();
									return false;
								},
    dragOver: function(event)	{
									event.preventDefault();
									return false;
								},
    drop: function(event)		{
									MFT.MCSController.set('mcsDropId', parseInt($("#" + this.get('elementId') ).attr('position')));
									MFT.MCSController.stopDrag();
									event.preventDefault();
									return false;
								},
    attributeBindings: 'draggable',
    draggable: 'true',
    dragStart: function(event) {
		MFT.MCSController.set('mcsDragId', parseInt($("#" + this.get('elementId') ).attr('position')));
        var dataTransfer = event.originalEvent.dataTransfer;
        dataTransfer.setData('Text', this.get('elementId'));
    }
});