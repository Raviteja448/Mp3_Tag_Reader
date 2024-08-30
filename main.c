#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "sample.h"


int main(int argc,char **argv)
{
	if (argc==1)
		error();
	if (argc==2)
	{
		char arr[20];
		strcpy(arr,argv[1]);
		if (strcmp(arr,"--help")==0)
			help();//help function call
		else
			error();//error function
	}
	d_type mp3_tag;
	if (argc>2)
	{
		int op=check_operation(argv);
		if (op == mp3_view)
			view(argv,&mp3_tag);//function call for View:
		if (op == mp3_edit)
			edit(argv,&mp3_tag);//function call for Edit:

	}

}

