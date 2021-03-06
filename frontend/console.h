/**
 * Reaver Project Assembler License
 *
 * Copyright © 2013-2014 Michał "Griwes" Dominiak
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
 **/

#pragma once

#include <boost/program_options.hpp>

#include <reaver/target.h>
#include <reaver/error.h>

#include "frontend.h"
#include "../preprocessor/define.h"

namespace reaver
{
    namespace assembler
    {
        class console_frontend : public frontend
        {
        public:
            console_frontend(int, char **, error_engine &);
            virtual ~console_frontend() {}

            virtual bool preprocess_only() const override
            {
                return _prep_only;
            }

            virtual bool assemble_only() const override
            {
                return _asm_only;
            }

            virtual std::string preprocessor() const override
            {
                return _variables["preprocessor"].as<std::string>();
            }

            virtual std::string syntax() const override
            {
                return _variables["syntax"].as<std::string>();
            }

            virtual ::reaver::target::triple target() const override
            {
                return _target;
            }

            virtual std::string format() const override
            {
                return _variables["format"].as<std::string>();
            }

            virtual std::istream & input() const override
            {
                return _input;
            }

            virtual std::ostream & output() const override
            {
                return _output;
            }

            virtual std::string input_name() const override
            {
                return _input_name;
            }

            virtual std::vector<file> & default_includes() const override
            {
                return _default_includes;
            }

            virtual file open_file(std::string) const override;

            virtual const std::map<std::string, std::shared_ptr<define>> & defines() const override
            {
                return _defines;
            }

            virtual logger::level warning_level() const override
            {
                return _werror ? logger::error : logger::warning;
            }

        private:
            boost::program_options::variables_map _variables;
            bool _prep_only = false;
            bool _asm_only = false;
            bool _wextra = false;
            bool _werror = false;
            bool _no_ss_warning = false;
            int _opt = 1;

            mutable std::ifstream _input;
            mutable std::ofstream _output;

            std::string _input_name;
            mutable std::vector<file> _default_includes;
            std::vector<std::string> _include_paths;

            std::map<std::string, std::shared_ptr<define>> _defines;

            ::reaver::target::triple _target;
        };
    }
}
