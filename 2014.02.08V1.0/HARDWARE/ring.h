/*********************************************************************** 
 * $Workfile:   ring.h  $ 
 * $Revision:   1.0  $ 
 * $Author:   FuraD  $ 
 * $Date:   May 18 2005 11:15:52  $ 
 * 
 * Project: Ring buffer manager
 * 
 * Description:
 *     This package provides a simple ring buffer manager with
 *     the following operations:
 *         RING_INIT: Define ring size and initialize ring
 *                    parameters.  This should be the first
 *                    operation used.
 *         RING_FLUSH: Initialize ring parameters.
 *         RING_COUNT: Return number of elements in buffer.
 *         RING_PUTC: Add element to buffer.
 *         RING_GETC: Remove element from buffer.
 *         RING_PUTC_MULTIPLE: Add multiple elements to buffer.
 *         RING_GETC_MULTIPLE: Remove multiple elements from buffer.
 *
 * Notes:
 *    The user of this ring buffer is responsible for specifying
 *    the ring buffer size and for providing the memory space used
 *    to store ring buffer data.
 *     
 * Revision History: 
 * $Log::   //smaicnt2/pvcs/VM/sharpmcu/archives/sharpmcu/software/abl/$
 * 
 *    Rev 1.0   May 18 2005 11:15:52   FuraD
 * Initial revision.
 * 
 *********************************************************************** 
 * 
 *  SHARP MICROELECTRONICS OF THE AMERICAS MAKES NO REPRESENTATION 
 *  OR WARRANTIES WITH RESPECT TO THE PERFORMANCE OF THIS SOFTWARE, 
 *  AND SPECIFICALLY DISCLAIMS ANY RESPONSIBILITY FOR ANY DAMAGES, 
 *  SPECIAL OR CONSEQUENTIAL, CONNECTED WITH THE USE OF THIS SOFTWARE. 
 * 
 *  SHARP MICROELECTRONICS OF THE AMERICAS PROVIDES THIS SOFTWARE SOLELY 
 *  FOR THE PURPOSE OF SOFTWARE DEVELOPMENT INCORPORATING THE USE OF A 
 *  SHARP MICROCONTROLLER OR SYSTEM-ON-CHIP PRODUCT. USE OF THIS SOURCE 
 *  FILE IMPLIES ACCEPTANCE OF THESE CONDITIONS. 
 * 
 *  COPYRIGHT (c) 2005 SHARP MICROELECTRONICS OF THE AMERICAS 
 *      CAMAS, WA
 * 
 **********************************************************************/

#ifndef RING_H
#define RING_H

//#include "abl_types.h"

#ifdef __cplusplus


extern "C"
{
#endif
#include "stm32f10x.h"


/*********************************************************************** 
 * 1. Ring Buffer Data Structure
 **********************************************************************/

/*
 * Ring buffer structure 
 */
typedef struct RING_S
{
  u16 size;   // buffer size    
   u16 count;  // buffer data count
   u16 wr_pos; // input position
   u16 rd_pos; // output position
   unsigned char*  data;   // pointer to data buffer
} RING_T;


/*********************************************************************** 
 * 2. Ring Buffer Operations
 **********************************************************************/

/* 
 * 2.1. Initialize ring buffer.
 */
#define RING_INIT(ring, nbytes)\
{\
    ((RING_T*)ring)->size = (nbytes);\
    ((RING_T*)ring)->count = 0;\
    ((RING_T*)ring)->wr_pos = 0;\
    ((RING_T*)ring)->rd_pos = 0;\
}

/* 
 * 2.2. Drain and reset ring buffer.
 */
#define RING_FLUSH(ring)\
{\
    ((RING_T*)ring)->count = 0;\
    ((RING_T*)ring)->wr_pos = 0;\
    ((RING_T*)ring)->rd_pos = 0;\
}

/* 
 * 2.3. Return the current buffer count.
 */
#define RING_COUNT(ring) ((RING_T*)ring)->count

/* 
 * 2.4. Put a byte into the buffer.
 */
#define RING_PUTC(ring, rdata)\
{\
    ((RING_T*)ring)->count++;\
    ((RING_T*)ring)->data[((RING_T*)ring)->wr_pos++] = (rdata);\
    if (((RING_T*)ring)->wr_pos >= ((RING_T*)ring)->size)\
    {\
        ((RING_T*)ring)->wr_pos = 0;\
    }\
}

/*
 * 2.5. Get a byte from the buffer.
 */
#define RING_GETC(ring, rdata)\
{\
    ((RING_T*)ring)->count--;\
    if (((RING_T*)ring)->rd_pos >= ((RING_T*)ring)->size)\
    {\
        ((RING_T*)ring)->rd_pos = 0;\
    }\
    (rdata) = ((RING_T*)ring)->data[((RING_T*)ring)->rd_pos++];\
} 

/* 
 * 2.6. Put multiple bytes into the buffer.
 * 
 * Note: This define assumes: nbytes <= ring->size - ring->count.
 *
 * Two cases:
 *
 *   1. No wrap:
 *
 *      Ex: size  wr_pos  nbytes 
 *           512     510       2
 *
 *          wr_pos_stop1: 512
 *          wr_pos      : 510,511
 *
 *   2. Wrap:
 *
 *      Ex: size  wr_pos  nbytes
 *           512     510       5
 *
 *          wr_pos_stop1: 512
 *          wr_pos_stop2: 3
 *          wr_pos      : 510,511,0,1,2
 *
 */
#define RING_PUTC_MULTIPLE(ring, buffer, nbytes)\
{\
    unsigned int wr_pos_stop1;\
    unsigned int wr_pos_stop2;\
    wr_pos_stop1 = ((RING_T*)ring)->wr_pos + (nbytes);\
    if (wr_pos_stop1 <= ((RING_T*)ring)->size)\
    {\
        while (((RING_T*)ring)->wr_pos < wr_pos_stop1)\
        {\
            ((RING_T*)ring)->data[((RING_T*)ring)->wr_pos++] = *(buffer)++;\
        }\
    }\
    else\
    {\
        wr_pos_stop2 = wr_pos_stop1 - ((RING_T*)ring)->size;\
        wr_pos_stop1 = ((RING_T*)ring)->size;\
        while (((RING_T*)ring)->wr_pos < wr_pos_stop1)\
        {\
            ((RING_T*)ring)->data[((RING_T*)ring)->wr_pos++] = *(buffer)++;\
        }\
        ((RING_T*)ring)->wr_pos = 0;\
        while (((RING_T*)ring)->wr_pos < wr_pos_stop2)\
        {\
            ((RING_T*)ring)->data[((RING_T*)ring)->wr_pos++] = *(buffer)++;\
        }\
    }\
    ((RING_T*)ring)->count += (nbytes);\
}

/* 
 * 2.7. Get multiple bytes from the buffer.
 * 
 * Note: This define assumes: nbytes <= ring->count.
 *
 * Two cases:
 *
 *   1. No wrap:
 *
 *      Ex: size  rd_pos  nbytes 
 *           512     510       2
 *
 *          rd_pos_stop1: 512
 *          rd_pos      : 510,511
 *
 *   2. Wrap:
 *
 *      Ex: size  rd_pos  nbytes
 *           512     510       5
 *
 *          rd_pos_stop1: 512
 *          rd_pos_stop2: 3
 *          rd_pos      : 510,511,0,1,2
 *
 */
#define RING_GETC_MULTIPLE(ring, buffer, nbytes)\
{\
    unsigned int rd_pos_stop1;\
    unsigned int rd_pos_stop2;\
    rd_pos_stop1 = ((RING_T*)ring)->rd_pos + (nbytes);\
    if (rd_pos_stop1 <= ((RING_T*)ring)->size)\
    {\
        while (((RING_T*)ring)->rd_pos < rd_pos_stop1)\
        {\
            *(buffer)++ = ((RING_T*)ring)->data[((RING_T*)ring)->rd_pos++];\
        }\
    }\
    else\
    {\
        rd_pos_stop2 = rd_pos_stop1 - ((RING_T*)ring)->size;\
        rd_pos_stop1 = ((RING_T*)ring)->size;\
        while (((RING_T*)ring)->rd_pos < rd_pos_stop1)\
        {\
            *(buffer)++ = ((RING_T*)ring)->data[((RING_T*)ring)->rd_pos++];\
        }\
        ((RING_T*)ring)->rd_pos = 0;\
        while (((RING_T*)ring)->rd_pos < rd_pos_stop2)\
        {\
            *(buffer)++ = ((RING_T*)ring)->data[((RING_T*)ring)->rd_pos++];\
        }\
    }\
    ((RING_T*)ring)->count -= (nbytes);\
}

#ifdef __cplusplus
}
#endif

#endif // RING_H

