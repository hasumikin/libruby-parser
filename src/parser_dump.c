#include <stdio.h>
#include "node.h"
#include "helper.h"
#include "parser_dump.h"

extern pm_constant_pool_t pool;

static void
print_literal(FILE *fp, VALUE lit)
{
    object_type_t type = ((Object *)lit)->type;

    switch (type) {
      case T_NUMBER:
        fprintf(fp, "(number) %ld", ((Number *)lit)->l);
        return;
      case T__STRING:
        fprintf(fp, "(string)");
        return;
      case T__HASH:
        fprintf(fp, "(hash)");
        return;
      case T_IO:
        fprintf(fp, "(io)");
        return;
      case T_ENCODING:
        fprintf(fp, "(encoding)");
        return;
      default:
        fprintf(fp, "(Unknown) %i", type);
        return;
    }
}

static void
print_indent(FILE *fp, int indent)
{
    for (int i = 0; i < indent; i++) {
        fprintf(fp, "    ");
    }
}

#define PRINT(io, str) \
    do { \
        print_indent(io, indent); \
        fprintf(io, str); \
    } while (0)

#define FPRINTF(io, fmt, ...) \
    do { \
        print_indent(io, indent); \
        fprintf(io, fmt, __VA_ARGS__); \
    } while (0)

#define PRINT_ID(io, id) \
    do { \
        if (id < POOL_ID_OFFSET) { \
            FPRINTF(io, "id: %c\n", (char)id); \
        } \
        else { \
            pm_constant_t *constant = pm_constant_pool_id_to_constant(&pool, id - POOL_ID_OFFSET); \
            FPRINTF(io, "id: %.*s\n", (int)constant->length, constant->start); \
        } \
    } while (0)

#define PRINT_STR(io, str) \
    do { \
        print_indent(io, indent); \
        fprintf(io, "str: %s\n", str->ptr); \
    } while (0)

#define PRINT_LIT(io, lit) \
    do { \
        print_indent(io, indent); \
        fprintf(io, "lit: "); \
        print_literal(io, lit); \
        fprintf(io, "\n"); \
    } while (0)

static void
print_array(FILE *fp, int indent, const NODE *node)
{
    parser_dump(RNODE_LIST(node)->nd_head, indent+1);
    while (RNODE_LIST(node)->nd_next && nd_type(RNODE_LIST(node)->nd_next) == NODE_LIST) {
        node = RNODE_LIST(node)->nd_next;
        parser_dump(RNODE_LIST(node)->nd_head, indent+1);
    }
}

void
parser_dump(const NODE *node, const int indent)
{
    enum node_type type;

    if (!node) {
        return;
    }

    type = nd_type(node);
    switch (type) {
      case NODE_SCOPE:
        PRINT(stderr, "SCOPE:\n");
        // parser_dump(RNODE_SCOPE(node)->nd_args, indent+1);
        parser_dump(RNODE_SCOPE(node)->nd_body, indent+1);
        return;
      case NODE_OPCALL:
        PRINT(stderr, "OPCALL:\n");
        PRINT_ID(stderr, RNODE_OPCALL(node)->nd_mid);
        parser_dump(RNODE_OPCALL(node)->nd_recv, indent+1);
        parser_dump(RNODE_OPCALL(node)->nd_args, indent+1);
        return;
      case NODE_LIST:
        PRINT(stderr, "LIST:\n");
        print_array(stderr, indent, node);
        return;
      case NODE_FCALL:
        PRINT(stderr, "FCALL:\n");
        PRINT_ID(stderr, RNODE_FCALL(node)->nd_mid);
        parser_dump(RNODE_FCALL(node)->nd_args, indent+1);
        return;
      case NODE_SYM:
        PRINT(stderr, "SYM:\n");
        PRINT_STR(stderr, RNODE_SYM(node)->string);
        return;
      case NODE_STR:
        PRINT(stderr, "STR:\n");
        PRINT_STR(stderr, RNODE_STR(node)->string);
        return;
      case NODE_INTEGER:
        PRINT(stderr, "INTEGER:\n");
        FPRINTF(stderr, "lit: %s\n", RNODE_INTEGER(node)->val);
        return;
      default:
        FPRINTF(stderr, "Unknown type: %i\n", type);
        return;
    }
}

