//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////�õ����֤��������
#include "viGetCardNumberRect.h"



//�Զ��� list �����������(����blob���ĵ�X����)
bool BlobInfoNodeXSortRule(const BlobInfoNode &leftData, const BlobInfoNode &rightData)
{
	return leftData.x < rightData.x;
}

//�ڶ��ν���blob����
//���˶�ֵͼ��srcBinary�е�blob, �����µĶ�ֵͼ�񷵻ص�srcBinary
//������Ҫ���blob���뵽BiList��
void GetFurtherBlobsAndNewBinaryImg(IplImage *srcBinary, BlobInfoList &BiList)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	BlobInfoNode mbn;
	CvScalar color_blue=cvScalar(255, 0, 0); 

	IplImage *blobRGB=cvCreateImage(cvGetSize(srcBinary), IPL_DEPTH_8U, 3);
	blobs = CBlobResult( srcBinary, NULL, 0 );											//Ѱ�Ҷ�ֵͼ�е�blobs
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );
	cvMerge(  srcBinary,  srcBinary,  srcBinary, NULL, blobRGB );		
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();									//�õ���ǰ��ͨ����������
		int wi=rct.width;
		int he=rct.height;
		int Height=srcBinary->height;
		int Width=srcBinary->width;
		if (	wi*1.0/he > 0.1																		//��߱��˲�
				&& he > Height/1.0/10															//�߶��˲�
				&& wi*he > Width*Height/1.0/300											//�ַ�����˲�
				//&& wi*he < Width*Height/1.0/25		
			)			
		{
			currentBlob->FillBlob( blobRGB, color_blue);
			mbn.x=rct.x+rct.width*0.5;
			mbn.y=rct.y+rct.height*0.5;															//blobs ���ĵ�����
			mbn.width=rct.width;
			mbn.height=rct.height;
			BiList.push_back(mbn);																	//��BiList�����blob�ṹ
		}
	}

	CvScalar stemp;
	for (int i=0; i<srcBinary->height; i++)
	{
		for (int j=0; j<srcBinary->width; j++)
		{
			stemp=cvGet2D(blobRGB, i, j);
			if (stemp.val[2]!=color_blue.val[2])
			{
				((uchar *)(srcBinary->imageData+i*srcBinary->widthStep))[j]=0;
			}
		}
	}

	cvReleaseImage(&blobRGB);
}

//�õ����˺��blob��ƽ�����
double GetFilteredBlobAverageWidth(BlobInfoList &BiList)
{
	BlobInfoList::iterator myIterator;		
	int nNum=0;
	double dbAveWidth=0;
	for (myIterator = BiList.begin(); myIterator !=BiList.end(); myIterator++)		
	{
		nNum++;
		if (nNum>=5 && nNum<=15)
		{
			dbAveWidth += (*myIterator).width;
		}
	}
	dbAveWidth=dbAveWidth/11.0;

	return dbAveWidth;
}

//�õ����֤����ǰ���һ�����ֵ����ĵ��x����
//aveWidth��blobs��ƽ�����
int GetLastCharacterEdgeX(BlobInfoList &BiList, double aveWidth)
{
	BlobInfoList::iterator myIterator = BiList.begin();
	int edgeX=0;

	for (int i=0; i < 5; i++ )		
	{
		int prex=(*myIterator).x;
		myIterator++;
		int postx=(*myIterator).x;
		if ( (postx-prex) > 3*aveWidth)
		{
			edgeX=prex;
		}
	}

	return edgeX;
}

//�˳����֤����ǰ���ĺ���blob
void RemoveFrontCharBlob(IplImage *srcBinary, BlobInfoList &BiList, int edgeX)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	BlobInfoNode mbn;
	CvScalar color_blue=cvScalar(255, 0, 0); 

	IplImage *blobRGB=cvCreateImage(cvGetSize(srcBinary), IPL_DEPTH_8U, 3);
	blobs = CBlobResult( srcBinary, NULL, 0 );											//Ѱ�Ҷ�ֵͼ�е�blobs
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );
	cvMerge(  srcBinary,  srcBinary,  srcBinary, NULL, blobRGB );		
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();										//�õ���ǰ��ͨ����������
		if (rct.x > edgeX)			                                                                    ///////////////////////////////////////////////
		{
			currentBlob->FillBlob( blobRGB, color_blue);
			mbn.x=rct.x;
			mbn.y=rct.y;															
			mbn.width=rct.width;
			mbn.height=rct.height;
			BiList.push_back(mbn);															
		}
	}

	CvScalar stemp;
	for (int i=0; i<srcBinary->height; i++)
	{
		for (int j=0; j<srcBinary->width; j++)
		{
			stemp=cvGet2D(blobRGB, i, j);
			if (stemp.val[2]!=color_blue.val[2])
			{
				((uchar *)(srcBinary->imageData+i*srcBinary->widthStep))[j]=0;
			}
		}
	}

	cvReleaseImage(&blobRGB);
}

//�õ����֤����ĵ�һ�������һ���ַ�������Ϣ
BlobInfoNode GetAccuteNumRect(BlobInfoList &BiList)
{
	BlobInfoNode mbnReturn;

	BlobInfoList::iterator myIterator = BiList.begin();
	mbnReturn.x=(*myIterator).x;
	mbnReturn.y=(*myIterator).y;			//��һ������

	if (BiList.size()>=18)
	{
		for (int k=1; k <= 17; k++)
		{
			myIterator++;
			if(myIterator==BiList.end())
				break;
		}
		mbnReturn.width=(*myIterator).x+(*myIterator).width;	
		mbnReturn.height=(*myIterator).y+(*myIterator).height;	
	}
	else 
	{
		mbnReturn.width=(*BiList.rbegin()).x+(*BiList.rbegin()).width;	
		mbnReturn.height=(*BiList.rbegin()).y+(*BiList.rbegin()).height;	
	}

	return mbnReturn;
}

//�õ����֤����ĵ�һ�������һ���ַ�������Ϣ
BlobInfoNode GetCardNumberRect(IplImage *accurate_binary_img)
{
	BlobInfoList BiList;	
	BiList.clear();

	cvThreshold(accurate_binary_img, accurate_binary_img, 100, 255, CV_THRESH_BINARY);
	GetFurtherBlobsAndNewBinaryImg(accurate_binary_img, BiList);	

	BiList.sort(BlobInfoNodeXSortRule);											//����blob���ĵ�X���������������
	double aveWidth=GetFilteredBlobAverageWidth(BiList);
	int intEdgeX=GetLastCharacterEdgeX(BiList, aveWidth);
	BiList.clear();

	RemoveFrontCharBlob(accurate_binary_img, BiList, intEdgeX);	//�˳����֤����ǰ���ĺ���
	BiList.sort(BlobInfoNodeXSortRule);	
	BlobInfoNode mbn=GetAccuteNumRect(BiList);

	BiList.clear();

	return mbn;
}
