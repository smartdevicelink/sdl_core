/**
 * @name MFT.USBModel
 * 
 * @desc USB Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/USBModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.USBModel = Em.Object.create({
	
	active: false,
	
	/** USB Player*/
	init: function(){
		this._super();
		this.set('player',MFT.MediaCDPlayer.create({data: this.PlayList}));
	},
	
	PlayList: MFT.Playlist.create({
		selectedIndex: 					0,
			items: {
				0:MFT.PlaylistItem.create({title: 'Track 1',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
				1:MFT.PlaylistItem.create({title: 'Track 2',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
				2:MFT.PlaylistItem.create({title: 'Track 3',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
				3:MFT.PlaylistItem.create({title: 'Track 4',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
				4:MFT.PlaylistItem.create({title: 'Track 5',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123}),
				5:MFT.PlaylistItem.create({title: 'Track 6',album:'Color of the Fall',artist:'Luk Marko',genre:'Rock',disk:'Fall',duration:123})
			},
			
			homeWidgetIcon: 'images/media/usb-h-ico.png',
			homeWidgetIcon_blur: 'images/media/help/usb-h-ico_blur.png'
			
	}),
		
	browseData : Em.Object.create({
		title: 'USB Hard Disk Drive DSK5',
		
		disableScrollbar: true,
		
		items:	[
			{
				type:MFT.Button,
				params:{
					text:'Music',
					disabled:	true
				}
			},
			{
				type:MFT.Button,
				params:{
					text:'Explore device',
					disabled:	true
				}
			}
		]
	}),
	
	optionsData: Em.Object.create({
				
		view: 'usb',
		
		titleBinding: 'MFT.locale.label.view_media_usb_options_title',
		
		items:[
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_options_soundSettings',
						arrow:		true,
						disabled: 	 true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_options_mediaPlayerSettings',
						arrow:		true,
						disabled: 	 true
					}							
				},
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_usb_options_compression_deviceInformation',
						arrow:		true,
						disabled: 	 true
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_usb_options_compression_updateIndex',
						buttonTexBindingt:	'MFT.locale.label.view_media_usb_options_compression_start',
						labelDisabled:		true,
						buttonDisabled: 	   true
					}							
				}
					
			]
		})
});