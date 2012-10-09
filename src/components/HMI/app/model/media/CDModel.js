/**
 * @name MFT.CDModel
 * 
 * @desc CD Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/CDModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.CDModel= Em.Object.create({
	
	PlayList: MFT.Playlist.create({
		
		selectedIndex: 					0,
		
		items: {
			0:MFT.PlaylistItem.create({title: 'The Electric Broncos',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			1:MFT.PlaylistItem.create({title: 'Track 2',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			2:MFT.PlaylistItem.create({title: 'Track 3',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			3:MFT.PlaylistItem.create({title: 'Track 4',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			4:MFT.PlaylistItem.create({title: 'Track 5',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			5:MFT.PlaylistItem.create({title: 'Track 6',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			6:MFT.PlaylistItem.create({title: 'Track 7',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			7:MFT.PlaylistItem.create({title: 'Track 8',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			8:MFT.PlaylistItem.create({title: 'Track 9',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
			9:MFT.PlaylistItem.create({title: 'Track 10',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
		},
		
		homeWidgetIcon: 'images/media/cd-ico-home.png',
		homeWidgetIcon_blur: 'images/media/help/cd-ico-home_blur.png',
	}),
	
	browseData : Em.Object.create({
				
		title: 'CD Track list',
		
		currentPage: 0,
		
		items: [
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.0.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.1.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.2.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.3.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.4.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.5.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.6.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.7.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.8.title',
						disabled:	true
					}
				},
				{
					type:MFT.Button,
					params:{
						textBinding:'MFT.CDModel.PlayList.items.9.title',
						disabled:	true
					}
				}
		]
	}),
	
	optionsData: Em.Object.create({
				
		view: 'cd',
		
		titleBinding: 'MFT.locale.label.view_media_cd_options_title',
		
		items:[
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_bt_options_soundSettings',
						arrow:		true,
						disabled: true
					}							
				},
				
				{
					type:		MFT.LablePlusToggleButton,
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_cd_options_compression',
						tButtonValue: 		 1,
						tButtonRange:		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_cd_options_onOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}		
				}
			]
	})
});