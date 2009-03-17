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

public class Version : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Version(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(Version obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  public virtual void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        throw new MethodAccessException("C++ destructor does not have public access");
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
    }
  }

  public static string String() {
    string ret = XapianPINVOKE.Version_String();
    return ret;
  }

  public static int Major() {
    int ret = XapianPINVOKE.Version_Major();
    return ret;
  }

  public static int Minor() {
    int ret = XapianPINVOKE.Version_Minor();
    return ret;
  }

  public static int Revision() {
    int ret = XapianPINVOKE.Version_Revision();
    return ret;
  }

}

}
