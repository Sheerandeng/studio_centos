/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  command.c
 *    Description:  This file is send command.
 *                 
 *        Version:  1.0.0(23/07/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "23/07/19 16:50:14"
 *                 
 *********************************************************************************/
#include <stdio.h>
#include "command.h"
#include "comport.h"

/*********************
 * 发送AT命令函数
 *
 * 成功：返回 0
 * 失败：返回 -1
 **********************/

int send_at_com( comport *comport_s,char *buf,int bufsize )
{
    int         fd = -1;

    fd = writeComport(comport_s->com_fd,buf,bufsize);

    if( fd > 0 )
    {
        return 0;
    }
    else
        return -1;
}

/******************************
 * 接收"AT+CPIN?"命令返回的信息
 *
 * 成功：返回 0
 * 失败：返回 -1
 ******************************/

int get_at_cpin( comport *comport_s,char *buf,int bufsize )
{
    readComport(comport_s->com_fd,buf,bufsize);
    //printf("读到的数据:\n%s\n",buf);

    if( (strstr(buf,"READY")) != NULL ) 
    {
        return 0;
    }
    else
        return -1;
}
    
/****************************** 
 *接收"AT+CREG?"命令返回的信息
 *
 *成功：返回 0
 *失败：返回 -1
 ******************************/

int get_at_creg( comport *comport_s,char *buf,int bufsize )
{

    int  i;
    char *ptr = buf;

    readComport(comport_s->com_fd,buf,bufsize);
    //printf("读到的数据：\n%s\n",buf);
    
    //遍历buf
    for( i=0;i<bufsize;i++ )
    {
        if( buf[i] == ',' )
        {
            *ptr = buf[i+1];
        }
    }

    if( *ptr == 0x31 )
        return 0;
    else
        return -1;
}

/******************************
 *接收"AT+CIMI"命令返回的信息
 *
 * 成功：返回 0 （移动）
 *       返回 1 （联通）
 * 失败：返回 -1
 ******************************/

int get_at_cimi( comport *comport_s,char *buf,int bufsize )
{
    int  i;
    char *ptr = buf;

    readComport(comport_s->com_fd,buf,bufsize);
    //printf("读到的数据：%s\n",buf);

    //遍历buf
    for( i=0;i<bufsize;i++ )
    {
        if( buf[i] == 0x34 )
        {
            *ptr = buf[i+4];
        }
    }

    if( *ptr == 0x30 )
        return 0;
    else if( *ptr == 0x31 )
        return 1;
    else 
        return -1;
}
 
/******************************
 *接收"AT+CSQ"命令返回的信息
 *
 * 成功：返回 0
 * 失败：返回 -1
 ******************************/

int get_at_csq( comport *comport_s,char *buf,int bufsize )
{
    int  i;
    char *ptr = buf;

    readComport(comport_s->com_fd,buf,bufsize);
    //printf("读到的数据：%s\n",buf);

    //遍历buf
    for( i=0;i<bufsize;i++ )
    {
        if( buf[i] == ',' )
        {
            *ptr = buf[i-1];
        }
    }
    
    if ( *ptr >= 1 )
        return 0;
    else
        return -1;    
}

