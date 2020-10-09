#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include<limits.h>
#ifndef BSWABE_DEBUG
#define NDEBUG
#endif
#include <assert.h>

#include <openssl/sha.h>
#include <glib.h>
#include <pbc.h>
#include <gmp.h>

#include "bswabe.h"
#include "private.h"

#define TYPE_A_PARAMS \
"type a\n" \
"q 87807107996633125224377819847540498158068831994142082" \
"1102865339926647563088022295707862517942266222142315585" \
"8769582317459277713367317481324925129998224791\n" \
"h 12016012264891146079388821366740534204802954401251311" \
"822919615131047207289359704531102844802183906537786776\n" \
"r 730750818665451621361119245571504901405976559617\n" \
"exp2 159\n" \
"exp1 107\n" \
"sign1 1\n" \
"sign0 1\n"

char last_error[256];

char*
bswabe_error()
{
	return last_error;
}

void
raise_error(char* fmt, ...)
{
	va_list args;

#ifdef BSWABE_DEBUG
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
#else
	va_start(args, fmt);
	vsnprintf(last_error, 256, fmt, args);
	va_end(args);
#endif
}

void
element_from_string( element_t h, char* s )
{
	unsigned char* r;

	r = malloc(SHA_DIGEST_LENGTH);
	SHA1((unsigned char*) s, strlen(s), r);
	element_from_hash(h, r, SHA_DIGEST_LENGTH);

	free(r);
}
int isPrime(int x){
    
    int i;
    for(i = 2;i < x ;i++){
        if(x % i == 0){
            return 0;
        }
    }
    return 1;
}

int powe(long long int x, long long int y, long long int p) 
{ 
    long long int res = 1;      // Initialize result 
  
    x = x % p;  // Update x if it is more than or  
                // equal to p 
  
    while (y > 0) 
    { 
        // If y is odd, multiply x with result 
        if (y & 1) 
            res = (res*x) % p; 
  
        // y must be even now 
        y = y>>1; // y = y/2 
        x = (x*x) % p;   
    } 
    return res; 
}

void concatenate(char ans[],char* s1, char* s2, char* s3){
    int i = 0;
	if(s1[0] != 'A'){
    while(s1[i] != '\0'){
        ans[i] = s1[i];
        i++;
    }
	}
    int j = i; 
    i = 0;
	if(s2[0] != 'A'){
    while(s2[i] != '\0'){
        ans[j] = s2[i];
        j++;
        i++;
    }
}

    i = 0;
	if(s3[0] != 'A'){
    while(s3[i] != '\0'){
        ans[j] = s3[i];
        j++;
        i++;
        }
    }
    return;
}

void take_Concatenate(char sol[], mpz_t m1, mpz_t m2, mpz_t m3){

    char s1[100];
    char s2[100];
    char s3[100];
    
	int flag = 0;
	int flag2 =0;
	int flag3 = 0;
     if(mpz_cmp_ui(m1, 0) == 0){
        s1[0] = "A";
	
    }else{
        mpz_get_str(s1, 10, m1);
	flag = strlen(s1);
    }

    if(mpz_cmp_ui(m2, 0) == 0){
        s2[0] = "A";

    }else{
        mpz_get_str(s2, 10, m2);
	flag2 = strlen(s2);
    }

    if(mpz_cmp_ui(m3, 0) == 0){
        s3[0] = "A";
    }else{
        mpz_get_str(s3, 10, m3);
	flag3 = strlen(s3);
    }


    int size = flag + flag2 + flag3;
    char ans[size];
    concatenate(ans, s1, s2, s3);
    int i;
    for( i = 0;i<size;i++){
        sol[i] = ans[i];
    }
}



long long int generate_random(mpz_t rand_Num,int limit)
{   
    
    long long int  seed= rand() % limit;
    mpz_init_set_ui(rand_Num, seed);

	return seed;
} 

void compute_hash(mpz_t hash,char str[]) 
{ 
 

   mpz_init(hash);
   mpz_t p;
   mpz_init_set_ui(p, 2);
   mpz_t mod;
   mpz_init_set_ui(mod,100);
   char c;
   
     for( c=0;c<strlen(str);c++)
   {    mpz_t m;
        mpz_init(m);
        mpz_t ans;
        mpz_init(ans);
       
        mpz_pow_ui (m, p, c); 
	mpz_mod(m,m,mod);
        mpz_t vall;
        int x = str[c] - '0';

        mpz_init_set_ui(vall, x);

        mpz_mul(ans, vall, m);
	mpz_mod(ans,ans,mod);
        mpz_add(hash,hash,ans);
	mpz_mod(hash,hash,mod);
   }
    return;

}

void compute_hash2(mpz_t hash,long long int x) 
{ 
 
	long long int num = x;
   mpz_init(hash);
   mpz_t p;
   mpz_init_set_ui(p, 2);
   mpz_t mod;
   mpz_init_set_ui(mod,100);
   int c = 0;
   
     while(num != 0)
   {    
	mpz_t m;
        mpz_init(m);
        mpz_t ans;
        mpz_init(ans);
       
        mpz_pow_ui (m, p, c); 
    	mpz_mod(m,m,mod);
        mpz_t vall;
        long long int x_ = num % 10;

        mpz_init_set_ui(vall, x_);

        mpz_mul(ans, vall, m);
	    mpz_mod(ans,ans,mod);
        mpz_add(hash,hash,ans);
	    mpz_mod(hash,hash,mod);
	c++;
	num = num / 10;
   }
 
   return;

}

int *delete_subarr(int arr_primes[],int arr_remove[], int n,int m)
{  
  int* ans_array;
  ans_array = malloc(sizeof(int) * (n - m));
   int i,j=0;
    int k = 0;
   for( i=0;i<n;i++)
   {
       int flag = 0;
       for(j=0;j<m;j++)
       {   
           if(arr_remove[j] ==arr_primes[i])
             {
                flag = 1;
              
             }      
       }
       
       if(flag == 0){
           ans_array[k]= arr_primes[i]; 
           k++;
       }
   }
  return ans_array; 
}



void
bswabe_setup( bswabe_pub_t** pub, bswabe_msk_t** msk ,int n) 
{
  n++;
  printf("n_1 is %d \n", n);
	*pub = malloc(sizeof(bswabe_pub_t));
	*msk = malloc(sizeof(bswabe_msk_t));
	mpz_init_set_ui((*pub) -> n, n);
	mpz_init_set_ui((*msk) -> n, n);
	mpz_init_set_ui((*msk) -> p ,961748941);
	mpz_init_set_ui((*msk) -> q,982451653);

	mpz_mul((*pub) -> N, (*msk) -> p, (*msk) -> q);
	mpz_t p_1;
	mpz_t q_1;
	mpz_init(p_1);
	mpz_init(q_1);
	mpz_sub_ui(p_1, (*msk) -> p, 1);
	mpz_sub_ui(q_1, (*msk) -> q, 1);
	mpz_t totient;
	mpz_init(totient);
	mpz_mul(totient, p_1, q_1);
  mpz_t pr;
  mpz_init_set_ui(pr,2);
  mpz_t inter;
  mpz_init(inter);
	int i,iter=0;
    for(i = 0; i < n;i++){ // n_1 is changed
        int flag=1;
        while(flag==1){
            mpz_nextprime(pr,pr);
            if(mpz_invert(inter, pr, totient)==0)
                continue;
            mpz_t gc;
            mpz_init(gc);
            mpz_gcd(gc, totient, pr);
            if(mpz_cmp_ui(gc,1)==0)
                flag=0;
        }
        mpz_set((*pub)->p_i[i], pr);
        mpz_set((*msk)->q_i[i], inter);
	}
  gmp_printf("q0 == %Zd and q1 = %Zd\n", (*pub) -> p_i[0], (*pub) -> p_i[1]);
  gmp_printf("q0 == %Zd and q1 = %Zd\n", (*msk) -> q_i[0], (*msk) -> q_i[1]);
	mpz_set_ui(pr,3);
	while(1){
        mpz_t gc;
        mpz_init(gc);
        mpz_gcd(gc, totient, pr);
        if(mpz_cmp_ui(gc,1)==0){
            for(int i=0;i<n;i++){ // n_1 is changed
                mpz_init(gc);
                mpz_gcd(gc, (*msk)->q_i[i], pr);
                if(mpz_cmp_ui(gc,1)==0)
                    continue;
                mpz_nextprime(pr, pr);
                break;
            }
            mpz_set((*msk)->k, pr);
            break;
        }
        else{
            mpz_nextprime(pr, pr);
        }
    }

    mpz_set_ui(pr,3);
	while(1){
        mpz_t gc;
        int flag=0;
        for(int i=0;i<n;i++){ //n_1 is changed
            mpz_init(gc);
            mpz_gcd(gc, (*msk)->q_i[i], pr);
            if(mpz_cmp_ui(gc,1)==0)
                continue;
            flag=1;
            mpz_nextprime(pr, pr);
            break;
        }
        if(flag==0){
            mpz_set((*msk)->x, pr);
            break;
        }
    }
   	mpz_t random_g;
	mpz_init_set_ui(random_g,3);
	
	mpz_t inter_R;
	mpz_t inter_Y;
	mpz_init(inter_R);
	mpz_init(inter_Y);
	mpz_powm(inter_R, random_g, (*msk) -> k, (*pub) -> N);
	mpz_powm(inter_Y, random_g, (*msk) -> x,(*pub) -> N);
	mpz_init_set((*pub) -> R, inter_R);
	mpz_init_set((*pub) -> Y, inter_Y);
        
	mpz_t mul;
	mpz_init_set_ui(mul, 1);
	int o;
	for( o = 0;o<n;o++){ // n_1 is changed
        mpz_mul(mul, mul, (*msk) -> q_i[o]);
	}

	mpz_init((*pub) -> D_u);
	mpz_powm((*pub) -> D_u, random_g, mul,(*pub) -> N); 
	
}

 
bswabe_cph_t*
bswabe_enc( bswabe_pub_t* pub, bswabe_msk_t* msk,element_t m, int attrib[]) 
{

//#######################################ODELU####################################################
        pairing_t px;
	char* pairing_desc;
	pairing_desc = strdup(TYPE_A_PARAMS);
	pairing_init_set_buf(px, pairing_desc, strlen(pairing_desc));
        element_init_GT(m,px); 
        element_random(m);
    mpz_t p_1;
	mpz_t q_1;
	mpz_init(p_1);
	mpz_init(q_1);
	mpz_sub_ui(p_1, msk -> p, 1);
	mpz_sub_ui(q_1, msk -> q, 1);
	mpz_t totient;
	mpz_init(totient);
	mpz_mul(totient, p_1, q_1);


	bswabe_cph_t* cph;

	cph = malloc(sizeof(bswabe_cph_t));

	mpz_t mod;
	mpz_init_set_ui(mod, 1000000000); 

	mpz_t blank1;
	mpz_init(blank1);

	mpz_t blank2;
	mpz_init(blank2);

	
	mpz_t r_m;
       generate_random(r_m,10); 

  gmp_printf("r_m is %Zd\n", r_m);
  //mpz_init_set_ui(r_m, 3);
	mpz_t e_u;
	mpz_init_set_ui(e_u, 1);

	mpz_t e_p;
	mpz_init_set_ui(e_p, 1);
	int n = mpz_get_ui(pub -> n); 
    int i;
	for(i = 0;i<n;i++){ // n+1 is done
		if(attrib[i] == 1){
			mpz_mul(e_p, e_p, pub -> p_i[i]);
		}
		mpz_mul(e_u, e_u, pub -> p_i[i]);
	}


	mpz_init_set(cph -> e_p,e_p);
	mpz_t inter;
	mpz_init_set(inter, pub -> D_u);

	mpz_cdiv_q(e_u, e_u, e_p);
	
	mpz_powm(inter,inter,r_m,pub -> N);
	mpz_powm(inter,inter,e_u,pub -> N);
	
	mpz_t H2_K_m;
	mpz_init(H2_K_m);


	mpz_t sigma;
	generate_random(sigma,30);
	
	char ans2[100];

	mpz_t blank3;
	mpz_t blank4;
	mpz_init(blank3);
	mpz_init(blank4);
	mpz_get_str(ans2, 10 ,inter);

	compute_hash(H2_K_m, ans2);


	mpz_init(cph -> C_sigma); 
	mpz_xor(cph -> C_sigma, H2_K_m, sigma);
	
	mpz_t H3_sigma;
	mpz_init(H3_sigma); 
	long long int ans3 = mpz_get_ui(sigma);


	compute_hash2(H3_sigma, ans3);

	mpz_t M;
	generate_random(M,30);
	mpz_init(cph -> C_m); 
	mpz_xor(cph -> C_m, M,H3_sigma); 

	/*
	char ans4[4];
	long long int M_v = mpz_get_ui(M);
	long long int sigma_v = mpz_get_ui(sigma);
	*/
	//long long int final_ans = sigma_v * 100 + M_v;
	mpz_t final_ans;
	mpz_init(final_ans);
        mpz_mul_ui(final_ans, sigma, 100);
	mpz_add(final_ans, final_ans, M);			

	compute_hash2(cph -> S_m, mpz_get_ui(final_ans));

	mpz_init(cph ->Y_m);
	mpz_init(cph -> R_m);
	mpz_powm(cph -> Y_m, pub -> Y, r_m, pub -> N);
	//long long int pub_R = mpz_get_ui(pub -> R); 
	//long long int ir_m = mpz_get_ui(r_m);
	//long long int pub_N = mpz_get_ui(pub -> N);
	mpz_t cph_Rm;
	mpz_init(cph_Rm);
	mpz_powm(cph_Rm, pub->R, r_m, pub->N);
	mpz_init_set(cph -> R_m, cph_Rm);
	return cph;
}

bswabe_prv_t* bswabe_keygen( bswabe_pub_t* pub, bswabe_msk_t* msk, int user_attr_set[])
{ 
                    // #################Odelu Implementation#####################
         mpz_t x;
	mpz_init_set_ui(x,10);
	mpz_t HYPER_MOD;
	mpz_init(HYPER_MOD);
	mpz_pow_ui(HYPER_MOD, x , 100000);

   bswabe_prv_t* prv;
   prv = malloc(sizeof(bswabe_prv_t)); 
  
   int n = mpz_get_ui(pub -> n);
   mpz_t p_1;
   mpz_t q_1;
   mpz_init(p_1);
   mpz_init(q_1);
   mpz_sub_ui(p_1, msk -> p, 1);
   mpz_sub_ui(q_1, msk -> q, 1);

   mpz_t totient;
   mpz_init(totient);
   mpz_mul(totient, p_1, q_1); 
  mpz_t d_A;
  mpz_init_set_ui(d_A, 1);
   
    int universal_attr_counter = 0;
        int m;
	for( m = 0;m<n;m++) // n+1 is done
       {
            if(user_attr_set[m] == 1)
	    		    mpz_mul(d_A, d_A, msk -> q_i[universal_attr_counter]);
		universal_attr_counter++;
		
	}
  mpz_t check;
  mpz_init(check);
  mpz_mod(check,d_A,totient);
  mpz_t e_A;
  mpz_init_set_ui(e_A, 1);
  int universal_attr_count = 0;
         int k;
         for( k = 0;k< n;k++) // n+1 is done
       {
		
                if(user_attr_set[k] == 1)
                {
		        	mpz_mul(e_A, e_A, pub-> p_i[universal_attr_count]);  
        		}
		universal_attr_count++;
	}

  mpz_init_set(prv -> e_a, e_A);
  mpz_t random_r_u;
  mpz_init(random_r_u);
  int i;
   mpz_t s_u;
   mpz_init(s_u);
	mpz_init(prv -> k1);
	mpz_init(prv -> k2);
  for( i=1;i<=1000000;i++) 
  {
    mpz_t df;
    mpz_init(df);
    mpz_t df1;
    mpz_init(df1);
    mpz_mul_ui(df1,msk ->x,i); 
    mpz_sub(df,d_A,df1);
    mpz_t mod1;
    mpz_init(mod1);
    mpz_mod(mod1,df,msk ->k);

        if(mpz_cmp_ui(mod1,0) < 0){ 
            break;
        }
        else if(mpz_cmp_ui(mod1,0) == 0)
        {
            mpz_init_set_ui(prv -> k2,i);
            mpz_cdiv_q(df, df, msk -> k);
            mpz_init_set(prv -> k1,df); //chala
            break;   
       }
  }

  mpz_t random_t_u;
  mpz_init_set_ui(random_t_u,1);
  mpz_t v1;
   mpz_init(v1);
    mpz_t v2;
   mpz_init(v2);
    mpz_t v3;
   mpz_init(v3);
   mpz_mul(v1,msk -> k,s_u);
   mpz_mul(v2,random_r_u,msk -> x);
   mpz_add(v1,v1,v2);
   mpz_mod(v3,v1,totient);

  mpz_t var1;
  mpz_init(var1);
  mpz_t var2;
  mpz_init(var2); 
  mpz_mul(var1,msk ->x,prv ->k2);  
  mpz_mul(var2,msk ->k,prv ->k1);  
  mpz_t add1;
  mpz_init(add1);
  mpz_t add2;
  mpz_init(add2);
  mpz_add(add1,var2,var1); 
  mpz_mod(add2,add1,totient); 
	return prv;
}

void bswabe_proxy(mpz_t k1, mpz_t C_attr, mpz_t C_user, int a){

	mpz_t random;
    mpz_init(random);
	generate_random(random, 10);
    mpz_add_ui(random, random, 1);
	mpz_t random2;
    mpz_init(random2);
	generate_random(random2, 10);
	mpz_add_ui(random2, random, 1);
	mpz_init(C_attr);
	int option=1;
	FILE* filePointer; //anadi
    int uid;
	filePointer = fopen("revo.txt", "r");
	while ( (fscanf(filePointer,"%d",&uid)) != EOF ){
        if(a==uid)
            option=0;
    }
	mpz_init_set(C_user, random);
    fclose(filePointer);
	if(option == 0){
        printf("\nWorking for NO REVOCATION\n");
        mpz_mul(C_attr,k1, random); 
		
	}
    else{
		printf("\nWorking for USER REVOCATION\n");
		mpz_mul(C_attr,k1, random2);
        printf("\nINVALID USER !!!\n");
	}

}

int bswabe_dec_attack( bswabe_pub_t* pub,bswabe_prv_t* prv1,bswabe_prv_t* prv2, bswabe_cph_t* cph,element_t m, int userid){

  pairing_t px;
  char* pairing_desc;
  pairing_desc = strdup(TYPE_A_PARAMS);
  pairing_init_set_buf(px, pairing_desc, strlen(pairing_desc));
  element_init_GT(m,px); 
  element_random(m);

   mpz_t product1;
   mpz_init(product1);
   mpz_t product2;
    mpz_init(product2);
    mpz_t product3;
    mpz_init(product3);
    mpz_t T1;
    mpz_init(T1);
    mpz_t T2;
    mpz_init(T2);
     mpz_t product4;
   mpz_init(product4);
   mpz_t product5;
    mpz_init(product5);
    mpz_t product6;
    mpz_init(product6);
    mpz_t K_m;
      mpz_init(K_m);
  // phle yha comment me likh jo jo bnana h
  // 1. Finding out km = T2 ^ a1 * T1 * a2
    //long long int a_p = mpz_get_ui(expo);
    // long long int pub_N = mpz_get_ui(pub -> N);
    
    // long long int cph_Rm = mpz_get_ui(cph -> R_m);
    
    // long long int cph_Ym = mpz_get_ui(cph -> Y_m);
    
    // long long int prv_k1 = mpz_get_ui(prv -> k1); 
    // long long int prv_k2 = mpz_get_ui(prv -> k2);
    
    // long long int Rm_k1 = powe(cph_Rm , prv_k1, pub_N);
    
    // long long int Ym_k2 = powe(cph_Ym, prv_k2, pub_N);  
    
    // K_mi = (Rm_k1 * Ym_k2) % pub_N;
    // final_km = powe(K_mi, a_p, pub_N);
    mpz_powm(product1, cph -> Y_m, prv1 -> k2,pub ->N); 
    mpz_powm(product2, cph -> R_m, prv1 -> k1,pub ->N);
    mpz_mul(product3,product1,product2);        
    mpz_mod(T1,product3,pub->N);
    // mpz_powm(K_m,a, expo,pub ->N); 
    gmp_printf("pub -> N is %Zd\n", pub -> N);   
    gmp_printf("T1 is %Zd\n", T1);
    gmp_printf("Y_m is %Zd.... R_m is  %Zd\n", cph -> Y_m, cph -> R_m);
    mpz_powm(product4, cph -> Y_m, prv2 -> k2,pub ->N); 
    mpz_powm(product5, cph -> R_m, prv2 -> k1,pub ->N);
    mpz_mul(product6,product4,product5);  
    mpz_mod(T2,product6, pub -> N);
    gmp_printf("T2 is %Zd\n", T2);

    // a1 = -3 and a2 = 2
    mpz_t us; mpz_init_set_ui(us, 2);
    mpz_t us2; mpz_init_set_ui(us2, 3);
    mpz_t num; mpz_init(num);
    mpz_t den; mpz_init(den);
    mpz_t wk; mpz_init(wk);

    mpz_powm(num, T1, us, pub -> N);
    mpz_powm(wk, T2,us2 , pub -> N);
    mpz_invert(den ,wk, pub -> N); 
    mpz_mul(K_m, num, den);
    mpz_mod(K_m, K_m, pub->N);

    gmp_printf("K_m in attack is %Zd\n", K_m);
     mpz_t sigma_d_m;
    mpz_init(sigma_d_m);
    mpz_t M_d;
    mpz_init(M_d);
    mpz_t blank1;
    mpz_init(blank1);


    /// -----------------------For Debugging ------------------------------///

    // mpz_t check_k_m;
    // mpz_init(check_k_m);
    // mpz_powm(check_k_m, T1,msk -> q_i[1],pub->N);
    // gmp_printf("Debugging K_m is %Zd\n", check_k_m);

    /// ------------------------------------------------------------------///





    ///


        mpz_t H2_K_m;
    mpz_init(H2_K_m);
  // mpz_init_set_ui(K_m, final_km); 
    char str1[10000];
  mpz_get_str(str1,10,K_m); 
    compute_hash(H2_K_m, str1);

  mpz_xor(sigma_d_m, H2_K_m,cph -> C_sigma);
    mpz_t H3_sigma_m;
  mpz_init(H3_sigma_m);

  int str2 = mpz_get_ui(sigma_d_m);
  compute_hash2(H3_sigma_m, str2); 

  mpz_xor(M_d, cph -> C_m,H3_sigma_m);
        mpz_t S_m;
        mpz_init(S_m);
        
        mpz_t H1;
  mpz_init(H1);
  char str3[10000];
  take_Concatenate(str3,sigma_d_m,M_d,blank1);
  long long int M_v = mpz_get_ui(M_d);
  long long int sigma_v = mpz_get_ui(sigma_d_m);
  
  //long long int final_ans = sigma_v * 100 + M_v;
  mpz_t final_ans;
  mpz_init(final_ans);  
  mpz_mul_ui(final_ans, sigma_d_m, 100);
  mpz_add(final_ans, final_ans, M_d);
  
  compute_hash2(H1, mpz_get_ui(final_ans)); 
    //printf("\n%lld  %lld", mpz_get_ui(H1), mpz_get_ui(cph->S_m));
        if(mpz_cmp(cph -> S_m,H1) == 0) 
        {   return 1;}
            return 0; 

  // 2. just copy paste from below
 
}


int bswabe_dec( bswabe_pub_t* pub,bswabe_prv_t* prv, bswabe_cph_t* cph,element_t m, int userid)// attrib is P
{
        pairing_t px;
	char* pairing_desc;
	pairing_desc = strdup(TYPE_A_PARAMS);
	pairing_init_set_buf(px, pairing_desc, strlen(pairing_desc));
         element_init_GT(m,px); 
         element_random(m);
      mpz_t product1;
      mpz_init(product1);
      mpz_t product2;
      mpz_init(product2);
      mpz_t product3;
      mpz_init(product3);
      mpz_t a;
      mpz_init(a);
      mpz_t expo;
      mpz_init(expo);
      mpz_t K_m;
      mpz_init(K_m);
	mpz_t C_attr;
	mpz_t C_user; // init
	mpz_init(C_attr);
	mpz_init(C_user);
	bswabe_proxy(prv -> k2, C_attr,C_user,userid);
    mpz_t expo2;
    mpz_init(expo2);
    mpz_cdiv_q(expo2, C_attr, C_user);
    long long int final_km = 0;
    long long int K_mi;
    // multiply kahha kia tha humne km nikaalne ke liye..
	// gmp_printf(" \ne_a is = %Zd", prv ->e_a);
	// gmp_printf(" \ne_p is = %Zd", cph ->e_p);

    if( mpz_divisible_p (prv -> e_a, cph -> e_p) !=0)
    {
    mpz_divexact(expo,prv -> e_a, cph -> e_p);
    long long int a_p = mpz_get_ui(expo);
    long long int pub_N = mpz_get_ui(pub -> N);
    
    long long int cph_Rm = mpz_get_ui(cph -> R_m);
    
    long long int cph_Ym = mpz_get_ui(cph -> Y_m);
    
    long long int prv_k1 = mpz_get_ui(prv -> k1); 
    long long int prv_k2 = mpz_get_ui(expo2);
    
    long long int Rm_k1 = powe(cph_Rm , prv_k1, pub_N);
    
    long long int Ym_k2 = powe(cph_Ym, prv_k2, pub_N);	
    
    // K_mi = (Rm_k1 * Ym_k2) % pub_N;
    // final_km = powe(K_mi, a_p, pub_N);
    mpz_powm(product1, cph -> Y_m, expo2,pub ->N); 
    mpz_powm(product2, cph -> R_m, prv -> k1,pub ->N);
    mpz_mul(product3,product1,product2);        
    mpz_mod(a,product3,pub->N);
    mpz_powm(K_m,a, expo,pub ->N);     
    gmp_printf("true K_m is %Zd\n", K_m);
    // printf("\n%lld  %lld", final_km, mpz_get_si(K_m));    
}

     else
     {
        printf("\nComputation of K_m is computationally infeasible\n");
        printf("INVALID USER !!!\n");
	    return 0;
     }

    mpz_t sigma_d_m;
    mpz_init(sigma_d_m);
    mpz_t M_d;
    mpz_init(M_d);
    mpz_t blank1;
    mpz_init(blank1);


        mpz_t H2_K_m;
	mpz_init(H2_K_m);
	// mpz_init_set_ui(K_m, final_km); 
	char str1[10000];
	mpz_get_str(str1,10,K_m); 
	compute_hash(H2_K_m, str1);

	mpz_xor(sigma_d_m, H2_K_m,cph -> C_sigma);
    mpz_t H3_sigma_m;
	mpz_init(H3_sigma_m);

	int str2 = mpz_get_ui(sigma_d_m);
	compute_hash2(H3_sigma_m, str2); 

	mpz_xor(M_d, cph -> C_m,H3_sigma_m);
        mpz_t S_m;
        mpz_init(S_m);
        
        mpz_t H1;
	mpz_init(H1);
	char str3[10000];
	take_Concatenate(str3,sigma_d_m,M_d,blank1);
	long long int M_v = mpz_get_ui(M_d);
	long long int sigma_v = mpz_get_ui(sigma_d_m);
	
	//long long int final_ans = sigma_v * 100 + M_v;
	mpz_t final_ans;
	mpz_init(final_ans);	
	mpz_mul_ui(final_ans, sigma_d_m, 100);
	mpz_add(final_ans, final_ans, M_d);
	
	compute_hash2(H1, mpz_get_ui(final_ans));	
    //printf("\n%lld  %lld", mpz_get_ui(H1), mpz_get_ui(cph->S_m));
        if(mpz_cmp(cph -> S_m,H1) == 0) 
        {   return 1;}
            return 0; 
}


