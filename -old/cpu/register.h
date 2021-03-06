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

#include <string>
#include <vector>
#include <algorithm>

#include <parser/helpers.h>

namespace reaver
{
    namespace assembler
    {
        inline const std::vector<std::string> & long_mode_registers()
        {
            static std::vector<std::string> ret = { "r8", "r8b", "r8l", "r8w", "r8d", "r9", "r9b", "r9l", "r9w", "r9d",
                "r10", "r10b", "r10l", "r10w", "r10d", "r11", "r11b", "r11l", "r11w", "r11d", "r12", "r12b", "r12l",
                "r12w", "r12d", "r13", "r13b", "r13l", "r13w", "r13d", "r14", "r14b", "r14l", "r14w", "r14d", "r15",
                "r15b", "r15l", "r15w", "r15d", "spl", "bpl", "sil", "dil" };

            return ret;
        }

        inline const std::vector<std::string> & rex_enforce_registers()
        {
            static std::vector<std::string> ret = { "spl", "bpl", "sil", "dil", "cr8" };

            return ret;
        }

        inline const std::vector<std::string> & rex_disable_registers()
        {
            static std::vector<std::string> ret = { "ah", "bh", "ch", "dh", "cr0" };

            return ret;
        }

        inline const std::vector<std::string> & byte_registers()
        {
            static std::vector<std::string> ret = { "ah", "al", "bh", "bl", "ch", "cl", "dh", "dl", "spl", "bpl", "sil",
                "dil", "r8b", "r8l", "r9b", "r9l", "r10b", "r10l", "r11b", "r11l", "r12b", "r12l", "r13b", "r13l", "r14b",
                "r15b", "r15l" };

            return ret;
        }

        inline const std::vector<std::string> & word_registers()
        {
            static std::vector<std::string> ret = { "ax", "bx", "cx", "dx", "sp", "bp", "si", "bi", "r8w", "r9w", "r10w",
                "r11w", "r12w", "r13w", "r14w", "r15w" };

            return ret;
        }

        inline const std::vector<std::string> & dword_registers()
        {
            static std::vector<std::string> ret = { "eax", "ebx", "ecx", "edx", "esp", "ebp", "esi", "edi", "r8d", "r9d",
                "r10d", "r11d", "r12d", "r13d", "r14d", "r15d" };

            return ret;
        }

        inline const std::vector<std::string> & qword_registers()
        {
            static std::vector<std::string> ret = { "rax", "rbx", "rcx", "rdx", "rbp", "rsp", "rsi", "rdi", "r8", "r9",
                "r10", "r11", "r12", "r13", "r14", "r15" };

            return ret;
        }

        inline const std::vector<std::string> & control_registers()
        {
            static std::vector<std::string> ret = { "cr0", "cr2", "cr3", "cr4", "cr8" };

            return ret;
        }

        inline const std::vector<std::string> & debug_registers()
        {
            static std::vector<std::string> ret = { "dr0", "dr1", "dr2", "dr3", "dr4", "dr5", "dr6", "dr7" };

            return ret;
        }

        inline const std::vector<std::string> & segment_registers()
        {
            static std::vector<std::string> ret = { "cs", "ds", "es", "fs", "gs", "ss" };

            return ret;
        }

        class cpu_register : public operand_base
        {
        public:
            cpu_register(std::string str) : register_name{ std::move(str) }
            {
                if (std::find(debug_registers().begin(), debug_registers().end(), register_name) != debug_registers().end())
                {
                    type = debug;
                    return;
                }

                if (std::find(control_registers().begin(), control_registers().end(), register_name) != control_registers().end())
                {
                    type = control;
                    return;
                }

                if (std::find(segment_registers().begin(), segment_registers().end(), register_name) != segment_registers().end())
                {
                    type = segment;
                    return;
                }

                if (std::find(long_mode_registers().begin(), long_mode_registers().end(), register_name) != long_mode_registers().end())
                {
                    long_mode_only = true;
                }

                if (std::find(byte_registers().begin(), byte_registers().end(), register_name) != byte_registers().end())
                {
                    size_enum = byte;
                }

                else if (std::find(word_registers().begin(), word_registers().end(), register_name) != word_registers().end())
                {
                    size_enum = word;
                }

                else if (std::find(dword_registers().begin(), dword_registers().end(), register_name) != dword_registers().end())
                {
                    size_enum = dword;
                }

                else if (std::find(qword_registers().begin(), qword_registers().end(), register_name) != qword_registers().end())
                {
                    long_mode_only = true;
                    size_enum = qword;
                }

                if (std::find(rex_enforce_registers().begin(), rex_enforce_registers().end(), register_name)
                    != rex_enforce_registers().end())
                {
                    rex = enforce;
                }

                else if (std::find(rex_disable_registers().begin(), rex_disable_registers().end(), register_name)
                    != rex_disable_registers().end())
                {
                    rex = disable;
                }
            }

            virtual uint64_t size() const
            {
                return size_enum;
            }

            virtual const cpu_register & get_register() const
            {
                return *this;
            }

            virtual std::string name() const
            {
                return register_name;
            }

            bool long_mode_only = false;

            enum types
            {
                general_purpose,
                debug,
                control,
                segment
            } type = general_purpose;

            enum sizes
            {
                implicit,
                byte = 8,
                word = 16,
                dword = 32,
                qword = 64
            } size_enum = implicit;

            enum
            {
                na,
                enforce,
                disable
            } rex = na;

            std::string register_name;
        };
    }
}
