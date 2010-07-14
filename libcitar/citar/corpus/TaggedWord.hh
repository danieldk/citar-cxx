/*
 * Copyright 2008 Daniel de Kok
 *
 * This file is part of Citar.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <QString>

#ifndef CITAR_TAGGEDWORD_HH
#define CITAR_TAGGEDWORD_HH

namespace citar {
namespace corpus {

struct TaggedWord {
	TaggedWord(QString const &newWord = "", QString const &newTag = "") :
		word(newWord), tag(newTag) {}
	QString word;
	QString tag;
};

}
}

#endif // CITAR_TAGGEDWORD_HH
