/**
 * @name MFT.MenuList
 * 
 * @desc
 * 
 * @category	Controlls
 * @filesource	app/controlls/MenuList.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.MenuList = Em.ContainerView.extend({

	/**
	 * Add new item to container
	 *
	 * @param buttons: SoftButton[]
	 */
	addItems: function( buttons, appId ) {

		this.deleteItems();

		if(buttons){
			for(var i=0; i<buttons.length; i++){
				this.get('content.childViews').pushObject( 
					MFT.Button.create(MFT.PresetEventsCustom, {
						text:			buttons[i].text,
						icon:			buttons[i].image,
						softButtonId:	buttons[i].softButtonID,
                        systemAction:   buttons[i].systemAction,
                        groupName:      this.groupName,
                        appId:          appId
					})
				);
			}
		}
	},
	
	/**
	 * Delete existing Soft Buttons from container
	 */
	deleteItems: function() {
	   
	   this.get('content.childViews').removeObjects(
	       this.get('content.childViews').filterProperty('softButtonId')
	   );
	},

	classNames: ['ffw_list_menu'],
	
	attributeBindings: ['elementHeight:style'],
	
	// Position of current page
	page:	0,
	
	// Items per one page
	itemsOnPage: 5,
	
	// Height of one item
	ITEM_HEIGHT: 50,
	
	pageHeight: function() {
		return this.itemsOnPage*this.ITEM_HEIGHT;
	}.property('this.itemsOnPage'),
	
	elementHeight: function() {
		return 'height:' + String( this.get('pageHeight') + this.ITEM_HEIGHT - 2 ) + 'px;';
	}.property('this.pageHeight'),
	
	// Position of content block
	contentPositon: function() {
		return 'top:' + String( -( this.get('page') * this.get('pageHeight') ) ) + 
		'px; height:' + String( this.get('pageHeight') - (Boolean(this.get('onLastPage') && this.get('page')) * this.ITEM_HEIGHT) ) + 'px;';
	}.property('onLastPage'),
	
	// Property for show or hide 'down' button
	onLastPage: function() {
		return ( ( this.get('page')  * this.itemsOnPage ) < ( this.get('content.childViews.length') - this.itemsOnPage ) );
	}.property(
		'page',
		'content.childViews.length'
	),
	
	// Handeler to go previous page
	pageUp: function() {
		if ( this.get('page') ) {
			this.set( 'page', ( this.get('page') - 1 ) );
		}
	},
	
	// Handeler to go next page
	pageDown: function() {
		if ( this.get('onLastPage') ) {
			this.set( 'page', ( this.get('page') + 1) );
		}				
	},
	
	childViews: [
		'upButton',
		'content',
		'downButton'
	],
	
	upButton: MFT.Button.extend({
		
		classNames:	['control','up_button'],
		
		hidden: function() {
			if ( this.get('parentView.page') ) {
				return false;
			} else {
				return true;
			}
		}.property('parentView.page'),
		
		
		templateName: 'icon',
		
		icon:	'images/media/ico_arrow_up.png',
		
		action:	'pageUp',
		
		target:	'parentView'				
	}),
	
	downButton: MFT.Button.extend({
		
		classNames:	['control','down_button'],
		
		hidden: function() {
			if ( this.get('parentView.onLastPage') ) {
				return false;
			} else {
				return true;
			}
		}.property('parentView.onLastPage'),
		
		templateName: 'icon',
		
		icon:	'images/media/ico_arrow_down.png',
		
		action:	'pageDown',
		
		target:	'parentView'				
	}),
	
	content: Em.ContainerView.extend({
		
		classNames: ['content'],
		
		attributeBindings: ['parentView.contentPositon:style']
	})
});