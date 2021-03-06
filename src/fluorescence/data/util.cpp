/*
 * fluorescence is a free, customizable Ultima Online client.
 * Copyright (C) 2011-2012, http://fluorescence-client.org

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "util.hpp"

namespace fluo {
namespace data {

uint8_t Util::getColorR(uint16_t color) {
    return ( ( color >> 10 ) & 0x1F ) << 3;
}

uint8_t Util::getColorG(uint16_t color) {
    return ( ( color >> 5 ) & 0x1F ) << 3;
}

uint8_t Util::getColorB(uint16_t color) {
    return ( color & 0x1F ) << 3;
}

uint32_t Util::getColorRGBA(uint16_t color) {
    return ( getColorR(color)<<24 ) |
           ( getColorG( color ) << 16 ) |
           ( getColorB( color ) << 8 ) |
           ((color) ? 0xFF : 0x00);
}

}
}
