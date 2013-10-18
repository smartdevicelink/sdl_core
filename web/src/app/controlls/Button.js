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

MFT.Button = Em.View.extend(Ember.TargetActionSupport, {
	classNames: ['ffw-button','notpressed'],
	
	classNameBindings: ['pressed','disabled','hidden'],
	
	/** Pressed state binding */
	pressed: false,
	
	/** Disable actions on button */
	disabled: false,
	
	/** Hide button from layout */
	hidden: false,

    /** Event stop propagation **/
    stopPropagation: false,
	
	/** Button icon class */
	icon: null,
	
	/** Button text */
	text: null,
	
	rightText:	null,
	
	target:		this.target ? this.target : this,
	
	/** Arrow icon */
	arrow: false,
	
	/** Button timer flag */
	timer:0,
	
	timerId: null,
	/** Touch leave event flag */
	touchleave: false,
	
	onDown: true,
	
	helpMode: false,
	/**  */
	targetElement: null,
	
	actionDown: function(event) {
		if ( this.get('disabled') ) {
			return;
		}
		
		var self = this;
		
		this.set('pressed',true);
		/** Set Mouse Leave Event Flag to false*/
		this.set('mouseleave', false);
		
		/**  
		  * Only for IOS
		  * Simulation of mouseleave event for touch devices
		  * Save reference to element on which touchstart event triggered  
		  */
		if(FFW.isIOS){
			if(MFT.helpMode && event && event.originalEvent.touches[0]){
				this.set('targetElement', document.elementFromPoint(event.originalEvent.touches[0].pageX,event.originalEvent.touches[0].pageY));
			}
		}
		/*
		 * Not trigger action when help mode active,
		 * only for helpMode buttons
		 */
		if ( this.helpMode && MFT.helpMode ) {
			return;
		}
		
		// Default trigger action
		if ( this.onDown ) {
			this.triggerAction();
		}
		
		// Call trigger with timeout
		if ( this.timer ) {
			clearInterval( this.timerId );
			this.timerId = setInterval(function(){
				self.triggerAction();
			},this.timer);
		}

        if (this.stopPropagation) {
            event.stopPropagation();
        }
	},
	
	actionUp: function(event) {
		this.set('pressed',false);
		/**  
		  * Only for IOS
		  * Checks the elemnt coordinate on touch start event 
		  */
		if(FFW.isIOS && event && event.originalEvent.changedTouches[0] && MFT.helpMode){
	    	this.set('touchleave', this.targetElement !== document.elementFromPoint(event.originalEvent.changedTouches[0].pageX,event.originalEvent.changedTouches[0].pageY));
		}
		
		if ( this.timer) {
			clearInterval(this.timerId);
		}
		
		if ( this.get('disabled') ) {
			return;
		}
		/**  
		  * Only for IOS
		  * if event element differ from touchstart event element abort trigger action on this element
		  */
		if (this.touchleave) {
			this.set('touchleave', false);
			return;
		}
		/*
		 * Not trigger action when help mode active,
		 * only for helpMode buttons
		 */					
		if ( this.helpMode && MFT.helpMode ) {
			return;
		}
		
		if ( !this.onDown) {
			this.triggerAction();
		}

        if (this.stopPropagation) {
            event.stopPropagation();
        }
	},
	
	/** Only for desktop */
	mouseLeave: function(event) {
		this.set('pressed',false);
								
		if ( this.timer) {
			clearInterval(this.timerId);
		}

        if (this.stopPropagation) {
            event.stopPropagation();
        }
	},
	
	// component default template
	defaultTemplate: Em.Handlebars.compile(
		'<img class="ico" {{bindAttr src="view.icon"}} />'+
		'<span>{{view.text}}</span>'
	),
	
	templates: {
		text: Em.Handlebars.compile(
			'<span>{{view.text}}</span>'
		),
		
		rightText: Em.Handlebars.compile(
			'<img class="ico" {{bindAttr src="view.icon"}} />'+
			'<span>{{view.text}}</span>'+
			'<span class="right_text">{{view.rightText}}</span>'
		),
		
		arrow: Em.Handlebars.compile(
			'<img class="ico" {{bindAttr src="view.icon"}} />'+
			'<span>{{view.text}}</span>'+
			'<img class="arrow-ico" src="images/common/arrow_ico.png" />'
		),
		
		rightIcon: Em.Handlebars.compile(
			'<img class="ico" {{bindAttr src="view.icon"}} />'+
			'<span>{{view.text}}</span>'+
			'<img class="right_ico" {{bindAttr src="view.righticon"}} />'
		),

        doubleTextArrow: Em.Handlebars.compile(
            '<span>{{view.text}}</span>'+
            '<span class="second-text">{{view.secondText}}</span>'+
            '<img class="arrow-ico" src="images/common/arrow_ico.png" />'
        )
	}
	
	/** Define button template */
	/*
	template: Ember.Handlebars.compile(
		'{{#with view}}'+
			'{{#if icon}}<img class="ico" {{bindAttr src="icon"}} />{{/if}}'+
			'{{#if text}}<span>{{text}}</span>{{/if}}'+
			'{{#if rightText}}<span class="right_text">{{rightText}}</span>{{/if}}'+
			'{{#if righticon}}<img class="right_ico" {{bindAttr src="righticon"}} />{{/if}}'+
			'{{#if arrow}}<img class="arrow-ico" src="images/common/arrow_ico.png" />{{/if}}'+
		'{{/with}}'
	)
	*/
});