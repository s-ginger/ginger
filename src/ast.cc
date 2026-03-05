#include "ast.h"
#include "core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Ast *new_bin_op_ast(Ast *left, Ast *right, char op) {
  Ast *ast_node = cast<Ast *>(malloc(sizeof(Ast)));
  if (!ast_node)
    return NULL;

  ast_node->type = AST_BINOP;
  ast_node->bin_op.l = left;
  ast_node->bin_op.r = right;
  ast_node->bin_op.op = op;

  return ast_node;
}

Ast *new_int_ast(i32 value) {
  Ast *ast_node = cast<Ast *>(malloc(sizeof(Ast)));
  if (!ast_node)
    return NULL;

  ast_node->type = AST_INT;
  ast_node->value = value;

  return ast_node;
}

Ast *new_ident_ast(const char *name, usize length) {
  Ast *node = cast<Ast *>(malloc(sizeof(Ast)));
  if (!node)
    return NULL;

  node->type = AST_IDENT;
  node->ident.name = cast<char *>(malloc(length + 1)); // выделяем память
  if (!node->ident.name) {
    free(node);
    return NULL;
  }
  memcpy(node->ident.name, name, length); // копируем содержимое
  node->ident.name[length] = '\0';        // добавляем нулевой терминатор
  node->ident.length = length;
  return node;
}

Ast *new_string_ast(const char *name, usize length) {
  Ast *node = cast<Ast *>(malloc(sizeof(Ast)));
  if (!node)
    return NULL;

  node->type = AST_STR;
  node->string_literal.value =
      cast<char *>(malloc(length + 1)); // выделяем память
  if (!node->string_literal.value) {
    free(node);
    return NULL;
  }

  memcpy(node->string_literal.value, name, length); // копируем содержимое
  node->string_literal.value[length] = '\0'; // добавляем нулевой терминатор
  node->string_literal.length = length;
  return node;
}

void free_ast(Ast *ast) {
  if (!ast)
    return;

  switch (ast->type) {
  case AST_INT:
    // ничего дополнительного
    break;

  case AST_IDENT:
    // name обычно указывает в исходный код → НЕ free
    break;

  case AST_BINOP:
    free_ast(ast->bin_op.l);
    free_ast(ast->bin_op.r);
    break;
  }

  free(ast);
}

Stmt *new_var_stmt(const char *name, usize len, Ast *value) {
  Stmt *stmt = cast<Stmt *>(malloc(sizeof(Stmt)));
  stmt->type = STMT_VAR;
  stmt->var_decl.name = name;
  stmt->var_decl.length = len;
  stmt->var_decl.value = value;
  return stmt;
}

Stmt *new_const_stmt(const char *name, usize len, Ast *value) {
  Stmt *stmt = cast<Stmt *>(malloc(sizeof(Stmt)));
  stmt->type = STMT_CONST;
  stmt->const_decl.name = name;
  stmt->const_decl.length = len;
  stmt->const_decl.value = value;
  return stmt;
}

Stmt *new_package_stmt(const char *name, usize len) {
  Stmt *stmt = cast<Stmt *>(malloc(sizeof(Stmt)));
  stmt->type = STMT_PACKAGE;
  stmt->package_decl.name = name;
  stmt->package_decl.lenght = len;
  return stmt;
}

Stmt *new_import_stmt(const char *name, usize len) {
  Stmt *stmt = cast<Stmt *>(malloc(sizeof(Stmt)));
  stmt->type = STMT_IMPORT;
  stmt->import_decl.name = name;
  stmt->import_decl.lenght = len;
  return stmt;
}

Stmt *new_assign_stmt(const char *name, usize len, Ast *value) {
  Stmt *stmt = cast<Stmt *>(malloc(sizeof(Stmt)));
  stmt->type = STMT_ASSIGN;
  stmt->assign.name = name;
  stmt->assign.length = len;
  stmt->assign.value = value;
  return stmt;
}

Stmt *new_expr_stmt(Ast *expr) {
  Stmt *s = cast<Stmt *>(malloc(sizeof(Stmt)));
  s->type = STMT_EXPR;
  s->expr_stmt.expr = expr;
  return s;
}

Stmt *new_block_stmt(Stmt **stmts, usize count) {
  Stmt *s = cast<Stmt *>(malloc(sizeof(Stmt)));
  s->type = STMT_BLOCK;
  s->block.stmts = stmts;
  s->block.count = count;
  return s;
}

void free_stmt(Stmt *stmt) {
  if (!stmt)
    return;

  switch (stmt->type) {
  case STMT_VAR:
    free_ast(stmt->var_decl.value);
    break;

  case STMT_CONST:
    free_ast(stmt->const_decl.value);
    break;

  case STMT_ASSIGN:
    free_ast(stmt->assign.value);
    break;

  case STMT_EXPR:
    free_ast(stmt->expr_stmt.expr);
    break;

  case STMT_BLOCK:
    for (usize i = 0; i < stmt->block.count; i++) {
      free_stmt(stmt->block.stmts[i]);
    }
    free(stmt->block.stmts);
    break;
  }

  free(stmt);
}

static void print_indent(i32 indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");
}

void print_ast(Ast *ast, i32 indent) {
  if (!ast)
    return;

  print_indent(indent);

  switch (ast->type) {
  case AST_INT:
    printf("Int(%d)\n", ast->value);
    break;

  case AST_STR:
    printf("String(%.*s)\n", (i32)ast->string_literal.length,
           ast->string_literal.value);
    break;
  case AST_IDENT:
    printf("Ident(%.*s)\n", (i32)ast->ident.length, ast->ident.name);
    break;

  case AST_BINOP:
    printf("BinOp('%c')\n", ast->bin_op.op);

    print_ast(ast->bin_op.l, indent + 1);
    print_ast(ast->bin_op.r, indent + 1);
    break;
  }
}

void print_stmt(Stmt *stmt, i32 indent) {
  if (!stmt)
    return;

  print_indent(indent);

  switch (stmt->type) {
  case STMT_VAR:
    printf("VarDecl(%.*s)\n", (i32)stmt->var_decl.length, stmt->var_decl.name);
    print_ast(stmt->var_decl.value, indent + 1);
    break;

  case STMT_CONST:
    printf("ConstDecl(%.*s)\n", (i32)stmt->const_decl.length,
           stmt->const_decl.name);
    print_ast(stmt->const_decl.value, indent + 1);
    break;

  case STMT_ASSIGN:
    printf("Assign(%.*s)\n", (i32)stmt->assign.length, stmt->assign.name);
    print_ast(stmt->assign.value, indent + 1);
    break;

  case STMT_PACKAGE:
    printf("Package(%.*s)\n", (i32)stmt->package_decl.lenght,
           stmt->package_decl.name);
    break;

  case STMT_IMPORT:
    printf("Import(%.*s)\n", (i32)stmt->import_decl.lenght,
           stmt->import_decl.name);
    break;

  case STMT_EXPR:
    printf("ExprStmt\n");
    print_ast(stmt->expr_stmt.expr, indent + 1);
    break;

  case STMT_BLOCK:
    printf("Block\n");
    for (usize i = 0; i < stmt->block.count; i++) {
      print_stmt(stmt->block.stmts[i], indent + 1);
    }
    break;
  }
}
