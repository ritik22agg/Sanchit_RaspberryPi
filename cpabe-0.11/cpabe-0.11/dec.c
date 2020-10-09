#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <time.h>

#include "bswabe.h"
#include "common.h"

char *usage =
	"Usage: cpabe-dec [OPTION ...] PUB_KEY PRIV_KEY FILE PRIV_KEY2\n"
	"\n"
	"Decrypt FILE using private key PRIV_KEY and assuming public key\n"
	"PUB_KEY. If the name of FILE is X.cpabe, the decrypted file will\n"
	"be written as X and FILE will be removed. Otherwise the file will be\n"
	"decrypted in place. Use of the -o option overrides this\n"
	"behavior.\n"
	"\n"
	"Mandatory arguments to long options are mandatory for short options too.\n\n"
	" -h, --help               print this message\n\n"
	" -v, --version            print version information\n\n"
	" -k, --keep-input-file    don't delete original file\n\n"
	" -o, --output FILE        write output to FILE\n\n"
	" -d, --deterministic      use deterministic \"random\" numbers\n"
	"                          (only for debugging)\n\n"
	"";

char *pub_file = 0;
char *prv_file = 0;
char *prvb_file = 0;
char *in_file = 0;
char *out_file = 0;
//char*  msk_file = 0;
int keep = 0;

void parse_args(int argc, char **argv)
{
	int i;

	for (i = 1; i < argc; i++)
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			printf("%s", usage);
			exit(0);
		}
		else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
		{
			printf(CPABE_VERSION, "-dec");
			exit(0);
		}
		else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--keep-input-file"))
		{
			keep = 1;
		}
		else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
		{
			if (++i >= argc)
				die(usage);
			else
				out_file = argv[i];
		}
		else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--deterministic"))
		{
			pbc_random_set_deterministic(0);
		}
		else if (!pub_file)
		{
			pub_file = argv[i];
		}
		else if (!prv_file)
		{
			prv_file = argv[i];
		}
		else if (!in_file)
		{
			in_file = argv[i];
		}else if(!prvb_file){
			prvb_file = argv[i];
		}
		else
			die(usage);

	if (!pub_file || !prv_file || !in_file)
		die(usage);

	if (!out_file)
	{
		if (strlen(in_file) > 6 &&
			!strcmp(in_file + strlen(in_file) - 6, ".cpabe"))
			out_file = g_strndup(in_file, strlen(in_file) - 6);
		else
			out_file = strdup(in_file);
	}

	if (keep && !strcmp(in_file, out_file))
		die("cannot keep input file when decrypting file in place (try -o)\n");
}

int main(int argc, char **argv)
{
	printf("\n\n### IN DECRYPTION ###\n");
	bswabe_pub_t *pub;
	bswabe_prv_t *prv;
	bswabe_prv_t *prv2;
	//bswabe_msk_t *msk;
	int file_len, temp = 0;

	GByteArray *aes_buf;
	GByteArray *plt;
	GByteArray *cph_buf;
	bswabe_cph_t *cph;
	element_t m1;
	clock_t t1, t2;
	float diff;

	t1 = clock();
	srand(time(NULL));
	int a;
	parse_args(argc, argv);
	pub = bswabe_pub_unserialize(suck_file(pub_file), 1);
	prv = bswabe_prv_unserialize(pub, suck_file(prv_file), 1, &a);
	prv2 = bswabe_prv_unserialize(pub, suck_file(prvb_file), 1, &a);
	//msk = bswabe_msk_unserialize(suck_file(msk_file), 1);
	read_cpabe_file(in_file, &cph_buf, &file_len, &aes_buf);

	cph = bswabe_cph_unserialize(pub, cph_buf, 1);

	printf("Do you Want to attack Or Decrypt\n Press 1 for Attack\n Press 0 for Decrypt");
	int opt ;
	scanf("%d", &opt);
	int res;
	if(opt == 1){
		res = bswabe_dec_attack(pub,prv,prv2, cph, m1, a);
	}else{
		res = bswabe_dec(pub, prv, cph, m1, a);
	}

	if (!res)
	{
		t2 = clock();
		diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
		printf("\nTime taken in seconds in=%f\n", diff);
		die("%s", bswabe_error());
	}
	
	bswabe_cph_free(cph);
	plt = aes_128_cbc_decrypt(aes_buf, m1);

	g_byte_array_set_size(plt, file_len);

	g_byte_array_free(aes_buf, 1);

	spit_file(out_file, plt, 1);

	t2 = clock();
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds=%f", diff);

	if (!keep)
		unlink(in_file);
	printf("\nunlink\n");
	printf("Decryption Succesful\n");
	t2 = clock();
	diff = ((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds af=%f", diff);
	return 0;
}
