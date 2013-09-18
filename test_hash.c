#include <stdio.h>
#include "src/hash.h"
#include <string.h>
char* test_str[]={
	"a",
	"aa",
	"aaa",
	"aaaa",
	"aaaaa",
	"aaaaaa",
	"aaaaaaa",
	"aaaaaaaa",
	NULL

};

int main(){
	int i=0;
	for(i=0;test_str[i]!=NULL;i++)
	{
		printf("%s hash=> %d\r\n",test_str[i],hash(test_str[i],strlen(test_str[i]),0)%7);		
	}

}
