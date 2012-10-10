/**
 * @name MFT.BlueToothView
 * 
 * @desc Media BlueTooth module visual representation
 * 
 * @category	View
 * @filesource	app/view/media/BlueToothView.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.bluetoothView = Em.ContainerView.create(MFT.LoadableView,{
		/** Bind class for visual representation */
		classNameBindings: 	['MFT.States.media.bluetooth.active:active_state'],
		/** Class Name */
		classNames: 'hidden',
		/** View ID*/
		elementId:	'media_bluetooth',
		
		stateObj: MFT.States.media.bluetooth,
		
		/** Template*/
		childViews: [
			'content'
		],
			
		content: Em.View.extend({
			
			template: Ember.Handlebars.compile(
				'<div class="bt-info">'+
					'<div id="bluetooth_blur" {{bindAttr class="MFT.helpMode:active_state:hidden"}}></div>'+
					'<div id="bt_info_wrapper" {{bindAttr class="MFT.helpMode:not-visible"}}>'+
						'<div class="right-stock-bt">'+
							
							'{{view MFT.Button '+
								'classNames="rs-item notpressed" '+
								'textBinding="MFT.locale.label.view_media_amFm_options_title" '+
								'action="turnOnOptions" '+
								'target="MFT.MediaController" '+
								'arrow="true" '+
							'}}'+
			
						'</div>'+
						'<div id="bt-bg" class="media-blocks bt-block">'+
							'<img src="images/media/bt_bg.png">'+
						'</div>'+
						'<div id="bt-dis-controls"></div>'+
					'</div>'+	
				'</div>'
			)
		}),
		
					action:			  'turnOnOptions',
			target:			  'MFT.MediaController',
});
	