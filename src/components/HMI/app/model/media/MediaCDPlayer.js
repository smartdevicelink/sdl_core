/**
 * @name MFT.MediaCDPlayer
 *
 * @desc Media CD Player logic
 *
 * @category    Model
 * @filesource    app/model/MediaCDPlayer.js
 * @version        2.0
 *
 * @author        Hoang Dinh
 */
SDL.MediaCDPlayer = Em.Object.extend({
    /** media Player Current Time in seconds*/
    currentTime: 0,
    /** media Player paused state*/
    isPlaying: false,
    /** total number of CD tracks*/
    totalCDTracks: 0,

    /** name of player */
    name: '',

    /** Form Current Track timer string*/
    formatTimeToString: function () {
        var minute = Math.floor(this.currentTime / 60);
        var second = (this.currentTime % 60).toFixed(0);
        return minute + ":" + (second < 10 ? '0' : '' ) + second;
    }.property('this.currentTime'),

    trackLength: function (obj) {
        var size = 0, key;
        for (key in obj) {
            if (obj.hasOwnProperty(key) && obj[key].duration) size++;
        }
        return size;
    },

    /** Form Total Track number*/
    totalTracks: function () {
        this.set('totalCDTracks', this.trackLength(this.data.items));
        return this.totalCDTracks;
    }.property(),

    /** Form Current Track number*/
    currentTrack: function () {
        return this.data.selectedIndex + 1;
    }.property('this.data.selectedIndex'),

    /** Timer to simulate track playback*/
    trackTimer: function () {
        var self = this;
        this.trackInterval = setInterval(function () {
            /** IF current track time >= track duration ----> switch to next track*/
            if (self.currentTime < self.data.get('selectedItem').duration) {
                self.set('currentTime', self.currentTime + 1);
            } else {
                self.nextTrack();
                self.refreshTimer();
            }
        }, 1000);
    },

    /** Refresh timer*/
    refreshTimer: function () {
        clearInterval(this.trackInterval);
        this.trackTimer();
    },

    /** Play Buton Event*/
    play: function () {
        if (!this.isPlaying) {
            /** Playing state to true*/
            this.set('isPlaying', true);
            this.trackTimer()
        } else {
            this.pause();
        }
    },
    /** Pause*/
    pause: function () {
        clearInterval(this.trackInterval)
        /** Playing state to false*/
        this.set('isPlaying', false);
    },
    /** Next Track Buton Event*/
    nextTrack: function () {
        if (this.isPlaying) this.refreshTimer();
        /** refresh currentTime value*/
        this.set('currentTime', 0);
        /** Switch to next track*/
        if (this.data.selectedIndex < (this.totalCDTracks - 1)) {
            this.data.set('selectedIndex', this.data.selectedIndex + 1);
        } else {
            this.data.set('selectedIndex', 0);
        }
    },
    /** Move to the beginning current track */
    moveToBegining: function () {
        this.refreshTimer();
        this.set('currentTime', 0);
    },
    /** Prev Track Buton Event*/
    prevTrack: function () {
        if (this.isPlaying) this.refreshTimer();
        /** refresh currentTime value*/
        this.set('currentTime', 0);

        /** Switch to prev track*/
        if (this.data.selectedIndex > 0) {
            this.data.set('selectedIndex', this.data.selectedIndex - 1);
        } else {
            this.data.set('selectedIndex', this.totalCDTracks - 1);
        }
    },

    prevTrackPress: function () {
        if (this.currentTime > 5) {
            this.moveToBegining();
        } else {
            this.prevTrack();
        }

        var self = this,
            media = self.data.get('selectedItem');

        var params = {
            "songInfo":{
                "name": media.name,
                "artist": media.artist,
                "genre": media.genre,
                "album": media.album,
                "year": media.year,
                "duration": media.duration,
                "currentTime" : self.get('currentTime')
            },
            "model" : self.name,
            "action" : "PREV"
        };

        FFW.VehicleInfo.OnPlayerDetails(params);
        SDL.SDLModel.resetControl();
    },

    playTrackPress: function () {
        this.play();

        var self = this,
            media = self.data.get('selectedItem');

        var params = {
            "songInfo":{
                "name": media.name,
                "artist": media.artist,
                "genre": media.genre,
                "album": media.album,
                "year": media.year,
                "duration": media.duration,
                "currentTime" : self.get('currentTime')
            },
            "model" : self.name,
            "action" : self.isPlaying ? "PLAY" : "PAUSE"
        };

        FFW.VehicleInfo.OnPlayerDetails(params);
        SDL.SDLModel.resetControl();
    },

    nextTrackPress: function () {
        this.nextTrack();

        var self = this,
            media = self.data.get('selectedItem');

        var params = {
            "songInfo":{
                "name": media.name,
                "artist": media.artist,
                "genre": media.genre,
                "album": media.album,
                "year": media.year,
                "duration": media.duration,
                "currentTime" : self.get('currentTime')
            },
            "model" : self.name,
            "action" : "NEXT"
        };

        FFW.VehicleInfo.OnPlayerDetails(params);
        SDL.SDLModel.resetControl();
    },

    onPrevTrackRequest: function (params) {
        if (params != null && params.model == this.name) {
            this.prevTrack();
        }
    },

    onPlayTrackRequest: function (params) {
        if (params != null && params.model == this.name) {
            this.play();
        }
    },

    onNextTrackRequest: function (params) {
        if (params != null && params.model == this.name) {
            this.nextTrack();
        }
    }
})