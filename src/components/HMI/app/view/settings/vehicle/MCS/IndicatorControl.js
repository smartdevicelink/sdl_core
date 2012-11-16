/**
 * @name MFT.IndicatorControl
 * 
 * @desc Universal buttons component with indicator for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/IndicatorControl.js
 * @version		2.0
 *
 * @author	Andriy Melnik
 */

MFT.IndicatorControl = Em.ContainerView.extend( {

	classNameBindings:	['disabled'],

	afterRender: function() {
		var element;

		element = Em.ContainerView.create( MFT.SelectedIndex, {
			
			index:	this.get('index'),
			
			classNameBindings:	['isSelected:active'],
			
			classNames: 		['control control-' + this.get('index')],	

			/** Container components */
			childViews: [
				'minusBtn',
				'led',
				'plusBtn'
			],

			/** Minus button */
			minusBtn: MFT.Button.extend({
				classNames:	'minus',
				icon: 		'images/common/minus-ico.png',
				action:		'onDecrease',
				target:		'MFT.MCSController',
				
				timer:		FLAGS.MCS_EMULATE_CAN ? 200 : 0,
				
				actionUp:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						this._super();
						
						if (!FLAGS.MCS_EMULATE_CAN) {
							MFT.MCSController.model.send(this._parentView.index,'off');
						}
						MFT.MCSController.set('highlighted', false);
					}
				},

				actionDown:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						MFT.MCSController.set('highlighted', true);
						this._super();
					}
				},
				
				mouseLeave:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						if (!FLAGS.MCS_EMULATE_CAN && this.pressed) {
							MFT.MCSController.model.send(this._parentView.index,'off');
						}
						MFT.MCSController.set('highlighted', false);
						
						this._super();
					}
				}
			}),

			/** adjust */
			led: MFT.Indicator.create({
				classNames:			'ledContainer ico',
				action:				'select',
				target:				'MFT.MCSController',
				contentBinding:		'MFT.MCSController.activeSeat.' + (this.get('index') - 1),
				indActiveClass:		'led',
				indDefaultClass:	'led-inactive',

				actionDown:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						MFT.MCSController.set('highlighted', true);
						this._super();
					}
				},

				actionUp:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						MFT.MCSController.set('highlighted', false);
					}
				},
				
				mouseLeave:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						MFT.MCSController.set('highlighted', false);
					}
				}
			}),

			/** Plus button */
			plusBtn: MFT.Button.extend({
				classNames:	'plus',
				icon: 		'images/common/plus-ico.png',
				action:		'onIncrease',
				target:		'MFT.MCSController',
				
				timer:		FLAGS.MCS_EMULATE_CAN ? 200 : 0,

				actionUp:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						this._super();
						
						if (!FLAGS.MCS_EMULATE_CAN) {
							MFT.MCSController.model.send(this._parentView.index,'off');
						}
						MFT.MCSController.set('highlighted', false);
					}
				},

				actionDown:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						MFT.MCSController.set('highlighted', true);
						this._super();
					}
				},
				
				mouseLeave:	function() {
					if(!this._parentView._parentView.get('disabled')) {
						if (!FLAGS.MCS_EMULATE_CAN && this.pressed) {
							MFT.MCSController.model.send(this._parentView.index,'off');
						}
						MFT.MCSController.set('highlighted', false);
						
						this._super();
					}
				}
			})
		})
		
		this.get('childViews').pushObject(element);	
	}
});

