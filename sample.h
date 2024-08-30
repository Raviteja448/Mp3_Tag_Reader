#ifndef ENCODE_H
#define ENCODE_H

typedef enum
{
	mp3_success,
	mp3_failure,
	mp3_view,
	mp3_edit,
}status;
typedef struct details
{
	char *source_fname;
	FILE *source_fptr;
	char *dest_fname;
	FILE *dest_fptr;
	char edit_op[5];
	char edit_content[70];
	int size_text;
	long int size;
}d_type;


//error function:
void error();
//help msg function:
void help();
//check operation function:
status check_operation(char **argv);
//copy header
status copy_header(d_type *mp3_tag);
//copy data
status copy_data(FILE *source_fptr,FILE *dest_fptr);
//open files
status open_files(d_type *mp3_tag);
//read and validate function:
status read_and_validate(char **argv,d_type *mp3_tag);
//view the content 
status view(char **argv,d_type *mp3_tag);
//edit the content
status edit(char **argv,d_type *mp3_tag);
//little indian to big indian
int little_bigindian(int *ptr);



#endif
