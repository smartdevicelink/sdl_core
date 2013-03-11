/**
 * @name MFT.SliderView
 * 
 * @desc Slider visual representation
 * 
 * @category	View
 * @filesource	app/view/applink/shared/sliderView.js
 * @version		1.0
 *
 * @author		Artem Petrosyan
 */
 
MFT.SliderView = MFT.ApplinkAbstractView.create({
	
	elementId: 'slider_view',
	
	childViews: [
		'backButton',
		'captionText',
		'headerLabel',
		'footerLabel',
		'adjustControl'
	],
	
	headerLabel: MFT.Label.extend({
		classNames:	'slider-header',
		content:	'Header Label'
	}),
	
	footerLabel: MFT.Label.extend({
		classNames:	'slider-footer',
		content:	'Footer Label',
		data:       []
	}),

	/**
     * Identifier of current request
     */
	sliderRequestId:	null,
	
	/**
	 * Extend deactivate method
	 * send SUCCESS response on deactivate with current slider value
	 */
	deactivate: function() {
		this._super();
		FFW.UI.sendSliderResult( "SUCCESS", this.get( 'sliderRequestId' ), this.get('adjustControl.sliderValue.value') );
	},
		
	adjustControl: Em.ContainerView.extend({
		
		classNames:         'sliderControl',
		
		childViews: [
			'minusBtn',
			'led',
			'plusBtn'
		],
		
		sliderValue:	MFT.RangedValue.create({range: 10, value:0, cycle: false, minValue: 1}),
		
		minusBtn: MFT.Button.extend({
			classNames: 'control minus',
			icon:       'images/common/minus-ico.png',
			actionDown: function() {
				this._super();
				this.get('parentView.sliderValue').decrease();
			}
		}),
		
		led: MFT.Indicator.create({
			classNames:         'ledContainer ico',
			contentBinding:     'parentView.sliderValue',
			indActiveClass:     'led',
			indDefaultClass:    'led-inactive',
		}),
		
		plusBtn: MFT.Button.extend({
			classNames: 'control plus',
			icon:       'images/common/plus-ico.png',
			actionDown: function() {
				this._super();
				this.get('parentView.sliderValue').increase();
			}
		})
	}),
	
    loadData: function( message ){

    	var data = message.params;

    	this.set( 'sliderRequestId', message.id);

        this.set( 'headerLabel.content', data.sliderHeader );
        this.set( 'footerLabel.content', data.sliderFooter[0] );
		this.get( 'adjustControl.sliderValue').set('range',data.numTicks);
		this.get( 'adjustControl.sliderValue').set('value',data.position);
        
        this.footerLabel.data = data.sliderFooter;
        
		setTimeout(function(){
			MFT.SliderView.adjustControl.rerender();
		},1);
    },
    
    
    /**
     * Change footer text depends on current slider position
     * works only for dynamic footer mode
     */
    changeFooterText: function() {
        if ( this.footerLabel.data.length > 1 ) {
             this.set( 'footerLabel.content', this.footerLabel.data[this.adjustControl.sliderValue.value-1] );
        }
    }.observes('adjustControl.sliderValue.value')
});
