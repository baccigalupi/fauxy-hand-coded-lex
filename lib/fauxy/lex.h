#ifndef __fauxy_lex_h
#define __fauxy_lex_h

#include <ctype.h>

#include "../list.h"
#include "../dstring.h"
#include "token.h"

typedef struct LexState {
  String *code;
  int current;
  int line;
  int column;
  ClosingBookend expects_closing;
} LexState;

#define char_is_line_end(C)         (C == '\n' || C == '\r')
#define char_is_string_bookend(C)   (C == '\'' || C == '"')



#define lex_state_current(L)          ((L)->current)
#define lex_state_line(L)             ((L)->line)
#define lex_state_column(L)           ((L)->column)
#define lex_state_length(L)           (((L)->code)->length)
#define lex_state_current_char(L)     (((L)->code)->value[lex_state_current(L)])
#define lex_state_end_of_word(L)      (lex_state_next_char(L)) && isspace(lex_state_next_char(L))
#define lex_state_next_char(L)        (( lex_state_length(L) > lex_state_current(L) ) ? (((L)->code)->value[lex_state_current(L) + 1]) : '\0')
#define lex_state_advance(L)          ((++ lex_state_current(lex_state)) && (++ lex_state_column(L)))
#define lex_state_expects_closing(L)  ((L)->expects_closing)
#define lex_state_is_open(L)          ((L)->expects_closing)
#define lex_state_will_close(L, C)    (                                                                           \
                                        (L)->expects_closing &&                                                   \
                                        (                                                                         \
                                          ((((L)->expects_closing) == FX_CLOSING_SINGLE_QUOTE) && C == '\'')  ||  \
                                          ((((L)->expects_closing) == FX_CLOSING_DOUBLE_QUOTE) && C == '"')   ||  \
                                          ((((L)->expects_closing) == FX_CLOSING_BLOCK_COMMENT) && C == '*' &&    \
                                              lex_state_next_char(L) == '/')                                      \
                                        )                                                                         \
                                      )
#define lex_state_close(L)                      (                                                                         \
                                                  ((L)->expects_closing) == FX_CLOSING_BLOCK_COMMENT ?                    \
                                                    (lex_state_advance(L)) && ((L)->expects_closing = FX_CLOSING_NULL) :  \
                                                    ((L)->expects_closing = FX_CLOSING_NULL)                              \
                                                )

#define lex_state_start_new_line(L)             ((++ lex_state_line(lex_state)) && (lex_state_column(lex_state) = 0))
#define lex_state_opening_block_comment(L, C)   (C == '/' && lex_state_next_char(L) == '*')
#define lex_state_is_opening(L, C)              (char_is_string_bookend(C) || lex_state_opening_block_comment(L, C))

#define lex_state_closer(L, C)                  (                                                                                               \
                                                  C == '\'' ? FX_CLOSING_SINGLE_QUOTE :                                                         \
                                                  (C == '"'  ? FX_CLOSING_DOUBLE_QUOTE :                                                        \
                                                  (lex_state_opening_block_comment(L, C) ? FX_CLOSING_BLOCK_COMMENT : FX_CLOSING_LINE_COMMENT)  \
                                                ))

#define lex_state_opened_comment(L)             ((lex_state_expects_closing(L) == FX_CLOSING_BLOCK_COMMENT) || (lex_state_expects_closing(L) == FX_CLOSING_LINE_COMMENT))

#define lex_state_current_is_significant(L, C)  (                                                                 \
                                                  (!lex_state_opened_comment(L)) && (                             \
                                                    (C == '\n' || C == '\r' || !isspace(C)) ||                    \
                                                    (lex_state_is_open(L))                                        \
                                                  )                                                               \
                                                )
#define lex_state_in_progress(L)                (lex_state_current(lex_state) < lex_state_length(lex_state))

typedef struct Lexeme {
  String *word;
  int line;
  int column;
} Lexeme;

#define lexeme_word(L)             ((L)->word)
#define lexeme_line(L)             ((L)->line)
#define lexeme_column(L)           ((L)->column)


List      *lex(char *code);

LexState  *LexState_create(String *code);
Lexeme    *lex_get_next_lexeme(LexState *lex_state);
Token     *lexeme_to_token(Lexeme *lexeme);

Lexeme    *Lexeme_create(String *word, int line, int column);

#endif