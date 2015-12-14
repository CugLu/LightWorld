# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    :=	engine
LOCAL_MODULE_FILENAME:= engine 

LOCAL_CPP_FEATURES += exceptions

LOCAL_SRC_FILES := \
Camera.cpp\
Game.cpp\
glutils.cpp\
Image.cpp\
image_load_jpg.cpp\
image_load_png.cpp\
lgeo.cpp\
lrender.cpp\
Material.cpp\
Mesh.cpp\
luautils.cpp\
MeshLoader3DS.cpp\
MeshLoaderB3D.cpp\
MeshLoaderLwo.cpp\
Model.cpp\
r_public.cpp\
ResourceSystem.cpp\
ScriptSystem.cpp\
Shader.cpp\
Shape.cpp\
Sprite.cpp\
Texture.cpp\
tr_stencilshadow.cpp\
tr_trisurf.cpp\
renderer/draw_common.cpp\
renderer/draw_common1.cpp\
renderer/RenderSystem.cpp\


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../build/prebuild/android

LOCAL_STATIC_LIBRARIES += common
#LOCAL_STATIC_LIBRARIES += png_static
#LOCAL_STATIC_LIBRARIES += jpeg_static
#LOCAL_STATIC_LIBRARIES += zlib_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,src/common)
$(call import-module,build/prebuild/jpeg/prebuilt/android)
$(call import-module,build/prebuild/lua/luajit/prebuilt/android)
$(call import-module,build/prebuild/png/prebuilt/android)
$(call import-module,build/prebuild/zlib/prebuilt/android)
