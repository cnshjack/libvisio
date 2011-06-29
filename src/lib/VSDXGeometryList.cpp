/* libvisio
 * Copyright (C) 2011 Fridrich Strba <fridrich.strba@bluewin.ch>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02111-1301 USA
 */

#include "VSDXGeometryList.h"

namespace libvisio {

class VSDXGeometryListElement
{
public:
  VSDXGeometryListElement() {}
  virtual ~VSDXGeometryListElement() {}
  virtual void handle(VSDXCollector *collector) = 0;
};

class VSDXGeometry : public VSDXGeometryListElement
{
public:
  VSDXGeometry(unsigned id, unsigned level, unsigned geomFlags) :
    m_id(id), m_level(level), m_geomFlags(geomFlags) {}
  ~VSDXGeometry() {}
  void handle(VSDXCollector *collector) { collector->collectGeometry(m_id, m_level, m_geomFlags); }
private:
  unsigned m_id;
  unsigned m_level;
  unsigned m_geomFlags;
};

class VSDXMoveTo : public VSDXGeometryListElement
{
public:
  VSDXMoveTo(unsigned id, unsigned level, double x, double y) :
    m_id(id), m_level(level), m_x(x), m_y(y) {}
  ~VSDXMoveTo() {}
  void handle(VSDXCollector *collector) { collector->collectMoveTo(m_id, m_level, m_x, m_y); }  
private:
  unsigned m_id, m_level;
  double m_x, m_y;
};

class VSDXLineTo : public VSDXGeometryListElement
{
public:
  VSDXLineTo(unsigned id, unsigned level, double x, double y) :
    m_id(id), m_level(level), m_x(x), m_y(y) {}
  ~VSDXLineTo() {}
  void handle(VSDXCollector *collector) { collector->collectLineTo(m_id, m_level, m_x, m_y); }  
private:
  unsigned m_id, m_level;
  double m_x, m_y;
};

class VSDXArcTo : public VSDXGeometryListElement
{
public:
  VSDXArcTo(unsigned id, unsigned level, double x2, double y2, double bow) :
    m_id(id), m_level(level), m_x2(x2), m_y2(y2), m_bow(bow) {}
  ~VSDXArcTo() {}
  void handle(VSDXCollector *collector) { collector->collectArcTo(m_id, m_level, m_x2, m_y2, m_bow); }
private:
  unsigned m_id, m_level;
  double m_x2, m_y2, m_bow;
};

class VSDXEllipse : public VSDXGeometryListElement
{
public:
  VSDXEllipse(unsigned id, unsigned level, double cx, double cy, double aa, double dd) :
    m_id(id), m_level(level), m_cx(cx), m_cy(cy), m_aa(aa), m_dd(dd) {}
  ~VSDXEllipse() {}
  void handle(VSDXCollector *collector) { collector->collectEllipse(m_id, m_level, m_cx, m_cy, m_aa, m_dd); }
private:
  unsigned m_id, m_level;
  double m_cx, m_cy, m_aa, m_dd;
};

class VSDXEllipticalArcTo : public VSDXGeometryListElement
{
public:
  VSDXEllipticalArcTo(unsigned id, unsigned level, double x3, double y3, double x2, double y2, double angle, double ecc) :
    m_id(id), m_level(level), m_x3(x3), m_y3(y3), m_x2(x2), m_y2(y2), m_angle(angle), m_ecc(ecc) {}
  ~VSDXEllipticalArcTo() {}
  void handle(VSDXCollector *collector) { collector->collectEllipticalArcTo(m_id, m_level, m_x3, m_y3, m_x2, m_y2, m_angle, m_ecc); }
private:
  unsigned m_id, m_level;
  double m_x3, m_y3, m_x2, m_y2, m_angle, m_ecc;
};

} // namespace libvisio


libvisio::VSDXGeometryList::VSDXGeometryList()
{
}

libvisio::VSDXGeometryList::~VSDXGeometryList()
{
  clear();
}

void libvisio::VSDXGeometryList::addGeometry(unsigned id, unsigned level, unsigned geomFlags)
{
  m_elements[id] = new VSDXGeometry(id, level, geomFlags);
}

void libvisio::VSDXGeometryList::addMoveTo(unsigned id, unsigned level, double x, double y)
{
  m_elements[id] = new VSDXMoveTo(id, level, x, y);
}

void libvisio::VSDXGeometryList::addLineTo(unsigned id, unsigned level, double x, double y)
{
  m_elements[id] = new VSDXLineTo(id, level, x, y);
}

void libvisio::VSDXGeometryList::addArcTo(unsigned id, unsigned level, double x2, double y2, double bow)
{
  m_elements[id] = new VSDXArcTo(id, level, x2, y2, bow);
}

void libvisio::VSDXGeometryList::addEllipse(unsigned id, unsigned level, double cx, double cy, double aa, double dd)
{
  m_elements[id] = new VSDXEllipse(id, level, cx, cy, aa, dd);
}

void libvisio::VSDXGeometryList::addEllipticalArcTo(unsigned id, unsigned level, double x3, double y3, double x2, double y2, double angle, double ecc)
{
  m_elements[id] = new VSDXEllipticalArcTo(id, level, x3, y3, x2, y2, angle, ecc);
}

void libvisio::VSDXGeometryList::setElementsOrder(const std::vector<unsigned> &elementsOrder)
{
  m_elementsOrder.clear();
  for (unsigned i = 0; i<elementsOrder.size(); i++)
    m_elementsOrder.push_back(elementsOrder[i]);
}

void libvisio::VSDXGeometryList::handle(VSDXCollector *collector)
{
  std::map<unsigned, VSDXGeometryListElement *>::iterator iter;
  if (m_elementsOrder.size())
  {
    for (unsigned i = 0; i < m_elementsOrder.size(); i++)
    {
      iter = m_elements.find(m_elementsOrder[i]);
      if (iter != m_elements.end())
        iter->second->handle(collector);
    }
  }
  else
  {
    for (iter = m_elements.begin(); iter != m_elements.end(); iter++)
      iter->second->handle(collector);
  }
}

void libvisio::VSDXGeometryList::clear()
{
  for (std::map<unsigned, VSDXGeometryListElement *>::iterator iter = m_elements.begin(); iter != m_elements.end(); iter++)
    delete iter->second;
}
