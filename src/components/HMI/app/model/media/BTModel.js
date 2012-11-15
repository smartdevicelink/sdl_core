/**
 * @name MFT.BtModel
 * 
 * @desc Bt Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/BtModel.js
 * @version		2.0
 *
 * @author		Gerashchenko Maksym
 */
MFT.BTModel = Em.Object.create( {
	
	active: false,
	
	optionsData: Em.Object.create({
				
		view: 'bt',
		
		titleBinding: 'MFT.locale.label.view_media_bt_options_title',
		
		items:[
			{
				type:		MFT.Button,
			
				params:{
					textBinding: 'MFT.locale.label.view_media_bt_options_soundSettings',
					templateName:		'arrow',
					action: 'turnOnSoundSettings',
					target: 'MFT.MediaController'
				}							
			},
			
			{
				type:		MFT.Button,
			
				params:{
					textBinding: 'MFT.locale.label.view_media_bt_options_mediaPlayerSettings',
					templateName:		'arrow',
					disabled: 	 true
				}							
			},
			
			{
				type:		MFT.Button,
			
				params:{
					textBinding: 'MFT.locale.label.view_media_bt_options_deviceInformation',
					templateName:		'arrow',
					disabled: 	 true
				}							
			},
		]
	})
	
});