/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2019 Erik Moqvist
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * This file was generated by cantools version 37.2.0 Wed Aug 21 17:17:16 2024.
 */

#include <string.h>

#include "hawkmoto.h"

static inline uint8_t pack_left_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_left_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_right_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t pack_right_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

static inline uint8_t unpack_left_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value & mask) << shift);
}

static inline uint16_t unpack_left_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) << shift);
}

static inline uint8_t unpack_right_shift_u8(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value & mask) >> shift);
}

static inline uint16_t unpack_right_shift_u16(
    uint8_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint16_t)((uint16_t)(value & mask) >> shift);
}

int hawkmoto_example_message_pack(
    uint8_t *dst_p,
    const struct hawkmoto_example_message_t *src_p,
    size_t size)
{
    uint16_t temperature;

    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    temperature = (uint16_t)src_p->temperature;
    dst_p[0] |= pack_left_shift_u16(temperature, 0u, 0xffu);
    dst_p[1] |= pack_right_shift_u16(temperature, 8u, 0x0fu);
    dst_p[1] |= pack_left_shift_u8(src_p->average_radius, 4u, 0xf0u);
    dst_p[2] |= pack_right_shift_u8(src_p->average_radius, 4u, 0x03u);
    dst_p[2] |= pack_left_shift_u8(src_p->enable, 2u, 0x04u);

    return (8);
}

int hawkmoto_example_message_unpack(
    struct hawkmoto_example_message_t *dst_p,
    const uint8_t *src_p,
    size_t size)
{
    uint16_t temperature;

    if (size < 8u) {
        return (-EINVAL);
    }

    temperature = unpack_right_shift_u16(src_p[0], 0u, 0xffu);
    temperature |= unpack_left_shift_u16(src_p[1], 8u, 0x0fu);

    if ((temperature & (1u << 11)) != 0u) {
        temperature |= 0xf000u;
    }

    dst_p->temperature = (int16_t)temperature;
    dst_p->average_radius = unpack_right_shift_u8(src_p[1], 4u, 0xf0u);
    dst_p->average_radius |= unpack_left_shift_u8(src_p[2], 4u, 0x03u);
    dst_p->enable = unpack_right_shift_u8(src_p[2], 2u, 0x04u);

    return (0);
}

int16_t hawkmoto_example_message_temperature_encode(double value)
{
    return (int16_t)((value - 250.0) / 0.01);
}

double hawkmoto_example_message_temperature_decode(int16_t value)
{
    return (((double)value * 0.01) + 250.0);
}

bool hawkmoto_example_message_temperature_is_in_range(int16_t value)
{
    return ((value >= -2048) && (value <= 2047));
}

uint8_t hawkmoto_example_message_average_radius_encode(double value)
{
    return (uint8_t)(value / 0.1);
}

double hawkmoto_example_message_average_radius_decode(uint8_t value)
{
    return ((double)value * 0.1);
}

bool hawkmoto_example_message_average_radius_is_in_range(uint8_t value)
{
    return (value <= 50u);
}

uint8_t hawkmoto_example_message_enable_encode(double value)
{
    return (uint8_t)(value);
}

double hawkmoto_example_message_enable_decode(uint8_t value)
{
    return ((double)value);
}

bool hawkmoto_example_message_enable_is_in_range(uint8_t value)
{
    return (value <= 1u);
}