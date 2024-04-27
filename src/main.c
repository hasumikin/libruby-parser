#include <stdio.h>
#include <stdlib.h>
#include "ruby.h"
#include "helper.h"
#include "parser_dump.h"
//#include "parser_node_dump.h"

static char *
read_file(const char *path)
{
  FILE *fp = fopen(path, "rb");
  if (!fp) {
    printf("cannot open file\n");
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *source = malloc(fsize + 1);
  if (fread(source, fsize, 1, fp) != 1) {
    printf("cannot read file\n");
    return NULL;
  }
  fclose(fp);
  source[fsize] = 0;
  return source;
}

int
main(int argc, char **argv)
{
  rb_parser_config_t *config;
  rb_parser_t *parser;
  rb_ast_t *ast;
  VALUE str;

  ruby_init();

  if (argc < 2) {
    printf("Usage: %s <file>\n", argv[0]);
    return 1;
  }
  char *script = read_file(argv[1]);
  if (script == NULL) {
    printf("Failed to read file\n");
    return 1;
  }
  str = (VALUE)string_new_with_str(script);

  config = (rb_parser_config_t *)malloc(sizeof(rb_parser_config_t));
  parser_config_initialize(config);
  parser = rb_ruby_parser_new(config);
  ast = rb_ruby_parser_compile_string(parser, "name", str, 0);
  parser_dump(ast->body.root, 0);
  //rb_parser_dump_tree(ast->body.root, 0);

  ruby_finalize();

  return 0;
}
