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

/* AMCL basic functions for Large Finite Field support */

#include "ff_4096.h"

/* x=y */
void FF_4096_copy(BIG_512_60 x[],BIG_512_60 y[],int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_copy(x[i],y[i]);
}

/* x=y<<n */
static void FF_4096_dsucopy(BIG_512_60 x[],BIG_512_60 y[],int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        BIG_512_60_copy(x[n+i],y[i]);
        BIG_512_60_zero(x[i]);
    }
}

/* x=y */
static void FF_4096_dscopy(BIG_512_60 x[],BIG_512_60 y[],int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        BIG_512_60_copy(x[i],y[i]);
        BIG_512_60_zero(x[n+i]);
    }
}

/* x=y>>n */
static void FF_4096_sducopy(BIG_512_60 x[],BIG_512_60 y[],int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_copy(x[i],y[n+i]);
}

/* set to zero */
void FF_4096_zero(BIG_512_60 x[],int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_zero(x[i]);
}

/* test equals 0 */
int FF_4096_iszilch(BIG_512_60 x[],int n)
{
    int i;
    for (i=0; i<n; i++)
        if (!BIG_512_60_iszilch(x[i])) return 0;
    return 1;
}

/* shift right by BIGBITS-bit words */
static void FF_4096_shrw(BIG_512_60 a[],int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        BIG_512_60_copy(a[i],a[i+n]);
        BIG_512_60_zero(a[i+n]);
    }
}

/* shift left by BIGBITS-bit words */
static void FF_4096_shlw(BIG_512_60 a[],int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        BIG_512_60_copy(a[i+n],a[i]);
        BIG_512_60_zero(a[i]);
    }
}

/* extract last bit */
int FF_4096_parity(BIG_512_60 x[])
{
    return BIG_512_60_parity(x[0]);
}

/* extract last m bits */
int FF_4096_lastbits(BIG_512_60 x[],int m)
{
    return BIG_512_60_lastbits(x[0],m);
}

/* x=1 */
void FF_4096_one(BIG_512_60 x[],int n)
{
    int i;
    BIG_512_60_one(x[0]);
    for (i=1; i<n; i++)
        BIG_512_60_zero(x[i]);
}

/* x=m, where m is 32-bit int */
void FF_4096_init(BIG_512_60 x[],sign32 m,int n)
{
    int i;
    BIG_512_60_zero(x[0]);
#if CHUNK<64
    x[0][0]=(chunk)(m&BMASK_512_60);
    x[0][1]=(chunk)(m>>BASEBITS_512_60);
#else
    x[0][0]=(chunk)m;
#endif
    for (i=1; i<n; i++)
        BIG_512_60_zero(x[i]);
}

/* compare x and y - must be normalised */
int FF_4096_comp(BIG_512_60 x[],BIG_512_60 y[],int n)
{
    int i,j;
    for (i=n-1; i>=0; i--)
    {
        j=BIG_512_60_comp(x[i],y[i]);
        if (j!=0) return j;
    }
    return 0;
}

/* recursive add */
static void FF_4096_radd(BIG_512_60 z[],int zp,BIG_512_60 x[],int xp,BIG_512_60 y[],int yp,int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_add(z[zp+i],x[xp+i],y[yp+i]);
}

/* recursive inc */
static void FF_4096_rinc(BIG_512_60 z[],int zp,BIG_512_60 y[],int yp,int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_add(z[zp+i],z[zp+i],y[yp+i]);
}

/* recursive dec */
static void FF_4096_rdec(BIG_512_60 z[],int zp,BIG_512_60 y[],int yp,int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_sub(z[zp+i],z[zp+i],y[yp+i]);
}

/* simple add */
void FF_4096_add(BIG_512_60 z[],BIG_512_60 x[],BIG_512_60 y[],int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_add(z[i],x[i],y[i]);
}

/* simple sub */
void FF_4096_sub(BIG_512_60 z[],BIG_512_60 x[],BIG_512_60 y[],int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_sub(z[i],x[i],y[i]);
}

/* increment/decrement by a small integer */
void FF_4096_inc(BIG_512_60 x[],int m,int n)
{
    BIG_512_60_inc(x[0],m);
    FF_4096_norm(x,n);
}

void FF_4096_dec(BIG_512_60 x[],int m,int n)
{
    BIG_512_60_dec(x[0],m);
    FF_4096_norm(x,n);
}

/* normalise - but hold any overflow in top part unless n<0 */
static void FF_4096_rnorm(BIG_512_60 z[],int zp,int n)
{
    int i,trunc=0;
    chunk carry;
    if (n<0)
    {
        /* -v n signals to do truncation */
        n=-n;
        trunc=1;
    }
    for (i=0; i<n-1; i++)
    {
        carry=BIG_512_60_norm(z[zp+i]);

        z[zp+i][NLEN_512_60-1]^=carry<<P_TBITS_4096; /* remove it */
        z[zp+i+1][0]+=carry;
    }
    carry=BIG_512_60_norm(z[zp+n-1]);
    if (trunc) z[zp+n-1][NLEN_512_60-1]^=carry<<P_TBITS_4096;
}

void FF_4096_norm(BIG_512_60 z[],int n)
{
    FF_4096_rnorm(z,0,n);
}

/* shift left by one bit */
void FF_4096_shl(BIG_512_60 x[],int n)
{
    int i;
    int carry,delay_carry=0;
    for (i=0; i<n-1; i++)
    {
        carry=BIG_512_60_fshl(x[i],1);
        x[i][0]|=delay_carry;
        x[i][NLEN_512_60-1]^=(chunk)carry<<P_TBITS_4096;
        delay_carry=carry;
    }
    BIG_512_60_fshl(x[n-1],1);
    x[n-1][0]|=delay_carry;
}

/* shift right by one bit */
void FF_4096_shr(BIG_512_60 x[],int n)
{
    int i;
    int carry;
    for (i=n-1; i>0; i--)
    {
        carry=BIG_512_60_fshr(x[i],1);
        x[i-1][NLEN_512_60-1]|=(chunk)carry<<P_TBITS_4096;
    }
    BIG_512_60_fshr(x[0],1);
}

void FF_4096_output(BIG_512_60 x[],int n)
{
    int i;
    FF_4096_norm(x,n);
    for (i=n-1; i>=0; i--)
    {
        BIG_512_60_output(x[i]);
        printf(" ");
    }
}

void FF_4096_rawoutput(BIG_512_60 x[],int n)
{
    int i;
    for (i=n-1; i>=0; i--)
    {
        BIG_512_60_rawoutput(x[i]);
        printf(" ");
    }
}

/* Convert FFs to/from octet strings */
void FF_4096_toOctet(octet *w,BIG_512_60 x[],int n)
{
    int i;
    w->len=n*MODBYTES_512_60;
    for (i=0; i<n; i++)
    {
        BIG_512_60_toBytes(&(w->val[(n-i-1)*MODBYTES_512_60]),x[i]);
    }
}

void FF_4096_fromOctet(BIG_512_60 x[],octet *w,int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        BIG_512_60_fromBytes(x[i],&(w->val[(n-i-1)*MODBYTES_512_60]));
    }
}

/* in-place swapping using xor - side channel resistant */
static void FF_4096_cswap(BIG_512_60 a[],BIG_512_60 b[],int d,int n)
{
    int i;
    for (i=0; i<n; i++)
        BIG_512_60_cswap(a[i],b[i],d);
    return;
}

/* z=x*y, t is workspace */
static void FF_4096_karmul(BIG_512_60 z[],int zp,BIG_512_60 x[],int xp,BIG_512_60 y[],int yp,BIG_512_60 t[],int tp,int n)
{
    int nd2;
    if (n==1)
    {
        BIG_512_60_norm(x[xp]);
        BIG_512_60_norm(y[yp]);
        BIG_512_60_mul(t[tp],x[xp],y[yp]);
        BIG_512_60_split(z[zp+1],z[zp],t[tp],BIGBITS_512_60);
        return;
    }

    nd2=n/2;
    FF_4096_radd(z,zp,x,xp,x,xp+nd2,nd2);
    FF_4096_rnorm(z,zp,nd2);  /* needs this if recursion level too deep */

    FF_4096_radd(z,zp+nd2,y,yp,y,yp+nd2,nd2);
    FF_4096_rnorm(z,zp+nd2,nd2);
    FF_4096_karmul(t,tp,z,zp,z,zp+nd2,t,tp+n,nd2);
    FF_4096_karmul(z,zp,x,xp,y,yp,t,tp+n,nd2);
    FF_4096_karmul(z,zp+n,x,xp+nd2,y,yp+nd2,t,tp+n,nd2);
    FF_4096_rdec(t,tp,z,zp,n);
    FF_4096_rdec(t,tp,z,zp+n,n);
    FF_4096_rinc(z,zp+nd2,t,tp,n);
    FF_4096_rnorm(z,zp,2*n);
}

static void FF_4096_karsqr(BIG_512_60 z[],int zp,BIG_512_60 x[],int xp,BIG_512_60 t[],int tp,int n)
{
    int nd2;
    if (n==1)
    {
        BIG_512_60_norm(x[xp]);
        BIG_512_60_sqr(t[tp],x[xp]);
        BIG_512_60_split(z[zp+1],z[zp],t[tp],BIGBITS_512_60);
        return;
    }
    nd2=n/2;
    FF_4096_karsqr(z,zp,x,xp,t,tp+n,nd2);
    FF_4096_karsqr(z,zp+n,x,xp+nd2,t,tp+n,nd2);
    FF_4096_karmul(t,tp,x,xp,x,xp+nd2,t,tp+n,nd2);
    FF_4096_rinc(z,zp+nd2,t,tp,n);
    FF_4096_rinc(z,zp+nd2,t,tp,n);

    FF_4096_rnorm(z,zp+nd2,n);  /* was FF_rnorm(z,zp,2*n)  */
}

static void FF_4096_karmul_lower(BIG_512_60 z[],int zp,BIG_512_60 x[],int xp,BIG_512_60 y[],int yp,BIG_512_60 t[],int tp,int n)
{
    /* Calculates Least Significant bottom half of x*y */
    int nd2;
    if (n==1)
    {
        /* only calculate bottom half of product */
        BIG_512_60_norm(x[xp]);
        BIG_512_60_norm(y[yp]);
        BIG_512_60_smul(z[zp],x[xp],y[yp]);
        return;
    }
    nd2=n/2;
    FF_4096_karmul(z,zp,x,xp,y,yp,t,tp+n,nd2);
    FF_4096_karmul_lower(t,tp,x,xp+nd2,y,yp,t,tp+n,nd2);
    FF_4096_rinc(z,zp+nd2,t,tp,nd2);
    FF_4096_karmul_lower(t,tp,x,xp,y,yp+nd2,t,tp+n,nd2);
    FF_4096_rinc(z,zp+nd2,t,tp,nd2);
    FF_4096_rnorm(z,zp+nd2,-nd2);  /* truncate it */
}

static void FF_4096_karmul_upper(BIG_512_60 z[],BIG_512_60 x[],BIG_512_60 y[],BIG_512_60 t[],int n)
{
    /* Calculates Most Significant upper half of x*y, given lower part */
    int nd2;

    nd2=n/2;
    FF_4096_radd(z,n,x,0,x,nd2,nd2);
    FF_4096_radd(z,n+nd2,y,0,y,nd2,nd2);
    FF_4096_rnorm(z,n,nd2);
    FF_4096_rnorm(z,n+nd2,nd2);

    FF_4096_karmul(t,0,z,n+nd2,z,n,t,n,nd2);  /* t = (a0+a1)(b0+b1) */
    FF_4096_karmul(z,n,x,nd2,y,nd2,t,n,nd2); /* z[n]= a1*b1 */
    /* z[0-nd2]=l(a0b0) z[nd2-n]= h(a0b0)+l(t)-l(a0b0)-l(a1b1) */
    FF_4096_rdec(t,0,z,n,n);              /* t=t-a1b1  */
    FF_4096_rinc(z,nd2,z,0,nd2);   /* z[nd2-n]+=l(a0b0) = h(a0b0)+l(t)-l(a1b1)  */
    FF_4096_rdec(z,nd2,t,0,nd2);   /* z[nd2-n]=h(a0b0)+l(t)-l(a1b1)-l(t-a1b1)=h(a0b0) */
    FF_4096_rnorm(z,0,-n);					/* a0b0 now in z - truncate it */
    FF_4096_rdec(t,0,z,0,n);         /* (a0+a1)(b0+b1) - a0b0 */
    FF_4096_rinc(z,nd2,t,0,n);

    FF_4096_rnorm(z,nd2,n);
}

/* z=x*y */
void FF_4096_mul(BIG_512_60 z[],BIG_512_60 x[],BIG_512_60 y[],int n)
{
#ifndef C99
    BIG_512_60 t[2*FFLEN_4096];
#else
    BIG_512_60 t[2*n];
#endif
    FF_4096_karmul(z,0,x,0,y,0,t,0,n);
}

/* return low part of product */
static void FF_4096_lmul(BIG_512_60 z[],BIG_512_60 x[],BIG_512_60 y[],int n)
{
#ifndef C99
    BIG_512_60 t[2*FFLEN_4096];
#else
    BIG_512_60 t[2*n];
#endif
    FF_4096_karmul_lower(z,0,x,0,y,0,t,0,n);
}

/* Set b=b mod c */
void FF_4096_mod(BIG_512_60 b[],BIG_512_60 c[],int n)
{
    int k=0;

    FF_4096_norm(b,n);
    if (FF_4096_comp(b,c,n)<0)
        return;
    do
    {
        FF_4096_shl(c,n);
        k++;
    }
    while (FF_4096_comp(b,c,n)>=0);

    while (k>0)
    {
        FF_4096_shr(c,n);
        if (FF_4096_comp(b,c,n)>=0)
        {
            FF_4096_sub(b,b,c,n);
            FF_4096_norm(b,n);
        }
        k--;
    }
}

/* z=x^2 */
void FF_4096_sqr(BIG_512_60 z[],BIG_512_60 x[],int n)
{
#ifndef C99
    BIG_512_60 t[2*FFLEN_4096];
#else
    BIG_512_60 t[2*n];
#endif
    FF_4096_karsqr(z,0,x,0,t,0,n);
}

/* r=t mod modulus, N is modulus, ND is Montgomery Constant */
static void FF_4096_reduce(BIG_512_60 r[],BIG_512_60 T[],BIG_512_60 N[],BIG_512_60 ND[],int n)
{
    /* fast karatsuba Montgomery reduction */
#ifndef C99
    BIG_512_60 t[2*FFLEN_4096];
    BIG_512_60 m[FFLEN_4096];
#else
    BIG_512_60 t[2*n];
    BIG_512_60 m[n];
#endif
    FF_4096_sducopy(r,T,n);  /* keep top half of T */
    FF_4096_karmul_lower(m,0,T,0,ND,0,t,0,n);  /* m=T.(1/N) mod R */

    FF_4096_karmul_upper(T,N,m,t,n);  /* T=mN */
    FF_4096_sducopy(m,T,n);

    FF_4096_add(r,r,N,n);
    FF_4096_sub(r,r,m,n);
    FF_4096_norm(r,n);
}


/* Set r=a mod b */
/* a is of length - 2*n */
/* r,b is of length - n */
void FF_4096_dmod(BIG_512_60 r[],BIG_512_60 a[],BIG_512_60 b[],int n)
{
    int k;
#ifndef C99
    BIG_512_60 m[2*FFLEN_4096];
    BIG_512_60 x[2*FFLEN_4096];
#else
    BIG_512_60 m[2*n];
    BIG_512_60 x[2*n];
#endif
    FF_4096_copy(x,a,2*n);
    FF_4096_norm(x,2*n);
    FF_4096_dsucopy(m,b,n);
    k=BIGBITS_512_60*n;

    while (FF_4096_comp(x,m,2*n)>=0)
    {
        FF_4096_sub(x,x,m,2*n);
        FF_4096_norm(x,2*n);
    }

    while (k>0)
    {
        FF_4096_shr(m,2*n);

        if (FF_4096_comp(x,m,2*n)>=0)
        {
            FF_4096_sub(x,x,m,2*n);
            FF_4096_norm(x,2*n);
        }

        k--;
    }
    FF_4096_copy(r,x,n);
    FF_4096_mod(r,b,n);
}

/* Set r=1/a mod p. Binary method - a<p on entry */

void FF_4096_invmodp(BIG_512_60 r[],BIG_512_60 a[],BIG_512_60 p[],int n)
{
#ifndef C99
    BIG_512_60 u[FFLEN_4096],v[FFLEN_4096],x1[FFLEN_4096],x2[FFLEN_4096],t[FFLEN_4096],one[FFLEN_4096];
#else
    BIG_512_60 u[n],v[n],x1[n],x2[n],t[n],one[n];
#endif
    FF_4096_copy(u,a,n);
    FF_4096_copy(v,p,n);
    FF_4096_one(one,n);
    FF_4096_copy(x1,one,n);
    FF_4096_zero(x2,n);

    // reduce n in here as well!
    while (FF_4096_comp(u,one,n)!=0 && FF_4096_comp(v,one,n)!=0)
    {
        while (FF_4096_parity(u)==0)
        {
            FF_4096_shr(u,n);
            if (FF_4096_parity(x1)!=0)
            {
                FF_4096_add(x1,p,x1,n);
                FF_4096_norm(x1,n);
            }
            FF_4096_shr(x1,n);
        }
        while (FF_4096_parity(v)==0)
        {
            FF_4096_shr(v,n);
            if (FF_4096_parity(x2)!=0)
            {
                FF_4096_add(x2,p,x2,n);
                FF_4096_norm(x2,n);
            }
            FF_4096_shr(x2,n);
        }
        if (FF_4096_comp(u,v,n)>=0)
        {

            FF_4096_sub(u,u,v,n);
            FF_4096_norm(u,n);
            if (FF_4096_comp(x1,x2,n)>=0) FF_4096_sub(x1,x1,x2,n);
            else
            {
                FF_4096_sub(t,p,x2,n);
                FF_4096_add(x1,x1,t,n);
            }
            FF_4096_norm(x1,n);
        }
        else
        {
            FF_4096_sub(v,v,u,n);
            FF_4096_norm(v,n);
            if (FF_4096_comp(x2,x1,n)>=0) FF_4096_sub(x2,x2,x1,n);
            else
            {
                FF_4096_sub(t,p,x1,n);
                FF_4096_add(x2,x2,t,n);
            }
            FF_4096_norm(x2,n);
        }
    }
    if (FF_4096_comp(u,one,n)==0)
        FF_4096_copy(r,x1,n);
    else
        FF_4096_copy(r,x2,n);
}

/* nesidue mod m */
static void FF_4096_nres(BIG_512_60 a[],BIG_512_60 m[],int n)
{
#ifndef C99
    BIG_512_60 d[2*FFLEN_4096];
#else
    BIG_512_60 d[2*n];
#endif
    if (n==1)
    {
        BIG_512_60_dscopy(d[0],a[0]);
        BIG_512_60_dshl(d[0],NLEN_512_60*BASEBITS_512_60);
        BIG_512_60_dmod(a[0],d[0],m[0]);
    }
    else
    {
        FF_4096_dsucopy(d,a,n);
        FF_4096_dmod(a,d,m,n);
    }
}

static void FF_4096_redc(BIG_512_60 a[],BIG_512_60 m[],BIG_512_60 ND[],int n)
{
#ifndef C99
    BIG_512_60 d[2*FFLEN_4096];
#else
    BIG_512_60 d[2*n];
#endif
    if (n==1)
    {
        BIG_512_60_dzero(d[0]);
        BIG_512_60_dscopy(d[0],a[0]);
        BIG_512_60_monty(a[0],m[0],((chunk)1<<BASEBITS_512_60)-ND[0][0],d[0]);
    }
    else
    {
        FF_4096_mod(a,m,n);
        FF_4096_dscopy(d,a,n);
        FF_4096_reduce(a,d,m,ND,n);
        FF_4096_mod(a,m,n);
    }
}

/* U=1/a mod 2^m - Arazi & Qi */
void FF_4096_invmod2m(BIG_512_60 U[],BIG_512_60 a[],int n)
{
    int i;
#ifndef C99
    BIG_512_60 t1[FFLEN_4096],b[FFLEN_4096],c[FFLEN_4096];
#else
    BIG_512_60 t1[2*n],b[n],c[n];
#endif

    FF_4096_zero(U,n);
    FF_4096_zero(b,n);
    FF_4096_zero(c,n);
    FF_4096_zero(t1,2*n);

    BIG_512_60_copy(U[0],a[0]);
    BIG_512_60_invmod2m(U[0]);
    for (i=1; i<n; i<<=1)
    {
        FF_4096_copy(b,a,i);
        FF_4096_mul(t1,U,b,i);
        FF_4096_shrw(t1,i); // top half to bottom half, top half=0

        FF_4096_copy(c,a,2*i);
        FF_4096_shrw(c,i); // top half of c
        FF_4096_lmul(b,U,c,i); // should set top half of b=0
        FF_4096_add(t1,t1,b,i);
        FF_4096_norm(t1,2*i);
        FF_4096_lmul(b,t1,U,i);
        FF_4096_copy(t1,b,i);
        FF_4096_one(b,i);
        FF_4096_shlw(b,i);
        FF_4096_sub(t1,b,t1,2*i);
        FF_4096_norm(t1,2*i);
        FF_4096_shlw(t1,i);
        FF_4096_add(U,U,t1,2*i);
    }

    FF_4096_norm(U,n);
}

void FF_4096_random(BIG_512_60 x[],csprng *rng,int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        BIG_512_60_random(x[i],rng);
    }
    /* make sure top bit is 1 */
    while (BIG_512_60_nbits(x[n-1])<MODBYTES_512_60*8) BIG_512_60_random(x[n-1],rng);
}

/* generate random x mod p */
void FF_4096_randomnum(BIG_512_60 x[],BIG_512_60 p[],csprng *rng,int n)
{
    int i;
#ifndef C99
    BIG_512_60 d[2*FFLEN_4096];
#else
    BIG_512_60 d[2*n];
#endif
    for (i=0; i<2*n; i++)
    {
        BIG_512_60_random(d[i],rng);
    }
    FF_4096_dmod(x,d,p,n);
}

static void FF_4096_modmul(BIG_512_60 z[],BIG_512_60 x[],BIG_512_60 y[],BIG_512_60 p[],BIG_512_60 ND[],int n)
{
#ifndef C99
    BIG_512_60 d[2*FFLEN_4096];
#else
    BIG_512_60 d[2*n];
#endif
    chunk ex=P_EXCESS_4096(x[n-1]);
    chunk ey=P_EXCESS_4096(y[n-1]);
#ifdef dchunk
    if ((dchunk)(ex+1)*(ey+1)>(dchunk)P_FEXCESS_4096)
#else
    if ((ex+1)>P_FEXCESS_4096/(ey+1))
#endif
    {
#ifdef DEBUG_REDUCE
        printf("Product too large - reducing it %d %d\n",ex,ey);
#endif
        FF_4096_mod(x,p,n);
    }

    if (n==1)
    {
        BIG_512_60_mul(d[0],x[0],y[0]);
        BIG_512_60_monty(z[0],p[0],((chunk)1<<BASEBITS_512_60)-ND[0][0],d[0]);
    }
    else
    {
        FF_4096_mul(d,x,y,n);
        FF_4096_reduce(z,d,p,ND,n);
    }
}

static void FF_4096_modsqr(BIG_512_60 z[],BIG_512_60 x[],BIG_512_60 p[],BIG_512_60 ND[],int n)
{
#ifndef C99
    BIG_512_60 d[2*FFLEN_4096];
#else
    BIG_512_60 d[2*n];
#endif
    chunk ex=P_EXCESS_4096(x[n-1]);
#ifdef dchunk
    if ((dchunk)(ex+1)*(ex+1)>(dchunk)P_FEXCESS_4096)
#else
    if ((ex+1)>P_FEXCESS_4096/(ex+1))
#endif
    {
#ifdef DEBUG_REDUCE
        printf("Product too large - reducing it %d\n",ex);
#endif
        FF_4096_mod(x,p,n);
    }
    if (n==1)
    {
        BIG_512_60_sqr(d[0],x[0]);
        BIG_512_60_monty(z[0],p[0],((chunk)1<<BASEBITS_512_60)-ND[0][0],d[0]);
    }
    else
    {
        FF_4096_sqr(d,x,n);
        FF_4096_reduce(z,d,p,ND,n);
    }
}

/* r=x^e mod p using side-channel resistant Montgomery Ladder, for large e */
void FF_4096_skpow(BIG_512_60 r[],BIG_512_60 x[],BIG_512_60 e[],BIG_512_60 p[],int n, int en)
{
    int i,b;
#ifndef C99
    BIG_512_60 R0[FFLEN_4096],R1[FFLEN_4096],ND[FFLEN_4096];
#else
    BIG_512_60 R0[n],R1[n],ND[n];
#endif
    FF_4096_invmod2m(ND,p,n);

    FF_4096_one(R0,n);
    FF_4096_copy(R1,x,n);
    FF_4096_nres(R0,p,n);
    FF_4096_nres(R1,p,n);

    for (i=8*MODBYTES_512_60*en-1; i>=0; i--)
    {
        b=BIG_512_60_bit(e[i/BIGBITS_512_60],i%BIGBITS_512_60);
        FF_4096_modmul(r,R0,R1,p,ND,n);

        FF_4096_cswap(R0,R1,b,n);
        FF_4096_modsqr(R0,R0,p,ND,n);

        FF_4096_copy(R1,r,n);
        FF_4096_cswap(R0,R1,b,n);
    }
    FF_4096_copy(r,R0,n);
    FF_4096_redc(r,p,ND,n);
}

/* r=x^e mod p using side-channel resistant Montgomery Ladder, for short e */
void FF_4096_skspow(BIG_512_60 r[],BIG_512_60 x[],BIG_512_60 e,BIG_512_60 p[],int n)
{
    int i,b;
#ifndef C99
    BIG_512_60 R0[FFLEN_4096],R1[FFLEN_4096],ND[FFLEN_4096];
#else
    BIG_512_60 R0[n],R1[n],ND[n];
#endif
    FF_4096_invmod2m(ND,p,n);
    FF_4096_one(R0,n);
    FF_4096_copy(R1,x,n);
    FF_4096_nres(R0,p,n);
    FF_4096_nres(R1,p,n);
    for (i=8*MODBYTES_512_60-1; i>=0; i--)
    {
        b=BIG_512_60_bit(e,i);
        FF_4096_modmul(r,R0,R1,p,ND,n);
        FF_4096_cswap(R0,R1,b,n);
        FF_4096_modsqr(R0,R0,p,ND,n);
        FF_4096_copy(R1,r,n);
        FF_4096_cswap(R0,R1,b,n);
    }
    FF_4096_copy(r,R0,n);
    FF_4096_redc(r,p,ND,n);
}

/* r=x^e*y^f mod p - side channel resistant */
void FF_4096_skpow2(BIG_512_60 r[],BIG_512_60 x[], BIG_512_60 e[], BIG_512_60 y[], BIG_512_60 f[], BIG_512_60 p[], int n, int en)
{
    int i,eb,fb;
#ifndef C99
    BIG_512_60 xn[FFLEN_4096],yn[FFLEN_4096],xy[FFLEN_4096],w[FFLEN_4096],ND[FFLEN_4096];
#else
    BIG_512_60 xn[n],yn[n],xy[n],w[n],ND[n];
#endif

    FF_4096_invmod2m(ND,p,n);

    FF_4096_copy(xn,x,n);
    FF_4096_copy(yn,y,n);
    FF_4096_nres(xn,p,n);
    FF_4096_nres(yn,p,n);
    FF_4096_modmul(xy,xn,yn,p,ND,n);
    FF_4096_one(w,n);
    FF_4096_nres(w,p,n);
    FF_4096_one(r,n);
    FF_4096_nres(r,p,n);

    for (i=8*MODBYTES_512_60*en-1; i>=0; i--)
    {
        eb=BIG_512_60_bit(e[i/BIGBITS_512_60],i%BIGBITS_512_60);
        fb=BIG_512_60_bit(f[i/BIGBITS_512_60],i%BIGBITS_512_60);

        FF_4096_cswap(w, xn, eb & (eb ^ fb), n);
        FF_4096_cswap(w, yn, fb & (eb ^ fb), n);
        FF_4096_cswap(w, xy, eb & fb, n);

        FF_4096_modsqr(r,r,p,ND,n);
        FF_4096_modmul(r,w,r,p,ND,n);

        FF_4096_cswap(w, xn, eb & (eb ^ fb), n);
        FF_4096_cswap(w, yn, fb & (eb ^ fb), n);
        FF_4096_cswap(w, xy, eb & fb, n);
    }

    FF_4096_redc(r,p,ND,n);
}

/* r=x^e*y^f mod p - side channel resistant */
void FF_4096_skpow3(BIG_512_60 r[],BIG_512_60 x[], BIG_512_60 e[], BIG_512_60 y[], BIG_512_60 f[], BIG_512_60 z[], BIG_512_60 g[], BIG_512_60 p[], int n, int en)
{
    int i,b;
#ifndef C99
    BIG_512_60 xn[FFLEN_4096],yn[FFLEN_4096],zn[FFLEN_4096],xy[FFLEN_4096],xz[FFLEN_4096],yz[FFLEN_4096],xyz[FFLEN_4096],w[FFLEN_4096],ND[FFLEN_4096];
#else
    BIG_512_60 xn[n],yn[n],zn[n],xy[n],xz[n],yz[n],xyz[n],w[n],ND[n];
#endif

    FF_4096_invmod2m(ND, p, n);

    FF_4096_copy(xn, x, n);
    FF_4096_copy(yn, y, n);
    FF_4096_copy(zn, z, n);
    FF_4096_nres(xn, p, n);
    FF_4096_nres(yn, p, n);
    FF_4096_nres(zn, p, n);
    FF_4096_modmul(xy,  xn, yn, p, ND, n);
    FF_4096_modmul(xz,  xn, zn, p, ND, n);
    FF_4096_modmul(yz,  yn, zn, p, ND, n);
    FF_4096_modmul(xyz, xy, zn, p, ND, n);
    FF_4096_one(w, n);
    FF_4096_one(r, n);
    FF_4096_nres(w, p, n);
    FF_4096_nres(r, p, n);

    for (i=8*MODBYTES_512_60*en-1; i>=0; i--)
    {
        b = BIG_512_60_bit(g[i/BIGBITS_512_60],i%BIGBITS_512_60);
        b <<= 1;
        b = b | BIG_512_60_bit(f[i/BIGBITS_512_60],i%BIGBITS_512_60);
        b <<= 1;
        b = b | BIG_512_60_bit(e[i/BIGBITS_512_60],i%BIGBITS_512_60);

        FF_4096_cswap(w, xn,  b == 0x01, n);
        FF_4096_cswap(w, yn,  b == 0x02, n);
        FF_4096_cswap(w, zn,  b == 0x04, n);
        FF_4096_cswap(w, xy,  b == 0x03, n);
        FF_4096_cswap(w, xz,  b == 0x05, n);
        FF_4096_cswap(w, yz,  b == 0x06, n);
        FF_4096_cswap(w, xyz, b == 0x07, n);

        FF_4096_modsqr(r, r, p, ND, n);
        FF_4096_modmul(r, w, r, p, ND, n);

        FF_4096_cswap(w, xn,  b == 0x01, n);
        FF_4096_cswap(w, yn,  b == 0x02, n);
        FF_4096_cswap(w, zn,  b == 0x04, n);
        FF_4096_cswap(w, xy,  b == 0x03, n);
        FF_4096_cswap(w, xz,  b == 0x05, n);
        FF_4096_cswap(w, yz,  b == 0x06, n);
        FF_4096_cswap(w, xyz, b == 0x07, n);
    }

    FF_4096_redc(r, p, ND, n);
}

/* raise to an integer power - right-to-left method */
void FF_4096_power(BIG_512_60 r[],BIG_512_60 x[],int e,BIG_512_60 p[],int n)
{
    int f=1;
#ifndef C99
    BIG_512_60 w[FFLEN_4096],ND[FFLEN_4096];
#else
    BIG_512_60 w[n],ND[n];
#endif
    FF_4096_invmod2m(ND,p,n);

    FF_4096_copy(w,x,n);
    FF_4096_nres(w,p,n);

    if (e==2)
    {
        FF_4096_modsqr(r,w,p,ND,n);
    }
    else for (;;)
        {
            if (e%2==1)
            {
                if (f) FF_4096_copy(r,w,n);
                else FF_4096_modmul(r,r,w,p,ND,n);
                f=0;
            }
            e>>=1;
            if (e==0) break;
            FF_4096_modsqr(w,w,p,ND,n);
        }

    FF_4096_redc(r,p,ND,n);
}

/* r=x^e mod p, faster but not side channel resistant */
void FF_4096_pow(BIG_512_60 r[],BIG_512_60 x[],BIG_512_60 e[],BIG_512_60 p[],int n)
{
    int i,b;
#ifndef C99
    BIG_512_60 w[FFLEN_4096],ND[FFLEN_4096];
#else
    BIG_512_60 w[n],ND[n];
#endif
    FF_4096_invmod2m(ND,p,n);

    FF_4096_copy(w,x,n);
    FF_4096_one(r,n);
    FF_4096_nres(r,p,n);
    FF_4096_nres(w,p,n);

    for (i=8*MODBYTES_512_60*n-1; i>=0; i--)
    {
        FF_4096_modsqr(r,r,p,ND,n);
        b=BIG_512_60_bit(e[i/BIGBITS_512_60],i%BIGBITS_512_60);
        if (b==1) FF_4096_modmul(r,r,w,p,ND,n);
    }
    FF_4096_redc(r,p,ND,n);
}

/* double exponentiation r=x^e.y^f mod p */
void FF_4096_pow2(BIG_512_60 r[],BIG_512_60 x[],BIG_512_60 e,BIG_512_60 y[],BIG_512_60 f,BIG_512_60 p[],int n)
{
    int i,eb,fb;
#ifndef C99
    BIG_512_60 xn[FFLEN_4096],yn[FFLEN_4096],xy[FFLEN_4096],ND[FFLEN_4096];
#else
    BIG_512_60 xn[n],yn[n],xy[n],ND[n];
#endif

    FF_4096_invmod2m(ND,p,n);

    FF_4096_copy(xn,x,n);
    FF_4096_copy(yn,y,n);
    FF_4096_nres(xn,p,n);
    FF_4096_nres(yn,p,n);
    FF_4096_modmul(xy,xn,yn,p,ND,n);
    FF_4096_one(r,n);
    FF_4096_nres(r,p,n);

    for (i=8*MODBYTES_512_60-1; i>=0; i--)
    {
        eb=BIG_512_60_bit(e,i);
        fb=BIG_512_60_bit(f,i);
        FF_4096_modsqr(r,r,p,ND,n);
        if (eb==1)
        {
            if (fb==1) FF_4096_modmul(r,r,xy,p,ND,n);
            else FF_4096_modmul(r,r,xn,p,ND,n);
        }
        else
        {
            if (fb==1) FF_4096_modmul(r,r,yn,p,ND,n);
        }
    }
    FF_4096_redc(r,p,ND,n);
}

/* Compute prod(x_i^e_i) mod p. Faster but not constant time */
void FF_4096_pown(BIG_512_60 *r, BIG_512_60 *x[], BIG_512_60 *e[], BIG_512_60 *p, int xlen, int elen, int n)
{
#ifndef C99
    BIG_512_60 products[15][FFLEN_4096], ND[FFLEN_4096];
#else
    BIG_512_60 products[(1<<n)-1][xlen], ND[xlen];
#endif

    int i, j, b;

    FF_4096_invmod2m(ND, p, xlen);

    // Precompute the possible products between the bases
    for (i = 1; i < (1<<n); i++)
    {
        // Select less significant non zero bit
        for (j = 0; (i & (1<<j)) == 0; j++);
        b = 1 << j;

        if (i == b)
        {
            // case 0..010..0. It identifies one of the bases
            FF_4096_copy(products[i-1], x[j], xlen);
            FF_4096_nres(products[i-1], p, xlen);
        }
        else
        {
            // Split into b and i-b. Both are already computed since 0 < b < i
            FF_4096_modmul(products[i-1], products[i-b-1], products[b-1], p, ND, xlen);
        }
    }

    FF_4096_one(r, xlen);
    FF_4096_nres(r, p, xlen);

    // Compute final result using a simple square and multiply
    for (i = 8 * MODBYTES_512_60 * elen - 1; i >= 0; i--)
    {
        FF_4096_modsqr(r,r,p,ND,xlen);

        // Build the index for the product using the exponent bit
        // for each of the bases, from last to first
        b = 0;
        for (j = n; j > 0; j--)
        {
            b<<=1;
            b+=BIG_512_60_bit(e[j-1][i/BIGBITS_512_60], i%BIGBITS_512_60);
        }

        if (b != 0)
        {
            FF_4096_modmul(r, r, products[b-1], p, ND, xlen);
        }
    }

    FF_4096_redc(r, p, ND, xlen);
}

void FF_4096_bpow2(BIG_512_60 *r,BIG_512_60 *x,BIG_512_60 *e, BIG_512_60 *y, BIG_512_60 *f, BIG_512_60 *p,int n, int en)
{
    BIG_512_60 *x_s[] = {x,y};
    BIG_512_60 *e_s[] = {e,f};

    FF_4096_pown(r, x_s, e_s, p, n, en, 2);
}

void FF_4096_pow3(BIG_512_60 *r,BIG_512_60 *x,BIG_512_60 *e, BIG_512_60 *y, BIG_512_60 *f, BIG_512_60 *z, BIG_512_60 *g, BIG_512_60 *p, int n, int en)
{
    BIG_512_60 *x_s[] = {x,y,z};
    BIG_512_60 *e_s[] = {e,f,g};

    FF_4096_pown(r, x_s, e_s, p, n, en, 3);
}

void FF_4096_pow4(BIG_512_60 *r,BIG_512_60 *x,BIG_512_60 *e, BIG_512_60 *y, BIG_512_60 *f, BIG_512_60 *z, BIG_512_60 *g, BIG_512_60 *w, BIG_512_60 *h, BIG_512_60 *p, int n, int en)
{
    BIG_512_60 *x_s[] = {x,y,z,w};
    BIG_512_60 *e_s[] = {e,f,g,h};

    FF_4096_pown(r, x_s, e_s, p, n, en, 4);
}

static sign32 igcd(sign32 x,sign32 y)
{
    /* integer GCD, returns GCD of x and y */
    sign32 r;
    if (y==0) return x;
    while ((r=x%y)!=0)
        x=y,y=r;
    return y;
}

/* quick and dirty check for common factor with s */
int FF_4096_cfactor(BIG_512_60 w[],sign32 s,int n)
{
    int r;
    sign32 g;
#ifndef C99
    BIG_512_60 x[FFLEN_4096],y[FFLEN_4096];
#else
    BIG_512_60 x[n],y[n];
#endif
    FF_4096_init(y,s,n);
    FF_4096_copy(x,w,n);
    FF_4096_norm(x,n);

    do
    {
        FF_4096_sub(x,x,y,n);
        FF_4096_norm(x,n);
        while (!FF_4096_iszilch(x,n) && FF_4096_parity(x)==0) FF_4096_shr(x,n);
    }
    while (FF_4096_comp(x,y,n)>0);
#if CHUNK<32
    g=x[0][0]+((sign32)(x[0][1])<<BASEBITS_512_60);
#else
    g=(sign32)x[0][0];
#endif
    r=igcd(s,g);
    if (r>1) return 1;
    return 0;
}

/* Miller-Rabin test for primality. Slow. */
int FF_4096_prime(BIG_512_60 p[],csprng *rng,int n)
{
    int i,j,loop,s=0;
#ifndef C99
    BIG_512_60 d[FFLEN_4096],x[FFLEN_4096],unity[FFLEN_4096],nm1[FFLEN_4096];
#else
    BIG_512_60 d[n],x[n],unity[n],nm1[n];
#endif
    sign32 sf=4849845;/* 3*5*.. *19 */

    FF_4096_norm(p,n);

    if (FF_4096_cfactor(p,sf,n)) return 0;

    FF_4096_one(unity,n);
    FF_4096_sub(nm1,p,unity,n);
    FF_4096_norm(nm1,n);
    FF_4096_copy(d,nm1,n);
    while (FF_4096_parity(d)==0)
    {
        FF_4096_shr(d,n);
        s++;
    }
    if (s==0) return 0;

    for (i=0; i<10; i++)
    {
        FF_4096_randomnum(x,p,rng,n);
        FF_4096_pow(x,x,d,p,n);
        if (FF_4096_comp(x,unity,n)==0 || FF_4096_comp(x,nm1,n)==0) continue;
        loop=0;
        for (j=1; j<s; j++)
        {
            FF_4096_power(x,x,2,p,n);
            if (FF_4096_comp(x,unity,n)==0) return 0;
            if (FF_4096_comp(x,nm1,n)==0 )
            {
                loop=1;
                break;
            }
        }
        if (loop) continue;
        return 0;
    }

    return 1;
}

/* Chinese Remainder Theorem to reconstruct results mod pq*/
void FF_4096_crt(BIG_512_60 *r, BIG_512_60 *rp, BIG_512_60 *rq, BIG_512_60 *p, BIG_512_60 *q, int n)
{
#ifndef C99
    BIG_512_60 pq[FFLEN_4096], invp[HFLEN_4096], c[FFLEN_4096], a[FFLEN_4096], b[2*FFLEN_4096];
#else
    BIG_512_60 pq[2*n], invp[n], c[2*n], a[2*n], b[4*n];
#endif

    FF_4096_mul(pq, p, q, n);

    // c = p * (p^-1 mod q)
    FF_4096_invmodp(invp, p, q, n);
    FF_4096_mul(c, p, invp, n);

    // a = (rq - rp) mod pq
    FF_4096_copy(a, pq, 2*n);
    FF_4096_sub(a, a, rp, n);
    FF_4096_add(a, a, rq, n);

    // (ac + rp) mod pq
    FF_4096_mul(b, a, c, 2*n);
    FF_4096_dmod(a, b, pq, 2*n);

    FF_4096_dscopy(r, rp, n);
    FF_4096_add(r, r, a, 2*n);
    FF_4096_mod(r, pq, 2*n);
}
