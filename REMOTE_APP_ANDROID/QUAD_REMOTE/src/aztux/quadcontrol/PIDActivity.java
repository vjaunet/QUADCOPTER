package aztux.quadcontrol;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import android.app.Activity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.Spinner;
import android.widget.Toast;



public class PIDActivity extends Activity {
	int curpid = 0;
	boolean pidlistloaded = false;
	public boolean initsent = false;
	public boolean started = false;
	
	public static String _kp="0",_ki="0",_kd="0", ip = "192.168.1.1";
	EditText kp;
	EditText ki;
	EditText kd;
	EditText kp_min;
	EditText ki_min;
	EditText kd_min;
	EditText kp_max;
	EditText ki_max;
	EditText kd_max;
	EditText ipaddr;
	SharedPreferences prefs;
	SeekBar seekBar_kp;
	SeekBar seekBar_ki;
	SeekBar seekBar_kd;
	
	private Button btn_init, btn_start, btn_stop, btn_doupdate, btn_exit;
	
	
	void updateBoxes(int pid) {
		kp.setText(prefs.getString("pid:"+pid+":kp", "2.5"));
		ki.setText(prefs.getString("pid:"+pid+":ki", "0.1"));
		kd.setText(prefs.getString("pid:"+pid+":kd", "0.1"));
		
		kp_min.setText(prefs.getString("pid:"+pid+":kp_min", "1.0"));
		ki_min.setText(prefs.getString("pid:"+pid+":ki_min", "0.0"));
		kd_min.setText(prefs.getString("pid:"+pid+":kd_min", "0.0"));
		
		kp_max.setText(prefs.getString("pid:"+pid+":kp_max", "4.0"));
		ki_max.setText(prefs.getString("pid:"+pid+":ki_max", "0.2"));
		kd_max.setText(prefs.getString("pid:"+pid+":kd_max", "0.2"));
	}
	
	void updateSeekbars (int pid){
		float sb_min = Float.parseFloat(prefs.getString("pid:"+pid+":kp_min", "1.0"));
		float sb_max = Float.parseFloat(prefs.getString("pid:"+pid+":kp_max", "4.0"));
		seekBar_kp.setProgress((int)((Float.parseFloat(prefs.getString("pid:"+pid+":kp", "2.0"))-sb_min)/(sb_max-sb_min)*100));
		
		sb_min = Float.parseFloat(prefs.getString("pid:"+pid+":ki_min", "1.0"));
		sb_max = Float.parseFloat(prefs.getString("pid:"+pid+":ki_max", "2.0"));
		seekBar_ki.setProgress((int)((Float.parseFloat(prefs.getString("pid:"+pid+":ki", "0.1"))-sb_min)/(sb_max-sb_min)*100));
		
		sb_min = Float.parseFloat(prefs.getString("pid:"+pid+":kd_min", "1.0"));
		sb_max = Float.parseFloat(prefs.getString("pid:"+pid+":kd_max", "2.0"));
		seekBar_kd.setProgress((int)((Float.parseFloat(prefs.getString("pid:"+pid+":kd", "0.1"))-sb_min)/(sb_max-sb_min)*100));
	}
	
	void saveBoxes(int pid) {
		prefs.edit().putString("pid:"+pid+":kp", kp.getText().toString()).commit();
		prefs.edit().putString("pid:"+pid+":ki", ki.getText().toString()).commit();
		prefs.edit().putString("pid:"+pid+":kd", kd.getText().toString()).commit();
	
		prefs.edit().putString("pid:"+pid+":kp_min", kp_min.getText().toString()).commit();
		prefs.edit().putString("pid:"+pid+":kd_min", kd_min.getText().toString()).commit();
		prefs.edit().putString("pid:"+pid+":ki_min", ki_min.getText().toString()).commit();
		
		prefs.edit().putString("pid:"+pid+":kp_max", kp_max.getText().toString()).commit();
		prefs.edit().putString("pid:"+pid+":kd_max", kd_max.getText().toString()).commit();
		prefs.edit().putString("pid:"+pid+":ki_max", ki_max.getText().toString()).commit();
	}
	
	void sendMsg(String str) {
		DatagramSocket s = null;
		InetAddress local = null;
		try {
			s = new DatagramSocket(); 
			local = InetAddress.getByName(ip);
		} catch (SocketException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try {
			byte[] message = str.getBytes();
			s.send(new DatagramPacket(message, str.length(), local, 7100));
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	void receiveMsg(String str) {
		DatagramSocket s = null;
		try {
			s = new DatagramSocket(7100, InetAddress.getByName("0.0.0.0"));
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		//Get bytes
		byte[] message = new byte [1500];
		DatagramPacket packet = new DatagramPacket(message, message.length);
		try {
			s.receive(packet);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		//Convert to String
		str = new String(message, 0, packet.getLength());
	
	}

	
	void generateMsg(int pid){
		
		String msg;
		
		switch(pid){
			case 0:
				msg = "pid pr_rate kp " + prefs.getString("pid:0:kp", "1.0") + " ki "+ prefs.getString("pid:0:ki", "1.0")
				+ " kd " + prefs.getString("pid:0:kd", "1.0") + "\n";
				sendMsg(msg);
				break;
			case 1:
				msg = "pid pr_stab kp " + prefs.getString("pid:1:kp", "1.0") + " ki " + prefs.getString("pid:1:ki", "1.0")
					+ " kd " + prefs.getString("pid:1:kd", "1.0") + "\n";
				sendMsg(msg);
				break;	
			case 2:
				msg = "pid yaw_rate kp " + prefs.getString("pid:2:kp", "1.0") + " ki " + prefs.getString("pid:2:ki", "1.0")
					+ " kd " + prefs.getString("pid:2:kd", "1.0") + "\n";
				sendMsg(msg);
				break;
			case 3:
				//this should never happens since PID YAW STAB is non sense
				//Yaw Stab has been reæoved from pid array in values
				msg = "pid yaw_stab kp " + prefs.getString("pid:3:kp", "1.0") + " ki " + prefs.getString("pid:3:ki", "1.0")
					+ " kd " + prefs.getString("pid:3:kd", "1.0") + "\n";
				sendMsg(msg);
				break;
			}
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.pid);
		prefs = PreferenceManager.getDefaultSharedPreferences(this);
		
		kp = (EditText) findViewById(R.id.kp);
		ki = (EditText) findViewById(R.id.ki);
		kd = (EditText) findViewById(R.id.kd);
		
		kp_min = (EditText) findViewById(R.id.kp_min);
		ki_min = (EditText) findViewById(R.id.ki_min);
		kd_min = (EditText) findViewById(R.id.kd_min);
		
		kp_max = (EditText) findViewById(R.id.kp_max);
		ki_max = (EditText) findViewById(R.id.ki_max);
		kd_max = (EditText) findViewById(R.id.kd_max);
		
		ipaddr = (EditText) findViewById(R.id.ipaddr);
		ipaddr.setText(ip);
		
		seekBar_kp = (SeekBar) findViewById(R.id.seekBar_kp);
		seekBar_ki = (SeekBar) findViewById(R.id.seekBar_ki);
		seekBar_kd = (SeekBar) findViewById(R.id.seekBar_kd);
		
		btn_doupdate = (Button)findViewById(R.id.doupdate);
		btn_init = (Button)findViewById(R.id.init);
		btn_start = (Button)findViewById(R.id.start);
		btn_stop = (Button)findViewById(R.id.stop);
		btn_exit = (Button)findViewById(R.id.Exit);
				
		updateBoxes(0);
			
		
		if (initsent==true) 
		{
			btn_init.setEnabled(false);
			btn_init.setBackgroundColor(Color.parseColor("#808080"));
		}
		
		if (started == true)
		{
			btn_start.setEnabled(false);
			btn_start.setBackgroundColor(Color.parseColor("#808080"));	
		}
		
		
		final Spinner pidSelect = (Spinner) findViewById(R.id.pidSelect);
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
		        R.array.pid_array, android.R.layout.simple_spinner_item);
		// Specify the layout to use when the list of choices appears
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		// Apply the adapter to the spinner
		pidSelect.setAdapter(adapter);
		
		
		
		pidSelect.setOnItemSelectedListener(new OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> parent, View view, 
		            int pos, long id) {
				if(pidlistloaded) 
					saveBoxes(curpid);
				
				updateBoxes(pos);
				updateSeekbars(pos);
				pidlistloaded = true;
				curpid = pos;
			}

			@Override
			public void onNothingSelected(AdapterView<?> arg0) {
			}
		});
		
		seekBar_kp.setOnSeekBarChangeListener(
                new OnSeekBarChangeListener() {
                	
                	double val;	
                	@Override
                	public void onProgressChanged(SeekBar seekBar_kp, 
                                            int progresValue, boolean fromUser) {
                		float sb_min = Float.parseFloat(kp_min.getText().toString());
                		float sb_max = Float.parseFloat(kp_max.getText().toString());
                		val = (double)progresValue/100.f*(sb_max-sb_min) + sb_min;
                		kp.setText(String.valueOf(val));  
                		}

                	@Override
                	public void onStartTrackingTouch(SeekBar seekBar_kp) {
                		// Do something here, 
                		//if you want to do anything at the start of
                		// touching the seekbar
                	}

                	@Override
                	public void onStopTrackingTouch(SeekBar seekBar_kp) {
                		_kp = kp.getText().toString();
        				ip = ipaddr.getText().toString();
        				saveBoxes(curpid);
        				
        				new Thread(new Runnable() {
        					
        					@Override
        					public void run() {
        						generateMsg(curpid);
        						
        						try {
        							Thread.sleep(10);
        						} catch (InterruptedException e) {
        							// TODO Auto-generated catch block
        							e.printStackTrace();
        						}
        					}
        				}).start();
                	}
                });
		
		
		seekBar_ki.setOnSeekBarChangeListener(
                new OnSeekBarChangeListener() {
                	
                	double val;	
                	@Override
                	public void onProgressChanged(SeekBar seekBar_ki, 
                                            int progresValue, boolean fromUser) {
                		float sb_min = Float.parseFloat(ki_min.getText().toString());
                		float sb_max = Float.parseFloat(ki_max.getText().toString());
                		val = (double)progresValue/100.f*(sb_max-sb_min) + sb_min;
                		ki.setText(String.valueOf(val));                	
                	}

                	@Override
                	public void onStartTrackingTouch(SeekBar seekBar_ki) {
                		// Do something here, 
                		//if you want to do anything at the start of
                		// touching the seekbar
                	}

                	@Override
                	public void onStopTrackingTouch(SeekBar seekBar_ki) {
                		_ki = ki.getText().toString();
        				ip = ipaddr.getText().toString();
        				saveBoxes(curpid);
        				
        				new Thread(new Runnable() {
        					
        					@Override
        					public void run() {
        						generateMsg(curpid);
        						
        						try {
        							Thread.sleep(10);
        						} catch (InterruptedException e) {
        							// TODO Auto-generated catch block
        							e.printStackTrace();
        						}
        					}
        				}).start();
                	
                	}
                });
		
		
		
		seekBar_kd.setOnSeekBarChangeListener(
                new OnSeekBarChangeListener() {
                	
                	double val;	
                	@Override
                	public void onProgressChanged(SeekBar seekBar_kd, 
                                            int progresValue, boolean fromUser) {
                		float sb_min = Float.parseFloat(kd_min.getText().toString());
                		float sb_max = Float.parseFloat(kd_max.getText().toString());
                		val = (double)progresValue/100.f*(sb_max-sb_min) + sb_min;
                		kd.setText(String.valueOf(val));   
                	}

                	@Override
                	public void onStartTrackingTouch(SeekBar seekBar_kd) {
                		// Do something here, 
                		//if you want to do anything at the start of
                		// touching the seekbar
                	}

                	@Override
                	public void onStopTrackingTouch(SeekBar seekBar_kd) {
                		_kd = kd.getText().toString();
        				ip = ipaddr.getText().toString();
        				saveBoxes(curpid);
        				
        				new Thread(new Runnable() {
        					
        					@Override
        					public void run() {
        						generateMsg(curpid);
        						
        						try {
        							Thread.sleep(10);
        						} catch (InterruptedException e) {
        							// TODO Auto-generated catch block
        							e.printStackTrace();
        						}
        					}
        				}).start();
                	}
                });
		
		
		//Button btn_doupdate
		btn_doupdate.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				_kp = kp.getText().toString();
				_ki = ki.getText().toString();
				_kd = kd.getText().toString();
				ip = ipaddr.getText().toString();
				saveBoxes(curpid);
				//updateSeekbars(curpid);
				
				new Thread(new Runnable() {
					
					@Override
					public void run() {
							
						generateMsg(curpid);
						
						try {
							Thread.sleep(10);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						
					}
				}).start();
				
				//close down the activity after clicking
				//finish();
			}
		});
		
		
		//Button btn_starts
		btn_start.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				_kp = kp.getText().toString();
				_ki = ki.getText().toString();
				_kd = kd.getText().toString();
				ip = ipaddr.getText().toString();
				saveBoxes(curpid);
								
				btn_start.setEnabled(false);
				btn_start.setBackgroundColor(Color.parseColor("#808080"));
				started = true;
				
				new Thread(new Runnable() {
					
					@Override
					public void run() {
														
						generateMsg(curpid);					
																		
						String msg = "START\n";
						sendMsg(msg);
						
						QuadcontrolActivity.resume_remote();
						
						try {
							Thread.sleep(10);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}	
				}).start();
				
				//close down the activity after clicking
				//finish();
			}
		});
		
		//Button btn_init
		btn_init.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				ip = ipaddr.getText().toString();
				saveBoxes(curpid);
				
				btn_init.setEnabled(false);
				btn_init.setBackgroundColor(Color.parseColor("#808080"));
				initsent = true;
				
				new Thread(new Runnable() {
					
					@Override
					public void run() {
								
						//Send INIT order
						String msg = "INIT\n";
						sendMsg(msg);
						
						runOnUiThread(new Runnable() {
						public void run(){
							 Toast customToast = new Toast(getBaseContext());
							 customToast = Toast.makeText(getBaseContext(), "INIT Sent...", Toast.LENGTH_SHORT);
							 customToast.setGravity(Gravity.BOTTOM, 0, 15);
							 customToast.show();
							 }
						});
						
						
						//wait for Acknowledgement from remote
						//String ack = null;
						//receiveMsg(ack);
						
						//runOnUiThread(new Runnable() {
						//	public void run(){
						//		 Toast customToast = new Toast(getBaseContext());
						//		 customToast = Toast.makeText(getBaseContext(), "INIT Done...", Toast.LENGTH_SHORT);
						//		 customToast.setGravity(Gravity.BOTTOM, 0, 15);
						//		 customToast.show();
						//		 }
						//	});
						
						
						
						try {
							Thread.sleep(10);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						
						}
					
				}).start();
				
				//close down the activity after clicking
				//finish();
			}
		});

		//Button btn_exit
		btn_exit.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				ip = ipaddr.getText().toString();
				saveBoxes(curpid);
				
				new Thread(new Runnable() {
					
					@Override
					public void run() {
						QuadcontrolActivity.pause_remote();
						
						String msg = "EXIT\n";
						sendMsg(msg);	
												
						try {
							Thread.sleep(10);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						}	
				}).start();
				
				//close down the activity after clicking
				//finish();
			}
		});
		
		//Button btn_stop
		btn_stop.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				ip = ipaddr.getText().toString();
				saveBoxes(curpid);
				
				btn_start.setEnabled(true);
				btn_start.setBackgroundColor(Color.parseColor("#66CDAA"));
				
				
				new Thread(new Runnable() {
					
					@Override
					public void run() {
						QuadcontrolActivity.pause_remote();
						
						String msg = "STOP\n";
						sendMsg(msg);	
						
						try {
							Thread.sleep(10);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						}	
				}).start();
				
				//close down the activity after clicking
				//finish();
			}
		});
	
	}
	
	
	//--------------------------------------------------------------
	//Menus	
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
}
