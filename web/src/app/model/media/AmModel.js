/**
 * @name MFT.AmModel
 * 
 * @desc AM Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/AmModel.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
MFT.AmModel = Em.Object.create( {
	
	band:		MFT.RangedValue.create({range:2, value: -1, activeBand:0}),
	
	directTunestations: MFT.Playlist.create({
		/** Direct tune dial station matrix */
		directTunes: [
			[6,0,0], 
			[9,5,0],
			[1,1,7,6],
			[1,3,7,0]
		],
		/** Direct tune Data */
		directTuneItems: {
			600:MFT.PlaylistItem.create({frequency: 600}),
			950:MFT.PlaylistItem.create({frequency: 950}),
			1176:MFT.PlaylistItem.create({frequency: 1176}),
			1370:MFT.PlaylistItem.create({frequency: 1370}),
		}
	
	}),
	
	/** Stations - AM1 , AM Ast */	
	am1: MFT.Playlist.create( {
		// Selected Station Index
		selectedIndex: 					3,

		items: {
			0:MFT.PlaylistItem.create({frequency: 530}),
			1:MFT.PlaylistItem.create({frequency: 950}),
			2:MFT.PlaylistItem.create({frequency: 1270}),
			3:MFT.PlaylistItem.create({frequency: 800}),
			4:MFT.PlaylistItem.create({frequency: 1130}),
			5:MFT.PlaylistItem.create({frequency: 760})
		}
		
	}),
	
	amAst: MFT.Playlist.create( {
		// Selected Station Index
		selectedIndex: 					2,
		
		items: {
			0:MFT.PlaylistItem.create({frequency: 540}) ,
			1:MFT.PlaylistItem.create({frequency: 630}),
			2:MFT.PlaylistItem.create({frequency: 760}),
			3:MFT.PlaylistItem.create({frequency: 800}),
			4:MFT.PlaylistItem.create({frequency: 950}),
			5:MFT.PlaylistItem.create({frequency: 1130})
		}
	}),
	
});