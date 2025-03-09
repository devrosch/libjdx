/* Copyright (C) 2025 Robert Schiwon
 *
 * This file is part of libjdx.
 *
 * libjdx is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * libjdx is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libjdx. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef JDX_API_JDXDATASCANNER_HPP
#define JDX_API_JDXDATASCANNER_HPP

#include "api/Scanner.hpp"

namespace libjdx::jdx::api
{
class JdxScanner : public libjdx::api::Scanner
{
public:
    bool isRecognized(const std::string& path) override;
    std::unique_ptr<libjdx::api::Converter> getConverter(
        const std::string& path) override;

private:
};
} // libjdx::jdx::api

#endif
