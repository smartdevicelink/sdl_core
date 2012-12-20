/**
 * @name MFT.ApplinkAbstractView
 * 
 * @desc Applink application abstract shared view
 * 
 * @category	View
 * @filesource	app/view/applink/shared/AbstractView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.ApplinkAbstractView = Em.ContainerView.extend({
	
	
	classNames: [
		'applink-window'
	],
	
	classNameBindings: [
		'active:active_state:inactive_state'
	],
	
	active: false,
    
    /**
     * Activate preform interaction window
     * and set caption text
     *
     * @param text: String
     */
    activate: function( text ) {
    	if (text) {
    		this.set('captionText.content',text);
    	}
    	this.set('active',true);
    },
    
    /**
     * Activate window
     */
    deactivate: function() {
    	this.set('active',false);
    },
    
	backButton: MFT.Button.extend({
		classNames:	[
			'back-button'
		],
		target:		'this.parentView',
		action:		'deactivate',
		icon:		'images/media/ico_back.png',
	}),
	
	captionText: MFT.Label.extend({
        classNames:	[
        	'caption-text'
        ],
        
        content:	'Caption Text'
    })
});