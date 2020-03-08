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
 * @brief Test for consistency with BIG_512_60
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arch.h"
#include "amcl.h"
#include "utils.h"
#include "big_512_60.h"

int main()
{

    int i,j;
    char raw[256], bytes[MODBYTES_512_60];
    csprng rng;

    BIG_512_60 F,G,H,I,Z;
    DBIG_512_60 DF,DG;

    /* Fake random source */
    RAND_clean(&rng);
    for (i=0; i<256; i++) raw[i]=(char)i;
    RAND_seed(&rng,256,raw);

    /* Set to zero */
    BIG_512_60_zero(F);
    BIG_512_60_zero(G);
    BIG_512_60_dzero(DF);
    BIG_512_60_dzero(DG);

    /* Testing equal function and set zero function */
    if(BIG_512_60_comp(G,F) | !BIG_512_60_iszilch(F) | !BIG_512_60_iszilch(G) | BIG_512_60_dcomp(DG,DF) | !BIG_512_60_diszilch(DF) | !BIG_512_60_diszilch(DG))
    {
        printf("ERROR comparing or setting zero BIG_512_60\n");
        exit(EXIT_FAILURE);
    }

    /* Testing coping and equal function */
    BIG_512_60_random(F,&rng);
    BIG_512_60_random(DF,&rng);
    BIG_512_60_copy(G,F);
    BIG_512_60_dcopy(DG,DF);
    if(BIG_512_60_comp(G,F) | BIG_512_60_dcomp(DG,DF))
    {
        printf("ERROR testing coping and equal BIG_512_60\n");
        exit(EXIT_FAILURE);
    }

    /* Testing addition, subtraction */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(F,&rng);
        BIG_512_60_random(H,&rng);
        BIG_512_60_copy(G,F);
        BIG_512_60_add(G,G,H);
        BIG_512_60_sub(G,G,H);
        BIG_512_60_sub(H,H,H);
        if(BIG_512_60_comp(G,F) | !BIG_512_60_iszilch(H))
        {
            printf("ERROR testing addition/subtraction BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }
    BIG_512_60_one(I);
    BIG_512_60_zero(Z);
    BIG_512_60_zero(F);
    BIG_512_60_add(F,F,F);
    BIG_512_60_add(Z,I,Z);
    if(BIG_512_60_comp(Z,I) | !BIG_512_60_iszilch(F))
    {
        printf("ERROR testing addition/subtraction BIG_512_60\n");
        exit(EXIT_FAILURE);
    }

    /* Testing small multiplication and division by 3 */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(F,&rng);
        BIG_512_60_copy(G,F);
        BIG_512_60_imul(G,G,3);
        BIG_512_60_div3(G);
        if(BIG_512_60_comp(G,F))
        {
            printf("ERROR testing small multiplication and division by 3 BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing small multiplication and addition */
    BIG_512_60_random(F,&rng);
    for (j = 1; j <= 20; ++j)
    {
        BIG_512_60_imul(H,F,j);
        BIG_512_60_copy(G,F);
        for (i = 1; i < j; ++i)
        {
            BIG_512_60_add(G,G,F);
        }
        BIG_512_60_norm(G);
        BIG_512_60_norm(H);
        if(BIG_512_60_comp(H,G) != 0)
        {
            printf("ERROR testing small multiplication and addition BIG_512_60, %d\n",j);
            exit(EXIT_FAILURE);
        }
    }

    /* Testing square */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(F,&rng);
        BIG_512_60_copy(G,F);
        BIG_512_60_sqr(DG,G);
        BIG_512_60_mul(DF,F,F);
        if(BIG_512_60_dcomp(DG,DF))
        {
            printf("ERROR testing square BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing square mod */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(H,&rng);
        BIG_512_60_randomnum(F,H,&rng);
        BIG_512_60_copy(G,F);
        BIG_512_60_modsqr(G,G,H);
        BIG_512_60_sqr(DF,F);
        BIG_512_60_dmod(F,DF,H);
        if(BIG_512_60_comp(G,F))
        {
            printf("ERROR testing mod square BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing from and to bytes conversion */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(F,&rng);
        BIG_512_60_copy(G,F);
        BIG_512_60_toBytes(bytes,G);
        BIG_512_60_fromBytes(G,bytes);
        if(BIG_512_60_comp(G,F))
        {
            printf("ERROR testing from and to bytes conversion BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }
    BIG_512_60_toBytes(bytes,G);
    BIG_512_60_fromBytesLen(G,bytes,MODBYTES_512_60);
    if(BIG_512_60_comp(G,F))
    {
        printf("ERROR testing from and to bytes conversion BIG_512_60\n");
        exit(EXIT_FAILURE);
    }

    /* Testing small increment and decrement */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(F,&rng);
        BIG_512_60_copy(G,F);
        BIG_512_60_inc(G,i);
        BIG_512_60_dec(G,i);
        if(BIG_512_60_comp(G,F))
        {
            printf("ERROR testing small increment and decrement BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing small increment and decrement */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(F,&rng);
        BIG_512_60_copy(G,F);
        if(BIG_512_60_comp(G,F))
        {
            printf("ERROR testing small increment and decrement BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing random with modulo */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(G,&rng);
        BIG_512_60_randomnum(F,G,&rng);
        if(BIG_512_60_comp(F,G)>0)
        {
            printf("ERROR testing random with modulo BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing mod neg */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(H,&rng);
        BIG_512_60_randomnum(F,H,&rng);
        BIG_512_60_modneg(G,F,H);
        BIG_512_60_modneg(G,G,H);
        BIG_512_60_norm(G);
        BIG_512_60_norm(F);
        if(BIG_512_60_comp(F,G))
        {
            printf("ERROR testing mod neg BIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Testing copy from/to BIG_512_60/DBIG_512_60 */
    for (i=0; i<100; i++)
    {
        BIG_512_60_random(F,&rng);
        BIG_512_60_copy(G,F);
        BIG_512_60_dzero(DF);
        BIG_512_60_dsucopy(DF,F);
        BIG_512_60_sducopy(F,DF);
        if(BIG_512_60_comp(F,G))
        {
            printf("ERROR testing copy from/to BIG_512_60/DBIG_512_60\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("SUCCESS TEST CONSISTENCY OF BIG_512_60 PASSED\n");
    exit(EXIT_SUCCESS);
}
