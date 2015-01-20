#include "types.h"
#include "user.h"
#include "bitmap.h"


void loadBitmap(PICNODE *pic, char pic_name[])
{
	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	char *BmpFileHeader;
	WORD *temp_WORD;
	DWORD *temp_DWORD;
	RGBQUAD *pRgb;
	BYTE mixIndex;
	WORD mixIndex16;
	int fd, n, i, j, k, index = 0;
	int width = bitInfoHead.biWidth;
	int height = bitInfoHead.biHeight;

	if((fd = open(pic_name, 0)) < 0){
		  printf(0,"cannot open %s\n", pic_name);
		  return;
	}

	//pfile = fopen(strFile,"rb");//打开文件
	BmpFileHeader=(char *)malloc(14*sizeof(char));

	printf(0, "file bmp open success.\n");
	//读取位图文件头信息
	n = read(fd, BmpFileHeader, sizeof(BmpFileHeader));
	//fread(BmpFileHeader,1,14,pfile);
	temp_WORD=(WORD* )(BmpFileHeader);
	bitHead.bfType=*temp_WORD;
	if(bitHead.bfType != 0x4d42)
	{
		printf(0, "file is not .bmp file!");
		return;
	}
	temp_DWORD=(DWORD *)(BmpFileHeader+sizeof(bitHead.bfType));
	bitHead.bfSize=*temp_DWORD;
	temp_WORD=(WORD*)(BmpFileHeader+sizeof(bitHead.bfType)+sizeof(bitHead.bfSize));
	bitHead.bfReserved1=*temp_WORD;
	temp_WORD=(WORD*)(BmpFileHeader+sizeof(bitHead.bfType)+sizeof(bitHead.bfSize)+sizeof(bitHead.bfReserved1));
	bitHead.bfReserved2=*temp_WORD;
	temp_DWORD=(DWORD*)(BmpFileHeader+sizeof(bitHead.bfType)+sizeof(bitHead.bfSize)+sizeof(bitHead.bfReserved1)+sizeof(bitHead.bfReserved2));
	bitHead.bfOffBits=*temp_DWORD;

//	showBmpHead(&bitHead);
//	printf("\n\n");
//
//	//读取位图信息头信息
//	fread(&bitInfoHead,1,sizeof(BITMAPINFOHEADER),pfile);
//	showBmpInforHead(&bitInfoHead);
//	printf("\n");
	if (n == 0)
	{
		printf(0, "0");
	}


	if(bitInfoHead.biBitCount < 24)//有调色板
	{
		//读取调色盘结信息
		int nPlantNum = (2 << (bitInfoHead.biBitCount-1)); // Mix color Plant Number;
		pRgb=(RGBQUAD *)malloc(nPlantNum*sizeof(RGBQUAD));
		memset(pRgb,0,(uint)nPlantNum*sizeof(RGBQUAD));
		n = read(fd,pRgb,nPlantNum);
//		printf("Color Plate Number: %d\n",nPlantNum);
//		printf("颜色板信息:\n");
//		for (int i =0; i<nPlantNum;i++)
//		{
//			if (i%5==0)
//			{
//				printf("\n");
//			}
//			showRgbQuan(&pRgb[i]);
//		}
//		printf("\n");
	}


	//分配内存空间把源图存入内存
	int l_width = WIDTHBYTES(width* bitInfoHead.biBitCount);//计算位图的实际宽度并确保它为32的倍数
	BYTE *pColorData=(BYTE *)malloc(height*l_width);
	memset(pColorData,0,(uint)height*l_width);
	long nData = height*l_width;
	//把位图数据信息读到数组里
	read(fd, pColorData, nData);
	//fread(pColorData,1,nData,pfile);

	//将位图数据转化为RGB数据
	RGBQUAD* dataOfBmp;
	dataOfBmp = (RGBQUAD *)malloc(width*height*sizeof(RGBQUAD));//用于保存各像素对应的RGB数据
	memset(dataOfBmp,0,(uint)width*height*sizeof(RGBQUAD));
	if(bitInfoHead.biBitCount<24)//有调色板，即位图为非真彩色
	{
		index = 0;
		if (bitInfoHead.biBitCount == 1)
		{
			for(i=0;i<height;i++)
				for(j=0;j<width;j++)
				{
					mixIndex= 0;
					k = i*l_width + j/8;//k:取得该像素颜色数据在实际数据数组中的序号
					//j:提取当前像素的颜色的具体值
					mixIndex = pColorData[k];
					switch(j%8)
					{
					case 0:
						mixIndex = mixIndex<<7;
						mixIndex = mixIndex>>7;
						break;
					case 1:
						mixIndex = mixIndex<<6;
						mixIndex = mixIndex>>7;
						break;
					case 2:
						mixIndex = mixIndex<<5;
						mixIndex = mixIndex>>7;
						break;
					case 3:
						mixIndex = mixIndex<<4;
						mixIndex = mixIndex>>7;
						break;
					case 4:
						mixIndex = mixIndex<<3;
						mixIndex = mixIndex>>7;
						break;
					case 5:
						mixIndex = mixIndex<<2;
						mixIndex = mixIndex>>7;
						break;
					case 6:
						mixIndex = mixIndex<<1;
						mixIndex = mixIndex>>7;
						break;
					case 7:
						mixIndex = mixIndex>>7;
						break;
					}
					//将像素数据保存到数组中对应的位置
					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;
				}
		}
		if(bitInfoHead.biBitCount==2)
		{
			for(i=0;i<height;i++)
				for(j=0;j<width;j++)
				{
					mixIndex= 0;
					k = i*l_width + j/4;//k:取得该像素颜色数据在实际数据数组中的序号
					//j:提取当前像素的颜色的具体值
					mixIndex = pColorData[k];
					switch(j%4)
					{
					case 0:
						mixIndex = mixIndex<<6;
						mixIndex = mixIndex>>6;
						break;
					case 1:
						mixIndex = mixIndex<<4;
						mixIndex = mixIndex>>6;
						break;
					case 2:
						mixIndex = mixIndex<<2;
						mixIndex = mixIndex>>6;
						break;
					case 3:
						mixIndex = mixIndex>>6;
						break;
					}
					//将像素数据保存到数组中对应的位置
					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;

				}
		}
		if(bitInfoHead.biBitCount == 4)
		{
			for(i=0;i<height;i++)
				for(j=0;j<width;j++)
				{
					mixIndex= 0;
					k = i*l_width + j/2;
					mixIndex = pColorData[k];
					if(j%2==0)
					{//低
						mixIndex = mixIndex<<4;
						mixIndex = mixIndex>>4;
					}
					else
					{//高
						mixIndex = mixIndex>>4;
					}
					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;
				}
		}
		if(bitInfoHead.biBitCount == 8)
		{
			for(i=0;i<height;i++)
				for(j=0;j<width;j++)
				{
					mixIndex= 0;
					k = i*l_width + j;
					mixIndex = pColorData[k];
					dataOfBmp[index].rgbRed = pRgb[mixIndex].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex].rgbReserved;
					index++;

				}
		}
		if(bitInfoHead.biBitCount == 16)
		{
			for(i=0;i<height;i++)
				for(j=0;j<width;j++)
				{
					mixIndex16= 0;
					k = i*l_width + j*2;
					WORD shortTemp;
					shortTemp = pColorData[k+1];
					shortTemp = shortTemp<<8;
					mixIndex16 = pColorData[k] + shortTemp;
					dataOfBmp[index].rgbRed = pRgb[mixIndex16].rgbRed;
					dataOfBmp[index].rgbGreen = pRgb[mixIndex16].rgbGreen;
					dataOfBmp[index].rgbBlue = pRgb[mixIndex16].rgbBlue;
					dataOfBmp[index].rgbReserved = pRgb[mixIndex16].rgbReserved;
					index++;
				}
		}
	}
	else//位图为24位真彩色
	{
		index = 0;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
			{
				k = i*l_width + j*3;
				dataOfBmp[index].rgbRed = pColorData[k+2];
				dataOfBmp[index].rgbGreen = pColorData[k+1];
				dataOfBmp[index].rgbBlue = pColorData[k];
				index++;
			}
	}

	//printf("像素数据信息:\n");
	/*
	for (int i=0; i<width*height; i++)
	{
	if (i%5==0)
	{
	printf("\n");
	}
	showRgbQuan(&dataOfBmp[i]);
	}
	*/
	close(fd);

	if (bitInfoHead.biBitCount<24)
	{
		free(pRgb);
	}
	//free(dataOfBmp);
	pic->data = dataOfBmp;
	pic->width = width;
	pic->height = height;
	free(pColorData);
	free(BmpFileHeader);
	//printf("\n");
}


//void showBmpHead(BITMAPFILEHEADER* pBmpHead)
//{
//	printf("位图文件头:\n");
//	printf("bmp格式标志bftype：0x%x\n",pBmpHead->bfType );
//	printf("文件大小:%d\n",pBmpHead->bfSize);
//	printf("保留字:%d\n",pBmpHead->bfReserved1);
//	printf("保留字:%d\n",pBmpHead->bfReserved2);
//	printf("实际位图数据的偏移字节数:%d\n",pBmpHead->bfOffBits);
//}
//
//void showBmpInforHead(tagBITMAPINFOHEADER* pBmpInforHead)
//{
//	printf("位图信息头:\n");
//	printf("结构体的长度:%d\n",pBmpInforHead->biSize);
//	printf("位图宽:%d\n",pBmpInforHead->biWidth);
//	printf("位图高:%d\n",pBmpInforHead->biHeight);
//	printf("biPlanes平面数:%d\n",pBmpInforHead->biPlanes);
//	printf("biBitCount采用颜色位数:%d\n",pBmpInforHead->biBitCount);
//	printf("压缩方式:%d\n",pBmpInforHead->biCompression);
//	printf("biSizeImage实际位图数据占用的字节数:%d\n",pBmpInforHead->biSizeImage);
//	printf("X方向分辨率:%d\n",pBmpInforHead->biXPelsPerMeter);
//	printf("Y方向分辨率:%d\n",pBmpInforHead->biYPelsPerMeter);
//	printf("使用的颜色数:%d\n",pBmpInforHead->biClrUsed);
//	printf("重要颜色数:%d\n",pBmpInforHead->biClrImportant);
//}
//void showRgbQuan(tagRGBQUAD* pRGB)
//{
//	printf("(%-3d,%-3d,%-3d) ",pRGB->rgbRed,pRGB->rgbGreen,pRGB->rgbBlue);
//}
