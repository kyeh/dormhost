/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace Xapian {

using System;
using System.Runtime.InteropServices;

public class Database : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Database(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(Database obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~Database() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        XapianPINVOKE.delete_Database(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public void AddDatabase(Database database) {
    XapianPINVOKE.Database_AddDatabase(swigCPtr, Database.getCPtr(database));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public Database() : this(XapianPINVOKE.new_Database__SWIG_0(), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public Database(string path) : this(XapianPINVOKE.new_Database__SWIG_1(path), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public Database(Database other) : this(XapianPINVOKE.new_Database__SWIG_2(Database.getCPtr(other)), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Reopen() {
    XapianPINVOKE.Database_Reopen(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public string GetDescription() {
    string ret = XapianPINVOKE.Database_GetDescription(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public PostingIterator PostListBegin(string tname) {
    PostingIterator ret = new PostingIterator(XapianPINVOKE.Database_PostListBegin(swigCPtr, tname), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public PostingIterator PostListEnd(string tname) {
    PostingIterator ret = new PostingIterator(XapianPINVOKE.Database_PostListEnd(swigCPtr, tname), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator TermListBegin(uint did) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_TermListBegin(swigCPtr, did), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator TermListEnd(uint did) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_TermListEnd(swigCPtr, did), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public PositionIterator PositionListBegin(uint did, string tname) {
    PositionIterator ret = new PositionIterator(XapianPINVOKE.Database_PositionListBegin(swigCPtr, did, tname), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public PositionIterator PositionListEnd(uint did, string tname) {
    PositionIterator ret = new PositionIterator(XapianPINVOKE.Database_PositionListEnd(swigCPtr, did, tname), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator AllTermsBegin() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_AllTermsBegin__SWIG_0(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator AllTermsEnd() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_AllTermsEnd__SWIG_0(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator AllTermsBegin(string prefix) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_AllTermsBegin__SWIG_1(swigCPtr, prefix), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator AllTermsEnd(string prefix) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_AllTermsEnd__SWIG_1(swigCPtr, prefix), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint GetDocCount() {
    uint ret = XapianPINVOKE.Database_GetDocCount(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint GetLastDocId() {
    uint ret = XapianPINVOKE.Database_GetLastDocId(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public double GetAvLength() {
    double ret = XapianPINVOKE.Database_GetAvLength(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint GetTermFreq(string tname) {
    uint ret = XapianPINVOKE.Database_GetTermFreq(swigCPtr, tname);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public bool TermExists(string tname) {
    bool ret = XapianPINVOKE.Database_TermExists(swigCPtr, tname);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint GetCollectionFreq(string tname) {
    uint ret = XapianPINVOKE.Database_GetCollectionFreq(swigCPtr, tname);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public double GetDocLength(uint docid) {
    double ret = XapianPINVOKE.Database_GetDocLength(swigCPtr, docid);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void KeepAlive() {
    XapianPINVOKE.Database_KeepAlive(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public Document GetDocument(uint did) {
    Document ret = new Document(XapianPINVOKE.Database_GetDocument(swigCPtr, did), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public string GetSpellingSuggestion(string word, uint max_edit_distance) {
    string ret = XapianPINVOKE.Database_GetSpellingSuggestion__SWIG_0(swigCPtr, word, max_edit_distance);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public string GetSpellingSuggestion(string word) {
    string ret = XapianPINVOKE.Database_GetSpellingSuggestion__SWIG_1(swigCPtr, word);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SpellingsBegin() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SpellingsBegin(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SpellingsEnd() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SpellingsEnd(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SynonymsBegin(string term) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SynonymsBegin(swigCPtr, term), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SynonymsEnd(string arg0) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SynonymsEnd(swigCPtr, arg0), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SynonymKeysBegin(string prefix) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SynonymKeysBegin__SWIG_0(swigCPtr, prefix), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SynonymKeysBegin() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SynonymKeysBegin__SWIG_1(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SynonymKeysEnd(string prefix) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SynonymKeysEnd__SWIG_0(swigCPtr, prefix), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator SynonymKeysEnd() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_SynonymKeysEnd__SWIG_1(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public string GetMetadata(string key) {
    string ret = XapianPINVOKE.Database_GetMetadata(swigCPtr, key);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator MetadataKeysBegin(string prefix) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_MetadataKeysBegin__SWIG_0(swigCPtr, prefix), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator MetadataKeysBegin() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_MetadataKeysBegin__SWIG_1(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator MetadataKeysEnd(string prefix) {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_MetadataKeysEnd__SWIG_0(swigCPtr, prefix), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermIterator MetadataKeysEnd() {
    TermIterator ret = new TermIterator(XapianPINVOKE.Database_MetadataKeysEnd__SWIG_1(swigCPtr), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
