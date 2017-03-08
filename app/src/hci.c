/**
 * Copyright (c) 2011-2016, Jack Mo (mobangjack@foxmail.com).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "hci.h"

/********************************************/
/*          Host Control Interface          */
/********************************************/

MouseButtonState_t mouseButtonStates[MOUSE_BTN_CNT];
MouseButtonEvent_t mouseButtonEvents[MOUSE_BTN_CNT];

static MouseButtonState_t lastRawMouseButtonStates[MOUSE_BTN_CNT];
static uint32_t mouseButtonPressedCounts[MOUSE_BTN_CNT];
static void GetMouseButtonStates(const HCP_t* hcp)
{
	const uint8_t* thisRawMouseButtonStates = hcp->mouse.b;
	uint32_t i = 0;
	for (; i < MOUSE_BTN_CNT; i++) {
		if (thisRawMouseButtonStates[i] == lastRawMouseButtonStates[i]) {
			if (mouseButtonPressedCounts[i] < MOUSE_BUTTON_PRESSED_CNT) {
				mouseButtonPressedCounts[i]++;
			} else {
				mouseButtonStates[i] = thisRawMouseButtonStates[i];
			}
		} else {
			mouseButtonPressedCounts[i] = 0;
		}
		lastRawMouseButtonStates[i] = thisRawMouseButtonStates[i];
	}
}

static MouseButtonState_t lastMouseButtonStates[MOUSE_BTN_CNT];
static void GetMouseButtonEvents(const HCP_t* hcp)
{
	uint32_t i = 0;
	for (; i < MOUSE_BTN_CNT; i++) {
		mouseButtonEvents[i] = GET_MOUSE_BUTTON_EVENT(lastMouseButtonStates[i],mouseButtonStates[i]);
		lastMouseButtonStates[i] = mouseButtonStates[i];
	}
}

static void GetFunctionalStateRef(const HCP_t* hcp)
{
	GetMouseButtonStates(hcp);
	GetMouseButtonEvents(hcp);
}

static Maf_t fx, fy, fz;
static float buf[3][KEY_CONTROL_MAF_LEN];
static void GetChassisVelocityRef(const HCP_t* hcp)
{
	float sx = hcp->key.press.Shift ? cfg.spd.x : cfg.spd.x / 2.f;
	float sy = hcp->key.press.Shift ? cfg.spd.y : cfg.spd.y / 2.f;
	float sz = hcp->key.press.Shift ? cfg.spd.z : cfg.spd.z / 2.f;
	float vx = hcp->key.press.A ? -sx : hcp->key.press.D ? sx : 0;
	float vy = hcp->key.press.S ? -sy : hcp->key.press.W ? sy : 0;
	float mx = constrain(hcp->mouse.x, -MOUSE_SPEED_MAX, MOUSE_SPEED_MAX);
	float vz = map(mx, -MOUSE_SPEED_MAX, MOUSE_SPEED_MAX, -sz, sz);
	cmd.cv.x = Maf_Proc(&fx, vx);
	cmd.cv.y = Maf_Proc(&fy, vy);
	cmd.cv.z = Maf_Proc(&fz, vz);
}

void Hci_Init(void)
{
	uint32_t i = 0;
	for (; i < MOUSE_BTN_CNT; i++) {
		lastRawMouseButtonStates[i] = 0;
		mouseButtonPressedCounts[i] = 0;
		mouseButtonStates[i] = 0;
		mouseButtonEvents[i] = 0;
		lastMouseButtonStates[i] = 0;
	}
	Maf_Init(&fx, buf[0], KEY_CONTROL_MAF_LEN);
	Maf_Init(&fy, buf[1], KEY_CONTROL_MAF_LEN);
	Maf_Init(&fz, buf[2], KEY_CONTROL_MAF_LEN);
}

void Hci_Proc(const HCP_t* hcp)
{
	GetFunctionalStateRef(hcp);
	GetChassisVelocityRef(hcp);
}
