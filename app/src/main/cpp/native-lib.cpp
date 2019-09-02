#include <jni.h>
#include <string>
#include "SLBase.h"
#include "SLPlayer.h"
#include "SLRecorder.h"
#define BUF_SIZE (1024*4)
#define DEF_FILE_PATH "/storage/emulated/0/SHEN_RES/aqjz.pcm"
#define TEMP_FILE_PATH "/storage/emulated/0/SHEN_RES/temp.pcm"
FILE *readFp = nullptr;     //播放器用到的读取文件
FILE *writeFp = nullptr;    //录音器用到的写入文件
char *readBuf = nullptr;    //读取缓存
char *writeBuf = nullptr;   //写入缓存
SLBase *player;             //播放器
SLBase *recorder;           //录音器

void initPlayerResource(){
    if(readFp == nullptr) readFp = fopen(TEMP_FILE_PATH, "rb"); //华为
    if(readBuf == nullptr) readBuf = new char[BUF_SIZE];
}

void initRecorderResource(){
    if(!writeFp) writeFp = fopen(TEMP_FILE_PATH, "wb+"); //华为
    if(writeBuf == nullptr) writeBuf = new char[BUF_SIZE];
}

void freePlayerResource(){
    if(readFp != nullptr) {
        fclose(readFp);
        readFp = nullptr;
    }
    if(readBuf != nullptr){
        delete readBuf;
        readBuf = nullptr;
    }
}

void freeRecorderResource(){
    if(writeFp != nullptr) {
        fclose(writeFp);
        writeFp = nullptr;
    }
    if(writeBuf != nullptr){
        delete writeBuf;
        writeBuf = nullptr;
    }
}

void PlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    if(readFp == nullptr) return;
    if (player == nullptr) return;
    if(!player->IsQueueSelf(bq)) return;
    if(player->IsQueueLooping()){
        int len = fread(readBuf,1,BUF_SIZE,readFp);
        LOGD("PlayerCallback:len=%d",len);
        if (len > 0) player->SendQueueLoop(readBuf,len);
    }
}
void RecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {
    if(!writeFp){
        LOGE("RecorderCallback:文件打开失败");
        return;
    }
    if (recorder == nullptr) {
        LOGE("RecorderCallback:recorder是空");
        return;
    }
    if(!recorder->IsQueueSelf(bq)) {    //检查是否是自己
        LOGE("RecorderCallback:队列不是自己");
        return;
    }
    if(recorder->IsQueueLooping()){
        fwrite(writeBuf,1,BUF_SIZE,writeFp);
        fflush(writeFp);
        recorder->SendQueueLoop(writeBuf,BUF_SIZE);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_shen_opensles_MainActivity_startPlay(JNIEnv *env, jobject instance) {
    // TODO
    initPlayerResource();
    player = new SLPlayer();
    player->SetQueueCallBack(PlayerCallback);
    player->Start();
    player->SendQueueLoop("",1);    //开启轮询
}

extern "C"
JNIEXPORT void JNICALL
Java_com_shen_opensles_MainActivity_stopPlay(JNIEnv *env, jobject instance) {
    // TODO
    player->Stop();
    freePlayerResource();
    delete player;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_shen_opensles_MainActivity_startRecord(JNIEnv *env, jobject instance) {
    // TODO
    initRecorderResource();
    recorder = new SLRecorder();
    recorder->SetQueueCallBack(RecorderCallback);
    recorder->Start();
    recorder->SendQueueLoop(writeBuf,BUF_SIZE);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_shen_opensles_MainActivity_stopRecord(JNIEnv *env, jobject instance) {
    // TODO
    recorder->Stop();
    freeRecorderResource();
    delete recorder;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_shen_opensles_MainActivity_volume(JNIEnv *env, jobject instance, jint volue) {
    // TODO
//    setVolue(volue);
}


//void setVolue(int volue){
//    LOGE("SHEN:setVolue");
//    if(bqPlayerVolume == NULL)
//        return;
//    SLmillibel vol;
//    SLresult result;
//    result = (*bqPlayerVolume)->GetMaxVolumeLevel(bqPlayerVolume,&vol);
//    LOGE("SHEN:setVolue-GetMaxVolumeLevel=%d,vol=%d",result,vol);
//    result = (*bqPlayerVolume)->GetVolumeLevel(bqPlayerVolume,&vol);
//    LOGE("SHEN:setVolue-GetVolumeLevel=%d,vol=%d",result,vol);
//    //音量这个参数比较奇怪
//    vol = (100-volue) * (-50);
//    LOGE("SHEN:setVolue-SetVolumeLevel key=%d,val=%d",volue,vol);
//    result = (*bqPlayerVolume)->SetVolumeLevel(bqPlayerVolume,vol);
////    问题1：参数设置50，参数不对
////    W/libOpenSLES: Leaving Volume::SetVolumeLevel (SL_RESULT_PARAMETER_INVALID)
//}