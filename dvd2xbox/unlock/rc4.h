 /* rc4.h */
#ifndef RC4
#define RC4

typedef struct rc4_key
{      
   unsigned char state[256];       
   unsigned char x;        
   unsigned char y;
} rc4_key;

void prepare_key(unsigned char *key_data_ptr,int key_data_len,
		 rc4_key *key);

void rc4(unsigned char *buffer_ptr,int buffer_len,rc4_key * key);

#endif