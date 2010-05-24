/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plSpan.h"

hsUint32 plSpan::deswizzleGeoFlags(hsUint32 flags) {
    hsUint32 props = (flags & plGeometrySpan::kPropRunTimeLight) << 5;

    if (flags & plGeometrySpan::kPropNoShadowCast)
        props |= kPropNoShadowCast;
    if (flags & plGeometrySpan::kPropNoShadow)
        props |= kPropNoShadow;
    if (flags & plGeometrySpan::kPropForceShadow)
        props |= kPropForceShadow;
    if (flags & plGeometrySpan::kPropReverseSort)
        props |= kPropReverseSort;
    if (flags & plGeometrySpan::kPartialSort)
        props |= kPartialSort;
    if (flags & plGeometrySpan::kLiteVtxPreshaded)
        props |= kLiteVtxPreshaded;
    if (flags & plGeometrySpan::kLiteVtxNonPreshaded)
        props |= kLiteVtxNonPreshaded;
    if (flags & plGeometrySpan::kWaterHeight)
        props |= kWaterHeight;
    if (flags & plGeometrySpan::kVisLOS)
        props |= kVisLOS;

    return props;
}

plSpan::plSpan()
      : fSubType(kVertexSpan), fMaterialIdx(0), fBaseMatrix(0), fNumMatrices(0),
        fLocalUVWChans(0), fMaxBoneIdx(0), fPenBoneIdx(0), fProps(0),
        fMinDist(0), fMaxDist(0), fWaterHeight(0) { }

plSpan::plSpan(const plSpan& init)
      : fSubType(init.fSubType), fMaterialIdx(init.fMaterialIdx),
        fLocalToWorld(init.fLocalToWorld), fWorldToLocal(init.fWorldToLocal),
        fBaseMatrix(init.fBaseMatrix), fNumMatrices(init.fNumMatrices),
        fLocalUVWChans(init.fLocalUVWChans), fMaxBoneIdx(init.fMaxBoneIdx),
        fPenBoneIdx(init.fPenBoneIdx), fProps(init.fProps),
        fLocalBounds(init.fLocalBounds), fWorldBounds(init.fWorldBounds),
        fFogEnvironment(init.fFogEnvironment), fMinDist(init.fMinDist),
        fMaxDist(init.fMaxDist), fWaterHeight(init.fWaterHeight),
        fPermaLights(init.fPermaLights), fPermaProjs(init.fPermaProjs) { }

void plSpan::read(hsStream* S) {
    fSubType = S->readInt();
    fMaterialIdx = S->readInt();
    fLocalToWorld.read(S);
    fWorldToLocal.read(S);
    fProps = S->readInt();
    fLocalBounds.read(S);
    fWorldBounds.read(S);
    fNumMatrices = S->readInt();
    fBaseMatrix = S->readInt();
    fLocalUVWChans = S->readShort();
    fMaxBoneIdx = S->readShort();
    fPenBoneIdx = S->readShort();
    fMinDist = S->readFloat();
    fMaxDist = S->readFloat();
    if (fProps & kWaterHeight)
        fWaterHeight = S->readFloat();
}

void plSpan::write(hsStream* S) {
    S->writeInt(fSubType);
    S->writeInt(fMaterialIdx);
    fLocalToWorld.write(S);
    fWorldToLocal.write(S);
    S->writeInt(fProps);
    fLocalBounds.write(S);
    fWorldBounds.write(S);
    S->writeInt(fNumMatrices);
    S->writeInt(fBaseMatrix);
    S->writeShort(fLocalUVWChans);
    S->writeShort(fMaxBoneIdx);
    S->writeShort(fPenBoneIdx);
    S->writeFloat(fMinDist);
    S->writeFloat(fMaxDist);
    if (fProps & kWaterHeight)
        S->writeFloat(fWaterHeight);
}

void plSpan::prcWrite(pfPrcHelper* prc) {
    prc->writeSimpleTag(ClassName());
    IPrcWrite(prc);
    prc->closeTag();
}

void plSpan::prcParse(const pfPrcTag* tag) {
    if (tag->getName() != ClassName())
        throw pfPrcTagException(__FILE__, __LINE__, tag->getName());

    const pfPrcTag* child = tag->getFirstChild();
    while (child != NULL) {
        IPrcParse(child);
        child = child->getNextSibling();
    }
}

void plSpan::IPrcWrite(pfPrcHelper* prc) {
    prc->startTag("SpanInfo");
      prc->writeParam("SubType", fSubType);
      prc->writeParam("Material", fMaterialIdx);
      prc->writeParamHex("Properties", fProps);
    prc->endTag(true);
    prc->writeSimpleTag("LocalToWorld");
      fLocalToWorld.prcWrite(prc);
    prc->closeTag();
    prc->writeSimpleTag("WorldToLocal");
      fWorldToLocal.prcWrite(prc);
    prc->closeTag();
    prc->writeSimpleTag("LocalBounds");
      fLocalBounds.prcWrite(prc);
    prc->closeTag();
    prc->writeSimpleTag("WorldBounds");
      fWorldBounds.prcWrite(prc);
    prc->closeTag();
    prc->startTag("MatrixInfo");
      prc->writeParam("NumMatrices", fNumMatrices);
      prc->writeParam("BaseMatrix", fBaseMatrix);
      prc->writeParam("UVWChans", fLocalUVWChans);
    prc->endTag(true);
    prc->startTag("BoneIndices");
      prc->writeParam("Max", fMaxBoneIdx);
      prc->writeParam("Pen", fPenBoneIdx);
    prc->endTag(true);
    prc->startTag("Dists");
      prc->writeParam("Min", fMinDist);
      prc->writeParam("Max", fMaxDist);
    prc->endTag(true);
    if (fProps & kWaterHeight) {
        prc->startTag("WaterHeight");
        prc->writeParam("value", fWaterHeight);
        prc->endTag(true);
    }
}

void plSpan::IPrcParse(const pfPrcTag* tag) {
    if (tag->getName() == "SpanInfo") {
        fSubType = tag->getParam("SubType", "0").toUint();
        fMaterialIdx = tag->getParam("Material", "0").toUint();
        fProps = tag->getParam("Properties", "0").toUint();
    } else if (tag->getName() == "LocalToWorld") {
        if (tag->hasChildren())
            fLocalToWorld.prcParse(tag->getFirstChild());
    } else if (tag->getName() == "WorldToLocal") {
        if (tag->hasChildren())
            fWorldToLocal.prcParse(tag->getFirstChild());
    } else if (tag->getName() == "LocalBounds") {
        if (tag->hasChildren())
            fLocalBounds.prcParse(tag->getFirstChild());
    } else if (tag->getName() == "WorldBounds") {
        if (tag->hasChildren())
            fWorldBounds.prcParse(tag->getFirstChild());
    } else if (tag->getName() == "MatrixInfo") {
        fNumMatrices = tag->getParam("NumMatrices", "0").toUint();
        fBaseMatrix = tag->getParam("BaseMatrix", "0").toUint();
        fLocalUVWChans = tag->getParam("UVWChans", "0").toUint();
    } else if (tag->getName() == "BoneIndices") {
        fMaxBoneIdx = tag->getParam("Max", "0").toUint();
        fPenBoneIdx = tag->getParam("Pen", "0").toUint();
    } else if (tag->getName() == "Dists") {
        fMinDist = tag->getParam("Min", "0").toFloat();
        fMaxDist = tag->getParam("Max", "0").toFloat();
    } else if (tag->getName() == "WaterHeight") {
        fWaterHeight = tag->getParam("value", "0").toFloat();
    } else {
        throw pfPrcTagException(__FILE__, __LINE__, tag->getName());
    }
}
