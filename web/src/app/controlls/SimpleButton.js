/**
 * @name MFT.Button
 * 
 * @desc Universal button component for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/Button.js
 * @version		2.0
 *
 * @author		Artem Petrosyan
 */

MFT.SimpleButton = FFW.AbstractButton.extend(Em.TargetActionSupport, {
	
	classNameBindings: [
		'pressed:pressed:notpressed',
		'disabled'
	],
	
	// component content text
	// @string
	text: '',
	
	// trigger action on down event
	// default behevior
	// @boolean
	onDown: true,
	
	// override
	actionDown: function() {
		if ( this.disabled ) {
			return;
		}
		
		// default trigger action
		if ( this.onDown ) {
			this.triggerAction();
		}
		
		this._super();
	},
	
	// override
	actionUp: function() {
		this.set('pressed', false);
		
		// trigger action onUp event
		if ( !this.onDown ) {
			this.triggerAction();
		}
	},
			
	// component template
	defaultTemplate: Em.Handlebars.compile(
		'<img class="ico" {{bindAttr src="view.icon"}} />'+
		'<span>{{view.text}}</span>'
	),
	
	templates: {
		rightText: Em.Handlebars.compile(
			'<img class="ico" {{bindAttr src="view.icon"}} />'+
			'<span>{{view.text}}</span>'+
			'<span class="right_text">{{view.rightText}}</span>'
		)
	}
});