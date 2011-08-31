/* libvisio
 * Version: MPL 1.1 / GPLv2+ / LGPLv2+
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License or as specified alternatively below. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Major Contributor(s):
 * Copyright (C) 2011 Fridrich Strba <fridrich.strba@bluewin.ch>
 * Copyright (C) 2011 Eilidh McAdam <tibbylickle@gmail.com>
 *
 *
 * All Rights Reserved.
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPLv2+"), or
 * the GNU Lesser General Public License Version 2 or later (the "LGPLv2+"),
 * in which case the provisions of the GPLv2+ or the LGPLv2+ are applicable
 * instead of those above.
 */

#ifndef VSDXDOCUMENTSTRUCTURE_H
#define VSDXDOCUMENTSTRUCTURE_H

#define VSD_FOREIGN_DATA 0x0c
#define VSD_OLE_INFO 0x0d
#define VSD_TEXT 0x0e

#define VSD_PAGE 0x15
#define VSD_COLORS 0x16
#define VSD_FONT_LIST 0x18
#define VSD_FONT_IX 0x19
#define VSD_STYLES 0x1a
#define VSD_STENCILS 0x1d
#define VSD_STENCIL_PAGE 0x1e
#define VSD_OLE_DATA 0x1f

#define VSD_PAGES 0x27

#define VSD_SHAPE_GROUP 0x47
#define VSD_SHAPE_SHAPE 0x48
#define VSD_SHAPE_GUIDE 0x4d
#define VSD_SHAPE_FOREIGN 0x4e

#define VSD_STYLE_SHEET 0x4a

#define VSD_SHAPE_LIST 0x65
#define VSD_PROP_LIST 0x68
#define VSD_CHAR_LIST 0x69
#define VSD_GEOM_LIST 0x6c

#define VSD_SHAPE_ID 0x83
#define VSD_EVENT 0x84
#define VSD_LINE 0x85
#define VSD_FILL_AND_SHADOW 0x86
#define VSD_TEXT_BLOCK 0x87
#define VSD_TABS_DATA_1 0x88
#define VSD_GEOMETRY 0x89
#define VSD_MOVE_TO 0x8a
#define VSD_LINE_TO 0x8b
#define VSD_ARC_TO 0x8c
#define VSD_INFINITE_LINE 0x8d

#define VSD_ELLIPSE 0x8f
#define VSD_ELLIPTICAL_ARC_TO 0x90

#define VSD_PAGE_PROPS 0x92
#define VSD_STYLE_PROPS 0x93
#define VSD_CHAR_IX 0x94
#define VSD_PARA_IX 0x95
#define VSD_TABS_DATA_2 0x96
#define VSD_TABS_DATA_3 0x97
#define VSD_FOREIGN_DATA_TYPE 0x98
#define VSD_CONNECTION_POINTS 0x99

#define VSD_XFORM_DATA 0x9b
#define VSD_TEXT_XFORM 0x9c
#define VSD_XFORM_1D 0x9d
#define VSD_SCRATCH 0x9e

#define VSD_PROTECTION 0xa0

#define VSD_CONTROL_ANOTHER_TYPE 0xa2

#define VSD_MISC 0xa4
#define VSD_SPLINE_START 0xa5
#define VSD_SPLINE_KNOT 0xa6
#define VSD_LAYER_MEMBERSHIP 0xa7
#define VSD_LAYER 0xa8
#define VSD_ACT_ID 0xa9
#define VSD_CONTROL 0xaa

#define VSD_USER_DEFINED_CELLS 0xb5
#define VSD_TABS_DATA_4 0xb5
#define VSD_CUSTOM_PROPS 0xb6
#define VSD_RULER_GRID 0xb7

#define VSD_CONNECTION_POINTS_ANOTHER_TYPE 0xba

#define VSD_DOC_PROPS 0xbc
#define VSD_IMAGE 0xbd
#define VSD_GROUP 0xbe
#define VSD_LAYOUT 0xbf
#define VSD_PAGE_LAYOUT_IX 0xc0

#define VSD_POLYLINE_TO 0xc1
#define VSD_NURBS_TO 0xc3
#define VSD_HYPERLINK 0xc4
#define VSD_REVIEWER 0xc5
#define VSD_ANNOTATION 0xc6
#define VSD_SMART_TAG_DEF 0xc7
#define VSD_PRINT_PROPS 0xc8

#define VSD_SHAPE_DATA 0xd1
#define VSD_FONTFACE 0xd7
#define VSD_FONTFACES 0xd8

#endif /* VSDXDOCUMENTSTRUCTURE_H */
