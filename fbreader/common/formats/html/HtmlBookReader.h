/*
 * FBReader -- electronic book reader
 * Copyright (C) 2005 Nikolay Pultsin <geometer@mawhrin.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __HTMLBOOKREADER_H__
#define __HTMLBOOKREADER_H__

#include <stack>

#include "HtmlReader.h"
#include "../../bookmodel/BookReader.h"

class BookModel;

class HtmlBookReader : public BookReader, public HtmlReader {

public:
	HtmlBookReader(BookModel &model);
	void readBook(ZLInputStream &stream) { readDocument(stream); }

protected:
	void startDocumentHandler();
	void endDocumentHandler();

	bool tagHandler(HtmlTag tag);
	bool characterDataHandler(const char *text, int len);

private:
	void flushTextBufferToParagraph();

private:
	int myIgnoreDataCounter;
	bool myIsPreformatted;
	bool myIsHyperlink;
	bool myIsStarted;
	std::stack<int> myListNumStack;
};

#endif /* __HTMLBOOKREADER_H__ */
