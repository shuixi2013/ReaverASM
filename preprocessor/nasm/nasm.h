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

#include <reaver/error.h>

#include "../preprocessor.h"
#include "../macro.h"
#include "../define.h"
#include "../define_chain.h"

namespace reaver
{
    namespace assembler
    {
        class nasm_preprocessor : public preprocessor
        {
        public:
            nasm_preprocessor(const frontend & front, error_engine & engine) : _front{ front }, _engine{ engine }
            {
            }

            virtual ~nasm_preprocessor() {}

            virtual std::vector<line> operator()() const override;

        private:
            void _include_stream(std::istream &, std::shared_ptr<utils::include_chain>) const;

            std::pair<std::string, define_chain> _apply_defines(std::string, std::shared_ptr<utils::include_chain>) const;
            define_chain _apply_defines(std::vector<lexer::token> &, std::shared_ptr<utils::include_chain>) const;

            const frontend & _front;

            error_engine & _engine;
        };
    }
}
