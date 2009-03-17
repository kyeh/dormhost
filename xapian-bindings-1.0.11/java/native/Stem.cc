/**
 Copyright (c) 2003, Technology Concepts & Design, Inc.
 Copyright (c) 2008, Olly Betts
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted
 provided that the following conditions are met:

 - Redistributions of source code must retain the above copyright notice, this list of conditions
 and the following disclaimer.

 - Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 and the following disclaimer in the documentation and/or other materials provided with the distribution.

 - Neither the name of Technology Concepts & Design, Inc. nor the names of its contributors may be used to
 endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.
 **/
 
#include "xapian_jni.h"

using namespace std;
using namespace Xapian;

JNIEXPORT jstring JNICALL Java_org_xapian_XapianJNI_stem_1get_1available_1languages (JNIEnv *env, jclass clazz) {
    return env->NewStringUTF(Stem::get_available_languages().c_str());
}

JNIEXPORT jlong JNICALL Java_org_xapian_XapianJNI_stem_1new__ (JNIEnv *env, jclass clazz) {
    TRY
        Stem *stem = new Stem();
        return _stem->put(stem);
    CATCH(-1)
}

JNIEXPORT jlong JNICALL Java_org_xapian_XapianJNI_stem_1new__Ljava_lang_String_2 (JNIEnv *env, jclass clazz, jstring language) {
    TRY    
        const char *c_language = env->GetStringUTFChars(language, 0);
        Stem *stem = new Stem (c_language);
        env->ReleaseStringUTFChars(language, c_language);
        return _stem->put(stem);
    CATCH(-1)
}

JNIEXPORT jstring JNICALL Java_org_xapian_XapianJNI_stem_1stem_1word (JNIEnv *env, jclass clazz, jlong stemid, jstring term) {
    TRY
        Stem *stem = _stem->get(stemid);
        const char *c_term = env->GetStringUTFChars(term, 0);
	string cpp_term(c_term, env->GetStringUTFLength(term));
        string stemmed = (*stem)(cpp_term);
        env->ReleaseStringUTFChars(term, c_term);
        return env->NewStringUTF(stemmed.c_str());
    CATCH(NULL)
}

JNIEXPORT jstring JNICALL Java_org_xapian_XapianJNI_stem_1get_1description (JNIEnv *env, jclass clazz, jlong stemid) {
    TRY
        Stem *stem = _stem->get(stemid);
        return env->NewStringUTF(stem->get_description().c_str());
    CATCH(NULL)
}

JNIEXPORT void JNICALL Java_org_xapian_XapianJNI_stem_1finalize (JNIEnv *env, jclass clazz, jlong stemid) {
    Stem *stem = _stem->remove(stemid);
    if (stem) delete stem;
}

