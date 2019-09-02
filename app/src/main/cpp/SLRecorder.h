//
// Created by Administrator on 2019/8/31 0031.
//

#ifndef SHENOPENSLES_SLRECORDER_H
#define SHENOPENSLES_SLRECORDER_H

#include "SLBase.h"

class SLRecorder : public SLBase{
public:
    SLRecorder();
    virtual ~SLRecorder();

protected:
    SLObjectItf recorderObject;
    SLRecordItf recorderItf;

    virtual bool SetDataSource();           //设置输入源
    virtual bool SetDataSink();             //设置输出源
    virtual bool CreateFeature();           //创建XXX器
    virtual void ReleaseFeature();          //释放XXX器
    virtual bool SetStateRuning();          //设置XXX器运行状态
    virtual bool SetStateStoping();         //设置XXX器停止状态
};


#endif //SHENOPENSLES_SLRECORDER_H
