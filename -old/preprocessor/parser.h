/**
 * Reaver Project Assembler License
 *
 * Copyright (C) 2013 Reaver Project Team:
 * 1. Michał "Griwes" Dominiak
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation is required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Michał "Griwes" Dominiak
 *
 **/

#pragma once

#include <reaver/parser.h>

namespace reaver
{
    namespace assembler
    {
        struct define_match
        {
            std::string directive;
            std::string name;
            boost::optional<std::vector<std::string>> args;
        };

        struct assign_match
        {
            std::string name;
            std::string value;
        };

        struct if_match
        {
            std::string type;
            std::string condition;
        };

        struct rep_match
        {
            std::string dummy;
        };

        struct macro_call_match
        {
            std::string name;
            std::vector<std::string> args;
        };

        enum preprocessing_tokens
        {
            pp_directive,
            pp_identifier,
            pp_number,
            pp_character,
            pp_string,
            pp_symbol,
            pp_whitespace
        };

        struct preprocessor_lexer
        {
            preprocessor_lexer() : directive{ reaver::assembler::pp_directive, "%[a-z]+" },
                identifier{ reaver::assembler::pp_identifier, "\\.?[a-zA-Z_][a-zA-Z0-9_]*" },
                number{ reaver::assembler::pp_number, "(0x[0-9a-fA-F]+)|(0b[01]+)|([0-9a-fA-F]+[hH])|([0-9]*\\.[0-9]*)|([0-9]+)" },
                character{ reaver::assembler::pp_character, "'\\\\?.'" },
                string{ reaver::assembler::pp_string, "\"([^\"\\\\]*(\\.[^\"\\\\]*)*)\"" },
                symbol{ reaver::assembler::pp_symbol, "[[:punct:]]" },
                whitespace{ reaver::assembler::pp_whitespace, "[ \t\r\n\v\f]+" }
            {
                desc.add(directive)(identifier)(number)(character)(string)(symbol)(whitespace);
            }

            lexer::token_definition<std::string> directive;
            lexer::token_definition<std::string> identifier;
            lexer::token_definition<std::string> number;
            lexer::token_definition<std::string> character;
            lexer::token_definition<std::string> string;
            lexer::token_definition<std::string> symbol;
            lexer::token_definition<std::string> whitespace;

            lexer::tokens_description desc;
        };

        struct preprocessor_parser
        {
            preprocessor_parser(const preprocessor_lexer & lex)
            {
                define = parser::token(lex.directive)({ "%define", "%xdefine" }) >> parser::token(lex.identifier) >>
                    -(~parser::token(lex.symbol)({ "(" }) >> parser::token(lex.identifier) % parser::token(lex.symbol)({ "," })
                    >> ~parser::token(lex.symbol)({ ")" }));
                include = ~parser::token(lex.directive)({ "%include" }) >> parser::token(lex.string);
                undef = ~parser::token(lex.directive)({ "%undef" }) >> parser::token(lex.identifier);
                assign = ~parser::token(lex.directive)({ "%assign" }) >> parser::token(lex.identifier) >> parser::token(lex.number);
                macro = ~parser::token(lex.directive)({ "%macro" }) >> parser::token(lex.identifier);
                endmacro = parser::token(lex.directive)({ "%endmacro" });           // parser TODO: `rule<>`
                if_directive = parser::token(lex.directive)({ "%if", "%ifdef" });   // TODO
                elseif = parser::token(lex.directive)({ "%elseif", "%elif" });      // TODO
                else_directive = parser::token(lex.directive)({ "%else" });         // TODO
                rep = parser::token(lex.directive)({ "%rep" });                     // TODO

                anything_but_comma_and_parens = +((parser::token(lex.identifier) | parser::token(lex.character)
                    | parser::token(lex.number) | parser::token(lex.string) | parser::token(lex.symbol))
                    - parser::token(lex.symbol)({ ",", "(", ")" }));

                macro_call = parser::token(lex.identifier) >> ~parser::token(lex.symbol)({ "(" }) >> anything_but_comma_and_parens
                    % parser::token(lex.symbol)({ "," }) >> ~parser::token(lex.symbol)({ ")" });

                skip = parser::token(lex.whitespace);
            }

            parser::rule<reaver::assembler::define_match> define;
            parser::rule<std::string> include;
            parser::rule<std::string> undef;
            parser::rule<reaver::assembler::assign_match> assign;
            parser::rule<std::string> macro;
            parser::rule<std::string> endmacro;                 // rule<>
            parser::rule<reaver::assembler::if_match> if_directive;
            parser::rule<std::string> elseif;
            parser::rule<std::string> else_directive;           // rule<>
            parser::rule<reaver::assembler::rep_match> rep;

            parser::rule<reaver::assembler::macro_call_match> macro_call;

            parser::rule<std::string> anything_but_comma_and_parens;

            parser::rule<std::string> skip;
        };
    }
}
