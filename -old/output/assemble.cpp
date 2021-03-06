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

#include <array>
#include <iomanip>

#include <parser/ast.h>
#include <output/section.h>
#include <cpu/generator.h>

std::map<std::string, reaver::assembler::section> reaver::assembler::ast::assemble(const reaver::assembler::frontend &) const
{
    std::map<std::string, reaver::assembler::section> ret;

    ret.emplace(std::make_pair(".text", section{ ".text", *this }));
    std::string section = ".text";

    pmode_generator gen16{ false };
    pmode_generator gen32{};
    lmode_generator gen64{};

    std::array<std::reference_wrapper<reaver::assembler::generator>, 3> generators = {{ gen16, gen32, gen64 }};
    std::reference_wrapper<reaver::assembler::generator> & generator = generators[2];

    for (uint64_t i = 0; i < _lines.size(); ++i)
    {
        try
        {
            if (_bitness_changes.find(i) != _bitness_changes.end())
            {
                generator = generators[_bitness_changes.at(i) >> 5];
            }

            if (_sections.find(i) != _sections.end())
            {
                section = _sections.at(i);

                if (ret.find(section) == ret.end())
                {
                    ret.emplace(std::make_pair(section, reaver::assembler::section{ section, *this }));
                }
            }

            if (_labels.find(i) != _labels.end())
            {
                ret.at(section).push(_labels.at(i));
            }

            if (_lines.at(i).which() == 0)
            {
                for (auto && x : generator.get().generate(boost::get<instruction>(_lines.at(i))))
                {
                    ret.at(section).push(std::move(x));
                }
            }

            else
            {
                auto size_str = boost::get<data>(_lines.at(i)).size;
                uint64_t size = size_str == "db" ? 8 : size_str == "dw" ? 16 : size_str == "dd" ? 32 : 64;

                struct visitor : public boost::static_visitor<>
                {
                    visitor(reaver::assembler::section & s, uint64_t size) : _section{ s }, _size{ size }
                    {
                    }

                    void operator()(const integer & i) const
                    {
                        for (auto && x : i.encode(_size / 8))
                        {
                            _section.push(std::move(x));
                        }
                    }

                    void operator()(const ch & c) const
                    {
                        push_char(c.character);
                    }

                    void operator()(const str & s) const
                    {
                        for (const auto & x : s.string)
                        {
                            push_char(x);
                        }
                    }

                    void push_char(char c) const
                    {
                        _section.push(c);

                        for (uint64_t i = 1; i < _size / 8; ++i)
                        {
                            _section.push(0);
                        }
                    }

                private:
                    reaver::assembler::section & _section;
                    uint64_t _size;
                };

                for (const auto & x : boost::get<data>(_lines.at(i)).operands)
                {
                    boost::apply_visitor(visitor{ ret.at(section), size }, x);
                }
            }
        }

        catch (const char * e)
        {
            std::cout << "in (ast's) line " << i + 1 << ": " << e << std::endl;
            throw std::runtime_error{"foo"};
        }
    }

    return ret;
}
