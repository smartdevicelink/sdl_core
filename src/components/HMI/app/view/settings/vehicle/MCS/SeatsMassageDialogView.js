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

MFT.SeatsMassageDialogView = Em.ContainerView.extend({

	classNames:			'SeatsMassageDialogView hide',

	afterRender: function() {
		var element;

		element = Em.ContainerView.create({

			childViews: [
				'titleQuestion',
				'question',
				'titleAnswer',
				'messageActionsBottom'
			],

			titleQuestion: MFT.Label.extend({
				elementId:	'titleQuestion',
				classNames:	'titleQuestion',
				content:	'Question:'
			}),

			question: MFT.Label.extend({
				elementId:	'question',
				classNames:	'question',
				content:	this.get('question')
			}),

			titleAnswer: MFT.Label.extend({
				elementId:	'titleAnswer',
				classNames:	'titleAnswer',
				content:	'Answers:'
			}),

			/** Container for three bottom buttons on Massage view */
			messageActionsBottom: MFT.ButtonsGroup.extend({
				classNames:			'seats-message-actions seats-message-actions-3',
				valueBinding:		this.get('value'),
				range:				this.get('range'),
				currIndex:			this.get('currIndex'),
				classes:			'quality button btn-48',
				action:				'chooseAnswer',
				target:				null,
				labels: 			this.get('answers'),
				delimeters:			false,
				chooseAnswer: function(){
					MFT.MCSController.set('massageCustom', false);
					MFT.MCSController.set('dialog.'+this.get('currIndex'), false);
					MFT.MCSController.set('dialog.6', false);
					MFT.VideoPlayerController.play();
				}
			})
		})

		this.get('childViews').pushObject(element);
	}
});