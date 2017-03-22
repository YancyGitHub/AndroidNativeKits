
#ifdef SUPPORT_NATIVE_BROADCAST

#include <NativeBroadcast.h>

using namespace android;

typedef enum VALUE_TYPE
{
	VAL_NULL = -1,
	VAL_STRING = 0,
	VAL_INTEGER = 1,
	VAL_MAP = 2,
	VAL_BUNDLE = 3,
	VAL_PARCELABLE = 4,
	VAL_SHORT = 5,
	VAL_LONG = 6,
	VAL_FLOAT = 7,
	VAL_DOUBLE = 8,
	VAL_BOOLEAN = 9,
	VAL_CHARSEQUENCE = 10,
	VAL_LIST  = 11,
	VAL_SPARSEARRAY = 12,
	VAL_BYTEARRAY = 13,
	VAL_STRINGARRAY = 14,
	VAL_IBINDER = 15,
	VAL_PARCELABLEARRAY = 16,
	VAL_OBJECTARRAY = 17,
	VAL_INTARRAY = 18,
	VAL_LONGARRAY = 19,
	VAL_BYTE = 20,
	VAL_SERIALIZABLE = 21,
	VAL_SPARSEBOOLEANARRAY = 22,
	VAL_BOOLEANARRAY = 23,
	VAL_CHARSEQUENCEARRAY = 24,
};

NativeBroadcast::NativeBroadcast(const char * action) {
    mAction = action;
    mSendData.writeInterfaceToken(String16("android.app.IActivityManager"));
    mSendData.writeStrongBinder(NULL);

    // intent begin
    mSendData.writeString16(String16(action)); // action
    mSendData.writeInt32(0); // URI mSendData type
    mSendData.writeString16(NULL, 0); // type
    mSendData.writeInt32(0); // flags
    mSendData.writeString16(NULL, 0); // package name
    mSendData.writeString16(NULL, 0); // component name
    mSendData.writeInt32(0); // source bound - size
    mSendData.writeInt32(0); // categories - size
    mSendData.writeInt32(0); // selector - size
    mSendData.writeInt32(0); // clipmSendData - size
    mSendData.writeInt32(-2); // contentUserHint: -2 -> UserHandle.USER_CURRENT
    
    mBundleExtrasLenPos = mSendData.dataPosition();  // Bundle extras length position
    
    mSendData.writeInt32(-1); // bundle extras length
    mSendData.writeInt32(0x4C444E42); // 'B' 'N' 'D' 'L'

    mBundleExtrasStartPos = mSendData.dataPosition();

    mSendData.writeInt32(0);  // map size
    mMapCount = 0;
}

NativeBroadcast::~NativeBroadcast() {
}

status_t NativeBroadcast::putInt32(const char * key, int32_t value) {
    mSendData.writeString16(String16(key));
    mSendData.writeInt32(VAL_INTEGER);
    mSendData.writeInt32(value);
    
    updateExtrasLength();
    updateMapCount();

    return OK;
}

status_t NativeBroadcast::putInt64(const char * key, int64_t value) {
    mSendData.writeString16(String16(key));
    mSendData.writeInt32(VAL_LONG);
    mSendData.writeInt64(value);
    
    updateExtrasLength();
    updateMapCount();

    return OK;
}

status_t NativeBroadcast::putFloat(const char * key, float value) {
    mSendData.writeString16(String16(key));
    mSendData.writeInt32(VAL_FLOAT);
    mSendData.writeFloat(value);
    
    updateExtrasLength();
    updateMapCount();

    return OK;
}

status_t NativeBroadcast::putDouble(const char * key, double value) {
    mSendData.writeString16(String16(key));
    mSendData.writeInt32(VAL_DOUBLE);
    mSendData.writeDouble(value);
    
    updateExtrasLength();
    updateMapCount();

    return OK;
}

status_t NativeBroadcast::putString16(const char * key, String16& value) {
    mSendData.writeString16(String16(key));
    mSendData.writeInt32(VAL_STRING);
    mSendData.writeString16(value);
    
    updateExtrasLength();
    updateMapCount();

    return OK;
}

status_t NativeBroadcast::putInt32Array(const char * key, size_t len, const int32_t *value) {
    mSendData.writeString16(String16(key));
    mSendData.writeInt32(VAL_INTARRAY);
    mSendData.writeInt32Array(len, value);
    
    updateExtrasLength();
    updateMapCount();

    return OK;
}

status_t NativeBroadcast::putByteArray(const char * key, size_t len, const uint8_t *value) {
    mSendData.writeString16(String16(key));
    mSendData.writeInt32(VAL_INTARRAY);
    mSendData.writeByteArray(len, value);
    
    updateExtrasLength();
    updateMapCount();

    return OK;
}

bool NativeBroadcast::sendBroadcast(bool isSticky, UserId userId) {
    // intent end
    mSendData.writeString16(NULL, 0); // resolvedType
    mSendData.writeStrongBinder(NULL); // resultTo
    mSendData.writeInt32(0); // resultCode
    mSendData.writeString16(NULL, 0); // resultData
    mSendData.writeInt32(-1); // resultExtras
    mSendData.writeString16(NULL, 0); // permission
    mSendData.writeInt32(false); // appOp
    mSendData.writeInt32(false); // serialized: != 0 -> ordered
    mSendData.writeInt32(isSticky); // sticky
    mSendData.writeInt32(userId); // userId: -2 -> UserHandle.USER_CURRENT

    Parcel reply;
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> am = sm->getService(String16("activity"));
    if (am != NULL) {
        status_t ret = am->transact(IBinder::FIRST_CALL_TRANSACTION + 13, mSendData, &reply); // BROADCAST_INTENT_TRANSACTION
        if (ret == NO_ERROR) {
            int exceptionCode = reply.readExceptionCode();
            if (exceptionCode) {
                ALOGE("sendBroadcastMessage(%s) caught exception %d\n",
                        mAction, exceptionCode);
                return false;
            }
        } else {
            return false;
        }
    } else {
        ALOGE("getService() couldn't find activity service!\n");
        return false;
    }
    printf("sOver\n");
    return true;
}

bool NativeBroadcast::sendBroadcast() {
    return this->sendBroadcast(false, NONE);
}

bool NativeBroadcast::sendStickyBroadcast() {
    return this->sendBroadcast(true, NONE);
}

bool NativeBroadcast::sendBroadcastByUser(UserId userId) {
    return this->sendBroadcast(false, userId);
}

void NativeBroadcast::updateExtrasLength() {
    int newPos = mSendData.dataPosition();
    mSendData.setDataPosition(mBundleExtrasLenPos);
    mSendData.writeInt32(newPos - mBundleExtrasStartPos); // refill bundle extras length
    mSendData.setDataPosition(newPos);
}

void NativeBroadcast::updateMapCount() {
    int newPos = mSendData.dataPosition();
    mSendData.setDataPosition(mBundleExtrasStartPos);
    mMapCount++;
    mSendData.writeInt32(mMapCount);  // map size
    mSendData.setDataPosition(newPos);
}


#endif /* SUPPORT_NATIVE_BROADCAST */
