/* $Author: zimoch $ */ 
/* $Date: 2008/03/20 11:04:07 $ */ 
/* $Id: regDev.h,v 1.1 2008/03/20 11:04:07 zimoch Exp $ */  
/* $Name:  $ */ 
/* $Revision: 1.1 $ */ 

#ifndef regDev_h
#define regDev_h

#ifndef __GNUC__
#define __attribute__(a)
#define __extension__
#define __inline__ static
#endif

#ifndef DEBUG
#define STATIC static
#else
#define STATIC
#endif

#include <dbScan.h>

#define REGDEV_FLAGS_FIFO  (0x0000001)

/* Every device driver may define struct regDevice as needed */
/* It's a handle to the device instance */
typedef struct regDevice regDevice;

/* Every driver must provide this function table */
/* It may be constant and is used for all device instances */
typedef struct regDevSupport {
    void (*report)(
        regDevice *driver,
        int level);
    
    IOSCANPVT (*getInScanPvt)(
        regDevice *driver,
        unsigned int offset);
    
    IOSCANPVT (*getOutScanPvt)(
        regDevice *driver,
        unsigned int offset);

    int (*readArray)(
        regDevice *driver,
        unsigned int offset,
        unsigned int dlen,
        unsigned int nelem,
        void* pdata,
        int flags);
    
    int (*writeMaskedArray)(
        regDevice *driver,
        unsigned int offset,
        unsigned int dlen,
        unsigned int nelem,
        void* pdata,
        void* pmask,
        int flags);
} regDevSupport;

/* Every driver must create and register each device instance */
/* together with name and function table */
int regDevRegisterDevice(
    const char* name,
    const regDevSupport* support,
    regDevice* driver);

extern int regDevDebug;
#define regDevDebugLog(level, fmt, args...) \
    if (level <= regDevDebug) __extension__ errlogSevPrintf(errlogInfo, fmt, ## args);

#define regDevRead(device, offset, dlen, pdata) \
    regDevReadArray((device), (offset), (dlen), 1, (pdata))
    
#define regDevReadArray(device, offset, dlen, nelem, pdata) \
    (device)->support->readArray == NULL ? -1 : \
    (device)->support->readArray((device)->driver, (offset), (dlen), (nelem), (pdata), 0)

#define regDevReadFifo(device, offset, dlen, nelem, pdata) \
    (device)->support->readArray == NULL ? -1 : \
    (device)->support->readArray((device)->driver, (offset), (dlen), (nelem), (pdata), REGDEV_FLAGS_FIFO)

#define regDevWrite(device, offset, dlen, pdata) \
    regDevWriteMaskedArray((device), (offset), (dlen), 1, (pdata), NULL)

#define regDevWriteArray(device, offset, dlen, nelem, pdata) \
    regDevWriteMaskedArray((device), (offset), (dlen), (nelem), (pdata), NULL)

#define regDevWriteMasked(device, offset, dlen, pdata, mask) \
    regDevWriteMaskedArray((device), (offset), (dlen), 1, (pdata), (mask))

#define regDevWriteFifo(device, offset, dlen, pdata) \
    regDevWriteMaskedFifo((device), (offset), (dlen), 1, (pdata), NULL)

#define regDevWriteMaskedArray(device, offset, dlen, nelem, pdata, mask) \
    (device)->support->writeMaskedArray == NULL ? -1 : \
    (device)->support->writeMaskedArray((device)->driver, \
        (offset), (dlen), (nelem), (pdata), (mask), 0)

#define regDevWriteMaskedFifo(device, offset, dlen, nelem, pdata, mask) \
    (device)->support->writeMaskedArray == NULL ? -1 : \
    (device)->support->writeMaskedArray((device)->driver, \
        (offset), (dlen), (nelem), (pdata), (mask), REGDEV_FLAGS_FIFO)



#include <epicsVersion.h>
#ifdef BASE_VERSION
#define EPICS_3_13
#else
#define EPICS_3_14
#endif

#endif /* regDev_h */