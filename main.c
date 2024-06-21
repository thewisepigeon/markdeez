#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint8_t *buffer;
  uint8_t buffer_size;
  uint8_t current_position;
  uint8_t current_char;
  int8_t read_position;
} Lexer;

Lexer *new_lexer(uint8_t *buffer, uint8_t buffer_size) {
  if (buffer == NULL || buffer_size == 0) {
    printf("Null buffer or invalid buffer size");
    return NULL;
  }
  Lexer *lexer = malloc(sizeof(Lexer));
  if (lexer == NULL) {
    perror("Error while allocating memory for lexer");
    return NULL;
  }
  lexer->buffer = buffer;
  lexer->buffer_size = buffer_size;
  lexer->current_position = 0;
  lexer->read_position = 0;
  lexer->current_char = lexer->buffer[lexer->current_position];
  return lexer;
}

void read_char(Lexer *lexer) {
  if (lexer->read_position >= lexer->buffer_size) {
    lexer->current_char = 0;
  } else {
    lexer->current_char = lexer->buffer[lexer->read_position];
  }
  lexer->current_position = lexer->read_position;
  lexer->read_position += 1;
}

void free_lexer(Lexer *lexer) {
  free(lexer->buffer);
  free(lexer);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: markdeez <file.md>\n");
    return EXIT_FAILURE;
  }
  char *file_name = argv[1];
  FILE *fp = NULL;
  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    perror("Error while opening file\n");
    return EXIT_FAILURE;
  }
  long buffer_size;
  fseek(fp, 1, SEEK_END);
  buffer_size = ftell(fp);
  rewind(fp);
  uint8_t *buffer = malloc(buffer_size + 1);
  if (buffer == NULL) {
    perror("Error while allocating memory for file\n");
    fclose(fp);
    return EXIT_FAILURE;
  }
  long result = fread(buffer, 1, buffer_size, fp);
  if (result == 0) {
    perror("Error while reading data into buffer");
    fclose(fp);
    return EXIT_FAILURE;
  }
  Lexer *lexer = new_lexer(buffer, buffer_size);
  if (lexer == NULL) {
    printf("Error while initializing lexer");
    return EXIT_FAILURE;
  }
  while (lexer->current_char != 0) {
    printf("%c", lexer->current_char);
    read_char(lexer);
  }
  free_lexer(lexer);
  return EXIT_SUCCESS;
}
