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
 
#include "com.h"

/***********************************/
/*          Communication          */
/***********************************/

/*
static uint8_t buf[COM_RX_BUF_SIZE];

static uint8_t id;
static uint8_t len;
static uint8_t key;
static uint32_t head;
*/

void Com_Init(void)
{
	//Cfg_Init();
}

void Com_Proc(void)
{

}

void Tty_Proc(uint8_t data)
{
	Wdg_Feed(WDG_IDX_COM);
}
