/**
 * @name MFT.FmData
 * 
 * @desc FM Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/FmData.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
MFT.FmModel = Em.Object.create({
	
	band:		MFT.RangedValue.create({value:-1, activeBand:0}),
	
	fm1: MFT.Playlist.create( {
		selectedIndex: 					1,

		items: {
			0:MFT.PlaylistItem.create({frequency: '99.1',genre: 'Pop',title: 'BlUE SKY',artist: 'THE MAX', isHd: false}) ,
			1:MFT.PlaylistItem.create({frequency: '98.8',genre: 'Club',title: 'JUMP AND DOWN',artist: 'THE PROJECT X'}),
			2:MFT.PlaylistItem.create({frequency: '100.9',genre: 'Rock',title: 'WELCOME HOME',artist: 'TODD SULLIVAN',isHd: true}),
			3:MFT.PlaylistItem.create({frequency: '103.5',genre: 'Pop',title: 'LETS DANCE',artist: 'MICHAEL JOHNSON',isHd: true}),
			4:MFT.PlaylistItem.create({frequency: '106.1',genre: 'Pop Rock',title: 'YESTERDAY NIGHT',artist: 'JOHN SMITH', isHd: false}),
			5:MFT.PlaylistItem.create({frequency: '105.2',genre: 'Classic',title: 'TENTH SYMPHONY',artist: 'SPENCER M.', isHd: false})
		},
		
		/** Direct tune dial station matrix */
		directTunes: [
			[9,8,7],
			[1,0,0,4],
			[1,0,2,0],
			[1,0,3,4],
			[1,0,7,1]
		],
		
		/** Direct tune Data */
		directTuneItems: {
			987:MFT.PlaylistItem.create({frequency: '98.7',genre: 'Trance',title: 'Space',artist: 'Dreamers', isHd: false}),
			1004:MFT.PlaylistItem.create({frequency: '100.4',genre: 'Pop',title: 'Take me with you',artist: 'Silva', isHd: false}),
			1020:MFT.PlaylistItem.create({frequency: '102.0',genre: 'Jazz',title: 'Dance alone',artist: 'Jazzy', isHd: false}),
			1034:MFT.PlaylistItem.create({frequency: '103.4',genre: 'Pop',title: 'Space',artist: 'Dreamers', isHd: false}),
			1071:MFT.PlaylistItem.create({frequency: '107.1',genre: 'Jazz',title: 'Dance alone',artist: 'Jazzy', isHd: false}),
		}
	}),
	
	fm2: MFT.Playlist.create( {
		selectedIndex: 					4,

		items:{
			0:MFT.PlaylistItem.create({frequency: '101.1',genre: 'Club',title: 'SPRING TIME',artist: 'DJ SKY', isHd: false}) ,
			1:MFT.PlaylistItem.create({frequency: '103.2',genre: 'Rock',title: 'RAINBOW',artist: 'THE BEES', isHd: false}),
			2:MFT.PlaylistItem.create({frequency: '99.3',genre: 'Classic',title: 'SUNSET',artist: 'SKYLARK',isHd: true}),
			3:MFT.PlaylistItem.create({frequency: '103.5',genre: 'Club',title: 'JUMP AND DOWN',artist: 'THE PROJECT X',isHd: true}),
			4:MFT.PlaylistItem.create({frequency: '104.1',genre: 'Pop',title: 'HONEY',artist: 'EPTON JOHN', isHd: false}),
			5:MFT.PlaylistItem.create({frequency: '105.2',genre: 'Pop',title: 'LETS DANCE',artist: 'MICHAEL JOHNSON', isHd: false})
		},
		
		/** Direct tune dial station matrix */
		directTunes: [
			[9,8,7],
			[1,0,0,4],
			[1,0,2,0],
			[1,0,3,4],
			[1,0,7,1]
		],
		
		/** Direct tune Data */
		directTuneItems: {
			987:MFT.PlaylistItem.create({frequency: '98.7',genre: 'Trance',title: 'Space',artist: 'Dreamers', isHd: false}),
			1004:MFT.PlaylistItem.create({frequency: '100.4',genre: 'Pop',title: 'Take me with you',artist: 'Silva', isHd: false}),
			1020:MFT.PlaylistItem.create({frequency: '102.0',genre: 'Jazz',title: 'Dance alone',artist: 'Jazzy', isHd: false}),
			1034:MFT.PlaylistItem.create({frequency: '103.4',genre: 'Pop',title: 'Space',artist: 'Dreamers', isHd: false}),
			1071:MFT.PlaylistItem.create({frequency: '107.1',genre: 'Jazz',title: 'Dance alone',artist: 'Jazzy', isHd: false}),
		}
	}),
	
	fmAst: MFT.Playlist.create( {				
		selectedIndex: 					1,

		items:{
			0:MFT.PlaylistItem.create({frequency: '98.2',genre: 'Club',title: 'SPRING TIME',artist: 'DJ SKY', isHd: false}) ,
			1:MFT.PlaylistItem.create({frequency: '106.6',genre: 'Rock',title: 'WELCOME HOME',artist: 'TODD SULLIVAN', isHd: false}),
			2:MFT.PlaylistItem.create({frequency: '99.3',genre: 'Pop',title: 'GOOD MORNING',artist: 'SUSAN BAKER',isHd: true}),
			3:MFT.PlaylistItem.create({frequency: '107.5',genre: 'Classic',title: 'SUNSET',artist: 'SKYLARK',isHd: true}),
			4:MFT.PlaylistItem.create({frequency: '106.1',genre: 'Pop',title: 'HONEY',artist: 'EPTON JOHN', isHd: false}),
			5:MFT.PlaylistItem.create({frequency: '104.2',genre: 'Classic',title: 'TENTH SYMPHONY',artist: 'SPENCER M.', isHd: false})
		},
		
		/** Direct tune dial station matrix */
		directTunes: [
			[9,8,7],
			[1,0,0,4],
			[1,0,2,0],
			[1,0,3,4],
			[1,0,7,1]
		],
		
		/** Direct tune Data */
		directTuneItems: {
			987:MFT.PlaylistItem.create({frequency: '98.7',genre: 'Trance',title: 'Space',artist: 'Dreamers', isHd: false}),
			1004:MFT.PlaylistItem.create({frequency: '100.4',genre: 'Pop',title: 'Take me with you',artist: 'Silva', isHd: false}),
			1020:MFT.PlaylistItem.create({frequency: '102.0',genre: 'Jazz',title: 'Dance alone',artist: 'Jazzy', isHd: false}),
			1034:MFT.PlaylistItem.create({frequency: '103.4',genre: 'Pop',title: 'Space',artist: 'Dreamers', isHd: false}),
			1071:MFT.PlaylistItem.create({frequency: '107.1',genre: 'Jazz',title: 'Dance alone',artist: 'Jazzy', isHd: false}),
		}

	}),
	
	optionsData: Em.Object.create({
		
		view: 'fm',		
		
		titleBinding: 'MFT.locale.label.view_media_amFm_options_title',
		
		items:[
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_soundSettings',
						arrow:		true,
						disabled: 	 true
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_setPTY',
						buttonTexBindingt:	'MFT.locale.label.view_media_amFm_options_noProgram',
						labelDisabled:		true,
						buttonArrow:		  true,
						buttonDisabled: 	   true
					}							
				},
	
				{
								
					type:		MFT.LablePlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_RDS',
						tButtonValue:		 0,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_amFm_options_onOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
						
				},	
	
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_autoset',
						buttonTexBindingt:	'MFT.locale.label.view_media_amFm_options_refresh',
						labelDisabled:		true,
						buttonArrow:		  true,
						buttonDisabled: 	   true
					}							
				},
				
								{
								
					type:		MFT.LablePlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_TAG',
						tButtonValue:		 1,
						tButtonRange: 		 2,
						tButtonLabelsBinding: 'MFT.locale.label.view_media_amFm_options_onOff',
						labelDisabled: 		 true,
						tButtonDisabled:	   true
					}			
						
				}
	
			]
	})
	
})