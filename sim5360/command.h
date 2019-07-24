/********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  command.h
 *    Description:  
 *
 *        Version:  1.0.0(24/07/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "24/07/19 17:22:16"
 *                 
 ********************************************************************************/

int send_at_com( char *buf,int bufsize );
int get_at_cpin( char *buf,int bufsize );
int get_at_creg( char *buf,int bufsize );
int get_at_cimi( char *buf,int bufsize );
int get_at_csq( char *buf,int bufsize );

