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

#include <iostream>
#include <vector>

#include <reaver/logger.h>
#include <reaver/style.h>

inline void print_include_chain(const std::vector<std::pair<std::string, uint64_t>> & include_chain)
{
    reaver::logger::log() << "In file " << reaver::style::style(reaver::style::colors::bgray, reaver::style::colors::def,
        reaver::style::styles::bold) << include_chain.back().first << reaver::style::style() << " in line "
        << include_chain.back().second << ":";

    for (auto it = include_chain.crbegin() + 1; it != include_chain.crend(); ++it)
    {
        if (it->second != (uint64_t)-1)
        {
            reaver::logger::log() << "Included from " << it->first << " in line " << it->second << ":";
        }

        else
        {
            reaver::logger::log() << "In expanded preprocessor directive `" << it->first << "`:";
        }
    }
}

inline std::string remove_leading_whitespace(const std::string & str)
{
    auto it = begin(str);

    while (it != end(str) && std::isspace(*it))
    {
        ++it;
    }

    return { it, end(str) };
}