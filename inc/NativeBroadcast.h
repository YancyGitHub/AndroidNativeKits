#ifndef __NATIVE_BROADCAST_H__
#define __NATIVE_BROADCAST_H__

#ifdef SUPPORT_NATIVE_BROADCAST


#define LOG_TAG "NativeBroadcast"

#include <fcntl.h>
#include <unistd.h>

#include <binder/MemoryHeapBase.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>

using namespace android;

typedef enum UserId {
    USER_ALL = -1,
    USER_CURRENT = -2,
    USER_CURRENT_OR_SELF = -3,
    USER_NULL = -10000,
    USER_OWNER = 0,
    NONE = false,
};

class NativeBroadcast {
public:
    NativeBroadcast(const char * action);
    
    status_t putInt32(const char * key, int32_t value);
    status_t putInt64(const char * key, int64_t value);
    status_t putFloat(const char * key, float value);
    status_t putDouble(const char * key, double value);
    status_t putString16(const char * key, String16& value);
    status_t putInt32Array(const char * key, size_t len, const int32_t *value);
    status_t putByteArray(const char * key, size_t len, const uint8_t *value);
    
    bool sendBroadcast();
    bool sendBroadcast(bool isSticky, UserId userId);
    bool sendStickyBroadcast();
    bool sendBroadcastByUser(UserId userId);

private:
    ~NativeBroadcast();
    
    void updateExtrasLength();
    void updateMapCount();
    
    Parcel mSendData;
    const char * mAction;
    int mBundleExtrasLenPos;
    int mBundleExtrasStartPos;
    int mMapCount;
};


#endif /* SUPPORT_NATIVE_BROADCAST */

#endif /* __NATIVE_BROADCAST_H__ */
