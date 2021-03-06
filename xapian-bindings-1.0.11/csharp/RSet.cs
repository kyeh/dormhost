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

public class RSet : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal RSet(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(RSet obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~RSet() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        XapianPINVOKE.delete_RSet(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public RSet(RSet other) : this(XapianPINVOKE.new_RSet__SWIG_0(RSet.getCPtr(other)), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public RSet() : this(XapianPINVOKE.new_RSet__SWIG_1(), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public uint Size() {
    uint ret = XapianPINVOKE.RSet_Size(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public bool Empty() {
    bool ret = XapianPINVOKE.RSet_Empty(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void AddDocument(uint did) {
    XapianPINVOKE.RSet_AddDocument__SWIG_0(swigCPtr, did);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddDocument(MSetIterator i) {
    XapianPINVOKE.RSet_AddDocument__SWIG_1(swigCPtr, MSetIterator.getCPtr(i));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveDocument(uint did) {
    XapianPINVOKE.RSet_RemoveDocument__SWIG_0(swigCPtr, did);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveDocument(MSetIterator i) {
    XapianPINVOKE.RSet_RemoveDocument__SWIG_1(swigCPtr, MSetIterator.getCPtr(i));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(uint did) {
    bool ret = XapianPINVOKE.RSet_Contains__SWIG_0(swigCPtr, did);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public bool Contains(MSetIterator i) {
    bool ret = XapianPINVOKE.RSet_Contains__SWIG_1(swigCPtr, MSetIterator.getCPtr(i));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public string GetDescription() {
    string ret = XapianPINVOKE.RSet_GetDescription(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
