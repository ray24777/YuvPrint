/*
* Copyright(C), 2008-2013, Ubuntu Inc.
* File name:        main.c
* Author:           xubinbin 徐彬彬 (Beijing China)
* Version:          1.0
* Date:             2013.06.09
* Description:
* Function List:    char *draw_Font_Func(char *ptr_frame,const unsigned char font[],int startx,int starty,int color)
* Email:            xubbwd@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#define FRAME_WIDTH             (1920)
#define FRAME_HEIGHT            (1080)
#define FRAME_SIZE              (FRAME_WIDTH*FRAME_HEIGHT*3/2)
#define IN_FILENAME             "c1.yuv"
#define OUT_FILENAME            "afterc1.yuv"
#define STRING_FILENAME         "input"
/*
* Function:     draw_Font_Func
* Description:  实现在yuv420图片上面画字
* Input:        char *ptr_frame             一帧视频的首地址
*               const unsigned char font[]  画的字模
*               int startx                  写字的起点坐标x
*               int starty                  写字的起点坐标y
*               int color                   字颜色的选择，具体颜色在程序代码
* Return:       这里会把传进来的一帧视频的地址返回，可以不调用
*/
char *draw_Font_Func(char *ptr_frame,const unsigned char font[],int startx,int starty,int color)
{

    assert( ptr_frame != NULL );

    int tagY=0,tagU=0,tagV=0;
    char *offsetY=NULL,*offsetU=NULL,*offsetV=NULL;
    unsigned short p16, mask16; // for reading hzk16 dots

    /*yuv 地址的设置 */
    offsetY = ptr_frame;
    offsetU = offsetY + FRAME_WIDTH * FRAME_HEIGHT;
    offsetV = offsetU + FRAME_WIDTH * FRAME_HEIGHT/4;

    switch (color)
    {
        case 0:         // Yellow
            tagY = 226;tagU = 0;tagV = 149;
            break;
        case 1:         // Red
            tagY = 76;tagU = 85;tagV = 255;
            break;
        case 2:         // Green
            tagY = 150;tagU = 44;tagV = 21;
            break;
        case 3:         // Blue
            tagY = 29;tagU = 255;tagV = 107;
            break;
        case 4:         // White
            tagY = 128;tagU = 128;tagV = 128;
            break;
        default:        // White
            tagY = 128;tagU = 128;tagV = 128;
            break;
    }

    int x=0,y=0,i=0,j=0,k=0;
    for(i = 0; i < 32; i++)
    {
    #if 0
        for (j = 0, y = starty; j < 16 && y < FRAME_HEIGHT - 1; j++, y+=2)    // line dots per char
        {
            p16 = *(unsigned short *)(font + j*2 + i*32);/*取字模数据*/
            mask16 = 0x0080;  /* 二进制 1000 0000 */
            for (k = 0, x = startx +i*36; k < 16 && x < FRAME_WIDTH - 1; k++, x+=2)   // dots in a line
            {
                if (p16 & mask16)
                {
                    *(offsetY + y*FRAME_WIDTH + x) = *(offsetY + y*FRAME_WIDTH + x+1) = tagY;
                    *(offsetY + (y+1)*FRAME_WIDTH + x) = *(offsetY + (y+1)*FRAME_WIDTH + x+1) = tagY;
                    *(offsetU + y * FRAME_WIDTH/4 + x/2) =tagU;
                    *(offsetV + y * FRAME_WIDTH/4 + x/2) = tagV;
                }
                mask16 = mask16 >> 1;  /* 循环移位取数据 */
                if (mask16 == 0)
                    mask16 = 0x8000;
            }
        }
    #else
        for (j = 0, y = starty; j < 16 && y < FRAME_HEIGHT - 1; j++, y++) // line dots per char
        {
            p16 = *(unsigned short *)(font + j*2 + i*32);/*取字模数据*/
            mask16 = 0x0080;  /* 二进制 1000 0000 */
            for (k = 0, x = startx +i*18; k < 16 && x < FRAME_WIDTH - 1; k++, x++) // dots in a line
            {
                if (p16 & mask16)
                {
                    *(offsetY + y*FRAME_WIDTH + x) = 255;
                //  *(offsetU + y * FRAME_WIDTH/4 + x/2) = 85;
                //  *(offsetV + y * FRAME_WIDTH/4 + x/2) = 255;
                }
                mask16 = mask16 >> 1;  /* 循环移位取数据 */
                if (mask16 == 0)
                    mask16 = 0x8000;
            }
        }
    #endif
    }

    return (char *)ptr_frame;
}

int findfont(unsigned char *buffer)
{
    FILE* fd = NULL;
    FILE* fin = NULL;
	int offset;
    unsigned char *p=buffer;
	
	int word[2]={0};
    
	fd = fopen("hzk16", "rb");
	if (fd == NULL)
	{
		fprintf(stderr, "error hzk16\n");
		return 1;
	}
    fin = fopen(STRING_FILENAME, "rb");
	if (fin == NULL)
	{
		fprintf(stderr, "error input file\n");
        return 1;
	}
    for(int i=0;i<32;i++)
    {
        if( feof(fin) )
        { 
          break;
        }
        word[0]=fgetc(fin);
		word[1]=fgetc(fin);
        offset = (94 * (unsigned int)(word[0] - 0xa0 - 1) + (word[1] - 0xa0 - 1)) * 32;
        fseek(fd, offset, SEEK_SET);
        fread(p, 1, 32, fd);
        p=p+32;
        
    }
    p=p-32;
    for(int i=0;i<32;i++,p++)
    {
        *p=0;
    }
    return 0;
}

int main()
{
    int ret = 0;
    FILE *in_file,*out_file;

    char *frame_buffer = NULL;
    frame_buffer = (char*)malloc(FRAME_SIZE);

    //read frame file 读原来的一帧数据
    in_file = fopen(IN_FILENAME,"rb");
    if (in_file == NULL)
    {
        printf("open in file error!\n");
    }

    ret = fread(frame_buffer,FRAME_SIZE,1,in_file);
    if (ret != 1)
    {
        printf("ret = %d\n", ret);
        printf("fread file error!\n");
    }
    fclose(in_file);

    //获取字模
    char table[1024] ={0};
    if(findfont(table)==1)
    {
        printf("Cannot generate bitmap!\n");
    }

    //数据转换
    draw_Font_Func(frame_buffer,table,20,10,1);

    //write frame file 把数据写回
    out_file = fopen(OUT_FILENAME,"w");
    if (out_file == NULL)
    {
        printf("open in file error!\n");
    }

    ret = fwrite(frame_buffer,FRAME_SIZE,1,out_file);
    if (ret != 1)
    {
        printf("ret = %d\n");
        printf("fwrite file error!\n");
    }
    fclose(out_file);
    free(frame_buffer);

    printf("Done!\n");
    return 0;
}
