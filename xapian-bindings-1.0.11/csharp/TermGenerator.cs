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

public class TermGenerator : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal TermGenerator(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(TermGenerator obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TermGenerator() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        XapianPINVOKE.delete_TermGenerator(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public TermGenerator() : this(XapianPINVOKE.new_TermGenerator(), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetStemmer(Stem stemmer) {
    XapianPINVOKE.TermGenerator_SetStemmer(swigCPtr, Stem.getCPtr(stemmer));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetStopper(Stopper stop) {
    XapianPINVOKE.TermGenerator_SetStopper__SWIG_0(swigCPtr, Stopper.getCPtr(stop));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetStopper() {
    XapianPINVOKE.TermGenerator_SetStopper__SWIG_1(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetDocument(Document doc) {
    XapianPINVOKE.TermGenerator_SetDocument(swigCPtr, Document.getCPtr(doc));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public Document GetDocument() {
    Document ret = new Document(XapianPINVOKE.TermGenerator_GetDocument(swigCPtr), false);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetDatabase(WritableDatabase db) {
    XapianPINVOKE.TermGenerator_SetDatabase(swigCPtr, WritableDatabase.getCPtr(db));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public TermGenerator.flags SetFlags(TermGenerator.flags toggle, TermGenerator.flags mask) {
    TermGenerator.flags ret = (TermGenerator.flags)XapianPINVOKE.TermGenerator_SetFlags__SWIG_0(swigCPtr, (int)toggle, (int)mask);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TermGenerator.flags SetFlags(TermGenerator.flags toggle) {
    TermGenerator.flags ret = (TermGenerator.flags)XapianPINVOKE.TermGenerator_SetFlags__SWIG_1(swigCPtr, (int)toggle);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void IndexText(string text, uint weight, string prefix) {
    XapianPINVOKE.TermGenerator_IndexText__SWIG_0(swigCPtr, text, weight, prefix);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void IndexText(string text, uint weight) {
    XapianPINVOKE.TermGenerator_IndexText__SWIG_1(swigCPtr, text, weight);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void IndexText(string text) {
    XapianPINVOKE.TermGenerator_IndexText__SWIG_2(swigCPtr, text);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void IndexTextWithoutPositions(string text, uint weight, string prefix) {
    XapianPINVOKE.TermGenerator_IndexTextWithoutPositions__SWIG_0(swigCPtr, text, weight, prefix);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void IndexTextWithoutPositions(string text, uint weight) {
    XapianPINVOKE.TermGenerator_IndexTextWithoutPositions__SWIG_1(swigCPtr, text, weight);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void IndexTextWithoutPositions(string text) {
    XapianPINVOKE.TermGenerator_IndexTextWithoutPositions__SWIG_2(swigCPtr, text);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void IncreaseTermpos(uint delta) {
    XapianPINVOKE.TermGenerator_IncreaseTermpos__SWIG_0(swigCPtr, delta);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void IncreaseTermpos() {
    XapianPINVOKE.TermGenerator_IncreaseTermpos__SWIG_1(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public uint GetTermPos() {
    uint ret = XapianPINVOKE.TermGenerator_GetTermPos(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetTermpos(uint termpos) {
    XapianPINVOKE.TermGenerator_SetTermpos(swigCPtr, termpos);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public string GetDescription() {
    string ret = XapianPINVOKE.TermGenerator_GetDescription(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public enum flags {
    FLAG_SPELLING = 128
  }

}

}
