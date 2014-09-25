/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.game.infinite.racing;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.AlertDialog;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.startapp.android.publish.StartAppAd;
import com.startapp.android.publish.StartAppSDK;
import com.umeng.message.PushAgent;
import com.umeng.mobclickcpp.MobClickCppHelper;

public class AppActivity extends Cocos2dxActivity {

	/** StartAppAd object declaration */
	private StartAppAd startAppAd = new StartAppAd(this);
	
	private static Handler handler;
	
	private static Context mContext;
	
	private Airpush bundle_air;
	
	private static native void rate(boolean value);
	private static native void quit();
	
	public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);

        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        MobClickCppHelper.init(this);
        mContext = this;
        return glSurfaceView;
    }
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		bundle_air = new Airpush(this);
		
		StartAppSDK.init(this, "109841477", "209232502", true); 
		
		PushAgent mPushAgent = PushAgent.getInstance(this);
		mPushAgent.enable();
		mPushAgent.onAppStart();
		mPushAgent.setDebugMode(true);
		final OnClickListener nowListener = new OnClickListener() {
			
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
				// TODO Auto-generated method stub
				Uri uri = Uri.parse("market://details?id=" + mContext.getPackageName());
				Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
				try {
					startActivity(goToMarket);
					rate(true);
				} catch (ActivityNotFoundException e) {
					startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("http://play.google.com/store/apps/details?id=" + mContext.getPackageName())));
					/*Toast.makeText(AppActivity.this, "Could not connect the Google Play",
							Toast.LENGTH_LONG).show();*/
				}
			}
		};
		
		final OnClickListener quitListener = new OnClickListener() {
			
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
				// TODO Auto-generated method stub
				finish();
			}
		};
		
		handler = new Handler() {

			public void handleMessage(Message msg)
			{
				switch (msg.what) {
				case 1://show ads
					Log.i("AppActivity", "show appstart========");
					startAppAd.showAd();
					break;
				case 2://quit 
					new AlertDialog.Builder(AppActivity.this)   
					.setTitle("Quit Game?")  
					.setMessage("If you like our game, please give us 5 stars, thanks!")  
					.setPositiveButton("Yes", quitListener)
					.setNeutralButton("Rate me", nowListener)
					.setNegativeButton("Cancel", null)
					.show();
					break;
				case 3://rait
					new AlertDialog.Builder(AppActivity.this)   
					.setTitle("Rate It?")  
					.setMessage("If you like our game, please give us 5 stars, thanks!")  
					.setPositiveButton("Now", nowListener)  
					.setNegativeButton("Later", null)  
					.show();
					break;
				case 4://own ads
					String pkg = msg.getData().getString("package");
					Uri uri = Uri.parse("market://details?id=" + pkg);
					Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
					try {
						startActivity(goToMarket);
						rate(true);
					} catch (ActivityNotFoundException e) {
						startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("http://play.google.com/store/apps/details?id=" + pkg)));
					}
					break;
				default:
					break;
				}
				
			}

		};

	}
	
	/**
	 * Part of the activity's life cycle, StartAppAd should be integrated here. 
	 */
	@Override
	public void onResume() {
		super.onResume();
		startAppAd.onResume();
	}

	/**
	 * Part of the activity's life cycle, StartAppAd should be integrated here
	 * for the home button exit ad integration.
	 */
	@Override
	public void onPause() {
		super.onPause();
		startAppAd.onPause();
	}

	/**
	 * Part of the activity's life cycle, StartAppAd should be integrated here
	 * for the back button exit ad integration.
	 */
	@Override
	public void onBackPressed() {
		startAppAd.onBackPressed();
		super.onBackPressed();
	}
	
	static void showStartAppAd(int type)
	{
		Message msg = new Message();
		msg.what = type;
		handler.sendMessage(msg);
	}
	
	static void showOwnAds(int type, String pkgname)
	{
		Bundle bundle = new Bundle();    
        bundle.putString("package", pkgname);
		Message msg = new Message();
		msg.what = type;
		msg.setData(bundle);
		handler.sendMessage(msg);
	}

}
