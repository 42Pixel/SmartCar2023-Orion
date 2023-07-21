/**
 * \file IfxCif_Cam.h
 * \brief CIF CAM details
 * \ingroup IfxLld_Cif
 *
 * \version iLLD_1_0_1_11_0
 * \copyright Copyright (c) 2018 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Use of this file is subject to the terms of use agreed between (i) you or 
 * the company in which ordinary course of business you are acting and (ii) 
 * Infineon Technologies AG or its licensees. If and as long as no such 
 * terms of use are agreed, use of this file is subject to following:


 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or 
 * organization obtaining a copy of the software and accompanying 
 * documentation covered by this license (the "Software") to use, reproduce,
 * display, distribute, execute, and transmit the Software, and to prepare
 * derivative works of the Software, and to permit third-parties to whom the 
 * Software is furnished to do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.

 *
 * \defgroup IfxLld_Cif_Cam CAM
 * \ingroup IfxLld_Cif
 * \defgroup IfxLld_Cif_Cam_camEnumerations Camera Enumerations
 * \ingroup IfxLld_Cif_Cam
 * \defgroup IfxLld_Cif_Cam_camFunctions Camera Functions
 * \ingroup IfxLld_Cif_Cam
 * \defgroup IfxLld_Cif_Cam_camStructures Camera Structures
 * \ingroup IfxLld_Cif_Cam
 */

#ifndef IFXCIF_CAM_H
#define IFXCIF_CAM_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cif/Std/IfxCif.h"
#include "_PinMap/IfxCif_PinMap.h"
#include "Emem/Std/IfxEmem.h"
#include "I2c/I2c/IfxI2c_I2c.h"
#include "string.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define IFXCIF_CAM_MEM_ALIGN_SIZE (64)

#define IFXCIF_CAM_MEM_GAPSIZE    0x0

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Cif_Cam_camEnumerations
 * \{ */
/** \brief CIF ISP mode
 */
typedef enum
{
    IfxCif_Cam_IspMode_undefined      = -1, /**< \brief undefined */
    IfxCif_Cam_IspMode_raw            = 0,  /**< \brief raw */
    IfxCif_Cam_IspMode_yuvInterleaved = 1,  /**< \brief yuv interleaved */
    IfxCif_Cam_IspMode_yuvPlanar      = 2   /**< \brief yuv planar */
} IfxCif_Cam_IspMode;

/** \brief Driver acquisition state
 */
typedef enum
{
    IfxCif_Cam_State_stopped = 0,  /**< \brief stopped */
    IfxCif_Cam_State_running = 1   /**< \brief running */
} IfxCif_Cam_State;

/** \brief Driver status
 */
typedef enum
{
    IfxCif_Cam_Status_ok              = 0,  /**< \brief ok */
    IfxCif_Cam_Status_notEnoughMemory = 1,  /**< \brief not enough Memory */
    IfxCif_Cam_Status_cameraCommError = 2,  /**< \brief camera comm Error */
    IfxCif_Cam_Status_notAvailable    = 3   /**< \brief not available */
} IfxCif_Cam_Status;

/** \} */

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Cif_Cam_camStructures
 * \{ */
typedef struct
{
    uint8            length;       /**< \brief length */
    IFX_CONST uint8 *entry;        /**< \brief entry */
} IfxCif_Cam_TableInfo;

/** \} */

/** \addtogroup IfxLld_Cif_Cam_camStructures
 * \{ */
/** \brief Single component JPEG tables
 */
typedef struct
{
    IfxCif_Cam_TableInfo q;        /**< \brief q */
    IfxCif_Cam_TableInfo dc;       /**< \brief dc */
    IfxCif_Cam_TableInfo ac;       /**< \brief ac */
} IfxCif_Cam_JpegTable;

/** \brief Picture info
 */
typedef struct
{
    uint16 hSize;         /**< \brief horizontal (number of pixel) */
    uint16 vSize;         /**< \brief vertical (number of lines) */
    uint16 hOffset;       /**< \brief horizontal offset */
    uint16 vOffset;       /**< \brief vertical offset */
} IfxCif_Cam_PictureInfo;

/** \} */

/** \addtogroup IfxLld_Cif_Cam_camStructures
 * \{ */
/** \brief JPEG tables for all components
 */
typedef struct
{
    IfxCif_Cam_JpegTable y;        /**< \brief y */
    IfxCif_Cam_JpegTable uv;       /**< \brief uv */
} IfxCif_Cam_JpegTables;

/** \brief Single path memory interface configuration (used in initialisation/configuration)
 */
typedef struct
{
    float32 memFactor;       /**< \brief multiples of frame count for memory allocation */
    uint16  hSize;           /**< \brief horizontal (number of pixel) */
    uint16  vSize;           /**< \brief vertical (number of lines) */
    uint16  hOffset;         /**< \brief horizontal offset */
    uint16  vOffset;         /**< \brief vertical offset */
    boolean byteSwap;        /**< \brief byte swapping */
} IfxCif_Cam_MemConfig;

/** \brief Single path memory interface information (used in Runtime)
 */
typedef struct
{
    Ifx_AddressValue       start;       /**< \brief start address */
    uint32                 size;        /**< \brief size in bytes */
    IfxCif_Cam_PictureInfo image;       /**< \brief image */
} IfxCif_Cam_MemInfo;

/** \} */

/** \addtogroup IfxLld_Cif_Cam_camStructures
 * \{ */
/** \brief Common configuration
 */
typedef struct
{
    IfxCif_Cam_PictureInfo           ispIn;                                /**< \brief ispIn */
    IfxCif_Cam_MemConfig             mainPath;                             /**< \brief main path */
    IfxCif_Cam_MemConfig             extraPaths[IFXCIF_EXTRA_PATHS];       /**< \brief extra paths */
    IfxCif_InputInterface            inputInterface;                       /**< \brief input interface */
    Ifx_IsrSetting                   ispInterrupt;                         /**< \brief isp interrupt */
    boolean                          debugPathEnabled;                     /**< \brief debug path enabled */
    boolean                          watchdogEnabled;                      /**< \brief watchdog enabled */
    IFX_CONST IfxCif_Cam_JpegTables *jpegTables;                           /**< \brief jpeg tables */
    boolean                          jpegEnabled;                          /**< \brief jpeg enabled */
} IfxCif_Cam_Common;

/** \brief Downscaler configuration
 */
typedef struct
{
    boolean                            enabled;          /**< \brief enable */
    IfxCif_LinearDownscalerScalingMode hMode;            /**< \brief hMode */
    IfxCif_LinearDownscalerScalingMode vMode;            /**< \brief vMode */
    uint8                              hFactor;          /**< \brief horizontal factor to program into IfxCif_setLinearDownscalerScalingFactors() */
    uint8                              vFactor;          /**< \brief vertical factor to program into IfxCif_setLinearDownscalerScalingFactors() */
    uint8                              sizeFactor;       /**< \brief memory size factor, e.g. half in both H and V usually gives factor of 4 */
} IfxCif_Cam_Downscaling;

/** \brief JPEG JFIF header structure
 */
typedef struct
{
    uint16 unknown;             /**< \brief unknown */
    uint16 app0;                /**< \brief app0 */
    uint16 length;              /**< \brief length */
    uint8  ident[5];            /**< \brief ident */
    uint16 version;             /**< \brief version */
    uint8  density;             /**< \brief density */
    uint16 densityX;            /**< \brief densityX */
    uint16 densityY;            /**< \brief densityY */
    uint8  thumbX;              /**< \brief thumbX */
    uint8  thumbY;              /**< \brief thumbY */
    uint8  thumbData[20];       /**< \brief thumbData */
} IfxCif_Cam_JfifHeader;

/** \brief All paths memory partitioning
 */
typedef struct
{
    IfxCif_Cam_MemInfo y;                            /**< \brief y */
    IfxCif_Cam_MemInfo cb;                           /**< \brief cb */
    IfxCif_Cam_MemInfo cr;                           /**< \brief cr */
    IfxCif_Cam_MemInfo ep[IFXCIF_EXTRA_PATHS];       /**< \brief ep */
} IfxCif_Cam_MemAreas;

/** \} */

/** \addtogroup IfxLld_Cif_Cam_camStructures
 * \{ */
/** \brief Structure which is used as handle for the CIF-CAM functions.
 * This stores cached variables useful for run-time operations.
 */
typedef struct
{
    IfxCif_Cam_MemAreas    memAreas;              /**< \brief memory area definition */
    uint32                 totalMemSize;          /**< \brief total occupied memory area (in bytes) */
    Ifx_AddressValue       nextFreeAddress;       /**< \brief next free storage address */
    uint32                 availMemSize;          /**< \brief total available RAM space left (in bytes) */
    Ifx_CIF               *cif;                   /**< \brief pointer to base address of CIF module SFRs */
    Ifx_EMEM              *emem;                  /**< \brief pointer to base address of EMEM module SFRs */
    IfxCif_Cam_JfifHeader *jfif;                  /**< \brief pointer to the last acquired JPEG header */
    IfxCif_Cam_Status      configResult;          /**< \brief status of last configuration (or initialisation) */
    IfxCif_Cam_IspMode     ispMode;               /**< \brief actual ISP mode */
    boolean                ispBpp;                /**< \brief byte per pixel */
} IfxCif_Cam;

/** \brief Camera specific configuration
 */
typedef struct
{
    IFX_CONST IfxCif_Cam_Common      *common;               /**< \brief pointer to common configuration */
    IfxCif_Cam_IspMode                ispMode;              /**< \brief ISP mode */
    uint16                            ispRawBpp;            /**< \brief bytes per pixel of the input stream */
    IfxCif_IspSyncPolarity            hSyncPolarity;        /**< \brief polarity of HSYNC signal */
    IfxCif_IspSyncPolarity            vSyncPolarity;        /**< \brief polarity of VSYNC signal */
    IfxCif_IspSamplingEdge            samplingEdge;         /**< \brief sampling edge */
    uint16                            setupDataCount;       /**< \brief number of camera-specific configuration data (in DWORDS) */
    IFX_CONST uint32                 *setupDataTable;       /**< \brief pointer to the first entry of camera-specific configuration data */
    IFX_CONST IfxCif_Cam_Downscaling *downscaling;          /**< \brief downscaling settings for ExtraPath 1 */
} IfxCif_Cam_Config;

/** \} */

/** \addtogroup IfxLld_Cif_Cam_camFunctions
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clear the frame reception flag
 * \param cam cam handle
 * \return None
 */
IFX_INLINE void IfxCif_Cam_clearFrameReceptionFlag(const IfxCif_Cam *cam);

/** \brief Clear the JPEG encoder completed flag
 * \param cam cam handle
 * \return None
 */
IFX_INLINE void IfxCif_Cam_clearJpegEncodingCompleteFlag(const IfxCif_Cam *cam);

/** \brief Convert the CPU address into BBB address
 * \param cam cam handle
 * \param cpuAddress Address value
 */
IFX_INLINE Ifx_AddressValue IfxCif_Cam_getBbbAddress(const IfxCif_Cam *cam, Ifx_AddressValue cpuAddress);

/** \brief Returns the size (in bytes) of one last acquired extra-path image data
 * \param cam cam handle
 * \param z Extra path
 */
IFX_INLINE uint32 IfxCif_Cam_getExtraFrameSize(const IfxCif_Cam *cam, IfxCif_ExtraPath z);

/**
 * \param cam cam handle
 * \param z Extra path
 */
IFX_INLINE IFX_CONST IfxCif_Cam_MemInfo *IfxCif_Cam_getExtraPathMemInfo(const IfxCif_Cam *cam, IfxCif_ExtraPath z);

/** \brief Returns the size (in bytes) of last acquired main-path image data
 * \param cam cam handle
 */
IFX_INLINE uint32 IfxCif_Cam_getFrameSize(const IfxCif_Cam *cam);

/** \brief Returns the size (in bytes) of last encoded main-path image data
 * \param cam cam handle
 */
IFX_INLINE uint32 IfxCif_Cam_getJpegEncodedSize(const IfxCif_Cam *cam);

/** \brief Returns the address location of last acquired extra-path image frame data
 * \param cam cam handle
 * \param z Extra path
 */
IFX_INLINE Ifx_AddressValue IfxCif_Cam_getLastExtraAddress(const IfxCif_Cam *cam, IfxCif_ExtraPath z);

/**
 * \param cam cam handle
 * \param z Extra path
 * \param address Address Value
 * \param size const IfxCif_Cam_MemInfo *m;
 *     *size    = IfxCif_Cam_getExtraFrameSize(cam, z);
 *     *address = IfxCif_Cam_getLastExtraAddress(cam, z);
 *     m        = IfxCif_Cam_getExtraPathMemInfo(cam, z);
 *     return m;
 */
IFX_INLINE IFX_CONST IfxCif_Cam_MemInfo *IfxCif_Cam_getLastExtraCaptureInfo(const IfxCif_Cam *cam, IfxCif_ExtraPath z, Ifx_AddressValue *address, uint32 *size);

/** \brief Returns the address location of last acquired main-path image frame data
 * \param cam cam handle
 * \return address location of last acquired main-path image frame data
 */
IFX_INLINE Ifx_AddressValue IfxCif_Cam_getLastFrameAddress(const IfxCif_Cam *cam);

/**
 * \param cam cam handle
 * \param address Address value
 * \param size Size
 */
IFX_INLINE IFX_CONST IfxCif_Cam_MemInfo *IfxCif_Cam_getLastFrameCaptureInfo(const IfxCif_Cam *cam, Ifx_AddressValue *address, uint32 *size);

/** \brief Returns the address location of next acquired main-path image frame data
 * \param cam cam handle
 */
IFX_INLINE Ifx_AddressValue IfxCif_Cam_getNextFrameAddress(const IfxCif_Cam *cam);

/** \brief Returns the address location of next acquired JPEG image frame data
 * \param cam cam handle
 */
IFX_INLINE Ifx_AddressValue IfxCif_Cam_getNextJpegFrameAddress(IfxCif_Cam *cam);

/** \brief Increment the address related to memory interface of for main-path. The address is wrapped around at the boundary of the allocated RAM size
 * \param cam cam handle
 * \param address Address value
 */
IFX_INLINE Ifx_AddressValue IfxCif_Cam_incrementAddress(const IfxCif_Cam *cam, Ifx_AddressValue address);

/** \brief Returns TRUE if JPEG encoding is completed
 * \param cam cam handle
 * \return TRUE if one frame reception is completed
 */
IFX_INLINE boolean IfxCif_Cam_isFrameReceptionComplete(const IfxCif_Cam *cam);

/** \brief
 * \param cam cam handle
 * \return TRUE if JPEG encoding is completed
 */
IFX_INLINE boolean IfxCif_Cam_isJpegEncodingComplete(const IfxCif_Cam *cam);

/**
 * \param cam cam handle
 * \return None
 */
IFX_INLINE void IfxCif_Cam_skipPicture(const IfxCif_Cam *cam);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clear all CIF module notification flags
 * \param cam cam handle
 * \return None
 */
IFX_EXTERN void IfxCif_Cam_clearAllFlags(const IfxCif_Cam *cam);

/** \brief Disable the JPEG encoder and route the acquisition path accordingly to the 'original' configuration, i.e. RAW or main path.
 * \param cam cam handle
 * \return None
 */
IFX_EXTERN void IfxCif_Cam_disableJpegEncoder(const IfxCif_Cam *cam);

/** \brief Enable the JPEG encoder and route the acquisition path accordingly
 * \param cam cam handle
 * \return None
 */
IFX_EXTERN void IfxCif_Cam_enableJpegEncoder(const IfxCif_Cam *cam);

/** \brief Returns pointer to the variable storing picture info of an extra-path
 * \param cam cam handle
 * \param z Extra Path
 * \return pointer to the variable storing picture info of an extra-path
 */
IFX_EXTERN IFX_CONST IfxCif_Cam_PictureInfo *IfxCif_Cam_getExtraPathPictureInfo(const IfxCif_Cam *cam, IfxCif_ExtraPath z);

/** \brief Returns pointer to the memory structure storing last JPEG encoding process
 * \param cam cam handle
 * \param address EMEM address which stores the JPEG frame
 * \param size size of JPEG frame
 * \return TRUE if there was new frame encoded, else FALSE
 */
IFX_EXTERN boolean IfxCif_Cam_getLastJpegEncodingInfo(IfxCif_Cam *cam, Ifx_AddressValue *address, sint32 *size);

/** \brief Initialise the CIF, EMEM, PORT, I2C and the camera itself
 * \param cam cam handel
 * \param config configurations structure
 * \param initCam if TRUE camera will be reconfigured, else camera will not be reconfigured.
 */
IFX_EXTERN IfxCif_Cam_Status IfxCif_Cam_init(IfxCif_Cam *cam, const IfxCif_Cam_Config *config, boolean initCam);

/** \brief Restart capture by enabling the ISP output
 * \param cam cam handle
 * \param frames Number of acquisitions. Set to zero for continuous acquisition
 * \return None
 */
IFX_EXTERN void IfxCif_Cam_restartCapture(const IfxCif_Cam *cam, uint8 frames);

/** \brief Start capture by enabling the ISP output and initialising the MI counter
 * \param cam cam handle
 * \param frames Number of acquisitions. Set to zero for continuous acquisition
 * \return None
 */
IFX_EXTERN void IfxCif_Cam_startCapture(const IfxCif_Cam *cam, uint8 frames);

/** \brief Stop capture by disabling the ISP output
 * \param cam cam handle
 * \return None
 */
IFX_EXTERN void IfxCif_Cam_stopCapture(const IfxCif_Cam *cam);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxCif_Cam_clearFrameReceptionFlag(const IfxCif_Cam *cam)
{
    (void)cam;
    MODULE_CIF.ISP.ICR.U = (IFX_CIF_ISP_ICR_ICR_FRAME_MSK << IFX_CIF_ISP_ICR_ICR_FRAME_OFF);
}


IFX_INLINE void IfxCif_Cam_clearJpegEncodingCompleteFlag(const IfxCif_Cam *cam)
{
    (void)cam;
    MODULE_CIF.JPE.STATUS_ICR.U = (IFX_CIF_JPE_STATUS_ICR_ENCODE_DONE_MSK << IFX_CIF_JPE_STATUS_ICR_ENCODE_DONE_OFF);
}


IFX_INLINE Ifx_AddressValue IfxCif_Cam_getBbbAddress(const IfxCif_Cam *cam, Ifx_AddressValue cpuAddress)
{
    (void)cam;

    return (Ifx_AddressValue)(((uint32)cpuAddress & 0x00FFFFFFU) + IFXEMEM_START_ADDR_BBB);
}


IFX_INLINE uint32 IfxCif_Cam_getExtraFrameSize(const IfxCif_Cam *cam, IfxCif_ExtraPath z)
{
    sint32 size = 0;

    if (z <= IfxCif_ExtraPath_5)
    {
        size = cam->memAreas.ep[z].image.hSize * cam->memAreas.ep[z].image.vSize;

        if ((cam->ispMode != IfxCif_Cam_IspMode_raw) || (cam->ispBpp > 1))
        {
            size = size * 2;
        }
    }
    else
    {
        IFXCIF_DEBUG;
    }

    return (uint32)size;
}


IFX_INLINE IFX_CONST IfxCif_Cam_MemInfo *IfxCif_Cam_getExtraPathMemInfo(const IfxCif_Cam *cam, IfxCif_ExtraPath z)
{
    return &(cam->memAreas.ep[z]);
}


IFX_INLINE uint32 IfxCif_Cam_getFrameSize(const IfxCif_Cam *cam)
{
    (void)cam;
    sint32 size = MODULE_CIF.MI.MP_Y_OFFS_CNT_SHD.U - MODULE_CIF.MI.MP_Y_OFFS_CNT_START.U;

    if (size <= 0)
    {
        size += MODULE_CIF.MI.MP_Y_SIZE_INIT.U;
    }

    return (uint32)size;
}


IFX_INLINE uint32 IfxCif_Cam_getJpegEncodedSize(const IfxCif_Cam *cam)
{
    (void)cam;

    /* 0x240 is size of JFIF header. */
    return 0x240 + IfxCif_Cam_getFrameSize(cam);
}


IFX_INLINE Ifx_AddressValue IfxCif_Cam_getLastExtraAddress(const IfxCif_Cam *cam, IfxCif_ExtraPath z)
{
    Ifx_AddressValue address = 0;

    if (z <= IfxCif_ExtraPath_5)
    {
        address = (Ifx_AddressValue)((uint32)cam->memAreas.ep[z].start + MODULE_CIF.MIEP.CH_1S[z].OFFS_CNT_START.U);
    }

    return address;
}


IFX_INLINE IFX_CONST IfxCif_Cam_MemInfo *IfxCif_Cam_getLastExtraCaptureInfo(const IfxCif_Cam *cam, IfxCif_ExtraPath z, Ifx_AddressValue *address, uint32 *size)
{
    const IfxCif_Cam_MemInfo *m;
    *size    = IfxCif_Cam_getExtraFrameSize(cam, z);
    *address = IfxCif_Cam_getLastExtraAddress(cam, z);
    m        = IfxCif_Cam_getExtraPathMemInfo(cam, z);
    return m;
}


IFX_INLINE Ifx_AddressValue IfxCif_Cam_getLastFrameAddress(const IfxCif_Cam *cam)
{
    (void)cam;

    return (Ifx_AddressValue)(IFXEMEM_START_ADDR_CPU_CACHED + MODULE_CIF.MI.MP_Y_OFFS_CNT_START.U);
}


IFX_INLINE IFX_CONST IfxCif_Cam_MemInfo *IfxCif_Cam_getLastFrameCaptureInfo(const IfxCif_Cam *cam, Ifx_AddressValue *address, uint32 *size)
{
    const IfxCif_Cam_MemInfo *m;
    *size    = IfxCif_Cam_getFrameSize(cam);
    *address = IfxCif_Cam_getLastFrameAddress(cam);
    m        = &(cam->memAreas.y);
    return m;
}


IFX_INLINE Ifx_AddressValue IfxCif_Cam_getNextFrameAddress(const IfxCif_Cam *cam)
{
    (void)cam;

    return (Ifx_AddressValue)(IFXEMEM_START_ADDR_CPU_CACHED + MODULE_CIF.MI.MP_Y_OFFS_CNT_SHD.U);
}


IFX_INLINE Ifx_AddressValue IfxCif_Cam_getNextJpegFrameAddress(IfxCif_Cam *cam)
{
    Ifx_AddressValue address = IfxCif_Cam_getNextFrameAddress(cam);
    cam->jfif = (IfxCif_Cam_JfifHeader *)address; /* note: no cast to (void *) to satisfy C++ compilers */
    return address;
}


IFX_INLINE Ifx_AddressValue IfxCif_Cam_incrementAddress(const IfxCif_Cam *cam, Ifx_AddressValue address)
{
    address = (Ifx_AddressValue)((uint32)address + 4U);

    if ((uint32)address >= (IFXEMEM_START_ADDR_CPU_CACHED + cam->memAreas.y.size))
    {   /* back to base */
        address = (Ifx_AddressValue)IFXEMEM_START_ADDR_CPU_CACHED;
    }

    return address;
}


IFX_INLINE boolean IfxCif_Cam_isFrameReceptionComplete(const IfxCif_Cam *cam)
{
    (void)cam;

    return MODULE_CIF.ISP.RIS.B.RIS_FRAME != 0;
}


IFX_INLINE boolean IfxCif_Cam_isJpegEncodingComplete(const IfxCif_Cam *cam)
{
    (void)cam;

    return MODULE_CIF.JPE.STATUS_RIS.B.ENCODE_DONE != 0;
}


IFX_INLINE void IfxCif_Cam_skipPicture(const IfxCif_Cam *cam)
{
    (void)cam;
    IfxCif_setMiOffsetCounterInitializationEnableState(IfxCif_State_Disabled);
    IfxCif_miSkipPicture();
}


#endif /* IFXCIF_CAM_H */
