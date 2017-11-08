//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////得到所有预定义的文字区域
#include "viGetPreDefinedBoxes.h"



void GetLabelAeras(IplImage *corrected_RGB_img, int real_width, int real_height, bool whetherDraw)
{
	gOrgRectBoxArray[NAME_ORG].x=real_width*NAME_SX;
	gOrgRectBoxArray[NAME_ORG].y=real_height*NAME_SY;
	gOrgRectBoxArray[NAME_ORG].width=real_width*(NAME_BX-NAME_SX);
	gOrgRectBoxArray[NAME_ORG].height=real_height*(NAME_BY-NAME_SY);

	gOrgRectBoxArray[SEX_ORG].x=real_width*SEX_SX;
	gOrgRectBoxArray[SEX_ORG].y=real_height*SEX_SY;
	gOrgRectBoxArray[SEX_ORG].width=real_width*(SEX_BX-SEX_SX);
	gOrgRectBoxArray[SEX_ORG].height=real_height*(SEX_BY-SEX_SY);

	gOrgRectBoxArray[MINZU_ORG].x=real_width*MINZU_SX;
	gOrgRectBoxArray[MINZU_ORG].y=real_height*MINZU_SY;
	gOrgRectBoxArray[MINZU_ORG].width=real_width*(MINZU_BX-MINZU_SX);
	gOrgRectBoxArray[MINZU_ORG].height=real_height*(MINZU_BY-MINZU_SY);

	gOrgRectBoxArray[YEAR_ORG].x=real_width*YEAR_SX;
	gOrgRectBoxArray[YEAR_ORG].y=real_height*YEAR_SY;
	gOrgRectBoxArray[YEAR_ORG].width=real_width*(YEAR_BX-YEAR_SX);
	gOrgRectBoxArray[YEAR_ORG].height=real_height*(YEAR_BY-YEAR_SY);

	gOrgRectBoxArray[MONTH_ORG].x=real_width*MONTH_SX;
	gOrgRectBoxArray[MONTH_ORG].y=real_height*MONTH_SY;
	gOrgRectBoxArray[MONTH_ORG].width=real_width*(MONTH_BX-MONTH_SX);
	gOrgRectBoxArray[MONTH_ORG].height=real_height*(MONTH_BY-MONTH_SY);

	gOrgRectBoxArray[DAY_ORG].x=real_width*DAY_SX;
	gOrgRectBoxArray[DAY_ORG].y=real_height*DAY_SY;
	gOrgRectBoxArray[DAY_ORG].width=real_width*(DAY_BX-DAY_SX);
	gOrgRectBoxArray[DAY_ORG].height=real_height*(DAY_BY-DAY_SY);

	gOrgRectBoxArray[ADDRESS_ORG].x=real_width*ADDRESS_SX;
	gOrgRectBoxArray[ADDRESS_ORG].y=real_height*ADDRESS_SY;
	gOrgRectBoxArray[ADDRESS_ORG].width=real_width*(ADDRESS_BX-ADDRESS_SX);
	gOrgRectBoxArray[ADDRESS_ORG].height=real_height*(ADDRESS_BY-ADDRESS_SY);

	gOrgRectBoxArray[PICTURE_ORG].x=real_width*PICTURE_SX;
	gOrgRectBoxArray[PICTURE_ORG].y=real_height*PICTURE_SY;
	gOrgRectBoxArray[PICTURE_ORG].width=real_width*(PICTURE_BX-PICTURE_SX);
	gOrgRectBoxArray[PICTURE_ORG].height=real_height*(PICTURE_BY-PICTURE_SY);

	gOrgRectBoxArray[NUMBER_ORG].x=real_width*NUMBER_SX;
	gOrgRectBoxArray[NUMBER_ORG].y=real_height*NUMBER_SY;
	gOrgRectBoxArray[NUMBER_ORG].width=real_width*(NUMBER_BX-NUMBER_SX);
	gOrgRectBoxArray[NUMBER_ORG].height=real_height*(NUMBER_BY-NUMBER_SY);

	if (whetherDraw)
	{
		cvRectangle(corrected_RGB_img, cvPoint(real_width*NAME_SX, real_height*NAME_SY), cvPoint(real_width*NAME_BX, real_height*NAME_BY), cvScalar(0, 0, 255), 1);								//姓名
		cvRectangle(corrected_RGB_img, cvPoint(real_width*SEX_SX, real_height*SEX_SY), cvPoint(real_width*SEX_BX, real_height*SEX_BY), cvScalar(0, 0, 255), 1);													//性别
		cvRectangle(corrected_RGB_img, cvPoint(real_width*MINZU_SX, real_height*MINZU_SY), cvPoint(real_width*MINZU_BX, real_height*MINZU_BY), cvScalar(0, 0, 255), 1);							//民族
		cvRectangle(corrected_RGB_img, cvPoint(real_width*YEAR_SX, real_height*YEAR_SY), cvPoint(real_width*YEAR_BX, real_height*YEAR_BY), cvScalar(0, 0, 255), 1);									//年
		cvRectangle(corrected_RGB_img, cvPoint(real_width*MONTH_SX, real_height*MONTH_SY), cvPoint(real_width*MONTH_BX, real_height*MONTH_BY), cvScalar(0, 0, 255), 1);						//月
		cvRectangle(corrected_RGB_img, cvPoint(real_width*DAY_SX, real_height*DAY_SY), cvPoint(real_width*DAY_BX, real_height*DAY_BY), cvScalar(0, 0, 255), 1);											//日
		cvRectangle(corrected_RGB_img, cvPoint(real_width*ADDRESS_SX, real_height*ADDRESS_SY), cvPoint(real_width*ADDRESS_BX, real_height*ADDRESS_BY), cvScalar(0, 0, 255), 1);		//住址
		cvRectangle(corrected_RGB_img, cvPoint(real_width*PICTURE_SX, real_height*PICTURE_SY), cvPoint(real_width*PICTURE_BX, real_height*PICTURE_BY), cvScalar(0, 0, 255), 1);				//照片
		cvRectangle(corrected_RGB_img, cvPoint(real_width*NUMBER_SX, real_height*NUMBER_SY), cvPoint(real_width*NUMBER_BX, real_height*NUMBER_BY), cvScalar(0, 0, 255), 1);			//身份证号码
	}
}

//得到所有预定义的文字区域，并将其存入数组gOrgRectBoxArray中
IplImage* GetPreDefinedAboutBoxes(IplImage* src, CvHaarClassifierCascade *cascade)
{
	CenterPointList CpList;									
	BlobInfoList BiList;			
	IplImage *small_img=ResizeInputImage(src);

	CvRect rface=GetFacialRect(small_img, cascade);		//检测得到身份证上的人脸区域
	if (rface.x != -1)
	{
		CvRect rtNumberAera;											//包含身份证号码的区域
		rtNumberAera.x=rface.x-rface.width*2.31;
		rtNumberAera.y=rface.y+rface.height*1.36;
		rtNumberAera.width=rface.width*3.58;
		rtNumberAera.height=rface.height*0.9;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////获得包含身份证号码区域的自适应二值图像binary
		cvSetImageROI(small_img, rtNumberAera);
		IplImage *gray=cvCreateImage(cvGetSize(small_img), IPL_DEPTH_8U, 1);
		cvCvtColor(small_img, gray, CV_BGR2GRAY); 
		cvResetImageROI(small_img);
		Mat binaryMat;
		GetBinaryImageAdaptive(gray, binaryMat);
		cvReleaseImage(&gray);
		IplImage binary=binaryMat;															
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		GetOriginalCenterPointList(&binary, CpList);							//提取二值图像中的blobs
		int averagey=RemovePartCenterLinePoints(CpList);				//筛选blobs并得到其中心点的 y 坐标均值
		if (CpList.size()<4)
		{
			cvReleaseImage(&small_img);
			CpList.clear();		
			return NULL;
		}
		CenterPointNode SndPt=*(++CpList.begin());							//选取第二个和倒数第二个点作为中心线的端点
		CenterPointNode DSndPt=*(++CpList.rbegin());			
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		float average_dx=(DSndPt.x-SndPt.x)/1.0/(CpList.size()-2);	//求两个数字的平均 x 间隔
		if(DSndPt.x-SndPt.x)
		{
			double angle=atan((DSndPt.y-SndPt.y)/1.0/(DSndPt.x-SndPt.x));
			angle=angle*180.0/1.0/CV_PI;
			IplImage *rotated_binary_img=cvMyRotateImage(&binary, angle);
			IplImage *rotated_rgb_img=cvMyRotateImage(small_img, angle);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////得到包含身份证号码的较精确区域图像accurate_binary_img
			CvRect rt;		
			rt.x=2*average_dx;
			rt.y=averagey-average_dx;
			rt.width=rotated_binary_img->width-rt.x;
			rt.height=2.5*average_dx;
			cvSetImageROI(rotated_binary_img, rt);
			IplImage *accurate_binary_img=cvCreateImage(cvGetSize(rotated_binary_img), rotated_binary_img->depth, rotated_binary_img->nChannels);
			cvCopy(rotated_binary_img, accurate_binary_img, NULL);
			cvResetImageROI(rotated_binary_img);
			cvReleaseImage(&rotated_binary_img);
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////得到包含身份证号码的精确区域mbn
			BlobInfoNode mbn=GetCardNumberRect(accurate_binary_img);
			cvReleaseImage(&accurate_binary_img);
			int lefttopX=rt.x+rtNumberAera.x+mbn.x;
			int lefttopY=rt.y+rtNumberAera.y+mbn.y;
			int numberHeight=mbn.height-mbn.y;
			int numberWidth=mbn.width-mbn.x;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			CvRect rtFinalCard;													//估计出的身份证区域
			rtFinalCard.x=lefttopX-181.0/269.0*numberWidth;
			rtFinalCard.y=lefttopY-263.0/269.0*numberWidth;
			rtFinalCard.width=498.0/269.0*numberWidth;
			rtFinalCard.height=308.0/269.0*numberWidth;
			cvSetImageROI(rotated_rgb_img, rtFinalCard);
			GetLabelAeras(rotated_rgb_img, cvGetSize(rotated_rgb_img).width, cvGetSize(rotated_rgb_img).height, FALSE);		//在身份证上标记出各个部分
			IplImage *finalBoxesImg=cvCreateImage(cvGetSize(rotated_rgb_img), rotated_rgb_img->depth, rotated_rgb_img->nChannels);	
			cvCopy(rotated_rgb_img, finalBoxesImg, NULL);
			cvResetImageROI(rotated_rgb_img);
			cvReleaseImage(&rotated_rgb_img);

			cvReleaseImage(&small_img);
			BiList.clear();	
			CpList.clear();

			return finalBoxesImg;
		}
		else
		{
			cvReleaseImage(&small_img);
			CpList.clear();		
			return NULL;
		}
	}
	else
	{
		cvReleaseImage(&small_img);
		return NULL;
	}
}

//寻找二值图像srcBinary中可能涉及到文字的blob
//将估计出的精确区域返回到rtAccurate中
void GetTextContainedBlobs(IplImage *srcBinary, CvRect &rtAccurate, int blobFilterSize)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	bool IsFind=false;

	blobs = CBlobResult( srcBinary, NULL, 0 );			//寻找二值图中的连通域
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, blobFilterSize );

	int minx=10000, maxx=-1, miny=10000, maxy=-1;
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();		//得到当前 Blob 的外包矩形
		int wi=rct.width;
		int he=rct.height;
		int Height=srcBinary->height;
		int Width=srcBinary->width;
		if ( wi*he > Width*Height/1.0/200 )		
		{
			if(rct.x<minx)	minx=rct.x;
			if(rct.y<miny)	miny=rct.y;
			if(rct.x+rct.width>maxx)	maxx=rct.x+rct.width;
			if(rct.y+rct.height>maxy)	maxy=rct.y+rct.height;
			IsFind=true;
		}
	}

	if (IsFind)
	{
		rtAccurate.x=minx;
		rtAccurate.y=miny;
		rtAccurate.width=maxx-minx;
		rtAccurate.height=maxy-miny;
	}
	else
	{
		rtAccurate.x=0;
		rtAccurate.y=0;
		rtAccurate.width=srcBinary->width;
		rtAccurate.height=srcBinary->height;
	}
}

//根据每一个小区域gOrgRectBoxArray[]的绝对坐标和
//该区域内文字所在精确区域gAccurateRectBoxArray[]的相对坐标
//得到文字所在精确区域的绝对坐标gAbsoluteBoxRectArray
void GetAbsoluteBoxRect()
{
	int intSX, intSY, intBX, intBY;
	for (int i=0; i<NUM_BOX_ORG; i++)
	{
		intSX=gOrgRectBoxArray[i].x+gAccurateRectBoxArray[i].x;
		intSY=gOrgRectBoxArray[i].y+gAccurateRectBoxArray[i].y;
		intBX=intSX+gAccurateRectBoxArray[i].width;
		intBY=intSY+gAccurateRectBoxArray[i].height;
		gAbsoluteBoxRectArray[i].x=intSX-1;
		gAbsoluteBoxRectArray[i].y=intSY-1;
		gAbsoluteBoxRectArray[i].width=intBX+1;		//其实gAbsoluteBoxRectArray中存储的不是真正的CvRect结构，而是该结构的左上角和右下角的坐标值
		gAbsoluteBoxRectArray[i].height=intBY+1;
	}
}

//得到文字所在的精确区域, 绝对坐标
//存储在gAbsoluteBoxRectArray[]中
void GetAbsoluteAccurateBoxRects(IplImage *img)
{
	for (int j=0; j<NUM_BOX_ORG; j++)					//得到文字所在的精确区域，相对坐标；存储在gAccurateRectBoxArray[]中
	{
		cvSetImageROI(img, gOrgRectBoxArray[j] );
		IplImage *binary=cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		getBinaryImageOTSU(img, binary);
		GetTextContainedBlobs(binary, gAccurateRectBoxArray[j], gBlobFilterSize[j]);
		cvReleaseImage(&binary);
		cvResetImageROI(img);
	}
	GetAbsoluteBoxRect();											//得到文字所在的精确区域，绝对坐标；存储在gAbsoluteBoxRectArray[]中
}



