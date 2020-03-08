/*
	Licensed to the Apache Software Foundation (ASF) under one
	or more contributor license agreements.  See the NOTICE file
	distributed with this work for additional information
	regarding copyright ownership.  The ASF licenses this file
	to you under the Apache License, Version 2.0 (the
	"License"); you may not use this file except in compliance
	with the License.  You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing,
	software distributed under the License is distributed on an
	"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
	KIND, either express or implied.  See the License for the
	specific language governing permissions and limitations
	under the License.
*/

/**
 * @file config_ff_2048.h
 * @author Mike Scott
 * @brief COnfig FF Header File
 *
 */

#ifndef CONFIG_RSA_2048_H
#define CONFIG_RSA_2048_H

#include "amcl.h"
#include "config_big_1024_58.h"

// FF stuff

#define FFLEN_2048 2 /**< 2^n multiplier of BIGBITS to specify supported Finite Field size, e.g 2048=256*2^3 where BIGBITS=256 */


#endif
