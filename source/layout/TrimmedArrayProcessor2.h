/*
 *
 * (C) Copyright IBM Corp.  and others 1998-2014 - All Rights Reserved
 *
 */

#ifndef __TRIMMEDARRAYPROCESSOR2_H
#define __TRIMMEDARRAYPROCESSOR2_H

/**
 * \file
 * \internal
 */

#include "LETypes.h"
#include "MorphTables.h"
#include "SubtableProcessor2.h"
#include "NonContextualGlyphSubst.h"
#include "NonContextualGlyphSubstProc2.h"

U_NAMESPACE_BEGIN

class LEGlyphStorage;

class TrimmedArrayProcessor2 : public NonContextualGlyphSubstitutionProcessor2
{
public:
    virtual void process(LEGlyphStorage &glyphStorage, LEErrorCode &success);

    TrimmedArrayProcessor2(const LEReferenceTo<MorphSubtableHeader2> &morphSubtableHeader, LEErrorCode &success);

    virtual ~TrimmedArrayProcessor2();

    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     *
     * @deprecated see {@link LayoutEngine}
     */
    virtual UClassID getDynamicClassID() const;

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     *
     * @deprecated see {@link LayoutEngine}
     */
    static UClassID getStaticClassID();

private:
    TrimmedArrayProcessor2();

protected:
    TTGlyphID firstGlyph;
    TTGlyphID lastGlyph;
    LEReferenceTo<TrimmedArrayLookupTable> trimmedArrayLookupTable;
    LEReferenceToArrayOf<LookupValue> valueArray;
};

U_NAMESPACE_END
#endif

