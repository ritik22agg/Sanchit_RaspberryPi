/*
	Include glib.h, pbc.h, and bswabe.h before including this file.
*/

struct bswabe_pub_s		//  USE IT AS mpk AB SUN CHANGES KARTE H ISMEIN..VAISE MAINE //SETUP MEIN KARTA HU MEIN
{
	// int count;
	// char* pairing_desc;
	// int total_attr;
	// pairing_t p;
	// element_t g1;        /* G_1 */
	// element_t h;           /* G_1 */
	// element_t Y;          /* G_T */
	// element_t T[40][3]; /* G_1 */
	// element_t U[256];
	mpz_t n;
        mpz_t N;
	mpz_t Y;
	mpz_t R;
	mpz_t p_i[10000];
	mpz_t D_u; // CAPS OFF 


};

struct bswabe_msk_s		// use it as MSk
{
	// element_t y;    /* Z_r */
	// element_t t[40][3]; /* G_2 */
	// element_t u[256];

	mpz_t k;
	mpz_t x; 
	mpz_t p;
	mpz_t q;
	mpz_t n ;
	mpz_t q_i[10000];	


};

struct bswabe_cph_s
{
	// element_t cs; /* G_T *///M*Y^s//C1
	// element_t c;  /* G_1 *///g^s//C2
	// element_t c3; /* G_1 */
	// GArray* comps_enc; /* bswabe_enc_comp_t's */
	// bswabe_signature_t* signature;//bswabe_policy_t* p;
	mpz_t Y_m;
	mpz_t R_m;
	mpz_t C_sigma;
	mpz_t C_m;
	mpz_t S_m;
        mpz_t e_p;
};

struct bswabe_prv_s{

	mpz_t k1;
	mpz_t k2;
	mpz_t e_a;
};

struct bswabe_sig_s{
	
	element_t x;	
};

struct bswabe_verification_s{

	element_t y;
	element_t g;
	element_t g_y;
	element_t g_xy;
	char* y_s;
};

struct bswabe_signature_s{

	element_t sigma;
};

struct bswabe_sigver_s{
	
	bswabe_verification_t* a;
	bswabe_sig_t* b;
};
//proxy
struct bswabe_proxy_s
{
	element_t Cuser;    /* G_1*/
	element_t Cattr;
	int on; 
};
typedef struct
{
	/* these actually get serialized */
	char* attr;
	element_t d;  /* G_2 */
	element_t dp; /* G_2 */

	/* only used during dec (only by dec_merge) */
	int used;
	element_t z;  /* G_1 */
	element_t zp; /* G_1 */
}
bswabe_prv_comp_t;


typedef struct
{
	int deg;
	/* coefficients from [0] x^0 to [deg] x^deg */
	element_t* coef; /* G_T (of length deg + 1) */
}
bswabe_polynomial_t;

typedef struct
{
	/* serialized */
	int k;            /* one if leaf, otherwise threshold */
	char* attr;       /* attribute string if leaf, otherwise null */
	element_t c;      /* G_1, only for leaves */
	element_t cp;     /* G_1, only for leaves */
	GPtrArray* children; /* pointers to bswabe_policy_t's, len == 0 for leaves */

	/* only used during encryption */
	bswabe_polynomial_t* q;

	/* only used during decryption */
	int satisfiable;
	int min_leaves;
	int attri;
	GArray* satl;
}
bswabe_policy_t;

typedef struct 
{
	char* attr;
}
bswabe_enc_comp_t;

