#include "lfSpine.h"

void _spAtlasPage_createTexture (spAtlasPage* self, const char* path)
{
	printf(path);
}

void _spAtlasPage_disposeTexture (spAtlasPage* self)
{

}

char* _spUtil_readFile (const char* path, int* length)
{

}

lfSpine::lfSpine()
{

}

lfSpine::~lfSpine()
{

}

void lfSpine::SetFile( const char* skeletonDataFile, const char* atlasFile )
{
	_atlas = spAtlas_createFromFile(atlasFile, 0);

	spSkeletonJson* json = spSkeletonJson_create(_atlas);
	json->scale = 1.0;
	spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, skeletonDataFile);
	spSkeletonJson_dispose(json);

	_skeleton = spSkeleton_create(skeletonData);
}

void lfSpine::Update (float deltaTime) {
	spSkeleton_update(_skeleton, deltaTime);

	const float* uvs = nullptr;
	int verticesCount = 0;
	const int* triangles = nullptr;
	int trianglesCount = 0;
	float r = 0, g = 0, b = 0, a = 0;
	for (int i = 0, n = _skeleton->slotsCount; i < n; i++) {
		spSlot* slot = _skeleton->drawOrder[i];
		if (!slot->attachment) continue;
		switch (slot->attachment->type) {
		case SP_ATTACHMENT_REGION: {
			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
			texture = getTexture(attachment);
			uvs = attachment->uvs;
			verticesCount = 8;
			triangles = quadTriangles;
			trianglesCount = 6;
			r = attachment->r;
			g = attachment->g;
			b = attachment->b;
			a = attachment->a;
			break;
		}
		case SP_ATTACHMENT_MESH: {
			spMeshAttachment* attachment = (spMeshAttachment*)slot->attachment;
			spMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
			texture = getTexture(attachment);
			uvs = attachment->uvs;
			verticesCount = attachment->verticesCount;
			triangles = attachment->triangles;
			trianglesCount = attachment->trianglesCount;
			r = attachment->r;
			g = attachment->g;
			b = attachment->b;
			a = attachment->a;
			break;
		}
		case SP_ATTACHMENT_SKINNED_MESH: {
			spSkinnedMeshAttachment* attachment = (spSkinnedMeshAttachment*)slot->attachment;
			spSkinnedMeshAttachment_computeWorldVertices(attachment, slot, _worldVertices);
			texture = getTexture(attachment);
			uvs = attachment->uvs;
			verticesCount = attachment->uvsCount;
			triangles = attachment->triangles;
			trianglesCount = attachment->trianglesCount;
			r = attachment->r;
			g = attachment->g;
			b = attachment->b;
			a = attachment->a;
			break;
		}
		default: ;
		} 
		if (texture) {
			if (slot->data->blendMode != blendMode) {
				_batch->flush();
				blendMode = slot->data->blendMode;
				switch (slot->data->blendMode) {
				case SP_BLEND_MODE_ADDITIVE:
					GL::blendFunc(_premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA, GL_ONE);
					break;
				case SP_BLEND_MODE_MULTIPLY:
					GL::blendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case SP_BLEND_MODE_SCREEN:
					GL::blendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
					break;
				default:
					GL::blendFunc(_blendFunc.src, _blendFunc.dst);
				}
			}
			color.a = _skeleton->a * slot->a * a * 255;
			float multiplier = _premultipliedAlpha ? color.a : 255;
			color.r = _skeleton->r * slot->r * r * multiplier;
			color.g = _skeleton->g * slot->g * g * multiplier;
			color.b = _skeleton->b * slot->b * b * multiplier;
			_batch->add(texture, _worldVertices, uvs, verticesCount, triangles, trianglesCount, &color);
		}
	}
	_batch->flush();

	//if (_debugSlots || _debugBones) {
	//	Director* director = Director::getInstance();
	//	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	//	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);

	//	if (_debugSlots) {
	//		// Slots.
	//		DrawPrimitives::setDrawColor4B(0, 0, 255, 255);
	//		glLineWidth(1);
	//		Vec2 points[4];
	//		V3F_C4B_T2F_Quad quad;
	//		for (int i = 0, n = _skeleton->slotsCount; i < n; i++) {
	//			spSlot* slot = _skeleton->drawOrder[i];
	//			if (!slot->attachment || slot->attachment->type != SP_ATTACHMENT_REGION) continue;
	//			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
	//			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
	//			points[0] = Vec2(_worldVertices[0], _worldVertices[1]);
	//			points[1] = Vec2(_worldVertices[2], _worldVertices[3]);
	//			points[2] = Vec2(_worldVertices[4], _worldVertices[5]);
	//			points[3] = Vec2(_worldVertices[6], _worldVertices[7]);
	//			DrawPrimitives::drawPoly(points, 4, true);
	//		}
	//	}
	//	if (_debugBones) {
	//		// Bone lengths.
	//		glLineWidth(2);
	//		DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
	//		for (int i = 0, n = _skeleton->bonesCount; i < n; i++) {
	//			spBone *bone = _skeleton->bones[i];
	//			float x = bone->data->length * bone->m00 + bone->worldX;
	//			float y = bone->data->length * bone->m10 + bone->worldY;
	//			DrawPrimitives::drawLine(Vec2(bone->worldX, bone->worldY), Vec2(x, y));
	//		}
	//		// Bone origins.
	//		DrawPrimitives::setPointSize(4);
	//		DrawPrimitives::setDrawColor4B(0, 0, 255, 255); // Root bone is blue.
	//		for (int i = 0, n = _skeleton->bonesCount; i < n; i++) {
	//			spBone *bone = _skeleton->bones[i];
	//			DrawPrimitives::drawPoint(Vec2(bone->worldX, bone->worldY));
	//			if (i == 0) DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
	//		}
	//	}
	//}
}

Texture2D* SkeletonRenderer::getTexture (spRegionAttachment* attachment) const {
	return (Texture2D*)((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
}

Texture2D* SkeletonRenderer::getTexture (spMeshAttachment* attachment) const {
	return (Texture2D*)((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
}

Texture2D* SkeletonRenderer::getTexture (spSkinnedMeshAttachment* attachment) const {
	return (Texture2D*)((spAtlasRegion*)attachment->rendererObject)->page->rendererObject;
}

Rect SkeletonRenderer::getBoundingBox () const {
	float minX = FLT_MAX, minY = FLT_MAX, maxX = FLT_MIN, maxY = FLT_MIN;
	float scaleX = getScaleX(), scaleY = getScaleY();
	for (int i = 0; i < _skeleton->slotsCount; ++i) {
		spSlot* slot = _skeleton->slots[i];
		if (!slot->attachment) continue;
		int verticesCount;
		if (slot->attachment->type == SP_ATTACHMENT_REGION) {
			spRegionAttachment* attachment = (spRegionAttachment*)slot->attachment;
			spRegionAttachment_computeWorldVertices(attachment, slot->bone, _worldVertices);
			verticesCount = 8;
		} else if (slot->attachment->type == SP_ATTACHMENT_MESH) {
			spMeshAttachment* mesh = (spMeshAttachment*)slot->attachment;
			spMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
			verticesCount = mesh->verticesCount;
		} else if (slot->attachment->type == SP_ATTACHMENT_SKINNED_MESH) {
			spSkinnedMeshAttachment* mesh = (spSkinnedMeshAttachment*)slot->attachment;
			spSkinnedMeshAttachment_computeWorldVertices(mesh, slot, _worldVertices);
			verticesCount = mesh->uvsCount;
		} else
			continue;
		for (int ii = 0; ii < verticesCount; ii += 2) {
			float x = _worldVertices[ii] * scaleX, y = _worldVertices[ii + 1] * scaleY;
			minX = min(minX, x);
			minY = min(minY, y);
			maxX = max(maxX, x);
			maxY = max(maxY, y);
		}
	}
	Vec2 position = getPosition();
	return Rect(position.x + minX, position.y + minY, maxX - minX, maxY - minY);
}

// --- Convenience methods for Skeleton_* functions.

void SkeletonRenderer::updateWorldTransform () {
	spSkeleton_updateWorldTransform(_skeleton);
}

void SkeletonRenderer::setToSetupPose () {
	spSkeleton_setToSetupPose(_skeleton);
}
void SkeletonRenderer::setBonesToSetupPose () {
	spSkeleton_setBonesToSetupPose(_skeleton);
}
void SkeletonRenderer::setSlotsToSetupPose () {
	spSkeleton_setSlotsToSetupPose(_skeleton);
}

spBone* SkeletonRenderer::findBone (const std::string& boneName) const {
	return spSkeleton_findBone(_skeleton, boneName.c_str());
}

spSlot* SkeletonRenderer::findSlot (const std::string& slotName) const {
	return spSkeleton_findSlot(_skeleton, slotName.c_str());
}

bool SkeletonRenderer::setSkin (const std::string& skinName) {
	return spSkeleton_setSkinByName(_skeleton, skinName.empty() ? 0 : skinName.c_str()) ? true : false;
}
bool SkeletonRenderer::setSkin (const char* skinName) {
	return spSkeleton_setSkinByName(_skeleton, skinName) ? true : false;
}

spAttachment* SkeletonRenderer::getAttachment (const std::string& slotName, const std::string& attachmentName) const {
	return spSkeleton_getAttachmentForSlotName(_skeleton, slotName.c_str(), attachmentName.c_str());
}
bool SkeletonRenderer::setAttachment (const std::string& slotName, const std::string& attachmentName) {
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName.empty() ? 0 : attachmentName.c_str()) ? true : false;
}
bool SkeletonRenderer::setAttachment (const std::string& slotName, const char* attachmentName) {
	return spSkeleton_setAttachment(_skeleton, slotName.c_str(), attachmentName) ? true : false;
}

