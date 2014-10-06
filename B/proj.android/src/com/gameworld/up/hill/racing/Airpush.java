package com.gameworld.up.hill.racing;

import android.app.Activity;
import android.util.Log;
import android.widget.RelativeLayout;

import com.yrkfgo.assxqx4.AdListener;
import com.yrkfgo.assxqx4.AdListener.AdType;
import com.yrkfgo.assxqx4.AdView;
import com.yrkfgo.assxqx4.MA;

public class Airpush {
	// protected Cocos2dxActivity mActivity;
	 protected Activity mActivity;
	 static Activity currentActivity;
	 static MA airsdk = null;
		private AdType radtype;
		
		
		static AdListener adCallbackListener=new AdListener() {
			
			@Override
			public void onSmartWallAdShowing() {
				// TODO Auto-generated method stub
				Log.i("onSmartWallAdShowing", "test======================");
			}
			
			@Override
			public void onSmartWallAdClosed() {
				// TODO Auto-generated method stub
				Log.i("onSmartWallAdClosed", "test======================");
			}
			
			@Override
			public void onSDKIntegrationError(String arg0) {
				Log.i("onSDKIntegrationError", "test======================" + arg0);
				
			}
			
			@Override
			public void onAdError(String arg0) {
				// TODO Auto-generated method stub
				Log.i("onAdError", "test======================" + arg0);
			}
			
			@Override
			public void onAdCached(AdType arg0) {
				// TODO Auto-generated method stub
				Log.i("onAdCached", "test======================" + arg0);
			}
			
			@Override
			public void noAdAvailableListener() {
				// TODO Auto-generated method stub
				Log.i("noAdAvailableListener", "test======================");
			}
		};
		
		public Airpush()
		{
			
		}
		
		public Airpush(Activity tActivity)
		{
			
		 currentActivity = tActivity;
		 currentActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					
					// TODO Auto-generated method stub
					
				airsdk=new MA(currentActivity, adCallbackListener, false);
					
				}
			});
		
		}
		

	 
	 public static void airSmartWallAd()
		{
		 		Log.i("airSmartWallAd", "test======================");
				airsdk.callSmartWallAd();
			}
		
		
			
		
	 
	 static AdListener.MraidAdListener adlistener = new AdListener.MraidAdListener() {
		
		@Override
		public void onErrorListener(String arg0) {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void onCloseListener() {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void onAdLoadingListener() {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void onAdLoadedListener() {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void onAdExpandedListner() {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void onAdClickListener() {
			// TODO Auto-generated method stub
			
		}
		
		@Override
		public void noAdAvailableListener() {
			// TODO Auto-generated method stub
			
		}
	};   
	/* public static void airBannerBottomAd()
		{
			
		 currentActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					
				
					RelativeLayout relativeLayout=new RelativeLayout(currentActivity);
					RelativeLayout.LayoutParams layoutParams1=new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
					RelativeLayout.LayoutParams layoutParams=new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
					layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);
					layoutParams.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);

					AdView adView = new AdView(currentActivity, AdView.BANNER_TYPE_IMAGE,
					AdView.PLACEMENT_TYPE_INTERSTITIAL, true, false,
					AdView.ANIMATION_TYPE_LEFT_TO_RIGHT);
					adView.setAdListener(adlistener);
					relativeLayout.addView(adView, layoutParams);
					currentActivity.addContentView(relativeLayout, layoutParams1);
					}
				});
			
		}*/
	 
	 public static void airBannerTopAd()
		{
			 
		 currentActivity.runOnUiThread(new Runnable() {
				
				@Override
				public void run() {
					
				
					RelativeLayout relativeLayout=new RelativeLayout(currentActivity);
					RelativeLayout.LayoutParams layoutParams1=new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
					RelativeLayout.LayoutParams layoutParams=new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
					layoutParams.addRule(RelativeLayout.ALIGN_PARENT_TOP, RelativeLayout.TRUE);
					layoutParams.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);

					AdView adView = new AdView(currentActivity, AdView.BANNER_TYPE_IMAGE,
					AdView.PLACEMENT_TYPE_INTERSTITIAL, true, false,
					AdView.ANIMATION_TYPE_LEFT_TO_RIGHT);
					adView.setAdListener(adlistener);
					relativeLayout.addView(adView, layoutParams);
					currentActivity.addContentView(relativeLayout, layoutParams1);
					}
				});
			
		}
	 
	 
	}
