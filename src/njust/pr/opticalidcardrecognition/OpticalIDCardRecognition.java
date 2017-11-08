package njust.pr.opticalidcardrecognition;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.Config;
import android.graphics.Matrix;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

@SuppressLint("HandlerLeak")
public class OpticalIDCardRecognition extends Activity 
{
	private String TAG = "JAVA_OpticalIdCardRecognition";
	private int SCALE = 4;
	ImageView imgImageIn = null;
	ImageView imgImageOut = null;
	TextView txtResult = null;
	private Bitmap bmpOrigIn = null;
	private Bitmap bmpMiddleOut = null;
	private Bitmap bmpDstOut = null;
	boolean bIsProcessingImg =  false;
	
	
	static 
	{
		System.loadLibrary("JniOpenCVDemo");
	}
	
	public native ZcBmpInfo RecognizeIDCard(Bitmap bmpIn, Bitmap bmpOut);
	public native void CutImageFromSource(Bitmap bmpIn, Bitmap bmpOut);
	
	@SuppressLint("SdCardPath")
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		this.imgImageIn = (ImageView)findViewById(R.id.imgBmpIn);
		this.imgImageOut = (ImageView)findViewById(R.id.imgBmpOut);
		this.txtResult = (TextView)findViewById(R.id.tvResult);
		
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inPreferredConfig = Config.ARGB_8888;
        bmpDstOut = BitmapFactory.decodeResource(this.getResources(), R.drawable.idcard, options);
        if (bmpDstOut != null) 
        {
        	imgImageIn.setImageBitmap(bmpDstOut);
        	imgImageOut.setImageBitmap(bmpDstOut);
        }   	
        
        String []fileNameArray={"0.jpg", "1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg", "6.jpg", "7.jpg", "8.jpg", "9.jpg", "x.jpg", "haarcascade_frontalface_alt.xml"};
       if(isFolderExists("/sdcard/OpticalIDCardRecognition/"))
       {
    	   for(int i=0; i<fileNameArray.length; i++)
    	   {
    		   if(!copyFile(fileNameArray[i], "/sdcard/OpticalIDCardRecognition/"+fileNameArray[i]))
    			   Toast.makeText(this, "文件"+fileNameArray[i]+"读取失败!", Toast.LENGTH_SHORT).show();  
    	   }
       }       
	}

	public void OnButtonRecognizeIDCard(View view) 
	{
		if(bIsProcessingImg)
			return;
		
		Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		Uri imageUri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),"image.jpg"));
        intent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
		startActivityForResult(intent, 11);
    }
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		super.onActivityResult(requestCode, resultCode, data);
		if(requestCode == 11)
		{
			if(resultCode == Activity.RESULT_OK)
			{
				if(bmpOrigIn != null)
					bmpOrigIn.recycle();
				if(bmpMiddleOut != null)
					bmpMiddleOut.recycle();
				
				bmpOrigIn = BitmapFactory.decodeFile(Environment.getExternalStorageDirectory()+"/image.jpg");
				Bitmap newBitmap = zoomBitmap(bmpOrigIn, bmpOrigIn.getWidth() / SCALE, bmpOrigIn.getHeight() / SCALE);
				imgImageIn.setImageBitmap(newBitmap);

				new Thread(
						new Runnable()
						{
							public void run()
							{
								bmpMiddleOut = Bitmap.createBitmap(bmpOrigIn.getWidth(), bmpOrigIn.getHeight(), Config.ARGB_8888);
								bIsProcessingImg = true;

								Log.i(TAG, "Before image processing");
								ZcBmpInfo zmi = RecognizeIDCard(bmpOrigIn, bmpMiddleOut);
								Log.i(TAG, "Image processed");
								
								if(bmpDstOut != null)
									bmpDstOut.recycle();
								
								if(zmi.width != 0)
								{			
									bmpDstOut = Bitmap.createBitmap(zmi.width, zmi.height, Config.ARGB_8888);
									CutImageFromSource(bmpMiddleOut, bmpDstOut);
									
									Bundle data=new Bundle();
									data.putString("result", zmi.strNumber);
									Message message=handler.obtainMessage();
									message.what=1;
									message.setData(data);
									handler.sendMessage(message);
								}
								else
								{
									Message message=handler.obtainMessage();
									message.what=2;
									handler.sendMessage(message);
								}
							}
						}).start();
			}
		}
	}
	
    public Bitmap zoomBitmap(Bitmap bitmap, int width, int height) 
    {
        int w = bitmap.getWidth();
        int h = bitmap.getHeight();
        Matrix matrix = new Matrix();
        float scaleWidth = ((float) width / w);
        float scaleHeight = ((float) height / h);
        matrix.postScale(scaleWidth, scaleHeight);
        Bitmap newbmp = Bitmap.createBitmap(bitmap, 0, 0, w, h, matrix, true);
        return newbmp;
    }
    
	public Handler handler=new Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
			switch(msg.what)
			{
			case 1:
				imgImageOut.setImageBitmap(bmpDstOut);
				txtResult.setText("身份证号码为: "+msg.getData().getString("result"));
				bIsProcessingImg = false;
				break;
			case 2:
				OnShowDlg("身份证识别错误，请重新拍摄!");
				bIsProcessingImg = false;
				break;
			}
		}
	};
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) 
	{
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	
	boolean isFolderExists(String strFolder)
	 {
		 File file = new File(strFolder);     
		 if (!file.exists())
		 {
			 if (file.mkdir())
			 {
				 return true;
			 }
			 else
				 return false;
		 }
		 return true;
	}
	 
    private boolean copyFile(String fileFromName, String toDir) 
    {  
        try 
        {  
            InputStream its = getAssets().open(fileFromName);  
            int fileLength = its.available();  
            File book_file = new File(toDir);  
            if(book_file.exists())
            {
            	its.close(); 
            	return true;
            }
            book_file.createNewFile();  

            FileOutputStream fots = new FileOutputStream(book_file, true);   
            byte[] buffer = new byte[fileLength];  
            int readCount = 0;
            while (readCount < fileLength) 
            {  
                readCount += its.read(buffer, readCount, fileLength - readCount);  
            }  
            fots.write(buffer, 0, fileLength);  
      
            its.close();  
            fots.close();  
            
            return true;  
        } 
        catch (IOException e)
        {  
            return false;  
        }  
    }  
    
	public void OnShowDlg(String msg)
	{         
    	Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
    }
}
