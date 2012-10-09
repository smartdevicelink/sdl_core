/**
 * @name 		MFT.ListItemButton
 * 
 * @desc 		Button component for list component
 * 
 * @category	Controlls
 * @filesource	app/controlls/ListItemButton.js
 * @version		2.0
 *
 * @author		Maksym Gerashchenko
 */
 
MFT.List.ListItemButton = MFT.Button.extend({
	/** Binding object class which define active status for each item */
	classNameBindings: 	['pressed', 'record.isDisabled:disabled'],
	
	/** Path to video record*/
	videoPath: null,
	
	/** Item reccord*/
	record: 			 null,
	
	/** Define tag of object */
	tagName: 			'li',
	
	/** Event: mousedown for list item */
	actionDown: function(event) {
		if( !this.record.isDisabled ){ 
			this.set('pressed',true);		
		}
	},
	
	/** Event: mouseup for list item  */
	actionUp: function(event) {
		if( !this.record.isDisabled ){
			this.set('pressed',false);
			this.triggerAction();
		}
	},
	
	/** Define componant template*/
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#if record.icon}}'+
				'<img class="ico" {{bindAttr src="record.icon"}}/>'+
			'{{/if}}'+
			'<section class="label"> {{record.label}} </section>'+
			'{{#if record.postlabel}}'+
				'<section class="postlabel"> {{record.postlabel}} </section>'+
			'{{/if}}'+
			'{{#if record.posticon}}'+
				'<img class="postico" {{bindAttr src="record.posticon"}}/>'+
			'{{/if}}'+
			'{{#if record.toggleStatus}}'+
				'{{view MFT.TogButton classNames="button toggleTwo" textBinding="record.toggleTwo" disabled=true}}'+
				'{{view MFT.TogButton classNames="button toggleOne" textBinding="record.toggleOne" disabled=true}}'+
			'{{/if}}'+
		'{{/with}}'
	)
});