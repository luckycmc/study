/*
   +----------------------------------------------------------------------+
   | Zend Engine                                                          |
   +----------------------------------------------------------------------+
   | Copyright (c) 1998-2018 Zend Technologies Ltd. (http://www.zend.com) |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.00 of the Zend license,     |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.zend.com/license/2_00.txt.                                |
   | If you did not receive a copy of the Zend license and are unable to  |
   | obtain it through the world-wide-web, please send a note to          |
   | license@zend.com so we can mail you a copy immediately.              |
   +----------------------------------------------------------------------+
   | Authors: Bob Weinand <bwoebi@php.net>                                |
   |          Dmitry Stogov <dmitry@php.net>                              |
   |          Nikita Popov <nikic@php.net>                                |
   +----------------------------------------------------------------------+
*/

#ifndef ZEND_AST_H
#define ZEND_AST_H

#include "zend.h"

#ifndef ZEND_AST_SPEC
# define ZEND_AST_SPEC 1
#endif

#define ZEND_AST_SPECIAL_SHIFT      6
#define ZEND_AST_IS_LIST_SHIFT      7
#define ZEND_AST_NUM_CHILDREN_SHIFT 8
//抽象语法书的类型
/**
 * @brief 
 * 
 * ast 抽象语法书的类型 抽象语法树的类型
 */
enum _zend_ast_kind {
	/* special nodes */
	ZEND_AST_ZVAL = 1 << ZEND_AST_SPECIAL_SHIFT, //64 代表这个节点存放的是个zval值
	ZEND_AST_CONSTANT,   //常量 节点 65
	ZEND_AST_ZNODE,     // znode 节点 66

	/* declaration nodes */
	ZEND_AST_FUNC_DECL,  //声明函数的节点 67
	ZEND_AST_CLOSURE, //闭包函数 68
	ZEND_AST_METHOD,  // 类中的方法 69
	ZEND_AST_CLASS,  // 类节点 70   

	/* list nodes */  //要进行 zend_ast_list转换
	ZEND_AST_ARG_LIST = 1 << ZEND_AST_IS_LIST_SHIFT,
	ZEND_AST_ARRAY,        // array 数组 129
	ZEND_AST_ENCAPS_LIST,
	ZEND_AST_EXPR_LIST,   //表达式 例如 $a+1

	ZEND_AST_STMT_LIST, // 132 表达式 
	ZEND_AST_IF,        //133 if
	ZEND_AST_SWITCH_LIST, // 134
	ZEND_AST_CATCH_LIST, //135
	ZEND_AST_PARAM_LIST, // 136  表示函数的参数
	ZEND_AST_CLOSURE_USES,
	ZEND_AST_PROP_DECL,
	ZEND_AST_CONST_DECL,
	ZEND_AST_CLASS_CONST_DECL,
	ZEND_AST_NAME_LIST,
	ZEND_AST_TRAIT_ADAPTATIONS,
	ZEND_AST_USE,

	/* 0 child nodes */
	ZEND_AST_MAGIC_CONST = 0 << ZEND_AST_NUM_CHILDREN_SHIFT,
	ZEND_AST_TYPE,
	ZEND_AST_CONSTANT_CLASS,

	/* 1 child node */
	ZEND_AST_VAR = 1 << ZEND_AST_NUM_CHILDREN_SHIFT,  //变量 代表该节点是个变量 256
	ZEND_AST_CONST,     // 常量257 
	ZEND_AST_UNPACK,
	ZEND_AST_UNARY_PLUS,
	ZEND_AST_UNARY_MINUS,
	ZEND_AST_CAST,
	ZEND_AST_EMPTY,
	ZEND_AST_ISSET,   // isset 函数 263
	ZEND_AST_SILENCE,
	ZEND_AST_SHELL_EXEC,
	ZEND_AST_CLONE,
	ZEND_AST_EXIT,
	ZEND_AST_PRINT,
	ZEND_AST_INCLUDE_OR_EVAL,
	ZEND_AST_UNARY_OP,  // 270
	ZEND_AST_PRE_INC,   
	ZEND_AST_PRE_DEC,    // 272
	ZEND_AST_POST_INC,  // 273    // while的变量自增
	ZEND_AST_POST_DEC,  // 274    // while自减 
	ZEND_AST_YIELD_FROM, // 275

	ZEND_AST_GLOBAL,  //276
	ZEND_AST_UNSET,   //277
	ZEND_AST_RETURN,  //278
	ZEND_AST_LABEL,    // 279
	ZEND_AST_REF,     //280
	ZEND_AST_HALT_COMPILER, // 281
	ZEND_AST_ECHO,  // 282 echo  一个子女
	ZEND_AST_THROW,
	ZEND_AST_GOTO,
	ZEND_AST_BREAK,
	ZEND_AST_CONTINUE,

	/* 2 child nodes */  // 有两个子节点的可能会有一个没有
	ZEND_AST_DIM = 2 << ZEND_AST_NUM_CHILDREN_SHIFT,
	ZEND_AST_PROP,
	ZEND_AST_STATIC_PROP,
	ZEND_AST_CALL,
	ZEND_AST_CLASS_CONST,
	ZEND_AST_ASSIGN,        // 517 赋值操作 一般情况下赋值比较多点
	ZEND_AST_ASSIGN_REF,
	ZEND_AST_ASSIGN_OP,
	ZEND_AST_BINARY_OP,   // 表达式 比较
	ZEND_AST_GREATER,     // 521 比较
	ZEND_AST_GREATER_EQUAL,
	ZEND_AST_AND,
	ZEND_AST_OR,
	ZEND_AST_ARRAY_ELEM,  // 525 array 中的元素
	ZEND_AST_NEW,         // 526
	ZEND_AST_INSTANCEOF,
	ZEND_AST_YIELD,
	ZEND_AST_COALESCE,

	ZEND_AST_STATIC,
	ZEND_AST_WHILE,     // while 531
	ZEND_AST_DO_WHILE,  // do while 532
	ZEND_AST_IF_ELEM,   // if
	ZEND_AST_SWITCH,    //switch
	ZEND_AST_SWITCH_CASE, 
	ZEND_AST_DECLARE,
	ZEND_AST_USE_TRAIT,
	ZEND_AST_TRAIT_PRECEDENCE,
	ZEND_AST_METHOD_REFERENCE,
	ZEND_AST_NAMESPACE,
	ZEND_AST_USE_ELEM,
	ZEND_AST_TRAIT_ALIAS,
	ZEND_AST_GROUP_USE,

	/* 3 child nodes */
	ZEND_AST_METHOD_CALL = 3 << ZEND_AST_NUM_CHILDREN_SHIFT,
	ZEND_AST_STATIC_CALL,
	ZEND_AST_CONDITIONAL,

	ZEND_AST_TRY,
	ZEND_AST_CATCH,
	ZEND_AST_PARAM,          // 函数的参数 773
	ZEND_AST_PROP_ELEM,
	ZEND_AST_CONST_ELEM,

	/* 4 child nodes */ //for 和foreach 都是4个子女
	ZEND_AST_FOR = 4 << ZEND_AST_NUM_CHILDREN_SHIFT,
	ZEND_AST_FOREACH,  // 1025 foreach
};

typedef uint16_t zend_ast_kind;
typedef uint16_t zend_ast_attr;

struct _zend_ast {
	zend_ast_kind kind; /* Type of the node (ZEND_AST_* enum constant) */
	zend_ast_attr attr; /* Additional attribute, use depending on node type */
	uint32_t lineno;    /* Line number */
	zend_ast *child[1]; /* Array of children (using struct hack) */
};

/* Same as zend_ast, but with children count, which is updated dynamically */
typedef struct _zend_ast_list {
	zend_ast_kind kind;        // kind标识的是ast节点类型
	zend_ast_attr attr;
	uint32_t lineno;
	uint32_t children;
	zend_ast *child[1];
} zend_ast_list;

/* Lineno is stored in val.u2.lineno */
typedef struct _zend_ast_zval {
	zend_ast_kind kind;
	zend_ast_attr attr;
	zval val;
} zend_ast_zval;

/* Separate structure for function and class declaration, as they need extra information. 
    另外函数、类还会用到另外一种ast节点结构：
 */
typedef struct _zend_ast_decl {
	zend_ast_kind kind;
	zend_ast_attr attr; /* Unused - for structure compatibility */
	uint32_t start_lineno;
	uint32_t end_lineno;
	uint32_t flags;
	unsigned char *lex_pos;
	zend_string *doc_comment;
	zend_string *name;
	zend_ast *child[4];
} zend_ast_decl;

typedef void (*zend_ast_process_t)(zend_ast *ast);
extern ZEND_API zend_ast_process_t zend_ast_process;

ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_zval_with_lineno(zval *zv, uint32_t lineno);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_zval_ex(zval *zv, zend_ast_attr attr);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_zval(zval *zv);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_zval_from_str(zend_string *str);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_zval_from_long(zend_long lval);

ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_constant(zend_string *name, zend_ast_attr attr);

#if ZEND_AST_SPEC
# define ZEND_AST_SPEC_CALL(name, ...) \
	ZEND_EXPAND_VA(ZEND_AST_SPEC_CALL_(name, __VA_ARGS__, _4, _3, _2, _1, _0)(__VA_ARGS__))
# define ZEND_AST_SPEC_CALL_(name, _, _4, _3, _2, _1, suffix, ...) \
	name ## suffix
# define ZEND_AST_SPEC_CALL_EX(name, ...) \
	ZEND_EXPAND_VA(ZEND_AST_SPEC_CALL_EX_(name, __VA_ARGS__, _4, _3, _2, _1, _0)(__VA_ARGS__))
# define ZEND_AST_SPEC_CALL_EX_(name, _, _5, _4, _3, _2, _1, suffix, ...) \
	name ## suffix

ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_0(zend_ast_kind kind);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_1(zend_ast_kind kind, zend_ast *child);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_2(zend_ast_kind kind, zend_ast *child1, zend_ast *child2);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_3(zend_ast_kind kind, zend_ast *child1, zend_ast *child2, zend_ast *child3);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_4(zend_ast_kind kind, zend_ast *child1, zend_ast *child2, zend_ast *child3, zend_ast *child4);

static zend_always_inline zend_ast * zend_ast_create_ex_0(zend_ast_kind kind, zend_ast_attr attr) {
	zend_ast *ast = zend_ast_create_0(kind);
	ast->attr = attr;
	return ast;
}
static zend_always_inline zend_ast * zend_ast_create_ex_1(zend_ast_kind kind, zend_ast_attr attr, zend_ast *child) {
	zend_ast *ast = zend_ast_create_1(kind, child);
	ast->attr = attr;
	return ast;
}
static zend_always_inline zend_ast * zend_ast_create_ex_2(zend_ast_kind kind, zend_ast_attr attr, zend_ast *child1, zend_ast *child2) {
	zend_ast *ast = zend_ast_create_2(kind, child1, child2);
	ast->attr = attr;
	return ast;
}
static zend_always_inline zend_ast * zend_ast_create_ex_3(zend_ast_kind kind, zend_ast_attr attr, zend_ast *child1, zend_ast *child2, zend_ast *child3) {
	zend_ast *ast = zend_ast_create_3(kind, child1, child2, child3);
	ast->attr = attr;
	return ast;
}
static zend_always_inline zend_ast * zend_ast_create_ex_4(zend_ast_kind kind, zend_ast_attr attr, zend_ast *child1, zend_ast *child2, zend_ast *child3, zend_ast *child4) {
	zend_ast *ast = zend_ast_create_4(kind, child1, child2, child3, child4);
	ast->attr = attr;
	return ast;
}

ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_list_0(zend_ast_kind kind);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_list_1(zend_ast_kind kind, zend_ast *child);
ZEND_API zend_ast * ZEND_FASTCALL zend_ast_create_list_2(zend_ast_kind kind, zend_ast *child1, zend_ast *child2);

# define zend_ast_create(...) \
	ZEND_AST_SPEC_CALL(zend_ast_create, __VA_ARGS__)
# define zend_ast_create_ex(...) \
	ZEND_AST_SPEC_CALL_EX(zend_ast_create_ex, __VA_ARGS__)
# define zend_ast_create_list(init_children, ...) \
	ZEND_AST_SPEC_CALL(zend_ast_create_list, __VA_ARGS__)

#else
ZEND_API zend_ast *zend_ast_create(zend_ast_kind kind, ...);
ZEND_API zend_ast *zend_ast_create_ex(zend_ast_kind kind, zend_ast_attr attr, ...);
ZEND_API zend_ast *zend_ast_create_list(uint32_t init_children, zend_ast_kind kind, ...);
#endif

ZEND_API zend_ast * ZEND_FASTCALL zend_ast_list_add(zend_ast *list, zend_ast *op);

ZEND_API zend_ast *zend_ast_create_decl(
	zend_ast_kind kind, uint32_t flags, uint32_t start_lineno, zend_string *doc_comment,
	zend_string *name, zend_ast *child0, zend_ast *child1, zend_ast *child2, zend_ast *child3
);

ZEND_API int ZEND_FASTCALL zend_ast_evaluate(zval *result, zend_ast *ast, zend_class_entry *scope);
ZEND_API zend_string *zend_ast_export(const char *prefix, zend_ast *ast, const char *suffix);

ZEND_API zend_ast_ref * ZEND_FASTCALL zend_ast_copy(zend_ast *ast);
ZEND_API void ZEND_FASTCALL zend_ast_destroy(zend_ast *ast);
ZEND_API void ZEND_FASTCALL zend_ast_ref_destroy(zend_ast_ref *ast);

typedef void (*zend_ast_apply_func)(zend_ast **ast_ptr);
ZEND_API void zend_ast_apply(zend_ast *ast, zend_ast_apply_func fn);

static zend_always_inline zend_bool zend_ast_is_list(zend_ast *ast) {
	return (ast->kind >> ZEND_AST_IS_LIST_SHIFT) & 1;
}
static zend_always_inline zend_ast_list *zend_ast_get_list(zend_ast *ast) {
	ZEND_ASSERT(zend_ast_is_list(ast));
	return (zend_ast_list *) ast;
}
//获取ast 中的zval 值的地址
static zend_always_inline zval *zend_ast_get_zval(zend_ast *ast) {
	ZEND_ASSERT(ast->kind == ZEND_AST_ZVAL);
	return &((zend_ast_zval *) ast)->val;
}
static zend_always_inline zend_string *zend_ast_get_str(zend_ast *ast) {
	zval *zv = zend_ast_get_zval(ast);
	ZEND_ASSERT(Z_TYPE_P(zv) == IS_STRING);
	return Z_STR_P(zv);
}

static zend_always_inline zend_string *zend_ast_get_constant_name(zend_ast *ast) {
	ZEND_ASSERT(ast->kind == ZEND_AST_CONSTANT);
	ZEND_ASSERT(Z_TYPE(((zend_ast_zval *) ast)->val) == IS_STRING);
	return Z_STR(((zend_ast_zval *) ast)->val);
}

static zend_always_inline uint32_t zend_ast_get_num_children(zend_ast *ast) {
	ZEND_ASSERT(!zend_ast_is_list(ast));
	return ast->kind >> ZEND_AST_NUM_CHILDREN_SHIFT;
}
static zend_always_inline uint32_t zend_ast_get_lineno(zend_ast *ast) {
	if (ast->kind == ZEND_AST_ZVAL) {
		zval *zv = zend_ast_get_zval(ast);
		return Z_LINENO_P(zv);
	} else {
		return ast->lineno;
	}
}

static zend_always_inline zend_ast *zend_ast_create_binary_op(uint32_t opcode, zend_ast *op0, zend_ast *op1) {
	return zend_ast_create_ex(ZEND_AST_BINARY_OP, opcode, op0, op1);
}
static zend_always_inline zend_ast *zend_ast_create_assign_op(uint32_t opcode, zend_ast *op0, zend_ast *op1) {
	return zend_ast_create_ex(ZEND_AST_ASSIGN_OP, opcode, op0, op1);
}
static zend_always_inline zend_ast *zend_ast_create_cast(uint32_t type, zend_ast *op0) {
	return zend_ast_create_ex(ZEND_AST_CAST, type, op0);
}
static zend_always_inline zend_ast *zend_ast_list_rtrim(zend_ast *ast) {
	zend_ast_list *list = zend_ast_get_list(ast);
	if (list->children && list->child[list->children - 1] == NULL) {
		list->children--;
	}
	return ast;
}
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
