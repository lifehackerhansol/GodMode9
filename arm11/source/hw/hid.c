/*
 *   This file is part of GodMode9
 *   Copyright (C) 2019 Wolfvak
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <types.h>
#include <hid_map.h>

#include "hw/codec.h"
#include "hw/hid.h"
#include "hw/mcu.h"

#define REG_HID	(~(*(vu16*)(0x10146000)) & BUTTON_ANY)

static u32 HID_ConvertCPAD(s16 cpad_x, s16 cpad_y)
{
	u32 ret = 0;

	if (cpad_x > 0) {
		ret |= BUTTON_RIGHT;
	} else if (cpad_x < 0) {
		ret |= BUTTON_LEFT;
	}

	if (cpad_y > 0) {
		ret |= BUTTON_UP;
	} else if (cpad_y < 0) {
		ret |= BUTTON_DOWN;
	}

	return ret;
}

u64 HID_GetState(void)
{
	CODEC_Input codec;
	u64 ret = 0;

	CODEC_Get(&codec);

	ret = REG_HID | mcuGetSpecialHID();
	if (!(ret & BUTTON_ARROW))
		ret |= HID_ConvertCPAD(0, 0);

	if (codec.ts_x <= 0xFFF)
		ret |= BUTTON_TOUCH;

	ret |= (((u64)codec.ts_x << 16) | (u64)codec.ts_y) << 32;

	return ret;
}
