#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char *buffer;
    size_t buffer_size;
    ssize_t input_size; // why use ssize (signed size) here??
} InputBuffer;

void print_prompt(){
    printf("db> ");
}

InputBuffer* new_input_buffer(){
    InputBuffer* buf = (InputBuffer*) malloc(sizeof(InputBuffer));
    buf->buffer = NULL;
    buf->buffer_size = 0;
    buf->input_size  = 0;
    return buf;
}

void read_input(InputBuffer* buf){
    ssize_t bytes_read = 
        getline(&(buf->buffer), &(buf->buffer_size), stdin);

    if(bytes_read<0){
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    // remove the trailing \n
    buf->buffer_size = bytes_read-1;
    buf->buffer[bytes_read-1] = '\0';
}

void free_input_buffer(InputBuffer* buf){
    free(buf->buffer);
    free(buf);
}

int main(int argc, char* argv[]){

    InputBuffer *input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if (strcmp(input_buffer->buffer, ".exit")==0){
            free_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        }

    }
    return EXIT_SUCCESS;
}
