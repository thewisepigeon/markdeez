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

void read_char(Lexer *lexer) {
  if (lexer->read_position >= lexer->buffer_size) {
    lexer->current_char = 0;
    lexer->read_position = EOF;
    return;
  }
  lexer->current_char = lexer->buffer[lexer->read_position];
  lexer->current_position = lexer->read_position;
  lexer->read_position += 1;
}

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
  read_char(lexer);
  return lexer;
}

uint8_t peek_char_n(Lexer *lexer, int n) {
  if (lexer->read_position + n == EOF) {
    return 0;
  }
  return lexer->buffer[lexer->read_position + n];
}

void free_lexer(Lexer *lexer) {
  free(lexer->buffer);
  free(lexer);
}

void skip_white_space(Lexer *lexer) {
  for (; lexer->current_char == ' ' || lexer->current_char == '\t' ||
         lexer->current_char == '\n' || lexer->current_char == '\r';) {
    read_char(lexer);
  }
}

int line_is_header(Lexer *lexer) {
  uint8_t ch;
  int i = 0;
  while (1) {
    if ((ch = peek_char_n(lexer, i)) != '#') {
      if (ch == ' ') {
        return i + 1;
      }
      i = 0;
      break;
    }
    i++;
  }
  return i;
}

uint8_t *parse_line(Lexer *lexer) {
  printf("Parsing line\n");
  return NULL;
}

uint8_t *parse_heading(Lexer *lexer) {
  printf("Parsing header\n");
  int8_t heading_level = 0;
  for (; lexer->current_char == '#';) {
    heading_level += 1;
    read_char(lexer);
  }
  return NULL;
}

uint8_t *parse_paragraph(Lexer *lexer) { return NULL; }

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
  if (result < buffer_size) {
    if (feof(fp)) {
      fprintf(stderr, "End of file reached, no data read\n");
    } else if (ferror(fp)) {
      perror("Error while reading data into buffer");
    }
    fclose(fp);
    return EXIT_FAILURE;
  }
  Lexer *lexer = new_lexer(buffer, buffer_size);
  if (lexer == NULL) {
    printf("Error while initializing lexer");
    return EXIT_FAILURE;
  }
  while (lexer->current_char != 0) {
    skip_white_space(lexer);
    if (lexer->current_char == '#') {
      if (line_is_header(lexer)) {
        parse_heading(lexer);
      } else {
        parse_line(lexer);
      }
    }
    read_char(lexer);
  }
  free_lexer(lexer);
  return EXIT_SUCCESS;
}
