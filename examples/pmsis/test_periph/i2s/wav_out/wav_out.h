/*
 * Copyright (C) 2018 GreenWaves Technologies
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __I2S_AUDIO_REC__
#define __I2S_AUDIO_REC_

#define WAV_HEADER_SIZE 44 //bytes

<<<<<<< HEAD
void dump_wav(char *filename, int width, int sampling_rate, int nb_channels, void *data, int size);
=======
void dump_wav(char *filename, int width, int sampling_rate, void *data, int size);
>>>>>>> 3.1.1_dev_001-edit_BitCraze_DD

#endif