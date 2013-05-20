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

#include <cpu/instruction.h>
#include "register.h"

namespace reaver
{
    namespace assembler
    {
        class operand_matcher
        {
        public:
            virtual ~operand_matcher() {}

            virtual bool operator()(const operand &) const = 0;
        };

        class register_matcher : public operand_matcher
        {
        public:
            register_matcher(std::string name) : _name{ name }
            {
            }

            register_matcher(cpu_register::sizes size) : _size{ size }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                if (op.is_register())
                {
                    if (_size && op.size() == _size)
                    {
                        return true;
                    }

                    if (op.name() == _name)
                    {
                        return true;
                    }
                }

                return false;
            }

        private:
            std::string _name;
            cpu_register::sizes _size;
        };

        class immediate_value_matcher : public operand_matcher
        {
        public:
            immediate_value_matcher(cpu_register::sizes size) : _size{ size }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                if (op.is_integer() || op.is_label())
                {
                    if (op.size() <= _size)
                    {
                        return true;
                    }
                }

                return false;
            }

        private:
            cpu_register::sizes _size;
        };

        class address_matcher : public operand_matcher
        {
        public:
            address_matcher()
            {
            }

            virtual bool operator()(const operand & op) const
            {
                return op.is_address();
            }
        };

        class rm_matcher : public operand_matcher
        {
        public:
            rm_matcher(cpu_register::sizes size) : _size{ size }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                if (op.is_address())
                {
                    return true;
                }

                if (op.is_register() && op.size() <= _size)
                {
                    return true;
                }

                return false;
            }

        private:
            cpu_register::sizes _size;
        };

        class special_register_matcher : public operand_matcher
        {
        public:
            special_register_matcher(cpu_register::types t) : _type{ t }
            {
            }

            virtual bool operator()(const operand & op) const
            {
                if (op.is_register())
                {
                    if (op.get_register()->type == _type)
                    {
                        return true;
                    }
                }

                return false;
            }

        private:
            cpu_register::types _type;
        };
    }
}