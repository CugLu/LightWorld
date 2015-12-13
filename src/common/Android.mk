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


LOCAL_MODULE    := common



LOCAL_CPP_FEATURES += exceptions


LOCAL_SRC_FILES := \
mat4.cpp\
Joint.cpp\
vec2.cpp\
hashtable.cpp\
Heap.cpp\
Lexer.cpp\
Math.cpp\
Parser.cpp\
Plane.cpp\
quat.cpp\
Str.cpp\
Timer.cpp\
Token.cpp\
vec2.cpp\
aabb3d.cpp\
List.cpp\


include $(BUILD_STATIC_LIBRARY)
