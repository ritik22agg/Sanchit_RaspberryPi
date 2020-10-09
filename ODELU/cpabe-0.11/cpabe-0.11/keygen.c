#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <glib.h>
#include <pbc.h>
#include <pbc_random.h>
#include <time.h>


#include "bswabe.h"
#include "common.h"
#include "policy_lang.h"

char* usage =
"Usage: cpabe-keygen [OPTION ...] PUB_KEY MASTER_KEY ATTR [ATTR ...]\n"
"\n"
"Generate a key with the listed attributes using public key PUB_KEY and\n"
"master secret key MASTER_KEY. Output will be written to the file\n"
"\"priv_key\" unless the -o option is specified.\n"
"\n"
"Attributes come in two forms: non-numerical and numerical. Non-numerical\n"
"attributes are simply any string of letters, digits, and underscores\n"
"beginning with a letter.\n"
"\n"
"Numerical attributes are specified as `attr = N', where N is a non-negative\n"
"integer less than 2^64 and `attr' is another string. The whitespace around\n"
"the `=' is optional. One may specify an explicit length of k bits for the\n"
"integer by giving `attr = N#k'. Note that any comparisons in a policy given\n"
"to cpabe-enc(1) must then specify the same number of bits, e.g.,\n"
"`attr > 5#12'.\n"
"\n"
"The keywords `and', `or', and `of', are reserved for the policy language\n"
"of cpabe-enc (1) and may not be used for either type of attribute.\n"
"\n"
"Mandatory arguments to long options are mandatory for short options too.\n\n"
" -h, --help               print this message\n\n"
" -v, --version            print version information\n\n"
" -o, --output FILE        write resulting key to FILE\n\n"
" -d, --deterministic      use deterministic \"random\" numbers\n"
"                          (only for debugging)\n\n"
"";


char*  pub_file = 0;
char*  msk_file = 0;
char* attrs    = 0;

char*  out_file = "priv_key";

gint
comp_string( gconstpointer a, gconstpointer b)
{
	return strcmp(a, b);
}

int
parse_args( int argc, char** argv )
{
	int i, flag=0;
	GSList* alist;
	GSList* ap;
	int n;

	alist = 0;
	for( i = 1; i < argc; i++ ){
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help") )
		{
			printf("%s", usage);
			exit(0);
		}
		else if( !strcmp(argv[i], "-v") || !strcmp(argv[i], "--version") )
		{
			printf(CPABE_VERSION, "-keygen");
			exit(0);
		}
		else if( !strcmp(argv[i], "-o") || !strcmp(argv[i], "--output") )
		{
			if( ++i >= argc )
				die(usage);
			else
				out_file = argv[i];
		}
		else if( !strcmp(argv[i], "-d") || !strcmp(argv[i], "--deterministic") )
		{
			pbc_random_set_deterministic(0);
		}
		else if( !pub_file )
		{
			pub_file = argv[i];
		}
		else if( !msk_file )
		{
			msk_file = argv[i];
		}
		else
		{
			flag=i;
			break;
		}
	}

	if( !pub_file || !msk_file || flag==0 )
		die(usage);

	n = argc-5;

	attrs = (char*)malloc((n+1) * sizeof(char));
	i = 0;
	int j;
	for( j=flag;j<argc;j++)
	{
		attrs[i] = argv[j][0];
		i++;
	}
	attrs[i] = '0';
	return n;
}

int
main( int argc, char** argv )
{
	printf("\n\n### IN KEYGEN ###\n");

	FILE* filePointer; //anadi
	filePointer = fopen("revo.txt", "r");

	bswabe_pub_t* pub;
	bswabe_msk_t* msk;
	bswabe_prv_t* prv;
	int num;
	int a=0; //anadi
	while ( (fscanf(filePointer,"%d",&a)) != EOF ){}
	a++;
	fclose(filePointer);
	filePointer = fopen("revo.txt", "a");
	fprintf(filePointer,"%d\n",a); 
	fclose(filePointer);
	clock_t t1,t2;
	float diff;
	
	t1=clock();
        srand(time(NULL));

	int n = parse_args(argc, argv);
	pub = bswabe_pub_unserialize(suck_file(pub_file), 1);
	msk = bswabe_msk_unserialize(suck_file(msk_file), 1);
	int attributes[n+1];
	int i;
	for( i = 0; i < n;i++){
		if(attrs[i]=='1'){
			attributes[i] = 1;
		}else{
			attributes[i] = 0;
		}
	}
	attributes[n] = 1;
	prv = bswabe_keygen(pub, msk, attributes);
	spit_file(out_file, bswabe_prv_serialize(prv, a), 1); //anadi
	t2=clock();
	diff=((double)(t2 - t1) / CLOCKS_PER_SEC);
	printf("\nTime taken in seconds=%f \n",diff);
	return 0;
}
