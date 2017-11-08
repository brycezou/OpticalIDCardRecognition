#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <opencv/highgui.h>
#include <opencv/cv.h>
#include "viCardRecogLib.h"

using namespace cv;



#define  LOG_TAG    "Native_OpticalIdCardRecognition"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


typedef struct 
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} rgba;


IplImage* MyBmp2IplImage(AndroidBitmapInfo infoOrg, void *pixOrg)
{
	IplImage *srcCaptured = cvCreateImage(cvSize(infoOrg.width, infoOrg.height), IPL_DEPTH_8U, 3);
	CvScalar stemp;
	for( int y = 0; y < infoOrg.height; y ++ ) 
    {
    	rgba * Orgline = (rgba *) pixOrg;
    	for ( int x = 0 ; x < infoOrg.width; x++ ) 
    	{
			stemp.val[2] = Orgline[x].red;
			stemp.val[1] = Orgline[x].green;
			stemp.val[0] = Orgline[x].blue;
			cvSet2D(srcCaptured, y, x, stemp);
    	}
    	
    	pixOrg = (char *) pixOrg + infoOrg.stride;
    }
	
	return srcCaptured;
}


void MyIplImage2Bmp(AndroidBitmapInfo infoDst, IplImage *idCardCutImg, void *pixDst)
{
	CvScalar stemp;
    for( int y = 0; y < idCardCutImg->height; y ++ ) 
    {
    	rgba * Dstline = (rgba *) pixDst;
    	for ( int x = 0 ; x < idCardCutImg->width; x++ ) 
    	{
			stemp=cvGet2D(idCardCutImg, y, x);
    		Dstline[x].red = stemp.val[2];
			Dstline[x].green = stemp.val[1];
			Dstline[x].blue = stemp.val[0];
    	}
    	
    	pixDst = (char *) pixDst + infoDst.stride;
    }
}



CvRect gOrgRectBoxArray[NUM_BOX_ORG];			
CvRect gAccurateRectBoxArray[NUM_BOX_ORG];
CvRect gAbsoluteBoxRectArray[NUM_BOX_ORG];
int gBlobFilterSize[NUM_BOX_ORG] = {3, 10, 60, 2, 10, 10, 10, 5, 10};

CvRect gEveryNumRect[18];			
IplImage *iplTemplateImgArray[11];
char strNumberRecogResult[19];



extern "C"
{

JNIEXPORT jobject JNICALL Java_njust_pr_opticalidcardrecognition_OpticalIDCardRecognition_MyObjReturnTest(	JNIEnv *env,
																											jobject obj  )
{
    jclass myobj = env->FindClass("njust/pr/opticalidcardrecognition/ZcBmpInfo");
	LOGI("11111");
    jmethodID construcMID = env->GetMethodID(myobj, "<init>", "(II)V");  
	LOGI("22222");
    jobject my_obj = env->NewObject(myobj, construcMID, 11, 22);
	LOGI("33333");
    return my_obj;  

}

JNIEXPORT jobject JNICALL Java_njust_pr_opticalidcardrecognition_OpticalIDCardRecognition_RecognizeIDCard(	JNIEnv *env,
																											jobject  obj, 
																											jobject bmpOrg,
																											jobject bmpDst )
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	jclass myobj = env->FindClass("njust/pr/opticalidcardrecognition/ZcBmpInfo");
	LOGI("Get into class");
	jmethodID construcMID = env->GetMethodID(myobj, "<init>", "(IILjava/lang/String;)V");  
	LOGI("Call construct method");
	jobject objReturn;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	LOGI("Go into native method");
    AndroidBitmapInfo  infoOrg;
    void*              pixOrg; 
    AndroidBitmapInfo  infoDst;
    void*              pixDst;
    int                retValue;

    if ((retValue = AndroidBitmap_getInfo(env, bmpOrg, &infoOrg)) < 0)
    { 
		LOGE("AndroidBitmap_getInfo() failed ! error=%d", retValue); 
    	objReturn = env->NewObject( myobj, construcMID, 0, 0, env->NewStringUTF("0"));
		return objReturn; 
	}
    if ((retValue = AndroidBitmap_getInfo(env, bmpDst, &infoDst)) < 0)
    { 
		LOGE("AndroidBitmap_getInfo() failed ! error=%d", retValue); 
		objReturn = env->NewObject( myobj, construcMID, 0, 0, env->NewStringUTF("0"));
		return objReturn; 
	}
    if (infoOrg.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
    { 
		LOGE("Bitmap format is not RGBA_8888 !");     	
		objReturn = env->NewObject( myobj, construcMID, 0, 0, env->NewStringUTF("0"));
		return objReturn;  
	}
    if (infoDst.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
    { 
		LOGE("Bitmap format is not RGBA_8888 !");     	
		objReturn = env->NewObject( myobj, construcMID, 0, 0, env->NewStringUTF("0"));
		return objReturn;  
	}
    if ((retValue = AndroidBitmap_lockPixels(env, bmpOrg, &pixOrg)) < 0)
    { 
		LOGE("AndroidBitmap_lockPixels() failed ! error=%d", retValue); 
		objReturn = env->NewObject( myobj, construcMID, 0, 0, env->NewStringUTF("0"));
		return objReturn;  
	}
    if ((retValue = AndroidBitmap_lockPixels(env, bmpDst, &pixDst)) < 0)
    { 
		LOGE("AndroidBitmap_lockPixels() failed ! error=%d", retValue); 
		objReturn = env->NewObject( myobj, construcMID, 0, 0, env->NewStringUTF("0"));
		return objReturn;  
	}


	LOGI("Bmp2IplImage");
	IplImage *srcCaptured=MyBmp2IplImage(infoOrg, pixOrg);
	LOGI("Begin to process image");
	IplImage *idCardCutImg=RecognizeIDCardNumber(srcCaptured);
	cvReleaseImage(&srcCaptured);
	if (idCardCutImg)
	{
		LOGI("successful");
		MyIplImage2Bmp(infoDst, idCardCutImg, pixDst);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		jstring strReturn = env->NewStringUTF(strNumberRecogResult);
    	objReturn = env->NewObject( myobj, construcMID, cvGetSize(idCardCutImg).width, 
											cvGetSize(idCardCutImg).height, strReturn);
		LOGI("Java object created");
		cvReleaseImage(&idCardCutImg);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		LOGI("Unlocking pixels");
		AndroidBitmap_unlockPixels(env, bmpOrg);
		AndroidBitmap_unlockPixels(env, bmpDst);

		return objReturn;
	}
	else
	{
		LOGI("failed");
		objReturn = env->NewObject( myobj, construcMID, 0, 0, env->NewStringUTF("0"));
		return objReturn;  
	}
}


JNIEXPORT void JNICALL Java_njust_pr_opticalidcardrecognition_OpticalIDCardRecognition_CutImageFromSource(	JNIEnv *env,
																											jobject  obj, 
																											jobject bmpOrg,
																											jobject bmpDst )
{
	LOGI("Go into native method");
    AndroidBitmapInfo  infoOrg;
    void*              pixOrg; 
    AndroidBitmapInfo  infoDst;
    void*              pixDst;
    int                retValue;

    if ((retValue = AndroidBitmap_getInfo(env, bmpOrg, &infoOrg)) < 0)
    { LOGE("AndroidBitmap_getInfo() failed ! error=%d", retValue); return; }
    if ((retValue = AndroidBitmap_getInfo(env, bmpDst, &infoDst)) < 0)
    { LOGE("AndroidBitmap_getInfo() failed ! error=%d", retValue); return; }
    if (infoOrg.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
    { LOGE("Bitmap format is not RGBA_8888 !");  return; }
    if (infoDst.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
    {  LOGE("Bitmap format is not RGBA_8888 !"); return; }
    if ((retValue = AndroidBitmap_lockPixels(env, bmpOrg, &pixOrg)) < 0)
    { LOGE("AndroidBitmap_lockPixels() failed ! error=%d", retValue); return; }
    if ((retValue = AndroidBitmap_lockPixels(env, bmpDst, &pixDst)) < 0)
    { LOGE("AndroidBitmap_lockPixels() failed ! error=%d", retValue); return; }

	for( int y = 0; y < infoDst.height; y ++ ) 
    {
    	rgba * orgline = (rgba *) pixOrg;
		rgba * dstline = (rgba *) pixDst;
    	for ( int x = 0 ; x < infoDst.width; x++ ) 
    	{
			dstline[x].red = orgline[x].red;
			dstline[x].green = orgline[x].green;
			dstline[x].blue = orgline[x].blue;
			dstline[x].alpha = orgline[x].alpha;
    	}
    	
    	pixOrg = (char *) pixOrg + infoOrg.stride;
    	pixDst = (char *) pixDst + infoDst.stride;

    }

	LOGI("Unlocking pixels");
    AndroidBitmap_unlockPixels(env, bmpOrg);
    AndroidBitmap_unlockPixels(env, bmpDst);
	
	return;
}

}


