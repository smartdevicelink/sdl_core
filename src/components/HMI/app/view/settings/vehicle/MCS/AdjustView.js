/**
 * @name MFT.Indicator
 * 
 * @desc Universal button component with value indicator for MFT application
 * 
 * @category	Controlls
 * @filesource	app/controlls/IndButton.js
 * @version		2.0
 *
 * @author	Artem Petrosyan
 */

MFT.seatsAdjustView = Em.ContainerView.extend({

	start:	null,
	end:	null,

	afterRender: function() {
		var element;

		/** Creation of containers of controls and five images of right side of controls */
		for( var i = this.start; i < this.end; i++ ){

			element = Em.ContainerView.create({

				/** Container components */
				childViews: [
					'control',
					'img'
				],

				/** Image of small seat*/
				img: Em.View.create({

					classNames: 'seats-'+(i+1)

				}),

				control: MFT.IndicatorControl.extend({

					index: i + 1,

					disabled:	function(){

						return this._parentView._parentView.get('disabled');

					}.property('this.parentView.parentView.disabled')

				})
			});
			this.get('childViews').pushObject(element);	
		}
	}
});

