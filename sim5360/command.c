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

int send_at_com( char *buf,int bufsize )
{
    comport     *comport_s = NULL;
    int         rv = -1;

    if( (rv = writeComport(comport_s->com_fd,buf,bufsize)) > 0 )
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

int get_at_cpin( char *buf,int bufsize )
{
    comport     *comport_s = NULL;
    int         rv = -1;

    if( (rv = readComport(comport_s->com_fd,buf,bufsize)) > 0 )
    {
        if( (strstr(buf,"READY")) != NULL ) 
        {
            return 0;
        }
        else
            return -1;
    }
}
    
/****************************** 
 *接收"AT+CREG?"命令返回的信息
 *
 *成功：返回 0
 *失败：返回 -1
 ******************************/

int get_at_creg( char *buf,int bufsize )
{
    comport     *comport_s = NULL;
    int         rv = -1;

    if( (rv = readComport(comport_s->com_fd,buf,bufsize)) > 0 )
    {
        if( buf[9] == 0x31 )
        {
            return 0;
        }
        else 
            return -1;
    }
}

/******************************
 *接收"AT+CIMI"命令返回的信息
 *
 * 成功：返回 0 （移动）
 *       返回 1 （联通）
 * 失败：返回 -1
 ******************************/

int get_at_cimi( char *buf,int bufsize )
{
    comport     *comport_s = NULL;
    int         rv = -1;

    if( (rv = readComport(comport_s->com_fd,buf,bufsize)) > 0 )
    {
        if( buf[4] == 0x30 )
        {
            return 0;
        }
        else if( buf[4] == 0x31 )
        {
            return 1;
        }
        else 
            return -1;
    }
}
 
/******************************
 *接收"AT+CSQ"命令返回的信息
 *
 * 成功：返回 0
 * 失败：返回 -1
 ******************************/

int get_at_csq( char *buf,int bufsize )
{
    comport     *comport_s = NULL;
    int         rv = -1;

     if( (rv = readComport(comport_s->com_fd,buf,bufsize)) > 0 )
     {
        if ( buf[6] >= 1 )
        {
            return 0;
        }
        else
            return -1;
     }
}


