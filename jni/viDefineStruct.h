#include <list> 

using namespace std;


//center point of one character
typedef struct CenterPointNode
{
	int x;
	int y;
}CenterPointNode;

//information of one blob
typedef struct BlobInfoNode
{
	int x;
	int y;			//center point of one blob
	int width;
	int height;
}BlobInfoNode;

typedef list<CenterPointNode> CenterPointList; 
typedef list<BlobInfoNode> BlobInfoList; 