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

#ifndef __HTMLREADER_H__
#define __HTMLREADER_H__

#include <string>

#include <abstract/ZLXMLReader.h>

class ZLInputStream;

class HtmlTextConverter : public ZLXMLReader {

protected:
	const Tag *tags() const;

public:
	HtmlTextConverter(const char *encoding) : ZLXMLReader(encoding) {}
	void convertBuffer(std::vector<std::string> &buffer);
	void convertString(std::string &string);
	
public:
	void startElementHandler(int tag, const char **attributes);
	void endElementHandler(int tag);
	void characterDataHandler(const char *text, int len);

protected:
	const std::vector<std::string> &externalDTDs() const;

private:
	std::vector<std::string> *myBuffer;
};

class HtmlReader {

public:
	enum TagCode {
		_UNKNOWN,
		_HEAD,
		_BODY,
		_TITLE,
		_H1, _H2, _H3, _H4, _H5, _H6,
		// 9. text
		_EM, _STRONG, _DFN, _CODE, _SAMP, _KBD, _VAR, _CITE, _ABBR, _ACRONYM,
		_BLOCKQUOTE, _Q,
		_SUB, _SUP,
		_P, _BR,
		_PRE,
		_INS, _DEL,
		// 10. lists
		_UL, _OL, _LI,
		_DL, _DT, _DD,
		//
		_TT,
		_B, _I,
		_STYLE,
		_A,
	};

protected:
	struct HtmlAttribute {
		std::string Name;
		std::string Value;
		bool hasValue;

		HtmlAttribute(const std::string &name) : Name(name), hasValue(false) {}
		void setValue(const std::string &value) { Value = value; hasValue = true; }
	};

	struct HtmlTag {
		TagCode Code;
		bool Start;
		std::vector<HtmlAttribute> Attributes;

		HtmlTag(TagCode code, bool start) : Code(code), Start(start) {}
		void addAttribute(const std::string &name) { Attributes.push_back(HtmlAttribute(name)); }
		void setLastAttributeValue(const std::string &value) { if (!Attributes.empty()) Attributes.back().setValue(value); }
	};

private:
	static HtmlTag tag(std::string &name);

public:
	void readDocument(ZLInputStream &stream);

protected:
	HtmlReader();
	virtual ~HtmlReader() {}

protected:
	virtual void startDocumentHandler() = 0;
	virtual void endDocumentHandler() = 0;

	virtual bool tagHandler(HtmlTag tag) = 0;
	virtual bool characterDataHandler(const char *text, int len) = 0;

protected:
	HtmlTextConverter myConverter;
};

#endif /* __HTMLREADER_H__ */
