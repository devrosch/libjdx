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
#ifndef API_CONVERTERSERVICE_HPP
#define API_CONVERTERSERVICE_HPP

#include "api/Converter.hpp"
#include "api/Scanner.hpp"

#include <string>
#include <vector>

namespace libjdx::api
{
class ConverterService : public Scanner
{
public:
    explicit ConverterService(std::vector<std::shared_ptr<Scanner>> scanners);
    bool isRecognized(const std::string& path) override;
    std::unique_ptr<Converter> getConverter(const std::string& path) override;

private:
    std::vector<std::shared_ptr<Scanner>> m_scanners;
};
} // libjdx::api

#endif
