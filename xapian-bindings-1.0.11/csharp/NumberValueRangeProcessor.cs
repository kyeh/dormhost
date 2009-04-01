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

public class NumberValueRangeProcessor : ValueRangeProcessor {
  private HandleRef swigCPtr;

  internal NumberValueRangeProcessor(IntPtr cPtr, bool cMemoryOwn) : base(XapianPINVOKE.NumberValueRangeProcessorUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(NumberValueRangeProcessor obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~NumberValueRangeProcessor() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        XapianPINVOKE.delete_NumberValueRangeProcessor(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public NumberValueRangeProcessor(uint valno_) : this(XapianPINVOKE.new_NumberValueRangeProcessor__SWIG_0(valno_), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public NumberValueRangeProcessor(uint valno_, string str_, bool prefix_) : this(XapianPINVOKE.new_NumberValueRangeProcessor__SWIG_1(valno_, str_, prefix_), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public NumberValueRangeProcessor(uint valno_, string str_) : this(XapianPINVOKE.new_NumberValueRangeProcessor__SWIG_2(valno_, str_), true) {
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
  }

  public override uint Apply(SWIGTYPE_p_std__string begin, SWIGTYPE_p_std__string end) {
    uint ret = XapianPINVOKE.NumberValueRangeProcessor_Apply(swigCPtr, SWIGTYPE_p_std__string.getCPtr(begin), SWIGTYPE_p_std__string.getCPtr(end));
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}