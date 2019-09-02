//
// Created by Administrator on 2019/8/31 0031.
//

#include <cstdio>
#include "SLRecorder.h"

SLRecorder::SLRecorder() : SLBase(){

}
SLRecorder::~SLRecorder() {
    ReleaseFeature();
}

bool SLRecorder::SetDataSource() {
    static SLDataLocator_IODevice loc_dev = {SL_DATALOCATOR_IODEVICE,
                                             SL_IODEVICE_AUDIOINPUT,
                                             SL_DEFAULTDEVICEID_AUDIOINPUT,
                                             nullptr};
    //注意loc_dev不用static修饰在该函数结束就会被释放
    dataSource = {&loc_dev, nullptr};
    return true;
}

bool SLRecorder::SetDataSink() {
    static SLDataLocator_AndroidSimpleBufferQueue loc_bq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 10};
    static SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM,
                                          2,
                                          SL_SAMPLINGRATE_44_1,
                                          SL_PCMSAMPLEFORMAT_FIXED_16,
                                          SL_PCMSAMPLEFORMAT_FIXED_16,
                                          SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,//声道
                                          SL_BYTEORDER_LITTLEENDIAN};
    dataSink = {&loc_bq, &format_pcm};
    return true;
}

bool SLRecorder::CreateFeature() {
    SLresult result;
    const int length = 1;
    const SLInterfaceID ids[length] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req[length] = {SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject, &dataSource,&dataSink, length, ids, req);
    if(SL_RESULT_SUCCESS != result)
        return false;
    result = (*recorderObject)->Realize(recorderObject,SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result)
        return false;
    result = (*recorderObject)->GetInterface(recorderObject,SL_IID_RECORD,&recorderItf);
    if(SL_RESULT_SUCCESS != result)
        return false;
    result = (*recorderObject)->GetInterface(recorderObject,SL_IID_ANDROIDSIMPLEBUFFERQUEUE,&queueItf);
    if(SL_RESULT_SUCCESS != result)
        return false;
    result = (*queueItf)->RegisterCallback(queueItf,queueCallBack, nullptr);
    if(SL_RESULT_SUCCESS != result)
        return false;
    return true;
}

void SLRecorder::ReleaseFeature() {
    if (recorderObject != nullptr) {
        (*recorderObject)->Destroy(recorderObject);
        recorderObject = nullptr;
        queueItf = nullptr;
    }
}

bool SLRecorder::SetStateRuning() {
    SLresult result = (*recorderItf)->SetRecordState(recorderItf,SL_RECORDSTATE_RECORDING);
    if(result!=SL_RESULT_SUCCESS)
        return false;
    return true;
}

bool SLRecorder::SetStateStoping() {
    SLresult result = (*recorderItf)->SetRecordState(recorderItf,SL_RECORDSTATE_STOPPED);
    if(result!=SL_RESULT_SUCCESS)
        return false;
    return true;
}