/*********************************************************************************
 *      Copyright:  (C) 2019 LingYun<lingyun@email.com>
 *                  All rights reserved.
 *
 *       Filename:  main_pthread.c
 *    Description:  This file is main phread of send short messages and surf the Internet.
 *                 
 *        Version:  1.0.0(18/07/19)
 *         Author:  LingYun <lingyun@email.com>
 *      ChangeLog:  1, Release initial version on "18/07/19 09:09:44"
 *                 
 ********************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

int getreq(char *buf,int bufsize);
void *internet(void *ptr);
void *message(void *ptr);

//定义一个结构体

typedef struct _Req_Exc
{
    int                 req;
    int                 exc;
    pthread_mutex_t     lock;
}Req_Exc;

int main(int argc,char **argv)
{
    pthread_t           tid_msg,tid_net;    //线程ID
    pthread_attr_t      thread_attr;        //线程属性
    Req_Exc             req_exc;
    char                cmd_buf[16];

    Req_Exc *ptr = &req_exc;                //指针指向结构体
    ptr->req = 0;
    ptr->exc = 0;

    //初始化锁

    pthread_mutex_init(&req_exc.lock, NULL);

    //初始化线程
    
    if( pthread_attr_init(&thread_attr) )
    {
        printf("pthread_attr_init() failure: %s\n",strerror(errno));
        goto Cleanup;
    }

    if( pthread_attr_setstacksize(&thread_attr,120*1024) )
    {
        printf("pthread_attr_setstacksize() failure: %s\n",strerror(errno));
        goto Cleanup;
    }

    if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) )
    {
        printf("pthread_attr_setdetachstate() failure: %s\n",strerror(errno));
        goto Cleanup;        
    }

    //创建子线程

    if( pthread_create(&tid_msg,&thread_attr,message,(void *)ptr) !=0 )
    {
        printf("pthread create [message] failure: %s\n",strerror(errno));
        goto Cleanup;
    }

    if( pthread_create(&tid_net,&thread_attr,internet,(void *)ptr) !=0 )
    {
        printf("pthread create [internet] failure: %s\n",strerror(errno));
        goto Cleanup;
    }

    //摧毁线程属性

    pthread_attr_destroy(&thread_attr);

    printf("\nWelcome!\n");
    printf("Please input 'internet' or 'message'\n");
    printf("Please input the command:\n");
    
    while(1)
    {
        fgets(cmd_buf,sizeof(cmd_buf),stdin);
    
        if( getreq(cmd_buf,sizeof(cmd_buf)) == 1 )
        {
            pthread_mutex_lock(&req_exc.lock);//上锁
            ptr->req = 1;
            pthread_mutex_unlock(&req_exc.lock);//释放锁
        }

        else if( getreq(cmd_buf,sizeof(cmd_buf)) == 2 )
        {
            pthread_mutex_lock(&req_exc.lock);//上锁
            ptr->req = 2;
            pthread_mutex_unlock(&req_exc.lock);//释放锁
        }

        else if( getreq(cmd_buf,sizeof(cmd_buf)) == 0 )
        {
            printf("req number is not 1 or 2 !\n");
        }

        //想要发短信但是有上网的情况

        if( ptr->req == 2 )
        {
            if( ptr->exc !=0 )
            {
                //关掉上网线程

                pthread_cancel(tid_net);
                sleep(1);
                printf("Close the internet thread\n");

                //重新打开上网线程

                if(pthread_create(&tid_net,&thread_attr,internet,(void *)ptr) != 0)
                {
                    printf("pthread create [internet] failure: %s\n",strerror(errno));
                    goto Cleanup;
                }

                printf("Restart the internet thread\n");

                //发短信操作

                printf("正在发短信，等待10秒\n");
                sleep(10);
                printf("\n发短信成功！\n");

                pthread_mutex_lock(&req_exc.lock);//上锁
                ptr->req = 0;
                ptr->exc = 0; 
                pthread_mutex_unlock(&req_exc.lock);//释放锁
            }
        }
    }

    pthread_mutex_destroy(&req_exc.lock);

    return 0;

Cleanup:
        pthread_mutex_destroy(&req_exc.lock);
        pthread_attr_destroy(&thread_attr);
        return -1;

}

//上网

void *internet(void *ptr)
{
    int oldstate;
    int oldtype;
    Req_Exc *temp_pctr = (Req_Exc *)ptr;

    //使能可取消该线程

    if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&oldstate) != 0)
    {
        printf("setcanclesate failure:%s\n",strerror(errno));
        exit(-1);
    }

    //设置取消类型，异步取消，不遇到取消点

    if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,&oldtype) != 0)
    {
        printf("pthread_setcanceltype failure:%s\n",strerror(errno));
        exit(-1);
    }

    while(1)
    {
        if( temp_pctr->req == 1 )
        {
            printf("请求上网\n");

            if( temp_pctr->exc == 0 )
            {
                pthread_mutex_lock(&temp_pctr->lock);//上锁
                temp_pctr->exc = temp_pctr->req;
                pthread_mutex_unlock(&temp_pctr->lock);//释放锁
            
                //上网操作

                printf("正在上网，等待20秒\n");
                sleep(20);
                printf("\n上网成功！\n");

                pthread_mutex_lock(&temp_pctr->lock);//上锁
                temp_pctr->req = 0;
                temp_pctr->exc = 0;
                pthread_mutex_unlock(&temp_pctr->lock);//释放锁
            }
            else
            {
                printf("正在使用发短信功能...\n");
                sleep(1);
            }
        }
    }
}

//发短信

void *message(void *ptr)
{
    Req_Exc *temp_pctr = (Req_Exc *)ptr;

    while(1)
    {
        if( temp_pctr->req == 2 )
        {   
            if( temp_pctr->exc == 0 )
            {
                printf("请求发短信\n");

                pthread_mutex_lock(&temp_pctr->lock);//上锁
                temp_pctr->exc = temp_pctr->req;
                pthread_mutex_unlock(&temp_pctr->lock);//释放锁

                //发短信操作

                printf("正在发短信，等待10秒\n");
                sleep(10);
                printf("\n发短信成功！\n");

                pthread_mutex_lock(&temp_pctr->lock);//上锁
                temp_pctr->req = 0;
                temp_pctr->exc = 0;
                pthread_mutex_unlock(&temp_pctr->lock);//释放锁
            }
        }
    }
}

//获取req值

int getreq(char *buf,int bufsize)
{
    char        buf_new[16];

    strncpy(buf_new,buf,bufsize);

    if(strcmp(buf_new,"message\n") == 0)
    {
        return 2;
    }
    else if(strcmp(buf_new,"internet\n") == 0)
    {
        return 1;
    }
    else
    {
        printf("Command error!\n");
        return 0;
    }
}
