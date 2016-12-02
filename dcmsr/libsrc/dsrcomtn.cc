/*
 *
 *  Copyright (C) 2000-2010, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  dcmsr
 *
 *  Author:  Joerg Riesmeier
 *
 *  Purpose:
 *    classes: DSRCompositeTreeNode
 *
 *  Last Update:      $Author: joergr $
 *  Update Date:      $Date: 2010-10-14 13:14:41 $
 *  CVS/RCS Revision: $Revision: 1.21 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmsr/dsrtypes.h"
#include "dcmtk/dcmsr/dsrcomtn.h"
#include "dcmtk/dcmsr/dsrxmld.h"

#include "dcmtk/dcmdata/dcuid.h"


DSRCompositeTreeNode::DSRCompositeTreeNode(const E_RelationshipType relationshipType)
 : DSRDocumentTreeNode(relationshipType, VT_Composite),
   DSRCompositeReferenceValue()
{
}


DSRCompositeTreeNode::~DSRCompositeTreeNode()
{
}


void DSRCompositeTreeNode::clear()
{
    DSRDocumentTreeNode::clear();
    DSRCompositeReferenceValue::clear();
}


OFBool DSRCompositeTreeNode::isValid() const
{
    return DSRDocumentTreeNode::isValid() && DSRCompositeReferenceValue::isValid();
}


OFCondition DSRCompositeTreeNode::print(STD_NAMESPACE ostream &stream,
                                        const size_t flags) const
{
    OFCondition result = DSRDocumentTreeNode::print(stream, flags);
    if (result.good())
    {
        stream << "=";
        result = DSRCompositeReferenceValue::print(stream, flags);
    }
    return result;
}


OFCondition DSRCompositeTreeNode::writeXML(STD_NAMESPACE ostream &stream,
                                           const size_t flags) const
{
    OFCondition result = EC_Normal;
    writeXMLItemStart(stream, flags);
    result = DSRDocumentTreeNode::writeXML(stream, flags);
    stream << "<value>" << OFendl;
    DSRCompositeReferenceValue::writeXML(stream, flags);
    stream << "</value>" << OFendl;
    writeXMLItemEnd(stream, flags);
    return result;
}


OFCondition DSRCompositeTreeNode::readContentItem(DcmItem &dataset)
{
    /* read ReferencedSOPSequence */
    return DSRCompositeReferenceValue::readSequence(dataset, "1" /* type */);
}


OFCondition DSRCompositeTreeNode::writeContentItem(DcmItem &dataset) const
{
    /* write ReferencedSOPSequence */
    return DSRCompositeReferenceValue::writeSequence(dataset);
}


OFCondition DSRCompositeTreeNode::readXMLContentItem(const DSRXMLDocument &doc,
                                                     DSRXMLCursor cursor)
{
    /* retrieve value from XML element "value" */
    return DSRCompositeReferenceValue::readXML(doc, doc.getNamedNode(cursor.gotoChild(), "value"));
}


OFCondition DSRCompositeTreeNode::renderHTMLContentItem(STD_NAMESPACE ostream &docStream,
                                                        STD_NAMESPACE ostream &annexStream,
                                                        const size_t /* nestingLevel */,
                                                        size_t &annexNumber,
                                                        const size_t flags) const
{
    /* render ConceptName */
    OFCondition result = renderHTMLConceptName(docStream, flags);
    /* render Reference */
    if (result.good())
    {
        result = DSRCompositeReferenceValue::renderHTML(docStream, annexStream, annexNumber, flags);
        docStream << OFendl;
    }
    return result;
}


/*
 *  CVS/RCS Log:
 *  $Log: dsrcomtn.cc,v $
 *  Revision 1.21  2010-10-14 13:14:41  joergr
 *  Updated copyright header. Added reference to COPYRIGHT file.
 *
 *  Revision 1.20  2009-10-13 14:57:51  uli
 *  Switched to logging mechanism provided by the "new" oflog module.
 *
 *  Revision 1.19  2007-11-15 16:43:43  joergr
 *  Fixed coding style to be more consistent.
 *
 *  Revision 1.18  2006/08/15 16:40:03  meichel
 *  Updated the code in module dcmsr to correctly compile when
 *    all standard C++ classes remain in namespace std.
 *
 *  Revision 1.17  2005/12/08 15:47:43  meichel
 *  Changed include path schema for all DCMTK header files
 *
 *  Revision 1.16  2003/09/15 14:13:42  joergr
 *  Introduced new class to facilitate checking of SR IOD relationship content
 *  constraints. Replaced old implementation distributed over numerous classes.
 *
 *  Revision 1.15  2003/08/07 17:29:13  joergr
 *  Removed libxml dependency from header files. Simplifies linking (MSVC).
 *
 *  Revision 1.14  2003/08/07 13:13:39  joergr
 *  Added readXML functionality.
 *
 *  Revision 1.13  2003/06/04 14:26:54  meichel
 *  Simplified include structure to avoid preprocessor limitation
 *    (max 32 #if levels) on MSVC5 with STL.
 *
 *  Revision 1.12  2001/11/09 16:13:42  joergr
 *  Added preliminary support for Mammography CAD SR.
 *
 *  Revision 1.11  2001/10/10 15:29:49  joergr
 *  Additonal adjustments for new OFCondition class.
 *
 *  Revision 1.10  2001/09/26 13:04:18  meichel
 *  Adapted dcmsr to class OFCondition
 *
 *  Revision 1.9  2001/05/07 16:14:22  joergr
 *  Updated CVS header.
 *
 *  Revision 1.8  2001/02/02 14:41:54  joergr
 *  Added new option to dsr2xml allowing to specify whether value and/or
 *  relationship type are to be encoded as XML attributes or elements.
 *
 *  Revision 1.7  2000/11/07 18:33:28  joergr
 *  Enhanced support for by-reference relationships.
 *
 *  Revision 1.6  2000/11/01 16:30:08  joergr
 *  Added support for conversion to XML.
 *
 *  Revision 1.5  2000/10/26 14:26:54  joergr
 *  Added support for "Comprehensive SR".
 *
 *  Revision 1.4  2000/10/20 10:14:57  joergr
 *  Renamed class DSRReferenceValue to DSRCompositeReferenceValue.
 *
 *  Revision 1.3  2000/10/18 17:13:58  joergr
 *  Added check for read methods (VM and type).
 *
 *  Revision 1.2  2000/10/16 12:01:55  joergr
 *  Reformatted print output.
 *
 *  Revision 1.1  2000/10/13 07:52:17  joergr
 *  Added new module 'dcmsr' providing access to DICOM structured reporting
 *  documents (supplement 23).  Doc++ documentation not yet completed.
 *
 *
 */
