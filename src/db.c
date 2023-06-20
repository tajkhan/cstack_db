#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT
} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;


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

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer* buf,
                                Statement* stmt) {
    if(strncmp(buf->buffer, "insert", 6)==0) {
        stmt->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    } else if (strcmp(buf->buffer, "select")==0) {
        stmt->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}


void execute_statement(Statement* stmt) {
    switch(stmt->type) {
        case (STATEMENT_INSERT):
            printf("This is where we would do an insert.\n");
            break;
        case (STATEMENT_SELECT):
            printf("This is where we would do an select.\n");
            break;
    }
}


int main(int argc, char* argv[]){

    InputBuffer *input_buffer = new_input_buffer();
    while(true){
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0]=='.') {
            switch(do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }


        Statement stmt;
        switch(prepare_statement(input_buffer, &stmt)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n",
                    input_buffer->buffer);
                continue;
        }

        execute_statement(&stmt);
        printf("Executed.\n");

    }

    return EXIT_SUCCESS;
}
