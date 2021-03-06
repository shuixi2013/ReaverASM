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

#include <iostream>

#include <assembler.h>
#include <preprocessor/preprocessor.h>
#include <parser/grammar.h>
#include <output/section.h>
#include <output/elf.h>

reaver::assembler::assembler::assembler(int argc, char ** argv) : _frontend(argc, argv)
{
    _buffer = _frontend.read_file();

    reaver::assembler::preprocessor preprocessor{ _frontend };
    _lines = preprocessor.preprocess(_buffer);

    if (_frontend.preprocess_only())
    {
        for (auto & x : _lines)
        {
            _frontend.output() << *x << std::endl;
        }
    }

    else
    {
        reaver::assembler::lexer lex;
        reaver::assembler::parser parser{ lex };
        _ast = parser.parse(_lines);

        _sections = _ast.assemble(_frontend);

        elf_output output; // temporarily here, until proper output mode switching is in place
        output.output(_frontend, _sections, _ast.externs(), _ast.globals(), _frontend.output());
    }
}

reaver::assembler::assembler::~assembler()
{
}
