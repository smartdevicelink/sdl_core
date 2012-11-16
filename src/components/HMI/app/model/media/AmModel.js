/**
 * @name MFT.AmData
 * 
 * @desc AM Media data model
 * 
 * @category	Model
 * @filesource	app/model/media/AmData.js
 * @version		2.0
 *
 * @author		Igor Zhavoronkin
 */
 
MFT.AmModel = Em.Object.create( {
	
	band:		MFT.RangedValue.create({range:2, activeBand:0}),
	
	active: true,
	
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
	
		
	am1: MFT.Playlist.create( {
		
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
	
	optionsData: Em.Object.create({
				
		view: 'am',
		
		titleBinding: 'MFT.locale.label.view_media_amFm_options_title',
		
		items:[
				
				{
					type:		MFT.Button,
	
					params:{
						textBinding: 'MFT.locale.label.view_media_amFm_options_soundSettings',
						templateName:		'arrow',
						action: 'turnOnSoundSettings',
						target: 'MFT.MediaController'
					}							
				},
				
				{
					type:		MFT.LablePlusButton ,
	
					params:{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_setPTY',
						buttonTexBindingt:	'MFT.locale.label.view_media_amFm_options_noProgram',
						labelDisabled:		true,
						templateName:		 'arrow',
						buttonDisabled: 	   true
					}							
				},
	
				{
								
					type:		MFT.LabelPlusToggleButton,
					params:		{
						labelContentBinding:  'MFT.locale.label.view_media_amFm_options_RDS',
						tButtonValue:		 2,
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
						templateName:		 'arrow',
						buttonDisabled: 	   true
					}							
				},
				
								{
								
					type:		MFT.LabelPlusToggleButton,
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
});
