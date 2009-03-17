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

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1convert_1to_1percent__JJ(JNIEnv *env, jclass clazz, jlong msetid, jlong msetiteratorid) {
    TRY
        MSet *this_mset = _mset->get(msetid);
        MSetIterator *itr = _msetiterator->get(msetiteratorid);
        return this_mset->convert_to_percent(*itr);
    CATCH(-1)
}

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1convert_1to_1percent__JD(JNIEnv *env, jclass clazz, jlong msetid, jdouble weight) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->convert_to_percent(weight);
    CATCH(-1)
}

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1get_1termfreq(JNIEnv *env, jclass clazz, jlong msetid, jstring term) {
    TRY
        MSet *mset = _mset->get(msetid);
        const char *c_term = env->GetStringUTFChars(term, 0);
	string cpp_term(c_term, env->GetStringUTFLength(term));
        int freq = mset->get_termfreq(cpp_term);
        env->ReleaseStringUTFChars(term, c_term);
        return freq;
    CATCH(-1)
}

JNIEXPORT jdouble JNICALL Java_org_xapian_XapianJNI_mset_1get_1termweight(JNIEnv *env, jclass clazz, jlong msetid, jstring term) {
    TRY
        MSet *mset = _mset->get(msetid);
        const char *c_term = env->GetStringUTFChars(term, 0);
	string cpp_term(c_term, env->GetStringUTFLength(term));
        double weight = mset->get_termweight(cpp_term);
        env->ReleaseStringUTFChars(term, c_term);
        return weight;
    CATCH(-1)
}

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1get_1firstitem(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->get_firstitem();
    CATCH(-1)
}

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1get_1matches_1lower_1bound(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->get_matches_lower_bound();
    CATCH(-1)
}

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1get_1matches_1estimated(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->get_matches_estimated();
    CATCH(-1)
}

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1get_1matches_1upper_1bound(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->get_matches_upper_bound();
    CATCH(-1)
}

JNIEXPORT jdouble JNICALL Java_org_xapian_XapianJNI_mset_1get_1max_1possible(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->get_max_possible();
    CATCH(-1)
}

JNIEXPORT jdouble JNICALL Java_org_xapian_XapianJNI_mset_1get_1max_1attained(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->get_max_attained();
    CATCH(-1)
}

JNIEXPORT jint JNICALL Java_org_xapian_XapianJNI_mset_1size(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->size();
    CATCH(-1)
}

JNIEXPORT jboolean JNICALL Java_org_xapian_XapianJNI_mset_1empty(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return mset->empty();
    CATCH(0)
}

JNIEXPORT jlong JNICALL Java_org_xapian_XapianJNI_mset_1begin(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        MSetIterator *itr = new MSetIterator(mset->begin());
        return _msetiterator->put(itr);
    CATCH(-1)
}

JNIEXPORT jlong JNICALL Java_org_xapian_XapianJNI_mset_1end(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        MSetIterator *itr = new MSetIterator(mset->end());
        return _msetiterator->put(itr);
    CATCH(-1)
}

JNIEXPORT jlong JNICALL Java_org_xapian_XapianJNI_mset_1back(JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        MSetIterator *itr = new MSetIterator(mset->back());
        return _msetiterator->put(itr);
    CATCH(-1)
}

JNIEXPORT jlong JNICALL Java_org_xapian_XapianJNI_mset_1element (JNIEnv *env, jclass clazz, jlong msetid, jlong index) {
    TRY
        MSet *mset = _mset->get(msetid);
        MSetIterator *itr = new MSetIterator((*mset)[index]);
        return _msetiterator->put(itr);
    CATCH(-1)
}

JNIEXPORT jstring JNICALL Java_org_xapian_XapianJNI_mset_1get_1description (JNIEnv *env, jclass clazz, jlong msetid) {
    TRY
        MSet *mset = _mset->get(msetid);
        return env->NewStringUTF(mset->get_description().c_str());
    CATCH(NULL)
}

JNIEXPORT void JNICALL Java_org_xapian_XapianJNI_mset_1finalize (JNIEnv *env, jclass clazz, jlong msetid) {
    MSet *mset = _mset->remove(msetid);
    if (mset) delete mset;
}

