/*
 * FBReader -- electronic book reader
 * Copyright (C) 2004-2006 Nikolay Pultsin <geometer@mawhrin.net>
 * Copyright (C) 2005 Mikhail Sobolev <mss@mawhrin.net>
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

#include <ZLApplication.h>
#include <ZLFile.h>
#include <ZLDir.h>

#include "GCOpenSceneHandler.h"
#include "GeometricCalculator.h"
#include "../io/SceneSetNameReader.h"

GCOpenSceneHandler::GCOpenSceneHandler() :
	FolderOption(ZLOption::LOOK_AND_FEEL_CATEGORY, "OpenSceneDialog", "Folder", AllScenesFolderName), myIsUpToDate(false) {
	const std::string &value = FolderOption.value();
	if ((value == AllScenesFolderName) || (value == UserFolderName)) {
		myStateDisplayName = value;
	} else {
		myStateDisplayName = SceneSetNameReader().readSetName(ZLFile(value));
		if (myStateDisplayName.empty()) {
			myStateDisplayName = AllScenesFolderName;
			FolderOption.setValue(AllScenesFolderName);
		}
	}
}

shared_ptr<ZLDir> GCOpenSceneHandler::currentDirectory() const {
	const std::string &value = FolderOption.value();
	if (value == UserFolderName) {
		return UserDirectory(false);
	} else {
		return ZLFile(value).directory(false);
	}
}

void GCOpenSceneHandler::changeFolder(const ZLTreeNode &node) {
	FolderOption.setValue(node.id());
	resetSubnodesList();
	myIsUpToDate = false;
}

const std::string GCOpenSceneHandler::stateDisplayName() const {
	return myStateDisplayName;
}

const std::vector<ZLTreeNodePtr> &GCOpenSceneHandler::subnodes() const {
	if (!myIsUpToDate) {
		if (FolderOption.value() == AllScenesFolderName) {
			addFolderSubnode(UserFolderName, UserFolderName);
			collectSceneArchives(SamplesDirectory());
		} else {
			addFolderSubnode(AllScenesFolderName, "..");
			collectScenes(currentDirectory());
		}
		myIsUpToDate = true;
	}
	return GCSceneHandler::subnodes();
}

std::string GCOpenSceneHandler::relativeId(const ZLTreeNode &node) const {
	return (node.id() == AllScenesFolderName) ? FolderOption.value() : "..";
}


bool GCOpenSceneHandler::accept(const ZLTreeNode &node) const {
	shared_ptr<ZLDir> dir = currentDirectory();
	myFileName = (dir.isNull()) ? node.id() : dir->itemName(node.id());
	return true;
}

const std::string &GCOpenSceneHandler::fileName() const {
	return myFileName;
}
