//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////得到身份证号码区域
#include "viGetCardNumberRect.h"



//自定义 list 升序排序规则(根据blob中心点X坐标)
bool BlobInfoNodeXSortRule(const BlobInfoNode &leftData, const BlobInfoNode &rightData)
{
	return leftData.x < rightData.x;
}

//第二次进行blob操作
//过滤二值图像srcBinary中的blob, 并将新的二值图像返回到srcBinary
//将符合要求的blob加入到BiList中
void GetFurtherBlobsAndNewBinaryImg(IplImage *srcBinary, BlobInfoList &BiList)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	BlobInfoNode mbn;
	CvScalar color_blue=cvScalar(255, 0, 0); 

	IplImage *blobRGB=cvCreateImage(cvGetSize(srcBinary), IPL_DEPTH_8U, 3);
	blobs = CBlobResult( srcBinary, NULL, 0 );											//寻找二值图中的blobs
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );
	cvMerge(  srcBinary,  srcBinary,  srcBinary, NULL, blobRGB );		
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();									//得到当前连通块的外包矩形
		int wi=rct.width;
		int he=rct.height;
		int Height=srcBinary->height;
		int Width=srcBinary->width;
		if (	wi*1.0/he > 0.1																		//宽高比滤波
				&& he > Height/1.0/10															//高度滤波
				&& wi*he > Width*Height/1.0/300											//字符面积滤波
				//&& wi*he < Width*Height/1.0/25		
			)			
		{
			currentBlob->FillBlob( blobRGB, color_blue);
			mbn.x=rct.x+rct.width*0.5;
			mbn.y=rct.y+rct.height*0.5;															//blobs 中心点坐标
			mbn.width=rct.width;
			mbn.height=rct.height;
			BiList.push_back(mbn);																	//向BiList中添加blob结构
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

//得到过滤后的blob的平均宽度
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

//得到身份证号码前最后一个汉字的中心点的x坐标
//aveWidth是blobs的平均宽度
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

//滤除身份证号码前方的汉字blob
void RemoveFrontCharBlob(IplImage *srcBinary, BlobInfoList &BiList, int edgeX)
{
	CBlobResult blobs;
	CBlob *currentBlob;
	BlobInfoNode mbn;
	CvScalar color_blue=cvScalar(255, 0, 0); 

	IplImage *blobRGB=cvCreateImage(cvGetSize(srcBinary), IPL_DEPTH_8U, 3);
	blobs = CBlobResult( srcBinary, NULL, 0 );											//寻找二值图中的blobs
	blobs.Filter( blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, 10 );
	cvMerge(  srcBinary,  srcBinary,  srcBinary, NULL, blobRGB );		
	for (int j = 0; j < blobs.GetNumBlobs(); j++ )
	{
		currentBlob = blobs.GetBlob(j);
		CvRect rct=currentBlob->GetBoundingBox();										//得到当前连通块的外包矩形
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

//得到身份证号码的第一个和最后一个字符坐标信息
BlobInfoNode GetAccuteNumRect(BlobInfoList &BiList)
{
	BlobInfoNode mbnReturn;

	BlobInfoList::iterator myIterator = BiList.begin();
	mbnReturn.x=(*myIterator).x;
	mbnReturn.y=(*myIterator).y;			//第一个数字

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

//得到身份证号码的第一个和最后一个字符坐标信息
BlobInfoNode GetCardNumberRect(IplImage *accurate_binary_img)
{
	BlobInfoList BiList;	
	BiList.clear();

	cvThreshold(accurate_binary_img, accurate_binary_img, 100, 255, CV_THRESH_BINARY);
	GetFurtherBlobsAndNewBinaryImg(accurate_binary_img, BiList);	

	BiList.sort(BlobInfoNodeXSortRule);											//根据blob中心点X坐标进行升序排序
	double aveWidth=GetFilteredBlobAverageWidth(BiList);
	int intEdgeX=GetLastCharacterEdgeX(BiList, aveWidth);
	BiList.clear();

	RemoveFrontCharBlob(accurate_binary_img, BiList, intEdgeX);	//滤除身份证号码前方的汉字
	BiList.sort(BlobInfoNodeXSortRule);	
	BlobInfoNode mbn=GetAccuteNumRect(BiList);

	BiList.clear();

	return mbn;
}
