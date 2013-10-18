package com.ford.sdlreverse.media;

import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.util.Log;

import com.ford.sdlreverse.activity.AvatarActivity;
import com.ford.sdlreverse.util.Const;

public class AvatarOnPreparedListener implements OnPreparedListener {

	private AvatarActivity activity;
	
	public AvatarOnPreparedListener(AvatarActivity activity) {
		this.activity = activity;
	}

	public void onPrepared(MediaPlayer mp) {
		Log.d(Const.APP_TAG, "onPrepared -> Play");
		try {
			if (activity.getVideoPausedPosition() > 0)
				mp.seekTo(activity.getVideoPausedPosition());
			mp.start();
			//set duration of video
			Log.i(Const.APP_TAG,"OnPrepareListener GetDuration");
			activity.getVideoController().setVideoDuration(mp.getDuration());
			activity.setVideoPlayed(true);
			activity.setVideoPrepared(true);
			if(activity.videoWasPaused())
				mp.pause();
			else 
				activity.startVideoTimer();
			
			
		} catch (IllegalStateException e) {
			Log.e(Const.APP_TAG, e.getMessage(),e);
		}	
	}
}
