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

#include "ZLWin32Dialog.h"
#include "ZLWin32DialogContent.h"
#include "ZLWin32DialogElement.h"
#include "../application/ZLWin32ApplicationWindow.h"

ZLWin32Dialog::ZLWin32Dialog(ZLWin32ApplicationWindow *window, const std::string &name) : myWindow(window), myTitle(name) {
	myWindow->blockMouseEvents(true);
	myPanel = new ZLWin32DialogPanel(myWindow->mainWindow(), myTitle);
	const short charHeight = myPanel->charDimension().Height;

	ZLWin32DialogVBox *panelBox = new ZLWin32DialogVBox();
	myPanel->setElement(panelBox);

	ZLWin32DialogContent *contentTab = new ZLWin32DialogContent();
	myTab = contentTab;
	ZLWin32DialogElementPtr contentBox = contentTab->content();
	panelBox->addElement(contentBox);
	((ZLWin32DialogBox&)*contentBox).setSpacing(charHeight / 2);
	((ZLWin32DialogBox&)*contentBox).setMargins(charHeight / 2, charHeight / 2, charHeight / 2, charHeight / 2);

	myButtonBox = new ZLWin32DialogHBox();
	panelBox->addElement(myButtonBox);
	myButtonBox->setHomogeneous(true);
	myButtonBox->setSpacing(charHeight / 2);
	myButtonBox->setMargins(charHeight / 2, charHeight / 2, charHeight / 2, charHeight / 2);
}

ZLWin32Dialog::~ZLWin32Dialog() {
	myWindow->blockMouseEvents(false);
}

void ZLWin32Dialog::addButton(const std::string &text, bool accept) {
	ZLWin32DialogElementPtr button = new ZLWin32PushButton(accept ? IDOK : IDCANCEL, text);
	button->setVisible(true);
	myButtonBox->addElement(button);
}

static BOOL CALLBACK DialogProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_COMMAND:
			switch (wParam) {
				case IDOK:
				case IDCANCEL:
					EndDialog(hDialog, wParam == IDOK);
					return true;
			}
	}
	return false;
}

bool ZLWin32Dialog::run() {
	return DialogBoxIndirect(GetModuleHandle(0), myPanel->dialogTemplate(), myWindow->mainWindow(), DialogProc);
}
