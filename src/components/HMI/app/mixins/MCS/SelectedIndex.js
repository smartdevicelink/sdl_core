MFT.SelectedIndex = Em.Mixin.create({
	
	controller: MFT.MCSController,

	isSelected: function() {
		
		if(null != MFT.MCSController.selectedIndex)
		{
			// Group select
			if ( this.controller.selectedIndex.length) {
			
				if( this.controller.selectedIndex.contains(this.index) ) {
					return MFT.MCSController.highlighted;
				} else {
					return false;
				}
			}
			
			if ( this.controller.selectedIndex == this.index ) {
				return MFT.MCSController.highlighted;
			} else {
				return false;
			}
		}else {
			return false;
		}
	}.property('this.controller.selectedIndex', 'this.controller.highlighted')
});