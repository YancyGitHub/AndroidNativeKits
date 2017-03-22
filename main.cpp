
#include <stdio.h>


#ifdef SUPPORT_NATIVE_BROADCAST
#include <NativeBroadcast.h>
#endif /* SUPPORT_NATIVE_BROADCAST */

#define ACTION "com.yancy.demo.native.broadcast.ACTION"

//using namespace android;

int main() {

    #ifdef SUPPORT_NATIVE_BROADCAST
    NativeBroadcast * nb = new NativeBroadcast(ACTION);
    String16 name = String16("Crias");
    String16 addr = String16("China");
    int32_t aInt[] = {12,13,14,15,16};

    nb->putString16("Name", name);
    nb->putInt32("Age", 25);
    nb->putString16("Address", addr);
    nb->putFloat("Scof", 19.6f);
    nb->putInt32Array("IntArray", 5, aInt);

    nb->sendStickyBroadcast();
    #else
    printf("NOT SUPPORT_NATIVE_BROADCAST\n\n");
    #endif /* SUPPORT_NATIVE_BROADCAST */

    return 0;
}






