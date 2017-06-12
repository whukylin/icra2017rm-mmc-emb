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
 
#include "main.h"

MPU_Data_t imu_data;

static uint32_t last_time = 0;
int main()
{
	// Boot KOS
	KOS_Boot();
	
	// Play startup music
	Snd_Play();
	// Wait for startup music
	Delay_Ms(1000);
	// Stop startup music
	Snd_Stop();
	
	while(1)
	{
		//Srs_Proc();
		//Dnl_Proc();
		//if (Clk_GetUsTick() % 2000 == 0) {
		//	Upl_Proc();
		//}
		if (Clk_GetUsTick() - last_time > 2000) {
			last_time = Clk_GetUsTick();
			if (MPU6500_Read(&imu_data)) {
				printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", imu_data.ax, imu_data.ay, imu_data.az, imu_data.temp, imu_data.gx, imu_data.gy, imu_data.gz);
			}
		}
  }
}
