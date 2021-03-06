/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is part of the libvisio project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VSDOutputElementList.h"

namespace libvisio
{

namespace
{

static void separateSpacesAndInsertText(librevenge::RVNGDrawingInterface *iface, const librevenge::RVNGString &text)
{
  if (!iface)
    return;
  if (text.empty())
  {
    iface->insertText(text);
    return;
  }
  librevenge::RVNGString tmpText;
  int numConsecutiveSpaces = 0;
  librevenge::RVNGString::Iter i(text);
  for (i.rewind(); i.next();)
  {
    if (*(i()) == ' ')
      numConsecutiveSpaces++;
    else
      numConsecutiveSpaces = 0;

    if (numConsecutiveSpaces > 1)
    {
      if (!tmpText.empty())
      {
        iface->insertText(tmpText);
        tmpText.clear();
      }

      if (iface)
        iface->insertSpace();
    }
    else
    {
      tmpText.append(i());
    }
  }
  iface->insertText(tmpText);
}

} // anonymous namespace

class VSDOutputElement
{
public:
  VSDOutputElement() {}
  virtual ~VSDOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter) = 0;
  virtual VSDOutputElement *clone() = 0;
};


class VSDStyleOutputElement : public VSDOutputElement
{
public:
  VSDStyleOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDStyleOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDStyleOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDPathOutputElement : public VSDOutputElement
{
public:
  VSDPathOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDPathOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDPathOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDGraphicObjectOutputElement : public VSDOutputElement
{
public:
  VSDGraphicObjectOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDGraphicObjectOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDGraphicObjectOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDStartTextObjectOutputElement : public VSDOutputElement
{
public:
  VSDStartTextObjectOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDStartTextObjectOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDStartTextObjectOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDOpenParagraphOutputElement : public VSDOutputElement
{
public:
  VSDOpenParagraphOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDOpenParagraphOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDOpenParagraphOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDStartLayerOutputElement : public VSDOutputElement
{
public:
  VSDStartLayerOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDStartLayerOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDStartLayerOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDEndLayerOutputElement : public VSDOutputElement
{
public:
  VSDEndLayerOutputElement();
  virtual ~VSDEndLayerOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDEndLayerOutputElement();
  }
};


class VSDOpenSpanOutputElement : public VSDOutputElement
{
public:
  VSDOpenSpanOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDOpenSpanOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDOpenSpanOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDInsertTextOutputElement : public VSDOutputElement
{
public:
  VSDInsertTextOutputElement(const librevenge::RVNGString &text);
  virtual ~VSDInsertTextOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDInsertTextOutputElement(m_text);
  }
private:
  librevenge::RVNGString m_text;
};


class VSDInsertLineBreakOutputElement : public VSDOutputElement
{
public:
  VSDInsertLineBreakOutputElement();
  virtual ~VSDInsertLineBreakOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDInsertLineBreakOutputElement();
  }
};


class VSDInsertTabOutputElement : public VSDOutputElement
{
public:
  VSDInsertTabOutputElement();
  virtual ~VSDInsertTabOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDInsertTabOutputElement();
  }
};


class VSDCloseSpanOutputElement : public VSDOutputElement
{
public:
  VSDCloseSpanOutputElement();
  virtual ~VSDCloseSpanOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDCloseSpanOutputElement();
  }
};


class VSDCloseParagraphOutputElement : public VSDOutputElement
{
public:
  VSDCloseParagraphOutputElement();
  virtual ~VSDCloseParagraphOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDCloseParagraphOutputElement();
  }
};


class VSDEndTextObjectOutputElement : public VSDOutputElement
{
public:
  VSDEndTextObjectOutputElement();
  virtual ~VSDEndTextObjectOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDEndTextObjectOutputElement();
  }
};

class VSDOpenListElementOutputElement : public VSDOutputElement
{
public:
  VSDOpenListElementOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDOpenListElementOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDOpenListElementOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDCloseListElementOutputElement : public VSDOutputElement
{
public:
  VSDCloseListElementOutputElement();
  virtual ~VSDCloseListElementOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDCloseListElementOutputElement();
  }
};


class VSDOpenUnorderedListLevelOutputElement : public VSDOutputElement
{
public:
  VSDOpenUnorderedListLevelOutputElement(const librevenge::RVNGPropertyList &propList);
  virtual ~VSDOpenUnorderedListLevelOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDOpenUnorderedListLevelOutputElement(m_propList);
  }
private:
  librevenge::RVNGPropertyList m_propList;
};


class VSDCloseUnorderedListLevelOutputElement : public VSDOutputElement
{
public:
  VSDCloseUnorderedListLevelOutputElement();
  virtual ~VSDCloseUnorderedListLevelOutputElement() {}
  virtual void draw(librevenge::RVNGDrawingInterface *painter);
  virtual VSDOutputElement *clone()
  {
    return new VSDCloseUnorderedListLevelOutputElement();
  }
};


} // namespace libvisio

libvisio::VSDStyleOutputElement::VSDStyleOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDStyleOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->setStyle(m_propList);
}


libvisio::VSDPathOutputElement::VSDPathOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDPathOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->drawPath(m_propList);
}


libvisio::VSDGraphicObjectOutputElement::VSDGraphicObjectOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDGraphicObjectOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->drawGraphicObject(m_propList);
}


libvisio::VSDStartTextObjectOutputElement::VSDStartTextObjectOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDStartTextObjectOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->startTextObject(m_propList);
}

libvisio::VSDOpenSpanOutputElement::VSDOpenSpanOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDOpenSpanOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->openSpan(m_propList);
}


libvisio::VSDStartLayerOutputElement::VSDStartLayerOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDStartLayerOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->startLayer(m_propList);
}


libvisio::VSDEndLayerOutputElement::VSDEndLayerOutputElement() {}

void libvisio::VSDEndLayerOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->endLayer();
}


libvisio::VSDOpenParagraphOutputElement::VSDOpenParagraphOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDOpenParagraphOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->openParagraph(m_propList);
}


libvisio::VSDInsertTextOutputElement::VSDInsertTextOutputElement(const librevenge::RVNGString &text) :
  m_text(text) {}

void libvisio::VSDInsertTextOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    separateSpacesAndInsertText(painter, m_text);
}

libvisio::VSDInsertLineBreakOutputElement::VSDInsertLineBreakOutputElement() {}

void libvisio::VSDInsertLineBreakOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->insertLineBreak();
}

libvisio::VSDInsertTabOutputElement::VSDInsertTabOutputElement() {}

void libvisio::VSDInsertTabOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->insertTab();
}

libvisio::VSDCloseSpanOutputElement::VSDCloseSpanOutputElement() {}

void libvisio::VSDCloseSpanOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->closeSpan();
}


libvisio::VSDCloseParagraphOutputElement::VSDCloseParagraphOutputElement() {}

void libvisio::VSDCloseParagraphOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->closeParagraph();
}


libvisio::VSDEndTextObjectOutputElement::VSDEndTextObjectOutputElement() {}

void libvisio::VSDEndTextObjectOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->endTextObject();
}


libvisio::VSDOpenListElementOutputElement::VSDOpenListElementOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDOpenListElementOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->openListElement(m_propList);
}


libvisio::VSDCloseListElementOutputElement::VSDCloseListElementOutputElement() {}

void libvisio::VSDCloseListElementOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->closeListElement();
}


libvisio::VSDOpenUnorderedListLevelOutputElement::VSDOpenUnorderedListLevelOutputElement(const librevenge::RVNGPropertyList &propList) :
  m_propList(propList) {}

void libvisio::VSDOpenUnorderedListLevelOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->openUnorderedListLevel(m_propList);
}


libvisio::VSDCloseUnorderedListLevelOutputElement::VSDCloseUnorderedListLevelOutputElement() {}

void libvisio::VSDCloseUnorderedListLevelOutputElement::draw(librevenge::RVNGDrawingInterface *painter)
{
  if (painter)
    painter->closeUnorderedListLevel();
}


libvisio::VSDOutputElementList::VSDOutputElementList()
  : m_elements()
{
}

libvisio::VSDOutputElementList::VSDOutputElementList(const libvisio::VSDOutputElementList &elementList)
  : m_elements()
{
  std::vector<libvisio::VSDOutputElement *>::const_iterator iter;
  for (iter = elementList.m_elements.begin(); iter != elementList.m_elements.end(); ++iter)
    m_elements.push_back((*iter)->clone());
}

libvisio::VSDOutputElementList &libvisio::VSDOutputElementList::operator=(const libvisio::VSDOutputElementList &elementList)
{
  for (std::vector<VSDOutputElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter)
    delete (*iter);

  m_elements.clear();

  for (std::vector<VSDOutputElement *>::const_iterator cstiter = elementList.m_elements.begin(); cstiter != elementList.m_elements.end(); ++cstiter)
    m_elements.push_back((*cstiter)->clone());

  return *this;
}

void libvisio::VSDOutputElementList::append(const libvisio::VSDOutputElementList &elementList)
{
  for (std::vector<VSDOutputElement *>::const_iterator cstiter = elementList.m_elements.begin(); cstiter != elementList.m_elements.end(); ++cstiter)
    m_elements.push_back((*cstiter)->clone());
}

libvisio::VSDOutputElementList::~VSDOutputElementList()
{
  for (std::vector<VSDOutputElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter)
    delete (*iter);
  m_elements.clear();
}

void libvisio::VSDOutputElementList::draw(librevenge::RVNGDrawingInterface *painter) const
{
  for (std::vector<VSDOutputElement *>::const_iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter)
    (*iter)->draw(painter);
}

void libvisio::VSDOutputElementList::addStyle(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDStyleOutputElement(propList));
}

void libvisio::VSDOutputElementList::addPath(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDPathOutputElement(propList));
}

void libvisio::VSDOutputElementList::addGraphicObject(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDGraphicObjectOutputElement(propList));
}

void libvisio::VSDOutputElementList::addStartTextObject(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDStartTextObjectOutputElement(propList));
}

void libvisio::VSDOutputElementList::addOpenParagraph(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDOpenParagraphOutputElement(propList));
}

void libvisio::VSDOutputElementList::addOpenSpan(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDOpenSpanOutputElement(propList));
}

void libvisio::VSDOutputElementList::addInsertText(const librevenge::RVNGString &text)
{
  m_elements.push_back(new VSDInsertTextOutputElement(text));
}

void libvisio::VSDOutputElementList::addInsertLineBreak()
{
  m_elements.push_back(new VSDInsertLineBreakOutputElement());
}

void libvisio::VSDOutputElementList::addInsertTab()
{
  m_elements.push_back(new VSDInsertTabOutputElement());
}

void libvisio::VSDOutputElementList::addCloseSpan()
{
  m_elements.push_back(new VSDCloseSpanOutputElement());
}

void libvisio::VSDOutputElementList::addCloseParagraph()
{
  m_elements.push_back(new VSDCloseParagraphOutputElement());
}

void libvisio::VSDOutputElementList::addEndTextObject()
{
  m_elements.push_back(new VSDEndTextObjectOutputElement());
}

void libvisio::VSDOutputElementList::addStartLayer(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDStartLayerOutputElement(propList));
}

void libvisio::VSDOutputElementList::addEndLayer()
{
  m_elements.push_back(new VSDEndLayerOutputElement());
}

void libvisio::VSDOutputElementList::addOpenListElement(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDOpenListElementOutputElement(propList));
}

void libvisio::VSDOutputElementList::addOpenUnorderedListLevel(const librevenge::RVNGPropertyList &propList)
{
  m_elements.push_back(new VSDOpenUnorderedListLevelOutputElement(propList));
}

void libvisio::VSDOutputElementList::addCloseListElement()
{
  m_elements.push_back(new VSDCloseListElementOutputElement());
}

void libvisio::VSDOutputElementList::addCloseUnorderedListLevel()
{
  m_elements.push_back(new VSDCloseUnorderedListLevelOutputElement());
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
