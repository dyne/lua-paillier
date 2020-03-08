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
 * @file test_big_consistency.c
 * @author Alessandro Budroni
 * @brief Test for consistency with BIG_1024_58
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arch.h"
#include "amcl.h"
#include "utils.h"
#include "big_1024_58.h"

int main()
{

    int i,j;
    char raw[256], bytes[MODBYTES_1024_58];
    csprng rng;

    BIG_1024_58 F,G,H,I,Z;
    DBIG_1024_58 DF,DG;

    /* Fake random source */
    RAND_clean(&rng);
    for (i=0; i<256; i++) raw[i]=(char)i;
    RAND_seed(&rng,256,raw);

    /* Set to zero */
    BIG_1024_58_zero(F);
    BIG_1024_58_zero(G);
    BIG_1024_58_dzero(DF);
    BIG_1024_58_dzero(DG);

    /* Testing equal function and set zero function */
    if(BIG_1024_58_comp(G,F) | !BIG_1024_58_iszilch(F) | !BIG_1024_58_iszilch(G) | BIG_1024_58_dcomp(DG,DF) | !BIG_1024_58_diszilch(DF) | !BIG_1024_58_diszilch(DG))
    {
        printf("ERROR comparing or setting zero BIG_1024_58\n");
        exit(EXIT_FAILURE);
    }

    /* Testing coping and equal function */
    BIG_1024_58_random(F,&rng);
    BIG_1024_58_random(DF,&rng);
    BIG_1024_58_copy(G,F);
    BIG_1024_58_dcopy(DG,DF);
    if(BIG_1024_58_comp(G,F) | BIG_1024_58_dcomp(DG,DF))
    {
        printf("ERROR testing coping and equal BIG_1024_58\n");
        exit(EXIT_FAILURE);
    }

    /* Testing addition, subtraction */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(F,&rng);
        BIG_1024_58_random(H,&rng);
        BIG_1024_58_copy(G,F);
        BIG_1024_58_add(G,G,H);
        BIG_1024_58_sub(G,G,H);
        BIG_1024_58_sub(H,H,H);
        if(BIG_1024_58_comp(G,F) | !BIG_1024_58_iszilch(H))
        {
            printf("ERROR testing addition/subtraction BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }
    BIG_1024_58_one(I);
    BIG_1024_58_zero(Z);
    BIG_1024_58_zero(F);
    BIG_1024_58_add(F,F,F);
    BIG_1024_58_add(Z,I,Z);
    if(BIG_1024_58_comp(Z,I) | !BIG_1024_58_iszilch(F))
    {
        printf("ERROR testing addition/subtraction BIG_1024_58\n");
        exit(EXIT_FAILURE);
    }

    /* Testing small multiplication and division by 3 */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(F,&rng);
        BIG_1024_58_copy(G,F);
        BIG_1024_58_imul(G,G,3);
        BIG_1024_58_div3(G);
        if(BIG_1024_58_comp(G,F))
        {
            printf("ERROR testing small multiplication and division by 3 BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing small multiplication and addition */
    BIG_1024_58_random(F,&rng);
    for (j = 1; j <= 20; ++j)
    {
        BIG_1024_58_imul(H,F,j);
        BIG_1024_58_copy(G,F);
        for (i = 1; i < j; ++i)
        {
            BIG_1024_58_add(G,G,F);
        }
        BIG_1024_58_norm(G);
        BIG_1024_58_norm(H);
        if(BIG_1024_58_comp(H,G) != 0)
        {
            printf("ERROR testing small multiplication and addition BIG_1024_58, %d\n",j);
            exit(EXIT_FAILURE);
        }
    }

    /* Testing square */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(F,&rng);
        BIG_1024_58_copy(G,F);
        BIG_1024_58_sqr(DG,G);
        BIG_1024_58_mul(DF,F,F);
        if(BIG_1024_58_dcomp(DG,DF))
        {
            printf("ERROR testing square BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing square mod */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(H,&rng);
        BIG_1024_58_randomnum(F,H,&rng);
        BIG_1024_58_copy(G,F);
        BIG_1024_58_modsqr(G,G,H);
        BIG_1024_58_sqr(DF,F);
        BIG_1024_58_dmod(F,DF,H);
        if(BIG_1024_58_comp(G,F))
        {
            printf("ERROR testing mod square BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing from and to bytes conversion */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(F,&rng);
        BIG_1024_58_copy(G,F);
        BIG_1024_58_toBytes(bytes,G);
        BIG_1024_58_fromBytes(G,bytes);
        if(BIG_1024_58_comp(G,F))
        {
            printf("ERROR testing from and to bytes conversion BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }
    BIG_1024_58_toBytes(bytes,G);
    BIG_1024_58_fromBytesLen(G,bytes,MODBYTES_1024_58);
    if(BIG_1024_58_comp(G,F))
    {
        printf("ERROR testing from and to bytes conversion BIG_1024_58\n");
        exit(EXIT_FAILURE);
    }

    /* Testing small increment and decrement */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(F,&rng);
        BIG_1024_58_copy(G,F);
        BIG_1024_58_inc(G,i);
        BIG_1024_58_dec(G,i);
        if(BIG_1024_58_comp(G,F))
        {
            printf("ERROR testing small increment and decrement BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing small increment and decrement */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(F,&rng);
        BIG_1024_58_copy(G,F);
        if(BIG_1024_58_comp(G,F))
        {
            printf("ERROR testing small increment and decrement BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing random with modulo */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(G,&rng);
        BIG_1024_58_randomnum(F,G,&rng);
        if(BIG_1024_58_comp(F,G)>0)
        {
            printf("ERROR testing random with modulo BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing mod neg */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(H,&rng);
        BIG_1024_58_randomnum(F,H,&rng);
        BIG_1024_58_modneg(G,F,H);
        BIG_1024_58_modneg(G,G,H);
        BIG_1024_58_norm(G);
        BIG_1024_58_norm(F);
        if(BIG_1024_58_comp(F,G))
        {
            printf("ERROR testing mod neg BIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing copy from/to BIG_1024_58/DBIG_1024_58 */
    for (i=0; i<100; i++)
    {
        BIG_1024_58_random(F,&rng);
        BIG_1024_58_copy(G,F);
        BIG_1024_58_dzero(DF);
        BIG_1024_58_dsucopy(DF,F);
        BIG_1024_58_sducopy(F,DF);
        if(BIG_1024_58_comp(F,G))
        {
            printf("ERROR testing copy from/to BIG_1024_58/DBIG_1024_58\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("SUCCESS TEST CONSISTENCY OF BIG_1024_58 PASSED\n");
    exit(EXIT_SUCCESS);
}
