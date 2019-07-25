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
#include "comport.h"

int send_at_com( comport *comport_s,char *buf,int bufsize );
int get_at_cpin( comport *comport_s,char *buf,int bufsize );
int get_at_creg( comport *comport_s,char *buf,int bufsize );
int get_at_cimi( comport *comport_s,char *buf,int bufsize );
int get_at_csq( comport *comport_s,char *buf,int bufsize );

