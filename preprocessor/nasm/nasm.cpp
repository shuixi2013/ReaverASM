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

#include <reaver/exception.h>
#include <reaver/lexer.h>

#include <preprocessor/nasm/nasm.h>
#include <preprocessor/nasm/lexer.h>
#include <preprocessor/nasm/parser.h>

std::vector<reaver::assembler::line> reaver::assembler::nasm_preprocessor::operator()() const
{
    std::vector<line> ret;

    if (_front.default_includes().size())
    {
        auto cmdline_inc = std::make_shared<utils::include_chain>("<command line>");

        for (const auto & x : _front.default_includes())
        {
            _include_stream(x.stream, ret, std::make_shared<utils::include_chain>(x.name, cmdline_inc));
        }
    }

    _include_stream(_front.input(), ret, std::make_shared<utils::include_chain>(_front.input_name()));

    return ret;
}

void reaver::assembler::nasm_preprocessor::_include_stream(const std::istream &, std::vector<reaver::assembler::line> &,
    std::shared_ptr<reaver::assembler::utils::include_chain>) const
{

}
