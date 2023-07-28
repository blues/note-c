// Copyright 2023 Blues Inc.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.

#include <stdint.h>

//**************************************************************************/
/*!
  @brief Decode a string encoded with COBS encoding

  @details Because the decoded length is guaranteed to be less than or equal to
  length, the decode may be done in-place. Default behavior (with eop == 0) is
  to restore all '\0' into output data, but if a different value is specified,
  the input is XOR'ed such that THAT byte is the one that is assumed can't be
  in the input.

  @param  ptr Pointer to the data to decode
  @param  length Length of the data to decode
  @param  eop Byte to use as the end-of-packet marker
  @param  dst Pointer to the buffer for the decoded data

  @return the length of the decoded data
 */
/**************************************************************************/
uint32_t cobsDecode(uint8_t *ptr, uint32_t length, uint8_t eop, uint8_t *dst)
{
    const uint8_t *start = dst, *end = ptr + length;
    uint8_t code = 0xFF, copy = 0;
    for (; ptr < end; copy--) {
        if (copy != 0) {
            *dst++ = (*ptr++) ^ eop;
        } else {
            if (code != 0xFF) {
                *dst++ = 0;
            }
            copy = code = (*ptr++) ^ eop;
            if (code == 0) {
                break;
            }
        }
    }
    return dst - start;
}

//**************************************************************************/
/*!
  @brief  Encode a string with Consistent Overhead Byte Stuffing (COBS) encoding

  @details behavior (with eop == 0) is to eliminate all '\0' from input data,
  but if a different value is specified, the output is XOR'ed such that THAT
  byte is the one that won't be contained in output.
  https://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing

  @param  ptr Pointer to the data to encode
  @param  length Length of the data to encode
  @param  eop Byte to use as the end-of-packet marker
  @param  dst Pointer to the buffer for the encoded data

  @return the length of the encoded data

  @note You may use `cobsEncodedLength()` to calculate the required size for
        the buffer pointed to by the `dst` parameter.

  @see cobsEncodedLength()
 */
/**************************************************************************/
uint32_t cobsEncode(uint8_t *ptr, uint32_t length, uint8_t eop, uint8_t *dst)
{
    uint8_t ch;
    uint8_t *start = dst;
    uint8_t code = 1;
    uint8_t *code_ptr = dst++;          // Where to insert the leading count
    while (length--) {
        ch = *ptr++;
        if (ch != 0) {                  // Input byte not zero
            *dst++ = ch ^ eop;
            code++;
        }
        if (ch == 0 || code == 0xFF) {  // Input is zero or complete block
            *code_ptr = code ^ eop;
            code = 1;
            code_ptr = dst++;
        }
    }
    *code_ptr = code ^ eop;             // Final code
    return (dst - start);
}

//**************************************************************************/
/*!
  @brief  Compute the encoding length of unencoded data

  @param  ptr Pointer to the data to encode
  @param  length Length of the data to encode

  @return the length required for encoded data
  @note  The computed length does not include the EOP (end-of-packet) marker
 */
/**************************************************************************/
uint32_t cobsEncodedLength(const uint8_t *ptr, uint32_t length)
{
    uint8_t ch;
    uint32_t dst = 1;
    uint8_t code = 1;
    while (length--) {
        ch = *ptr++;
        if (ch != 0) {                  // Input byte not zero
            dst++;
            code++;
        }
        if (ch == 0 || code == 0xFF) {  // Input is zero or complete block
            code = 1;
            dst++;
        }
    }
    return dst;
}

//**************************************************************************/
/*!
  @brief Compute the maximum length of unencoded data that can fit into a
  buffer of specified length.

  @param  bufLen Length of the buffer in bytes

  @return the length of unencoded data
  @note The computation may leave additional space at the end, including one
        byte for the EOP (end-of-packet) marker.
 */
/**************************************************************************/
uint32_t cobsGuaranteedFit(uint32_t bufLen)
{
    uint32_t cobsOverhead = 1 + (bufLen / 254) + 1;
    return (cobsOverhead > bufLen ? 0 : (bufLen - cobsOverhead));
}
