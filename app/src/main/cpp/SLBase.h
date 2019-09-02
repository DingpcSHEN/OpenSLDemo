//
// Created by Administrator on 2019/8/29 0029.
//

#ifndef SHENOPENSLES_SLBASE_H
#define SHENOPENSLES_SLBASE_H
#include <android/log.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"SHEN",__VA_ARGS__)
#define  LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"SHEN",__VA_ARGS__)
class SLBase{
public:
    SLBase();
    virtual ~SLBase();
    void Start();
    void Stop();
protected:
    //引擎
    SLObjectItf engineObject = nullptr;
    SLEngineItf engineEngine = nullptr;
    bool CreateEngine();                    //创建引擎
    void ReleaseEngine();                   //释放引擎
    //输入输出
    SLDataSource dataSource;
    SLDataSink dataSink;
    virtual bool SetDataSource()=0;         //设置输入源
    virtual bool SetDataSink()=0;           //设置输出源
    //播放器或者录音器
    virtual bool CreateFeature()=0;         //创建XXX器
    virtual void ReleaseFeature()=0;        //释放XXX器
    virtual bool SetStateRuning()=0;        //设置XXX器运行状态
    virtual bool SetStateStoping()=0;       //设置XXX器停止状态
    //队列
    bool isQueueLooping = false;
    SLAndroidSimpleBufferQueueItf queueItf;
    slAndroidSimpleBufferQueueCallback queueCallBack;
    void SetQueueState(bool isLoop);
public:
    void SetQueueCallBack(slAndroidSimpleBufferQueueCallback callback);
    bool SendQueueLoop(const void *pBuffer,SLuint32 size);
    bool IsQueueLooping() const { return isQueueLooping; };
    bool IsQueueSelf(SLAndroidSimpleBufferQueueItf queue);
};
#endif //SHENOPENSLES_SLBASE_H
