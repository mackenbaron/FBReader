/*
 * Copyright (C) 2007 Nikolay Pultsin <geometer@mawhrin.net>
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <ZLUnicodeUtil.h>

#include "ZLWin32DialogElement.h"

ZLWin32DialogElement::ZLWin32DialogElement() {
}

ZLWin32DialogElement::~ZLWin32DialogElement() {
}

int ZLWin32DialogUtil::allocateString(WORD *p, const std::string &text) {
	ZLUnicodeUtil::Ucs2String ucs2Str;
	ZLUnicodeUtil::utf8ToUcs2(ucs2Str, text.data(), text.length());
	ucs2Str.push_back(0);
	memcpy(p, &ucs2Str.front(), 2 * ucs2Str.size());
	return ucs2Str.size();
}

ZLWin32DialogPanel::ZLWin32DialogPanel(HWND mainWindow, const std::string &caption) : myCaption(caption), myAddress(0) {
	TEXTMETRIC metric;
	HDC hdc = GetDC(mainWindow);
	GetTextMetrics(hdc, &metric);
	ReleaseDC(mainWindow, hdc);
	DWORD dlgUnit = GetDialogBaseUnits();
	myCharDimension.Width = (metric.tmAveCharWidth + 1) * 4 / LOWORD(dlgUnit);
	myCharDimension.Height = (metric.tmHeight + metric.tmExternalLeading) * 8 / HIWORD(dlgUnit);
}

ZLWin32DialogPanel::~ZLWin32DialogPanel() {
	if (myAddress != 0) {
		delete[] myAddress;
	}
}

DLGTEMPLATE *ZLWin32DialogPanel::dialogTemplate() {
	if (myAddress != 0) {
		delete[] myAddress;
	}

	myElement->setDimensions(myCharDimension);
	mySize = myElement->minimumSize();
	myElement->setPosition(0, 0, mySize);

	int size = 12 + ZLUnicodeUtil::utf8Length(myCaption) + myElement->allocationSize();
	size += size % 2;
	myAddress = new WORD[size];

	WORD *p = myAddress;
	const DWORD style = DS_CENTER | DS_MODALFRAME | WS_POPUPWINDOW | WS_CAPTION;
	*p++ = LOWORD(style);
	*p++ = HIWORD(style);
	*p++ = 0;
	*p++ = 0;
	*p++ = myElement->controlNumber();
	*p++ = 0; // X
	*p++ = 0; // Y
	*p++ = mySize.Width;
	*p++ = mySize.Height;
	*p++ = 0;
	*p++ = 0;
	p += ZLWin32DialogUtil::allocateString(p, myCaption);
	if ((p - myAddress) % 2 == 1) {
		p++;
	}
	myElement->allocate(p);

	return (DLGTEMPLATE*)myAddress;
}

void ZLWin32DialogPanel::setElement(ZLWin32DialogElementPtr element) {
	myElement = element;
}

ZLWin32DialogElement::Size ZLWin32DialogPanel::charDimension() const {
	return myCharDimension;
}
