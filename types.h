

/*
 * This file is part of Codecrypt.
 *
 * Copyright (C) 2013-2016 Mirek Kratochvil <exa.exa@gmail.com>
 *
 * Codecrypt is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * Codecrypt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Codecrypt. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ccr_types_h_
#define _ccr_types_h_

/*
 * typedefs. uint should be able to comfortably hold the GF(2^m) elements of
 * underlying calculations (esp. with polynomials. Switching to 64bits is
 * adviseable when computing with m=16 and larger.
 */
typedef unsigned int uint;
typedef unsigned char byte;

//TODO add separate type for GF(2^m) elements!

#endif
