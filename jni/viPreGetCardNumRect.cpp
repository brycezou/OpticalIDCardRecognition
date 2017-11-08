//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ���������ֶ�λ���֤��������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����ͨ����ȷ�����֤����������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////ͨ����������ˮƽ����ļн���תУ��ͼ��
#include "viPreGetCardNumRect.h"


//�������ɫͼ�� img �м������
//���������򷵻ص�CvRect�ͱ�����
//�����������ļ�Ϊcascade
CvRect  GetFacialRect( IplImage *img, CvHaarClassifierCascade *cascade)
{
	CvMemStorage *storage = cvCreateMemStorage(0);
	IplImage* gray = cvCreateImage( cvSize(img->width, img->height), 8, 1 );
	cvCvtColor( img, gray, CV_BGR2GRAY );
	cvEqualizeHist( gray, gray );																//ʹ�Ҷ�ͼ��ֱ��ͼ���⻯
	cvClearMemStorage( storage );

	CvRect rMax;																					//��������������
	rMax.x=-1;
	rMax.y=-1;
	rMax.width=-1;
	rMax.height=-1;

	//����Haar�����������
	CvSeq* faces = cvHaarDetectObjects( gray, cascade, storage, 1.1, 2, 0, cvSize(20, 20) );
	for( int i = 0; i < (faces ? faces->total : 0); i++ )									//�õ����п��ܵ���������
	{
		CvRect* rtemp = (CvRect*)cvGetSeqElem( faces, i );
		if (rtemp->x > 0.5*img->width)														//���������˲���������Ƭλ�����֤���Ҳ�
		{
			if (rtemp->width*rtemp->height > rMax.width*rMax.height)		//�������˲����������������Ϊ��������
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
	return rMax;																						//������������������
}

//��������RGB��ɫͼ��srcRGB��OTSU��ֵͼ��
//�������ӳ�����ͼ��dstBinary��
void getBinaryImageOTSU(IplImage* srcRGB, IplImage* dstBinary)
{
	cvCvtColor(srcRGB, dstBinary, CV_BGR2GRAY); 
	cvThreshold(dstBinary, dstBinary, 25, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
}

//��������Ҷ�ͼ��srcGray������Ӧ����ֵͼ��
//�������ӳ�����ͼ��dstBinMat��
void GetBinaryImageAdaptive(IplImage* srcGray, Mat &dstBinMat)
{
	Mat srcGrayMat(srcGray);
	adaptiveThreshold(srcGrayMat, dstBinMat, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 7, 14);
}

//�������������������ͼ��, ���ȹ̶�Ϊ600
IplImage* ResizeInputImage(IplImage *src)
{
	IplImage *newSizeImg = cvCreateImage(cvSize(600, src->height*600/1.0/src->width), src->depth, src->nChannels);	//������С���Ŀ��ͼ��
	cvResize(src, newSizeImg, CV_INTER_LINEAR);
	return newSizeImg;
}

//��һ�ν���blob����, �ֹ���
//Ѱ�Ҷ�ֵͼ��srcBinary�е�blobs
//������һ��������blobs�����ĵ㱣�浽Cplist��
void GetOriginalCenterPointList(IplImage *srcBinary, CenterPointList &CpList)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	CenterPointNode cpn;												//���ڱ��� Blob ���ĵ�����

	blobs = CBlobResult( srcBinary, NULL, 0 );			//Ѱ�Ҷ�ֵͼ�е���ͨ��
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );

	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();		//�õ���ǰ Blob ���������
		int wi=rct.width;
		int he=rct.height;
		int Height=srcBinary->height;
		int Width=srcBinary->width;
		if (	wi*1.0/he > 0.3 && wi*1.0/he < 0.95			//��߱��˲�
				&& he > Height/1.0/10								//�߶��˲�
				&& wi*he > Width*Height/1.0/450				//�ַ�����˲�
				&& wi*he < Width*Height/1.0/100		)			
		{
			cpn.x=rct.x+rct.width*0.5;
			cpn.y=rct.y+rct.height*0.5;								//Blob���ĵ�����
			CpList.push_back(cpn);									//�� Cplist ��������� Blobs �����ĵ�����
		}
	}
}

//�Զ��� list �����������(����blob���ĵ�X����)
bool BlobCenterXSortRule(const CenterPointNode &leftData, const CenterPointNode &rightData)
{
	return leftData.x < rightData.x;
}

//ȷ�������ַ���������
//ɸѡblob, ɾ��ƫ���������ߵ�blob
//����CpListΪ������blob�е��list
//����blobs�е�y����ľ�ֵ
int RemovePartCenterLinePoints(CenterPointList &CpList)
{
	int sumy=0;
	int averagey=0;
	CenterPointList::iterator myIterator;		

	CpList.sort(BlobCenterXSortRule);												//�������ĵ�� x ���������������
	for (myIterator = CpList.begin(); myIterator !=CpList.end(); myIterator++)		
	{
		sumy+= (*myIterator).y ;
		averagey++;
	}
	averagey=sumy/1.0/averagey;															//�õ����ĵ�� y �����ֵ
	//////////////////////////////////////////////////////////////////////////////////////////////
	for (myIterator = CpList.begin(); myIterator !=CpList.end();  )				//ɾ��������Ҫ��� blobs (ƫ������)
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
	averagey=sumy/1.0/averagey;															//�����µ� y �����ֵ

	return averagey;
}

//����ʹ�õ�ͼ����ת��������ȫ����������ļ�
//��ת���ͼ����Զ��������µĴ�С
void bound(int x, int y, float ca, float sa, int *xmin, int *xmax, int *ymin, int *ymax)
{   
	//˳ʱ����ת
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
