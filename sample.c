#include<stdio.h>
#include<string.h>
#include "sample.h"
#include<stdlib.h>
//printng error funtions :
void error()
{
	printf("-----------------------------------------------------------\n");
	printf("ERROR : ./a.out :INVALID ARGUMENTS \n");
	printf("USAGE :\n");
	printf("To View please pass like : ./a.out -v mp3filename\n");
	printf("To edit please pass like : -e -t/-a/-A/-m/-y/-c changing_text mp3filenames\n");
	printf("To Get Help pass like : ./a.out --help\n");
	printf("-----------------------------------------------------------\n");
}
//printing messege help 
void help()
{
	printf("----------------HELP MENU-----------------------------\n");
	printf("1. -v -> to view mp3 file contents:\n");
	printf("2. -e -> t edit mp3 file contents:\n");
	printf("\t\t2.1 -t -> to edit song title\n");
	printf("\t\t2.2 -a -> to edit song name\n");
	printf("\t\t2.3 -A -> to edit song Album name\n");
	printf("\t\t2.4 -y -> to edit song year\n");
	printf("\t\t2.5 -m -> to edit song content\n");
	printf("\t\t2.6 -c -> to edit song comment\n");
	printf("-------------------------------------------------------\n");
}
//check operation funtion 
status check_operation(char **argv)
{
	char arr[10];
	if (argv[1]!=NULL)
	{
		strcpy(arr,argv[1]);
		if (strcmp(arr,"-v")==0)
			return mp3_view;
		if (strcmp(arr,"-e")==0)
			return mp3_edit;
	}
}
//converting data Little to big indian:
int little_bigindian(int *ptr)
{
	char *ptr1=(char *)ptr;//taking one char and swapping
	for (int i=0;i<2;i++)
	{
		char temp=ptr1[i];
		ptr1[i]=ptr1[3-i];
		ptr1[3-i]=temp;
	}

}
//open files function source file:
status open_files(d_type *mp3_tag)
{
	if ((mp3_tag->source_fptr=fopen(mp3_tag->source_fname,"r+"))!=NULL)//openonig in read and write mode
	{
		//printf("INFO : Opended source_fptr\n");
			return mp3_success;
	}
	else
		printf("INFO : Error Source_fptr Pointing to NULL \n");

}
//copy header function:
status copy_header(d_type *mp3_tag)
{
	char arr[11];//copy 10 bytes of header:
	fread(arr,10,1,mp3_tag->source_fptr);
	fwrite(arr,10,1,mp3_tag->dest_fptr);
	return mp3_success;
}
//copy data into file until EOF (remaining data :)
status copy_data(FILE *source_fptr,FILE *dest_fptr)
{
	char ch;
	while (fread(&ch,1,1,source_fptr))//if it reches EOF return fread function 0
	{
		fwrite(&ch,1,1,dest_fptr);
	}
	return mp3_success;
}
//read and validating functions:
status read_and_validate(char **argv,d_type *mp3_tag)
{
	char arr[50];//taking one array:
	if (argv[1]!=NULL)
	{
		strcpy(arr,argv[1]);
		//printf("arr1 = %s\n",arr);
		if (strcmp(arr,"-v")==0)//if it read mode validatiing arguments:
		{
			if (argv[2]!=NULL)
			{
				//printf("arr2 = %s\n",arr);
				strcpy(arr,argv[2]);
				if (strstr(arr,".mp3")!=NULL)//checking file extensions
				{
					//printf("done\n");
					mp3_tag->source_fname=argv[2];//assigning pointer to file name;
					return mp3_success;
				}
				else
				{	
					printf("INFO : Please Pass .mp3 Extension\n");
					return mp3_failure;
				}
			}
			else
			{
				printf("INFO : Please Pass Operation '-v' or '-e' \n");
				return mp3_failure;
			}


		}
		else if (strcmp(arr,"-e")==0)//if it edit mode
		{
			//printf("arr1 = %s\n",arr);
			if (argv[2]!=NULL)
			{
				//printf("arr2_ = %s\n",arr);
				strcpy(arr,argv[2]);
				strcpy(mp3_tag->edit_op,argv[2]);
				if (strcmp(arr,"-t")==0 || strcmp(arr,"-a")==0 || strcmp(arr,"-A")==0 || strcmp(arr,"-y")==0 || strcmp(arr,"-m")==0 || strcmp(arr,"-c")==0)
				{
					if (argv[3]!=NULL)
					{
						//printf("arr3 = %s\n",arr);
						strcpy(mp3_tag->edit_content,argv[3]);//copy the content into structure variable edit_content:
						//printf("arr3 = %s\n",mp3_tag->edit_content);
						mp3_tag->size_text=strlen(mp3_tag->edit_content);//copy its length
						//printf("len_=%d\n",mp3_tag->size_text);
						if (argv[4]!=NULL)
						{
							strcpy(arr,argv[4]);
							if (strstr(arr,".mp3")!=NULL)//checking its extensions:
							{
								//printf("arr4 = %s\n",arr);
								mp3_tag->source_fname=argv[4];//asssinging file into pointer
								return mp3_success;
							}
							else
							{
								printf("INFO : Please Pass .mp3 Extension\n");
								return mp3_failure;
							}
						}
						else
						{
							printf("INFO : Please Pass Source.mp3 \n");
							return mp3_failure;
						}

					}
					else
					{
						printf("INFO : Please Pass Operation '-v' or '-e' \n");
						return mp3_failure;
					}
				}
				else 
				{
					printf("INFO : Please Pass Operation '-v' or '-e' \n");
					return mp3_failure;
				}
			}
			else
			{	
				printf("INFO : Please Pass Operation '-v' or '-e' \n");
				return mp3_failure;
			}
		}
		else 
		{
			error();
			return mp3_failure;
		}
	}

}
//view function definations:
status view(char **argv,d_type *mp3_tag)
{
	if (read_and_validate(argv,mp3_tag)==mp3_success)//function call fioor read and validate functions
	{
		//printf("INFO : Read and validation Done\n");
		if ((open_files(mp3_tag) ==mp3_success))//function call for open file
		{
			char arr[20];
			fread(arr,3,1,mp3_tag->source_fptr);
			if (strcmp(arr,"ID3")==0)//checking Tag matched are not 
			{
					//printf("TAG = %s\n",arr);
				char ch;
				fread(&ch,1,1,mp3_tag->source_fptr);//reading one byte:
				if (ch == 03)//checking version
				{
					fread(&ch,1,1,mp3_tag->source_fptr);//again one more byte
					if (ch == 0)
					{
						fseek(mp3_tag->source_fptr,5,SEEK_CUR);//moving 5 bytes from header
						printf("----------------------------SELECTED VIEW DETAILS-------------------------------------\n\n\n");
						printf("--------------------------------------------------------------------------------------\n");
						printf("\t\t%s","MP3 TAG READER AND EDITOR FOR ID3V2\n");
						printf("--------------------------------------------------------------------------------------\n");
						for(int j=0;j<6;j++)
						{
							//printf("ptr =%d\n",ftell(mp3_tag->source_fptr));
							char arr2[5];

							fread(arr2,4,1,mp3_tag->source_fptr);//taking  TAG  into array
							//printf("arr=%s\n",arr2);
							if (strcmp(arr2,"TIT2")==0 || strcmp(arr2,"TPE1")==0 || strcmp(arr2,"TALB")==0 || strcmp(arr2,"TYER")==0 || strcmp(arr2,"TCON")==0 || strcmp(arr2,"COMM")==0)//checking all operations 
							{
								//fseek(mp3_tag->source_fptr,3,SEEK_CUR);
								int num;
								fread(&num,4,1,mp3_tag->source_fptr);//reading 4 bytes for length
								//printf("num = %x\n",num);
								little_bigindian(&num);//converting into bigindian
								//printf("Num = %x \n",num);
								fseek(mp3_tag->source_fptr,3,SEEK_CUR);//skipping 3 bytes flag
								char arr3[num];
								//printf("size = %d\n", num);
								fread(arr3,num-1,1,mp3_tag->source_fptr);
								arr3[num-1] = '\0';//last assigning null character:
								if (strcmp(arr2,"TIT2")==0) printf("%s\t\t:","Title");
								if (strcmp(arr2,"TPE1")==0) printf("%s\t\t:","Artist");
								if (strcmp(arr2,"TALB")==0) printf("%s\t\t:","Album");
								if (strcmp(arr2,"TYER")==0) printf("%s\t\t:","YEAR");
								if (strcmp(arr2,"TCON")==0) printf("%s\t\t:","Content");
								if (strcmp(arr2,"COMM")==0) printf("%s\t\t:","Comment");
								printf("\t\t%s\n",arr3);//printitng Content 
								//printf("Num = %d \n",num);
							}

						}
						printf("--------------------------------------------------------------------------------------\n");
						printf("----------------------------DETAILS DISPLAYED  SUCCESFULLY-----------------------------\n");
						//fclose(mp3_tag->source_fptr);

					}
					else
						printf("INFO : Version2 Not Matched \n");
				}
				else
					printf("INFO : Version1 Not Matched \n");

			}
			else
				printf("INFO : ID3 Not Matched \n");
		}
		else
			printf("INFO : Failwd toopen files\n");


	}
	else
		printf("INFO : failed to Read and validate:\n");


}
//edit function defination:
status edit(char **argv,d_type *mp3_tag)
{
	if (read_and_validate(argv,mp3_tag)==mp3_success)
	{
			printf("INFO : Read and validation Done");
		if ((open_files(mp3_tag) ==mp3_success))
		{
			mp3_tag->dest_fname="temp.mp3";//assingnig file name temp.mp3
			if ((mp3_tag->dest_fptr=fopen(mp3_tag->dest_fname,"w+"))!=NULL)//opening into w+ mode
				printf("Opended Temp.mp3\n");
			else
				printf("INFO : ERROR Opning temp.mp3\n");
			char arr[20];
			fread(arr,3,1,mp3_tag->source_fptr);
			if (strcmp(arr,"ID3")==0)//Tag 
			{
				//printf("TAG = %s\n",arr);
				char ch;
				fread(&ch,1,1,mp3_tag->source_fptr);
				if (ch == 03)
				{
					fread(&ch,1,1,mp3_tag->source_fptr);
					if (ch == 0)
					{
						//printf("Version matched \n");
						//fseek(mp3_tag->source_fptr,5,SEEK_CUR);
						rewind(mp3_tag->source_fptr);
					if ((copy_header(mp3_tag)==mp3_success));//function call for hrader copy
						//printf("INFO : Header copied\n");
						printf("----------------------------SELECTED EDIT DETAILS-------------------------------------\n\n\n");
						for(int j=0;j<6;j++)
						{
							//printf("ptr1 =%d\n",ftell(mp3_tag->source_fptr));
							//printf("ptr2 =%d\n",ftell(mp3_tag->dest_fptr));
							char arr2[5];//storing Tag

							fread(arr2,4,1,mp3_tag->source_fptr);//storing tag
							fwrite(arr2,4,1,mp3_tag->dest_fptr);//write tag into dest tag
							//printf("t=%s\n",arr2);
							if (strcmp(arr2,"TIT2")==0 || strcmp(arr2,"TPE1")==0 || strcmp(arr2,"TALB")==0 || strcmp(arr2,"TYER")==0 || strcmp(arr2,"TCON")==0 || strcmp(arr2,"COMM")==0)
							{
								//fseek(mp3_tag->source_fptr,3,SEEK_CUR);
								int num;
								fread(&num,4,1,mp3_tag->source_fptr);
								//printf("num = %x\n",num);
								little_bigindian(&num);
								//printf("edit_content=%s\n",mp3_tag->edit_content);
								//fseek(mp3_tag->source_fptr,3,SEEK_CUR);
								//printf("num = %d\n",num);
								char arr3[num+8+3];

								//printf("copy= %s\n",strcpy(arr3,mp3_tag->edit_content));
								if (strcmp(arr2,"TIT2")==0 && strcmp(mp3_tag->edit_op,"-t")==0) 
								{
									int temp_size = mp3_tag->size_text;//taking size tempareray variable
									mp3_tag->size_text += 1;//increasing one for null storing
									little_bigindian(&(mp3_tag->size_text));//converting that number into hexa
									fwrite(&(mp3_tag->size_text),4,1,mp3_tag->dest_fptr);//size of new tag content
									fread(arr3,3,1,mp3_tag->source_fptr);//flag copy
									fwrite(arr3,3,1,mp3_tag->dest_fptr);
									strcpy(arr3,mp3_tag->edit_content);//new content 
									fwrite(arr3,temp_size,1,mp3_tag->dest_fptr);//copying the new content 
									fseek(mp3_tag->source_fptr,num-1,SEEK_CUR);
									printf("\t\t\t------TITLE EDITED SUCCESFULLY-----\n");
									break;
								}
								else if (strcmp(arr2,"TPE1")==0 && strcmp(mp3_tag->edit_op,"-a")==0) 
								{
									//fseek(mp3_tag->source_fptr,-3,SEEK_CUR);
									//mp3_tag->size_text=num;
									int temp_size = mp3_tag->size_text;
									mp3_tag->size_text += 1;
									little_bigindian(&(mp3_tag->size_text));
									fwrite(&(mp3_tag->size_text),4,1,mp3_tag->dest_fptr);
									fread(arr3,3,1,mp3_tag->source_fptr);//storing tag
									fwrite(arr3,3,1,mp3_tag->dest_fptr);
									strcpy(arr3,mp3_tag->edit_content);
									fwrite(arr3,temp_size,1,mp3_tag->dest_fptr);
									fseek(mp3_tag->source_fptr,num-1,SEEK_CUR);
									printf("\t\t\t------ARTIST EDITED SUCCESFULLY------\n");
									break;
								}
								else if (strcmp(arr2,"TALB")==0 && strcmp(mp3_tag->edit_op,"-A")==0) 
								{
									//mp3_tag->size_text=num;
									int temp_size = mp3_tag->size_text;
									mp3_tag->size_text += 1;
									little_bigindian(&(mp3_tag->size_text));
									fwrite(&(mp3_tag->size_text),4,1,mp3_tag->dest_fptr);
									fread(arr3,3,1,mp3_tag->source_fptr);//storing tag
									fwrite(arr3,3,1,mp3_tag->dest_fptr);
									strcpy(arr3,mp3_tag->edit_content);
									mp3_tag -> size_text -= 1;
									fwrite(arr3,temp_size,1,mp3_tag->dest_fptr);
									fseek(mp3_tag->source_fptr,num-1,SEEK_CUR);
									printf("\t\t\t------ALBUM EDITED SUCCESFULLY------\n");
									break;
								}
								else if (strcmp(arr2,"TYER")==0 && strcmp(mp3_tag->edit_op,"-y")==0) 
								{
									//fseek(mp3_tag->source_fptr,-3,SEEK_CUR);
									//mp3_tag->size_text=num;
									int temp_size = mp3_tag->size_text;
									mp3_tag->size_text += 1;
									little_bigindian(&(mp3_tag->size_text));
									fwrite(&(mp3_tag->size_text),4,1,mp3_tag->dest_fptr);
									fread(arr3,3,1,mp3_tag->source_fptr);//storing tag
									fwrite(arr3,3,1,mp3_tag->dest_fptr);
									strcpy(arr3,mp3_tag->edit_content);
									mp3_tag -> size_text -= 1;
									fwrite(arr3,temp_size,1,mp3_tag->dest_fptr);
									//printf("size = %d\n", num);
									fseek(mp3_tag->source_fptr,num-1,SEEK_CUR);
									printf("\t\t\t-----YEAR EDITED SUCCESFULLY-----\n");
									break;
								}
								else if (strcmp(arr2,"TCON")==0 && strcmp(mp3_tag->edit_op,"-m")==0) 
								{
									//fseek(mp3_tag->source_fptr,-3,SEEK_CUR);
									//mp3_tag->size_text=num;
									int temp_size = mp3_tag->size_text;
									mp3_tag->size_text += 1;
									little_bigindian(&(mp3_tag->size_text));
									fwrite(&(mp3_tag->size_text),4,1,mp3_tag->dest_fptr);
									fread(arr3,3,1,mp3_tag->source_fptr);//storing tag
									fwrite(arr3,3,1,mp3_tag->dest_fptr);
									strcpy(arr3,mp3_tag->edit_content);
									mp3_tag -> size_text -= 1;
									fwrite(arr3,temp_size,1,mp3_tag->dest_fptr);
									fseek(mp3_tag->source_fptr,num-1,SEEK_CUR);
									printf("\t\t\t-----CONTENT EDITED SUCCESFULLY-----\n");
									break;
								}
								else if (strcmp(arr2,"COMM")==0 && strcmp(mp3_tag->edit_op,"-c")==0) 
								{
									//fseek(mp3_tag->source_fptr,-3,SEEK_CUR);
									//mp3_tag->size_text=num;
									int temp_size = mp3_tag->size_text;
									mp3_tag->size_text += 1;
									little_bigindian(&(mp3_tag->size_text));
									fwrite(&(mp3_tag->size_text),4,1,mp3_tag->dest_fptr);
									fread(arr3,3,1,mp3_tag->source_fptr);//storing tag
									fwrite(arr3,3,1,mp3_tag->dest_fptr);
									strcpy(arr3,mp3_tag->edit_content);
									mp3_tag -> size_text -= 1;
									fwrite(arr3,temp_size,1,mp3_tag->dest_fptr);
									fseek(mp3_tag->source_fptr,num-1,SEEK_CUR);
									printf("\t\t\t------COMMENT EDITED SUCCESFULLY-------\n");
									break;
								}
								else
								{
									//fseek(mp3_tag->source_fptr,num-1,SEEK_CUR);
									//printf("fseek\n");
									fseek(mp3_tag->source_fptr,-4,SEEK_CUR);
									fread(arr3,num+6,1,mp3_tag->source_fptr);
									fwrite(arr3,num+6,1,mp3_tag->dest_fptr);
							//printf("ptr =%d\n",ftell(mp3_tag->source_fptr));
							//printf("ptr =%d\n",ftell(mp3_tag->dest_fptr));
								}
							}

						}

						if (copy_data(mp3_tag->source_fptr,mp3_tag->dest_fptr)==mp3_success);
							//printf("Remaining Data copied succesfully \n");
						rewind(mp3_tag->source_fptr);//rewind pointers
						rewind(mp3_tag->dest_fptr);//rewind pointers
						if (copy_data(mp3_tag->dest_fptr,mp3_tag->source_fptr)==mp3_success);//storing Data into same file 
							//printf("Copied into Again main file:\n");
						printf("----------------------------DETAILS EDITED SUCCESFULLY-----------------------------\n");
						//fclose(mp3_tag->source_fptr);
					}
					else
						printf("INFO : Version2 Not Matched \n");
				}
				else
					printf("INFO : Version1 Not Matched \n");

			}
			else
				printf("INFO : ID3 Not Matched \n");
		}
		else
			printf("INFO : Failed to Open files\n");
	}
	else 
		printf("INFO : failed to Read and validate:\n");
}


