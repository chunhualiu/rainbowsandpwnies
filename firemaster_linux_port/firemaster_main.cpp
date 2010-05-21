#include <stdio.h>
#include "lowpbe.h"
#include "sha_fast.h"
#include <unistd.h>

#define MAX_PASSWORD LENGTH 128

/***** Function Prototypes ******/
void usage();
static int CheckMasterPassword(char*);
int FireMasterInit(char *);
void BruteCrack(const char *, char *, const int, int);
/**** End Function Prototypes ***/


/************* GLobal Declarations **************/

// Database items
SHA1Context pctx;
unsigned char encString[128];
NSSPKCS5PBEParameter *paramPKCS5 = NULL;
KeyCrackData keyCrackData;
// General Bruteforce
int bruteCharCount;
int brutePosMaxCount=-1;
int brutePosMinCount=-1;
const char* bruteCharSet = "";
// Pattern Cracking
char *brutePattern=NULL;
char brutePatternBitmap[MAX_PASSWORD_LENGTH];

/************ End GLobal Declarations ************/

int main(int argc, char* argv[]){
	
	char* patternmatch;
	char* profileDir = "/home/me/.mozilla/firefox/o4u8k1aq.default/";
	int i;

	FireMasterInit(profileDir);

	// Parse arguments
	for (int i = 1; i < argc; i++){
		if ((strcmp(argv[i], "-l")==0) && (i+1<argc)){
			brutePosMaxCount = atoi(argv[++i]);
			if (!((brutePosMaxCount > 0) && (brutePosMaxCount <= MAX_PASSWORD_LENGTH))) 
				usage();
			printf("Maximum password length = %i\n", brutePosMaxCount);
		}
		else if ((strcmp(argv[i], "-m")==0) && (i+1<argc)){
			brutePosMinCount = atoi(argv[++i]);
			if (!((brutePosMaxCount > 0) && (brutePosMinCount > 0) && (brutePosMinCount <= MAX_PASSWORD_LENGTH) && (brutePosMinCount<=brutePosMaxCount)))
				usage();
			printf("Minimum password length = %i\n", brutePosMinCount);
		}
		else if ((strcmp(argv[i], "-c")==0) && (i+1<argc))
			bruteCharSet = argv[++i];
		else if ((strcmp(argv[i], "-p")==0) && (i+1<argc))
			brutePattern = argv[++i];
		else usage();
	}

	// Default bruteCharSet
	if (strlen(bruteCharSet) == 0)
		bruteCharSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	bruteCharCount = strlen(bruteCharSet);
	
	printf("Character Set = %s\n", bruteCharSet);

	char brutePassword[MAX_PASSWORD_LENGTH]="";
	BruteCrack(bruteCharSet, brutePassword, 0, 0);

	printf("\n%s\n", "No Luck: try again with better options");

	return 0;		
}

void BruteCrack(const char *bruteCharSet, char *brutePasswd, const int index, int next )
{	
	int i;

	if (index >= brutePosMaxCount) return;

	for(i=0; i< bruteCharCount; i++ )
	{
		if (brutePattern == NULL){
			brutePasswd[index] = bruteCharSet[next%bruteCharCount];
			brutePasswd[index+1] = 0;
			next++;
		
			// printf("%s\n", brutePasswd);
		
			if( CheckMasterPassword(brutePasswd) )
			{
				printf("\nPassword: \t\"%s\"\n", brutePasswd);
				exit(0);
			}

			BruteCrack(bruteCharSet, brutePasswd,index+1, next);
		}
		/*
		else{
			if( brutePatternBitmap[index] == 1 )
				brutePasswd[index] = brutePattern[index];
			else{
				brutePasswd[index] = bruteCharSet[next%bruteCharCount];
				next++;
			}

			if( index == (brutePosMaxCount-1) )
			{
				brutePasswd[index+1] = 0;
				bruteCount++;
			
				if( !isQuiet )
					printf("\n%.0lf = Attempting %s ", bruteCount, brutePasswd);
			
				//Finally check if we have found the master password....			
				if( CheckMasterPassword(brutePasswd) )
				{
					PrintPassword(brutePasswd);
					FireMasterExit();
				}
			}
		}
		*/
	}
}


static int CheckMasterPassword(char *password)
{
		unsigned char passwordHash[SHA1_LENGTH+1];

        SHA1Context ctx;

        // Copy already calculated partial hash data..
        memcpy(&ctx, &pctx, sizeof(SHA1Context) );
        SHA1_Update(&ctx, (unsigned char *)password, strlen(password));
    	SHA1_End(&ctx, passwordHash);

        return nsspkcs5_CipherData(paramPKCS5, passwordHash, encString);  //&encStringItem );
}



void usage(){
	printf("Usage\n\t./firemaster_linux [firefox profile directory] [options]\n\n");	
	printf("Options:\n");	
	printf("\t-l\tmaximum length of password, not to exceed 128\n");
	printf("\t-m\tminimum length of password\n");
	printf("\t-p\tpattern to use for cracking (i.e. 'p???word')\n");
	printf("\t-c\tcharacter set to use for cracking (i.e. 'abcABC')\n");
	printf("\t\t(single quotes only necessary if bash special operators are in the character set)\n");  
	printf("\n");
	exit(2);	
}

int FireMasterInit(char *dirProfile)
{
    SECItem saltItem;
	
	if( CrackKeyData(dirProfile, keyCrackData) == false)
	{
		exit(0);	
	}

	// Initialize the pkcs5 structure...
	saltItem.type = (SECItemType) 0;
	saltItem.len  = keyCrackData.saltLen;
	saltItem.data = keyCrackData.salt;
	paramPKCS5 = nsspkcs5_NewParam(NULL, &saltItem, 1);
	
	if( paramPKCS5 == NULL)
	{
		printf("\n Failed to initialize NSSPKCS5 structure");
		exit(0);
   	}

	// Current algorithm is 
	// SEC_OID_PKCS12_PBE_WITH_SHA1_AND_TRIPLE_DES_CBC

	// Setup the encrypted password-check string
    memcpy(encString, keyCrackData.encData, keyCrackData.encDataLen );
	
	if( CheckMasterPassword("") == true )
	{
		printf("\n Master password is not set ...exiting FireMaster \n\n");
		exit(0);
	}

	// Calculate partial sha1 data for password hashing...
    SHA1_Begin(&pctx);
	SHA1_Update(&pctx, keyCrackData.globalSalt, keyCrackData.globalSaltLen);

	return true;
}



