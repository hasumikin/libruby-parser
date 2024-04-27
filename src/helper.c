#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helper.h"

#define Qfalse 0x00
#define Qnil   0x04
#define Qtrue  0x14
#define Qundef 0x24

#define rb_encoding void
#define OnigCodePoint unsigned int

ID next_id = 0;

#define POOL_CAPACITY 1024

pm_constant_pool_t pool;

static Number *
number_new(void)
{
    Number *number;

    number = (Number *)xmalloc(sizeof(Number));
    number->type = T_NUMBER;

    return number;
}

static Number *
number_new_with_long(long l)
{
    Number *number = number_new();
    number->l = l;

    return number;
}

static String *
string_new(void)
{
    String *string;

    string = (String *)xmalloc(sizeof(String));
    string->type = T__STRING;

    return string;
}

String *
string_new_with_str_len(const char *ptr, long len)
{
    String *string = string_new();
    string->s = xmalloc(sizeof(char) * len);
    if (ptr)
      memcpy(string->s, ptr, len);
    string->len = len;

    return string;
}

String *
string_new_with_str(const char *ptr)
{
    return string_new_with_str_len(ptr, strlen(ptr));
}

static Hash *
hash_new(void)
{
    Hash *hash;

    hash = (Hash *)xmalloc(sizeof(Hash));
    hash->type = T__HASH;

    return hash;
}

static IO *
io_new(void)
{
    IO *io;

    io = (IO *)xmalloc(sizeof(IO));
    io->type = T_IO;

    return io;
}

static Encoding *
encoding_new(void)
{
    Encoding *encoding;

    encoding = (Encoding *)xmalloc(sizeof(Encoding));
    encoding->type = T_ENCODING;

    return encoding;
}

static VALUE
debug_output_stdout(void)
{
    return (VALUE)io_new();
}

static rb_encoding *
utf8_encoding(void)
{
    return (rb_encoding *)encoding_new();
}

static VALUE
filesystem_str_new_cstr(const char *ptr)
{
    return (VALUE)string_new_with_str(ptr);
}

static rb_encoding *
enc_get(VALUE obj)
{
    return utf8_encoding();
}

static int
enc_asciicompat(rb_encoding *enc)
{
    return 1;
}

static VALUE
str_new_frozen(VALUE orig)
{
    return orig;
}

static int
nil_p(VALUE obj)
{
    return obj == Qnil;
}

static VALUE
fstring(VALUE obj)
{
    return obj;
}

static char *
string_value_cstr(volatile VALUE *ptr)
{
    return ((String *)*ptr)->s;
}

static rb_ast_t *
ast_new(VALUE nb)
{
    rb_ast_t *ast;

    ast = (rb_ast_t *)xmalloc(sizeof(rb_ast_t));
    ast->node_buffer = (node_buffer_t *)nb;

    return ast;
}

static VALUE
compile_callback(VALUE (*func)(VALUE), VALUE arg)
{
    return func(arg);
}

static char *
rstring_ptr(VALUE str)
{
    if(str<10000) return "dummy";
    return ((String *)str)->s;
}

static long
rstring_len(VALUE str)
{
    if(str<10000) return 0;
    return ((String *)str)->len;
}

static VALUE
str_subseq(VALUE str, long beg, long len)
{
    return (VALUE)string_new_with_str_len(rstring_ptr(str) + beg, len);
}

static int
obj_frozen(VALUE obj)
{
    return 0;
}

static VALUE
verbose(void)
{
    return Qfalse;
}

static int
rtest(VALUE obj)
{
    return obj != Qfalse;
}

static void *
alloc_n(size_t nelems, size_t elemsiz)
{
    return xmalloc(nelems * elemsiz);
}

/* defiened in <ctype.h> included by prism/include/prism/defines.h
static int
isupper(int c)
{
    return 'A' <= c && c <= 'Z';
}

static int
islower(int c)
{
    return 'a' <= c && c <= 'z';
}

static int
isdigit(int c)
{
    return '0' <= c && c <= '9';
}

static int
isalpha(int c)
{
    return isupper(c) || islower(c);
}

static int
isalnum(int c)
{
    return isalpha(c) || isdigit(c);
}
*/

static int
enc_isalnum(OnigCodePoint c, rb_encoding *enc)
{
    return isalnum(c);
}

static int
enc_precise_mbclen(const char *p, const char *e, rb_encoding *enc)
{
    return 1;
}

static int
mbclen_charfound_p(int len)
{
    return 1;
}

static ID
id_new(void)
{
    ID id = next_id;
    next_id++;
    return id;
}

static int
is_local_id(ID id)
{
    return 0;
}

static VALUE
cstr_to_inum(const char *str, int base, int badcheck)
{
    return (VALUE)number_new_with_long((long)atoi(str));
}

static VALUE
obj_written(VALUE old, VALUE slot, VALUE young)
{
    return old;
}

static void
sized_xfree(void *x, size_t size)
{
    xfree(x);
}

static VALUE
ident_hash_new(void)
{
    return (VALUE)hash_new();
}

static VALUE
obj_hide(VALUE obj)
{
    return obj;
}

static VALUE
sym_intern_ascii_cstr(const char *ptr)
{
    return id_new();
}

static VALUE
hash_aset(VALUE hash, VALUE key, VALUE val)
{
    return val;
}

static VALUE
obj_write(VALUE old, VALUE *slot, VALUE young)
{
    return old;
}

static VALUE
int2fix(long i)
{
    return (VALUE)number_new_with_long(i);
}

static VALUE
str_to_interned_str(VALUE str)
{
  return str;
}

static VALUE
id2sym(ID id)
{
    return (VALUE)id;
}

static VALUE
enc_str_new(const char *ptr, long len, rb_encoding *enc)
{
    return (VALUE)string_new_with_str_len(ptr, len);
}

static VALUE
id2str(ID id)
{
    pm_constant_t *constant = pm_constant_pool_id_to_constant(&pool, id);
    return (VALUE)enc_str_new((const char *)constant->start, constant->length, NULL) - POOL_ID_OFFSET;
}

static int
is_ascii_string(VALUE str)
{
    return 1;
}

static void *
my_zalloc(size_t size)
{
    return xcalloc(size, 1);
}

static VALUE
io_flush(VALUE io)
{
    return Qnil;
}

static VALUE
syntax_error_append(VALUE error_buffor, VALUE source_file_string, int line, int column, rb_encoding *enc, const char *fmt, va_list args)
{
    return error_buffor;
}

static VALUE
syntax_error_new(void)
{
    return (VALUE)string_new_with_str("SyntaxError");
}

static void
set_errinfo(VALUE error)
{
}

static VALUE
str_buf_new(long capa)
{
    return (VALUE)string_new_with_str_len(NULL, capa);
}

static VALUE
str_buf_cat(VALUE dst, const char *src, long srclen)
{
    String *string = (String *)dst;
    string->s = realloc(string->s, string->len + srclen);
    memcpy(string->s + string->len, src, srclen);
    string->len += srclen;
    return dst;
}

static VALUE
enc_associate(VALUE str, rb_encoding *enc)
{
    return str;
}

static int
id_type(ID id)
{
    return 0;
}

static ID
intern3(const char *name, long len, rb_encoding *enc)
{
    (void)enc;
    /*
     * FIXME: memory leak
     * 考察：たとえば、parse.yのtokenize_ident()のTOK_INTERN()がtok(p)
     * ではなくp->lex.ptokをつかえば、xmallocせずに済むかもしれない。
     */
    char *str = (char *)xmalloc(len + 1);
    memcpy(str, name, len);
    str[len] = '\0';
    return pm_constant_pool_insert_constant(&pool, (const uint8_t *)str, len) + POOL_ID_OFFSET;
}

static ID
intern2(const char *name, long len)
{
    return intern3(name, len, NULL);
}

static ID
intern(const char *name)
{
    return intern2(name, strlen(name));
}

static char *
enc_prev_char(const char *s, const char *p, const char *e, rb_encoding *enc)
{
    return (char *)p;
}

static int
stderr_tty_p(void)
{
    return 0;
}

static VALUE
str_catf(VALUE str, const char *fmt, ...)
{
    va_list args;
    char *buf;
    int len;

    va_start(args, fmt);
    len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    buf = xmalloc(len + 1);
    va_start(args, fmt);
    vsnprintf(buf, len + 1, fmt, args);
    va_end(args);

    str_buf_cat(str, buf, len);
    xfree(buf);

    return str;
}

static VALUE
write_error_str(VALUE str)
{
    return Qnil;
}

static VALUE
node_case_when_optimizable_literal(const NODE *const node)
{
    //switch (nd_type(node)) {
    //  case NODE_LIT: {
    //    VALUE v = RNODE_LIT(node)->nd_lit;
    //    if (SYMBOL_P(v)) {
    //        return v;
    //    }
    //    break;
    //  }
    //  case NODE_INTEGER:
    //    return rb_node_integer_literal_val(node);
    //  case NODE_FLOAT: {
    //    VALUE v = rb_node_float_literal_val(node);
    //    double ival;

    //    if (modf(RFLOAT_VALUE(v), &ival) == 0.0) {
    //        return FIXABLE(ival) ? LONG2FIX((long)ival) : rb_dbl2big(ival);
    //    }
    //    return v;
    //  }
    //  case NODE_RATIONAL:
    //  case NODE_IMAGINARY:
    //    return Qundef;
    //  case NODE_NIL:
    //    return Qnil;
    //  case NODE_TRUE:
    //    return Qtrue;
    //  case NODE_FALSE:
    //    return Qfalse;
    //  case NODE_SYM:
    //    return rb_node_sym_string_val(node);
    //  case NODE_LINE:
    //    return rb_node_line_lineno_val(node);
    //  case NODE_STR:
    //    return rb_fstring(rb_node_str_string_val(node));
    //  case NODE_FILE:
    //    return rb_fstring(rb_node_file_path_val(node));
    //}
    return Qundef;
}

static void *
xmalloc_mul_add(size_t x, size_t y, size_t z)
{
  //TODO: check overflow
    return xmalloc(x * y + z);
}

static bool
undef_p(VALUE obj)
{
    return obj == Qundef;
}

static void
hash_clear(VALUE hash)
{
    //st_clear(RHASH_TBL(hash));
}

static void *
nonempty_memcpy(void *dst, const void *src, size_t n)
{
    if (n > 0) {
        return memcpy(dst, src, n);
    }
    return dst;
}

static VALUE
str_new_cstr(const char *ptr)
{
    return (VALUE)string_new_with_str(ptr);
}

static int
long2int(long l)
{
    return (int)l;
}

void
parser_config_initialize(rb_parser_config_t *config)
{
    config->calloc = xcalloc;
    config->malloc = xmalloc;
    config->alloc = xmalloc;
    config->alloc_n = alloc_n;
    config->sized_xfree = sized_xfree;

    config->qnil = Qnil;
//    config->qtrue = Qtrue;
    config->qfalse = Qfalse;
//    config->qundef = Qundef;

    config->debug_output_stdout = debug_output_stdout;
    config->debug_output_stderr = debug_output_stdout;
    // config->io_write = ;
    // config->io_flush = ;
    config->utf8_encoding = utf8_encoding;
    config->filesystem_str_new_cstr = filesystem_str_new_cstr;

    config->str_to_interned_str = str_to_interned_str;

    config->enc_get = enc_get;
    config->enc_asciicompat = enc_asciicompat;
    config->str_new_frozen = str_new_frozen;
    config->nil_p = nil_p;
//    config->fstring = fstring;
    config->string_value_cstr = string_value_cstr;
    config->ast_new = ast_new;
    config->compile_callback = compile_callback;
    config->rstring_ptr = rstring_ptr;
    config->rstring_len = rstring_len;
    config->str_subseq = str_subseq;
//    config->obj_frozen = obj_frozen;
    config->verbose = verbose;
    config->rtest = rtest;
    config->enc_isalnum = enc_isalnum;
    config->enc_precise_mbclen = enc_precise_mbclen;
    config->mbclen_charfound_p = mbclen_charfound_p;
    config->intern3 = intern3;
    config->is_local_id = is_local_id;
//    config->cstr_to_inum = cstr_to_inum;
//    config->obj_written = obj_written;
//    config->ident_hash_new = ident_hash_new;
//    config->obj_hide = obj_hide;
//    config->sym_intern_ascii_cstr = sym_intern_ascii_cstr;
//    config->hash_aset = hash_aset;
//    config->obj_write = obj_write;
//    config->int2fix = int2fix;

    // added by hasumi
    config->id2sym = id2sym;
    config->id2str = id2str;
    config->enc_str_new = enc_str_new;
    config->is_ascii_string = is_ascii_string;
    config->zalloc = my_zalloc;
    config->free = xfree;
    config->io_flush = io_flush;
    config->syntax_error_append = syntax_error_append;
    config->syntax_error_new = syntax_error_new;
    config->set_errinfo = set_errinfo;
//    config->str_buf_new = str_buf_new;
//    config->str_buf_cat = str_buf_cat;
    config->enc_associate = enc_associate;
    config->id_type = id_type;
    config->intern2 = intern2;
    config->enc_prev_char = enc_prev_char;
    config->stderr_tty_p = stderr_tty_p;
    config->str_catf = str_catf;
    config->write_error_str = write_error_str;
//    config->node_case_when_optimizable_literal = node_case_when_optimizable_literal;
//    config->undef_p = undef_p;
//    config->hash_clear = hash_clear;
    config->nonempty_memcpy = nonempty_memcpy;
    // needed by node.c
    config->xmalloc_mul_add = xmalloc_mul_add;
    // needed by node_dump.c
    config->str_new_cstr = str_new_cstr;
    config->long2int = long2int;

    if (!pm_constant_pool_init(&pool, POOL_CAPACITY)) {
      fprintf(stderr, "pm_constant_pool_init failed\n");
    }
}

