package aztux.quadcontrol;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.os.Bundle;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.view.Display;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;


public class QuadcontrolActivity extends Activity {
    /** Called when the activity is first created. */
	
	public static float throttle = 6;  // 0 to 100
	public static float yaw = 0;  // -50 to + 50
	public static float pitch = 0; // -50 to + 50
	public static float roll = 0;  // -50 to + 50
	
	public static float trimpitch;
	public static float trimroll;
	
	public static float sensitivity; 
	SharedPreferences Remote_prefs;
	
	float height, width, thrCenterX, thrCenterY, pitCenterX, pitCenterY;
	static DatagramSocket s;
	static InetAddress local;
	
	static Timer t = new Timer();
	static TimerTask udptask;
	boolean listening = true;
	String serverMsgs = "";
	
	static Handler hand = new Handler();
	static Context ctx = null;

	View v = null;
	
	
	void set_default_values(){ 
		Remote_prefs.edit().putFloat("trimpitch", trimpitch).commit();
		Remote_prefs.edit().putFloat("trimroll", trimroll).commit();
		Remote_prefs.edit().putFloat("sensitivity", sensitivity).commit();
	}
	
	void load_default_values(){
		trimpitch = Remote_prefs.getFloat("trimpitch", 0);
		trimroll  = Remote_prefs.getFloat("trimroll", 0);
		sensitivity = Remote_prefs.getFloat("sensitivity", 2);
		
	}
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ctx = this;
        Remote_prefs = PreferenceManager.getDefaultSharedPreferences(this);
        
        Display display = getWindowManager().getDefaultDisplay(); 
		width  = (float) (display.getWidth());
		height = (float) (display.getHeight());
        
		thrCenterX = width/4;
		thrCenterY = height/2;
		pitCenterX = (width/4) * 3;
		pitCenterY = height/2;
		
		//load saved trim and sensitivity values
		load_default_values();		
		
		try {
			s = new DatagramSocket(); 
			
		} catch (SocketException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		Thread listenThread = new Thread(new Runnable() {
			@Override
			public void run() {
				
				try {
					DatagramSocket sock = new DatagramSocket(7001);
					while (listening) {
						byte[] message = new byte[1500];
						DatagramPacket p = new DatagramPacket(message, message.length);
						sock.receive(p);
						String text = new String(message, 0, p.getLength());
						serverMsgs = text;
						hand.post(new Runnable() {
							@Override
							public void run() {
								v.invalidate();
							}
						});
						
					}
				} catch (SocketException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			
			}
			
		});
		listenThread.start();
		
		
        v = new View(this) {
        	protected void onDraw(Canvas g) {
        		float w=width, h=height;
        
        		
        		Paint wpt = new Paint();
        		wpt.setColor(Color.WHITE);
        		wpt.setStyle(Style.FILL);
        		g.drawRect(0, 0, w,h, wpt);
        		
        		Paint p = new Paint();
        		p.setColor(Color.BLACK);
        		p.setStrokeWidth(3);
        		
        		float widthRange = w/2;
        		float heightRange = h;
        		
        		g.drawLine(thrCenterX, 0, thrCenterX, h, p);
        		g.drawLine(0, thrCenterY, w/2, thrCenterY, p);
        		g.drawCircle((int)(thrCenterX + (yaw/50.0)*(widthRange/2)), (int)(h - (throttle/100.0)*heightRange), 10, p);
        		
        		g.drawCircle((int)(pitCenterX  + (roll/50.0)*(widthRange/2)), (int)(pitCenterY - (pitch/50.0)*(heightRange/2)), 10, p);
        		g.drawLine(pitCenterX, 0, pitCenterX, h, p);
        		g.drawLine(w/2, pitCenterY, w, pitCenterY, p);

        		Paint p2 = new Paint();
        		
        		String disp_cmd = String.format(Locale.US,"T: %.1f Y: %.1f P: %.1f R: %.1f",throttle,yaw,pitch,roll);
        		
        		g.drawText(disp_cmd, 10, 10, p2);
        		Paint p3 = new Paint();
        		p3.setTextSize(20);
        		g.drawText(serverMsgs, 10, 40, p3);
        	}
        };
        
        
        v.setOnTouchListener(new OnTouchListener() {
			@Override
			//------------------------------------------------
			//Handles finger motions on the screen
			//and calculates their position = Remote commands
			//------------------------------------------------
			
			public boolean onTouch(View v, MotionEvent event) {
				int pointerCount = event.getPointerCount();
				for(int i=0; i<pointerCount; i++) {
					
					int act = event.getActionMasked();
					if(act == MotionEvent.ACTION_MOVE || act == MotionEvent.ACTION_DOWN) {
						float x = event.getX(i), y = event.getY(i);
						
						
						
						if ( x < width/2.0) { // throttle and yaw
							float n_throttle = (float) ((( height - y )/ height)*100.0);
							if(Math.abs(n_throttle - throttle) < 20)
								throttle = n_throttle;
							float n_yaw = (float) (((x - thrCenterX) / (width/2.0))*100.0);
							if(Math.sqrt(Math.pow(n_throttle - throttle,2) + Math.pow(n_yaw - yaw,2)) < 25) {
								
								yaw = n_yaw;
								throttle = n_throttle;
							}
						} else {
							float n_pitch = (float) (((pitCenterY - y) / (height))*100.0);							
							float n_roll  = (float) (((x - pitCenterX) / (width/2.0))*100.0);
							
							if(Math.sqrt(Math.pow(n_roll - roll,2) + Math.pow(n_pitch - pitch,2)) < 25) {
								roll = n_roll;
								pitch = n_pitch;
							}
						}
	
					} else if (act == MotionEvent.ACTION_UP || act == MotionEvent.ACTION_POINTER_UP ) {
						//
						//one finger or the two fingers went out of the screen
						float x = event.getX(i); 
						//int y = (int) event.getY(i);
						if ( x < width/2.0) { // throttle and yaw 
							yaw = 0;
						} else {//pitch and roll
							pitch = 0;
							roll = 0;
						}
					}
				}
				v.invalidate();
				
				return true;
				
			}
		});
        
        //hand.post(new Runnable() {
			
			//@Override
			//public void run() {
				//Toast.makeText(ctx, "PLEASE DISABLE PHONE CALLS", 500).show();
			//}
		//});
        
		setContentView(v);
    }
    
 
    public static void pause_remote() {
    	t.cancel();	
    }
    
    public static void resume_remote() {
        t = new Timer();
        udptask =  new TimerTask() {
			
			@Override
			public void run() {

				try {
					local = InetAddress.getByName(PIDActivity.ip);
					final String msg = "{ \"type\": \"rcinput\", \"thr\": " + throttle
							+ ", \"yaw\": "   + yaw
							+ ", \"pitch\": " + (pitch-trimpitch)/sensitivity 
							+ ", \"roll\": "  + (roll -trimroll) /sensitivity 
							+ "}\n";
					/*
					hand.post(new Runnable() {
						@Override
						public void run() {
							Toast.makeText(ctx, msg, Toast.LENGTH_SHORT).show();
						}
					});
					 */    
					
					int msg_length = msg.length();
					byte[] message = msg.getBytes();
					s.send(new DatagramPacket(message, msg_length, local, 7100));
					
				} catch (UnknownHostException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
					
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
		};
		//schedul the new remote
        t.schedule(udptask, 50, 50);
    }
    
    //--------------------------------------------------------------
    //	Menus
	public boolean onCreateOptionsMenu(Menu menu) 
    {
         super.onCreateOptionsMenu(menu);
         
         MenuItem Item = menu.add("PID");
         MenuItem Itemcam = menu.add("Camera");
         MenuItem Itemtrim = menu.add("Trim");
		return true;
    }
    
    public boolean onOptionsItemSelected(MenuItem item)
    {
    	if (item.getTitle() == "PID") {
    		Intent intent = new Intent(this, PIDActivity.class);
    		startActivity(intent);
    	}
    	if (item.getTitle() == "Camera") {
    		Intent intent = new Intent(this, CameraActivity.class);
    		startActivity(intent);
    	}
    	if (item.getTitle() == "Trim") {
    		Intent intent = new Intent(this, TrimActivity.class);
    		startActivity(intent);
    	}
    	return true;
    }
    
    @Override
    protected void onPause() {
    	super.onPause();
    	
    	//put values while pausing	
    	//throttle = 49; //should fall down slowly
    	yaw = roll = pitch = 0;
    	
    	//save trim and Sensitivity
    	set_default_values();
    	
    }
    
    @Override
    protected void onResume() {
    	super.onResume();

    }
        
    @Override
    protected void onDestroy() {
    	super.onDestroy();
    	udptask.cancel();

    	//save trim and Sensitivity
    	set_default_values();
    	
    	listening = false;
    }
}