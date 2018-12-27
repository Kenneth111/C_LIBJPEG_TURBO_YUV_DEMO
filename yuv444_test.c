//
// Created by Lenovo on 2018/12/27.
//
#include <stdio.h>
#include <stdlib.h>
#include "jpg_turbo/turbojpeg.h"
#include "yuv444_test.h"
#include "tj_test.h"

void recordFrameIntoYUVFile(char *filename, unsigned char *frame, int height, int width, int flag_yuv420){
    //记录图像到文件中
    FILE * pRecordFile;
    int ii;
    pRecordFile = fopen (filename, "wb+");
    if (pRecordFile == NULL)
    {
        printf("Error opening RECORD file |%s|\n.", filename);
    } else {
        fseek(pRecordFile, 0, SEEK_END);
        unsigned char *p = frame;

        for (ii=0;ii<height;ii++)
        {
            fwrite(p, 1, width, pRecordFile);
            p += width;
        }

        if(flag_yuv420){
            int height2 = (height + 1) / 2;
            int width2  = (width + 1) / 2;
            int stride2 = (width + 1) / 2;
            for (ii=0;ii<height2;ii++)
            {
                fwrite(p, 1, width2, pRecordFile);
                p += stride2;
            }

            for (ii=0;ii<height2;ii++)
            {
                fwrite(p, 1, width2, pRecordFile);
                p += stride2;
            }
        }else{
            for (ii=0;ii<height;ii++)
            {
                fwrite(p, 1, width, pRecordFile);
                p += width;
            }

            for (ii=0;ii<height;ii++)
            {
                fwrite(p, 1, width, pRecordFile);
                p += width;
            }
        }

        fclose(pRecordFile);
    }
}

int readFrameFromYUVFile(const char *filename, unsigned char *yuv_buffer, int height, int width, int frame_no, int flag_yuv420) {
    int success = -1;
    long skip_size;
    unsigned char *image_y = yuv_buffer;
    unsigned char *image_u = yuv_buffer + height * width;
    unsigned char *image_v;
    if(flag_yuv420){
        skip_size = frame_no * width * height * 3 / 2 ;
        image_v = image_u + height * width / 4;
    } else {
        skip_size = frame_no * width * height * 3;
        image_v = image_u + height * width;
    }
    int frame_width = width;
    int frame_height = height;
    int frame_stride = width;

    FILE * pFile;
    long read_size = frame_width;
    size_t result;
    int i;

    pFile = fopen (filename , "rb");
    if (pFile == NULL)
    {
        printf("Error opening file |%s|. \n", filename);
        success = -1;
    }
    else
    {
        if (flag_yuv420) {
            int frame_height2 = (frame_height + 1) / 2;
            int frame_width2 = (frame_width + 1) / 2;
            int frame_stride2 = frame_stride / 2;
            long read_size2 = frame_width2;

            long skip_size2 = frame_no * (frame_width * frame_height + frame_height2 * frame_width2 * 2);
            fseek(pFile, skip_size2, SEEK_SET);
            //y component
            for (i = 0; i < frame_height; i++) {
                //每次读一行，读完一行加stride
                result = fread(image_y, 1, (size_t) read_size, pFile);
                image_y += frame_stride;
                if ((result != read_size)) {
                    printf("Reading error.\n");
                    return -1;
                } else {
                    success = 0;
                }
            }
            // u component
            for (i = 0; i < frame_height2; i++) {
                //每次读一行，读完一行加stride
                result = fread(image_u, 1, (size_t) read_size2, pFile);
                image_u += frame_stride2;   //为便于统一，尽可能使用标准格式
                if ((result != read_size2)) {
                    printf("Reading error.\n");
                    return -1;
                } else {
                    success = 0;
                }
            }
            // v component
            for (i = 0; i < frame_height2; i++) {
                //每次读一行，读完一行加stride
                result = fread(image_v, 1, (size_t) read_size2, pFile);
                image_v += frame_stride2;
                if ((result != read_size2)) {
                    printf("Reading error.\n");
                    return -1;
                } else {
                    success = 0;
                }
            }
        } else {
            fseek(pFile , skip_size , SEEK_SET);
            for (i = 0; i < frame_height; i++) {
                //每次读一行，读完一行加stride
                result = fread (image_y, 1, (size_t)read_size, pFile);
                image_y += frame_stride;
                if ((result != read_size)) {
                    printf("Reading error.\n");
                    return -1;
                } else {
                    success = 0;
                }
            }
            for (i = 0; i < frame_height; i++) {
                //每次读一行，读完一行加stride
                result = fread (image_u, 1, (size_t)read_size, pFile);
                image_u += frame_stride;
                if ((result != read_size)) {
                    printf("Reading error.\n");
                    return -1;
                } else {
                    success = 0;
                }
            }
            for (i = 0; i < frame_height; i++) {
                //每次读一行，读完一行加stride
                result = fread (image_v, 1, (size_t)read_size, pFile);
                image_v += frame_stride;
                if ((result != read_size)) {
                    printf("Reading error.\n");
                    return -1;
                } else {
                    success = 0;
                }
            }
        }
    }
    if(NULL!=pFile){
        fclose (pFile);
    }
    return success;
}

int yuv_compress(unsigned char *yuv_buffer, int height, int width, int is_yuv420){
    tjhandle handle = NULL;
    handle=tjInitCompress();
    if (NULL == handle) {
        return -1;
    }
    int subsamp = TJSAMP_444;
    if(is_yuv420){
        subsamp = TJSAMP_420;
    }
    unsigned char *jpg_buffer = NULL;
    long unsigned int jpg_size = 0;
    int ret = tjCompressFromYUV(handle, yuv_buffer, width, 1, height, subsamp, &jpg_buffer, &jpg_size, 80, 0);
    write_buffer2file("yuv2jpg.jpg", jpg_buffer, jpg_size);
    free(jpg_buffer);
    tjDestroy(handle);
    return ret;
}

int yuv_decompress(unsigned char *jpg_buffer, unsigned long jpg_size, int height, int width, int is_yuv420){
    tjhandle handle = NULL;
    handle = tjInitDecompress();
    if (NULL == handle)  {
        return -1;
    }
    unsigned char *yuv_buffer;
    if(is_yuv420){
        yuv_buffer = (unsigned char*)malloc(height * width * 3 / 2);
    } else {
        yuv_buffer = (unsigned char*)malloc(height * width * 3);
    }
    int ret = tjDecompressToYUV2(handle, jpg_buffer, jpg_size, yuv_buffer, width, 1, height, 0);
    recordFrameIntoYUVFile("outyuv.yuv", yuv_buffer, height, width, 0);
    free(yuv_buffer);
    tjDestroy(handle);
    return ret;
}

int yuv444_test(){
//    int size = tjBufSizeYUV2(1920, 1, 1080, TJSAMP_444);
    // YUV444P
    unsigned char *yuv_buffer = (unsigned char*)malloc(1920 * 1080 * 3);
    if(readFrameFromYUVFile("enc_yuv.yuv", yuv_buffer, 1080, 1920, 0, 0)){
        return -1;
    }
    int cstart = get_timer_now();
    yuv_compress(yuv_buffer, 1080, 1920, 0);
    int cend = get_timer_now();
    printf("yuv compress time: %d\n", cend - cstart);
    tjp_info_t tinfo;
    tinfo.jpg_size = 0;
    tinfo.outheight = 1080;
    tinfo.outwidth = 1920;
    unsigned char *jpg_buffer = read_file2buffer("yuv2jpg.jpg", &tinfo);
    int dstart = get_timer_now();
    yuv_decompress(jpg_buffer, tinfo.jpg_size, 1080, 1920, 0);
    int dend = get_timer_now();
    printf("yuv decompress time: %d\n", dend - dstart);
    free(yuv_buffer);
    return 0;
}