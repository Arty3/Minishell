#ifndef PARSER_H

# define PARSER_H

# include "common.h"

t_parser			*parser_create(t_lexer *lexer);
t_program			*parser_parse(t_parser *parser);
void				parser_set_error(t_parser *parser, const char *message);
void				parser_destroy(t_parser *parser);

t_program			*parser_parse_program(t_parser *parser);
void				parser_destroy_program(t_program *program);

t_simple_command	*parser_parse_simple_command(t_parser *parser);
void				parser_destroy_simple_command(t_simple_command *command);

t_redir				*parser_parse_redir(t_parser *parser);
void				parser_destroy_redir(t_redir *redir);

t_pipeline			*parser_parse_pipeline(t_parser *parser);
void				parser_destroy_pipeline(t_pipeline *pipeline);

t_list				*parser_parse_and_or_list(t_parser *parser);
void				parser_destroy_and_or_list(t_list **and_or_list);
t_and_or			*parser_parse_and_or(t_parser *parser);
void				parser_destroy_and_or(t_and_or *and_or);

t_command			*parser_parse_command(t_parser *parser);
void				parser_destroy_command(t_command *command);

t_subshell			*parser_parse_subshell(t_parser *parser);
void				parser_destroy_subshell(t_subshell *subshell);

void				parser_skip_linebreaks(t_parser *parser);

#endif
