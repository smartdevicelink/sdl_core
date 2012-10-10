/**
 * @name MFT.SDModel
 * 
 * @desc SD Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/SDModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */

MFT.SDModel= Em.Object.create({
	
	active: false,
	
	/** SD Player*/
	init: function(){
		this._super();
		this.set('player',MFT.MediaCDPlayer.create({data: this.PlayList}));
	},
	
PlayList: MFT.Playlist.create({
	
	selectedIndex: 					4,
	
	items: {
		0:MFT.PlaylistItem.create({title: 'The Electric Broncos',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
		1:MFT.PlaylistItem.create({title: 'Track 2',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
		2:MFT.PlaylistItem.create({title: 'Track 3',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
		3:MFT.PlaylistItem.create({title: 'Track 4',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
		4:MFT.PlaylistItem.create({title: 'Track 5',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123}),
		5:MFT.PlaylistItem.create({title: 'Track 6',album:'Love Bomb',artist:'Epton Joe',genre:'Electronic',disk:'Lazerhorse',duration:123})
	},
	
	homeWidgetIcon: 'images/media/sd-h-ico.png',
	homeWidgetIcon_blur: 'images/media/help/sd-h-ico_blur.png'
}),

browseData : Em.Object.create({
		
		title: 'USB SD Drive DSK4',
		
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
	})
});