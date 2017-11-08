//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通过人脸检测粗定位身份证号码区域
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通过连通块检测确定身份证号码中心线
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////通过中心线与水平方向的夹角旋转校正图像
#include "viPreGetCardNumRect.h"


//在输入彩色图像 img 中检测人脸
//将人脸区域返回到CvRect型变量中
//分类器数据文件为cascade
CvRect  GetFacialRect( IplImage *img, CvHaarClassifierCascade *cascade)
{
	CvMemStorage *storage = cvCreateMemStorage(0);
	IplImage* gray = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvEqualizeHist( gray, gray );																//使灰度图象直方图均衡化
	cvClearMemStorage( storage );

	CvRect rMax;																					//真正的人脸区域
	rMax.x=-1;
	rMax.y=-1;
	rMax.width=-1;
	rMax.height=-1;

	//根据Haar特征检测人脸
	CvSeq* faces = cvHaarDetectObjects( gray, cascade, storage, 1.1, 2, 0, cvSize(20, 20) );
	for( int i = 0; i < (faces ? faces->total : 0); i++ )									//得到所有可能的人脸区域
	{
		CvRect* rtemp = (CvRect*)cvGetSeqElem( faces, i );
		if (rtemp->x > 0.5*img->width)														//首先区域滤波，人脸照片位于身份证的右侧
		{
			if (rtemp->width*rtemp->height > rMax.width*rMax.height)		//其次面积滤波，保留最大区域作为人脸区域
			{
				rMax.x=rtemp->x;
				rMax.y=rtemp->y;
				rMax.width=rtemp->width;
				rMax.height=rtemp->height;
			}
		}
	}

	cvReleaseImage( &gray );
	cvReleaseMemStorage( &storage );
	return rMax;																						//返回真正的人脸区域
}

//计算输入RGB彩色图像srcRGB的OTSU二值图像
//将结果反映到输出图像dstBinary中
void getBinaryImageOTSU(IplImage* srcRGB, IplImage* dstBinary)
{
	cvCvtColor(srcRGB, dstBinary, CV_BGR2GRAY); 
	cvThreshold(dstBinary, dstBinary, 25, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
}

//计算输入灰度图像srcGray的自适应化二值图像
//将结果反映到输出图像dstBinMat中
void GetBinaryImageAdaptive(IplImage* srcGray, Mat &dstBinMat)
{
	Mat srcGrayMat(srcGray);
	adaptiveThreshold(srcGrayMat, dstBinMat, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 7, 14);
}

//锁定长宽比例缩放输入图像, 长度固定为600
IplImage* ResizeInputImage(IplImage *src)
{
	IplImage *newSizeImg = cvCreateImage(cvSize(600, src->height*600/1.0/src->width), src->depth, src->nChannels);	//构造缩小后的目标图象
	cvResize(src, newSizeImg, CV_INTER_LINEAR);
	return newSizeImg;
}

//第一次进行blob操作, 粗过滤
//寻找二值图像srcBinary中的blobs
//将符合一定条件的blobs的中心点保存到Cplist中
void GetOriginalCenterPointList(IplImage *srcBinary, CenterPointList &CpList)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	CenterPointNode cpn;												//用于保存 Blob 中心点坐标

	blobs = CBlobResult( srcBinary, NULL, 0 );			//寻找二值图中的连通域
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );

	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();		//得到当前 Blob 的外包矩形
		int wi=rct.width;
		int he=rct.height;
		int Height=srcBinary->height;
		int Width=srcBinary->width;
		if (	wi*1.0/he > 0.3 && wi*1.0/he < 0.95			//宽高比滤波
				&& he > Height/1.0/10								//高度滤波
				&& wi*he > Width*Height/1.0/450				//字符面积滤波
				&& wi*he < Width*Height/1.0/100		)			
		{
			cpn.x=rct.x+rct.width*0.5;
			cpn.y=rct.y+rct.height*0.5;								//Blob中心点坐标
			CpList.push_back(cpn);									//向 Cplist 容器中添加 Blobs 的中心点坐标
		}
	}
}

//自定义 list 升序排序规则(根据blob中心点X坐标)
bool BlobCenterXSortRule(const CenterPointNode &leftData, const CenterPointNode &rightData)
{
	return leftData.x < rightData.x;
}

//确定数字字符的中心线
//筛选blob, 删除偏离了中心线的blob
//参数CpList为保存了blob中点的list
//返回blobs中点y坐标的均值
int RemovePartCenterLinePoints(CenterPointList &CpList)
{
	int sumy=0;
	int averagey=0;
	CenterPointList::iterator myIterator;		

	CpList.sort(BlobCenterXSortRule);												//根据中心点的 x 坐标进行升序排序
	for (myIterator = CpList.begin(); myIterator !=CpList.end(); myIterator++)		
	{
		sumy+= (*myIterator).y ;
		averagey++;
	}
	averagey=sumy/1.0/averagey;															//得到中心点的 y 坐标均值
	//////////////////////////////////////////////////////////////////////////////////////////////
	for (myIterator = CpList.begin(); myIterator !=CpList.end();  )				//删除不符合要求的 blobs (偏离轴线)
	{
		if ( abs((*myIterator).y-averagey)/1.0/averagey >= 0.3)
		{
			list <CenterPointNode>::iterator temp = myIterator++;   
			CpList.erase(temp);
		}
		else
			myIterator++;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
	sumy=0;
	averagey=0;
	for (myIterator = CpList.begin(); myIterator !=CpList.end(); myIterator++)		
	{
		sumy+= (*myIterator).y ;
		averagey++;
	}
	averagey=sumy/1.0/averagey;															//计算新的 y 坐标均值

	return averagey;
}

//本文使用的图像旋转方法，完全借鉴自网络文件
//旋转后的图像可自动调整至新的大小
void bound(int x, int y, float ca, float sa, int *xmin, int *xmax, int *ymin, int *ymax)
{   
	//顺时针旋转
	int rx = (int)floor(ca*(float)x+sa*(float)y);		
	int ry = (int)floor(-sa*(float)x+ca*(float)y);
	if (rx<*xmin) *xmin=rx; 
	if (rx>*xmax) *xmax=rx;
	if (ry<*ymin) *ymin=ry; 
	if (ry>*ymax) *ymax=ry;
}
IplImage* cvMyRotateImage(IplImage *src, double angle)
{
	double angle_rad  = (CV_PI* (angle/180.0)) ;
	int newheight = int (fabs(( sin(angle_rad)*src->width )) + fabs(( cos(angle_rad)*src->height )) );
	int newwidth  = int (fabs(( sin(angle_rad)*src->height)) + fabs(( cos(angle_rad)*src->width)) );
	float m[6];           
	// Matrix m looks like:
	//
	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]
	// [ m3  m4  m5 ]			[ A21  A22   b2 ]
	CvMat M = cvMat( 2, 3, CV_32F, m );
	int  xmin, xmax, ymin, ymax, sx, sy;
	float ca = (float)cos((double)(angle)*CV_PI/180.0);
	float sa = (float)sin((double)(angle)*CV_PI/180.0);
	int nx = src->width;
	int ny = src->height;
	xmin = xmax = ymin = ymax = 0;
	bound(nx-1, 0, ca, sa, &xmin, &xmax, &ymin, &ymax);
	bound(0, ny-1, ca, sa, &xmin, &xmax, &ymin, &ymax);
	bound(nx-1, ny-1, ca, sa, &xmin, &xmax, &ymin, &ymax);
	sx = xmax-xmin+1;
	sy = ymax-ymin+1;
	IplImage* rotated_dst=cvCreateImage(cvSize(sx, sy), src->depth, src->nChannels);
	m[0] = ca;
	m[1] = sa;
	m[2] =-(float)xmin; 
	m[3] =-m[1];
	m[4] = m[0];
	m[5] =-(float)ymin;
	cvWarpAffine( src, rotated_dst, &M, CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
	return rotated_dst;
}
