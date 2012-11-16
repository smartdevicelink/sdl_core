/**
 * @name MFT.Playlist
 * 
 * @desc Basic playlist for AM/FM/SIRIUS bands and CD/USB/SD Card applications
 * 
 * @category	Model
 * @filesource	app/model/media/Playlist.js
 *
 * @author		SergeySolovey
 */
MFT.Playlist = Em.Object.extend( {
		selectedIndex: 	0,
		
		selectedDirectTuneStation: null,
		
		selectedItem: 	function() {
			return this.items[this.selectedIndex];
		}.property('this.selectedIndex'),
		
		onSelectDirecTune: function() {
			 this.set('selectedDirectItem',this.directTuneItems[Number(this.selectedDirectTuneStation)]);
		}.observes('this.selectedDirectTuneStation'),
})
	

