/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * This file is part of the libvisio project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "VSDCollector.h"
#include "VSDCharacterList.h"

namespace libvisio
{

class VSDCharacterListElement
{
public:
  VSDCharacterListElement(unsigned id, unsigned level) : m_id(id), m_level(level) {}
  virtual ~VSDCharacterListElement() {}
  virtual void handle(VSDCollector *collector) const = 0;
  virtual VSDCharacterListElement *clone() = 0;
  virtual unsigned getCharCount() const = 0;
  virtual void setCharCount(unsigned charCount) = 0;

  unsigned m_id, m_level;
};

class VSDCharIX : public VSDCharacterListElement
{
public:
  VSDCharIX(unsigned id, unsigned level, unsigned charCount, const boost::optional<VSDName> &font,
            const boost::optional<Colour> &fontColour, const boost::optional<double> &fontSize, const boost::optional<bool> &bold,
            const boost::optional<bool> &italic, const boost::optional<bool> &underline, const boost::optional<bool> &doubleunderline,
            const boost::optional<bool> &strikeout, const boost::optional<bool> &doublestrikeout, const boost::optional<bool> &allcaps,
            const boost::optional<bool> &initcaps, const boost::optional<bool> &smallcaps, const boost::optional<bool> &superscript,
            const boost::optional<bool> &subscript, const boost::optional<double> &scaleWidth) : VSDCharacterListElement(id, level),
    m_style(charCount, font, fontColour, fontSize, bold, italic, underline, doubleunderline, strikeout,
            doublestrikeout, allcaps, initcaps,  smallcaps,  superscript,  subscript, scaleWidth) {}
  VSDCharIX(unsigned id, unsigned level, const VSDOptionalCharStyle &style) : VSDCharacterListElement(id, level), m_style(style) {}
  ~VSDCharIX() {}
  void handle(VSDCollector *collector) const;
  VSDCharacterListElement *clone();
  unsigned getCharCount() const
  {
    return m_style.charCount;
  }
  void setCharCount(unsigned charCount)
  {
    m_style.charCount = charCount;
  }

  VSDOptionalCharStyle m_style;
};
} // namespace libvisio


void libvisio::VSDCharIX::handle(VSDCollector *collector) const
{
  collector->collectCharIX(m_id, m_level, m_style.charCount, m_style.font, m_style.colour, m_style.size,
                           m_style.bold, m_style.italic, m_style.underline, m_style.doubleunderline, m_style.strikeout,
                           m_style.doublestrikeout, m_style.allcaps, m_style.initcaps, m_style.smallcaps,
                           m_style.superscript, m_style.subscript, m_style.scaleWidth);
}

libvisio::VSDCharacterListElement *libvisio::VSDCharIX::clone()
{
  return new VSDCharIX(m_id, m_level, m_style.charCount, m_style.font, m_style.colour, m_style.size,
                       m_style.bold, m_style.italic, m_style.underline, m_style.doubleunderline, m_style.strikeout,
                       m_style.doublestrikeout, m_style.allcaps, m_style.initcaps, m_style.smallcaps,
                       m_style.superscript, m_style.subscript, m_style.scaleWidth);
}


libvisio::VSDCharacterList::VSDCharacterList() :
  m_elements(),
  m_elementsOrder()
{
}

libvisio::VSDCharacterList::VSDCharacterList(const libvisio::VSDCharacterList &charList) :
  m_elements(),
  m_elementsOrder(charList.m_elementsOrder)
{
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter = charList.m_elements.begin();
  for (; iter != charList.m_elements.end(); ++iter)
    m_elements[iter->first] = iter->second->clone();
}

libvisio::VSDCharacterList &libvisio::VSDCharacterList::operator=(const libvisio::VSDCharacterList &charList)
{
  clear();
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter = charList.m_elements.begin();
  for (; iter != charList.m_elements.end(); ++iter)
    m_elements[iter->first] = iter->second->clone();
  m_elementsOrder = charList.m_elementsOrder;
  return *this;
}

libvisio::VSDCharacterList::~VSDCharacterList()
{
  clear();
}

void libvisio::VSDCharacterList::addCharIX(unsigned id, unsigned level, unsigned charCount,
                                           const boost::optional<VSDName> &font, const boost::optional<Colour> &fontColour, const boost::optional<double> &fontSize,
                                           const boost::optional<bool> &bold, const boost::optional<bool> &italic, const boost::optional<bool> &underline,
                                           const boost::optional<bool> &doubleunderline, const boost::optional<bool> &strikeout, const boost::optional<bool> &doublestrikeout,
                                           const boost::optional<bool> &allcaps, const boost::optional<bool> &initcaps, const boost::optional<bool> &smallcaps,
                                           const boost::optional<bool> &superscript, const boost::optional<bool> &subscript, const boost::optional<double> &scaleWidth)
{
  VSDCharIX *tmpElement = dynamic_cast<VSDCharIX *>(m_elements[id]);
  if (!tmpElement)
  {
    if (m_elements[id])
      delete m_elements[id];

    m_elements[id] = new VSDCharIX(id, level, charCount, font, fontColour, fontSize, bold, italic, underline, doubleunderline,
                                   strikeout, doublestrikeout, allcaps, initcaps, smallcaps, superscript, subscript, scaleWidth);
  }
  else
    tmpElement->m_style.override(VSDOptionalCharStyle(charCount, font, fontColour, fontSize, bold, italic, underline,
                                                      doubleunderline, strikeout, doublestrikeout, allcaps, initcaps, smallcaps, superscript, subscript, scaleWidth));
}

void libvisio::VSDCharacterList::addCharIX(unsigned id, unsigned level, const VSDOptionalCharStyle &style)
{
  addCharIX(id, level, style.charCount, style.font, style.colour, style.size, style.bold, style.italic, style.underline,
            style.doubleunderline, style.strikeout, style.doublestrikeout, style.allcaps, style.initcaps, style.smallcaps,
            style.superscript, style.subscript, style.scaleWidth);
}

unsigned libvisio::VSDCharacterList::getCharCount(unsigned id) const
{
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter = m_elements.find(id);
  if (iter != m_elements.end() && iter->second)
    return iter->second->getCharCount();
  else
    return MINUS_ONE;
}

void libvisio::VSDCharacterList::setCharCount(unsigned id, unsigned charCount)
{
  std::map<unsigned, VSDCharacterListElement *>::iterator iter = m_elements.find(id);
  if (iter !=  m_elements.end() && iter->second)
    iter->second->setCharCount(charCount);
}

void libvisio::VSDCharacterList::resetCharCount()
{
  for (std::map<unsigned, VSDCharacterListElement *>::iterator iter = m_elements.begin();
       iter != m_elements.end(); ++iter)
    iter->second->setCharCount(0);
}

unsigned libvisio::VSDCharacterList::getLevel() const
{
  if (m_elements.empty() || !m_elements.begin()->second)
    return 0;
  return m_elements.begin()->second->m_level;
}

void libvisio::VSDCharacterList::setElementsOrder(const std::vector<unsigned> &elementsOrder)
{
  m_elementsOrder.clear();
  for (unsigned i = 0; i<elementsOrder.size(); i++)
    m_elementsOrder.push_back(elementsOrder[i]);
}

void libvisio::VSDCharacterList::handle(VSDCollector *collector) const
{
  if (empty())
    return;
  std::map<unsigned, VSDCharacterListElement *>::const_iterator iter;
  if (!m_elementsOrder.empty())
  {
    for (unsigned i = 0; i < m_elementsOrder.size(); i++)
    {
      iter = m_elements.find(m_elementsOrder[i]);
      if (iter != m_elements.end() && (0 == i || iter->second->getCharCount()))
        iter->second->handle(collector);
    }
  }
  else
  {
    for (iter = m_elements.begin(); iter != m_elements.end(); ++iter)
      if (m_elements.begin() == iter || iter->second->getCharCount())
        iter->second->handle(collector);
  }
}

void libvisio::VSDCharacterList::clear()
{
  for (std::map<unsigned, VSDCharacterListElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter)
    delete iter->second;
  m_elements.clear();
  m_elementsOrder.clear();
}

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
