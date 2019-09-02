//
// Created by Administrator on 2019/8/29 0029.
//

#include <cstdio>
#include "SLPlayer.h"

SLPlayer::SLPlayer() : SLBase(){

}
SLPlayer::~SLPlayer() {
    ReleaseFeature();
}

bool SLPlayer::SetDataSource() {
    static SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 4};
    static SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM,
                                          2,
                                          SL_SAMPLINGRATE_44_1,
                                          SL_PCMSAMPLEFORMAT_FIXED_16,
                                          SL_PCMSAMPLEFORMAT_FIXED_16,
                                          SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,//声道
                                          SL_BYTEORDER_LITTLEENDIAN};
    //注意loc_bufq和format_pcm不用static修饰在该函数结束就会被释放
    dataSource = {&loc_bufq, &format_pcm};
    return true;
}

bool SLPlayer::SetDataSink() {
    SLresult result;
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outMixObject, 1, ids, req);
    if(SL_RESULT_SUCCESS != result)
        return false;
    result = (*outMixObject)->Realize(outMixObject, SL_BOOLEAN_FALSE);
    if(SL_RESULT_SUCCESS != result)
        return false;
//    result = (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,&outputMixEnvironmentalReverb);
//    if (SL_RESULT_SUCCESS == result) {
//        LOGE("SHEN:SetEnvironmentalReverbProperties");
//        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb, &reverbSettings);
//        if(SL_RESULT_SUCCESS != result){
//            //最初这里一直调用true 后面发现是权限没有加
//            LOGE("SHEN:SetEnvironmentalReverbProperties-flase");
//        }else{
//            LOGE("SHEN:SetEnvironmentalReverbProperties-true");
//        }
//    }
    static SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outMixObject};
    //注意loc_outmix不用static修饰在该函数结束就会被释放
    dataSink = {&loc_outmix, nullptr};
    return true;
}

bool SLPlayer::CreateFeature() {
    SLresult result;
    const int length = 2;
    const SLInterfaceID ids[length] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME};
    const SLboolean req[length] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &playerObject, &dataSource, &dataSink,length, ids, req);
    if (SL_RESULT_SUCCESS != result)
        return false;
    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result)
        return false;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerItf);
    if (SL_RESULT_SUCCESS != result)
        return false;
    result = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &queueItf);
    if (SL_RESULT_SUCCESS != result)
        return false;
    if (queueCallBack == nullptr)
        return true;
    result = (*queueItf)->RegisterCallback(queueItf, queueCallBack, nullptr);
    if (SL_RESULT_SUCCESS != result)
        return false;
    LOGE("SLPlayer：CreateFeature 回调函数注册成功");
    return true;
}

void SLPlayer::ReleaseFeature() {
    if(playerObject != nullptr) {
        (*playerObject)->Destroy(playerObject);
        playerObject = nullptr;
        playerItf = nullptr;
        queueItf = nullptr;
    }
    if(outMixObject != nullptr){
        (*outMixObject)->Destroy(outMixObject);
        outMixObject = nullptr;
    }
}

bool SLPlayer::SetStateRuning() {
    SLresult result = (*playerItf)->SetPlayState(playerItf,SL_PLAYSTATE_PLAYING);
    if(result!=SL_RESULT_SUCCESS)
        return false;
    return true;
}

bool SLPlayer::SetStateStoping() {
    SLresult result = (*playerItf)->SetPlayState(playerItf,SL_PLAYSTATE_STOPPED);
    if(result!=SL_RESULT_SUCCESS)
        return false;
    return true;
}