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

public class Remote : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal Remote(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(Remote obj) {
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

  public static Database Open(string host, uint port, uint timeout, uint connect_timeout) {
    Database ret = new Database(XapianPINVOKE.Remote_Open__SWIG_0(host, port, timeout, connect_timeout), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static Database Open(string host, uint port, uint timeout) {
    Database ret = new Database(XapianPINVOKE.Remote_Open__SWIG_1(host, port, timeout), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static Database Open(string host, uint port) {
    Database ret = new Database(XapianPINVOKE.Remote_Open__SWIG_2(host, port), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static WritableDatabase OpenWritable(string host, uint port, uint timeout, uint connect_timeout) {
    WritableDatabase ret = new WritableDatabase(XapianPINVOKE.Remote_OpenWritable__SWIG_0(host, port, timeout, connect_timeout), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static WritableDatabase OpenWritable(string host, uint port, uint timeout) {
    WritableDatabase ret = new WritableDatabase(XapianPINVOKE.Remote_OpenWritable__SWIG_1(host, port, timeout), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static WritableDatabase OpenWritable(string host, uint port) {
    WritableDatabase ret = new WritableDatabase(XapianPINVOKE.Remote_OpenWritable__SWIG_2(host, port), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static Database Open(string program, string args, uint timeout) {
    Database ret = new Database(XapianPINVOKE.Remote_Open__SWIG_3(program, args, timeout), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static Database Open(string program, string args) {
    Database ret = new Database(XapianPINVOKE.Remote_Open__SWIG_4(program, args), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static WritableDatabase OpenWritable(string program, string args, uint timeout) {
    WritableDatabase ret = new WritableDatabase(XapianPINVOKE.Remote_OpenWritable__SWIG_3(program, args, timeout), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static WritableDatabase OpenWritable(string program, string args) {
    WritableDatabase ret = new WritableDatabase(XapianPINVOKE.Remote_OpenWritable__SWIG_4(program, args), true);
    if (XapianPINVOKE.SWIGPendingException.Pending) throw XapianPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
