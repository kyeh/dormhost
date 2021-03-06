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

public class Weight : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Weight(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(Weight obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~Weight() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        XapianPINVOKE.delete_Weight(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public virtual string Name() {
    string ret = XapianPINVOKE.Weight_Name(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual string Serialise() {
    string ret = XapianPINVOKE.Weight_Serialise(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual Weight Unserialise(string s) {
    IntPtr cPtr = XapianPINVOKE.Weight_Unserialise(swigCPtr, s);
    Weight ret = (cPtr == IntPtr.Zero) ? null : new Weight(cPtr, false);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual double GetSumPart(uint wdf, double len) {
    double ret = XapianPINVOKE.Weight_GetSumPart(swigCPtr, wdf, len);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual double GetMaxPart() {
    double ret = XapianPINVOKE.Weight_GetMaxPart(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual double GetSumExtra(double len) {
    double ret = XapianPINVOKE.Weight_GetSumExtra(swigCPtr, len);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual double GetMaxExtra() {
    double ret = XapianPINVOKE.Weight_GetMaxExtra(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual bool GetSumPartNeedsDocLength() {
    bool ret = XapianPINVOKE.Weight_GetSumPartNeedsDocLength(swigCPtr);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
