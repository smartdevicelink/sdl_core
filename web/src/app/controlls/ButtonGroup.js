


MFT.ButtonGroup = Em.ContainerView.extend({
	
	afterRender: function() {
		
			var items = this.items;	
						
			for( i = 0; i < items.length; i++ ) {
		
				element = MFT.Button.create({
					// element id
					elementId:	this._parentView.elementId+'_button'+i,
					
					classNameBindings:['active'],
					
					activeIndex: i,
					
					action: this.action,
					
					target: this.target,
					
					// Dynamic property set
					init: function() {
						for( key in items[i].params ) {
							if(key.match('Binding') != null){			
								binding = Ember.Binding.from(items[i].params[key]).to(key.replace('Binding',''));
								binding.connect(this);
								//Set one way binding
								binding.oneWay();
							}else{
								this.set(key, items[i].params[key]);
							}
						}	
						this._super();
						// synchronize bindings
						Ember.run.sync();
					},
					
					actionDown: function(event) {
						if(this.activeIndex == this._parentView.selectedIndex){
							return;
						}
						//remove active state from prev tab
						this._parentView.get('childViews').filterProperty('active', true).get('firstObject').set('active',false);
						//set active state to current tab
						this._parentView.set('selectedIndex',this.activeIndex);
						
						this._super();
					}
					
					
				});
				
				// Push element 
				this.get('childViews').pushObject(element);
			}
	},
	
	// INIT PHONE BOOK
	didInsertElement: function(){
		this._super();
		this.set('selectedIndex',this.activeIndex);
		
	},
	
	
	setActiveButton: function(){
		this.get('childViews')[this.selectedIndex].set('active',true);
	}.observes('this.selectedIndex')
	

});