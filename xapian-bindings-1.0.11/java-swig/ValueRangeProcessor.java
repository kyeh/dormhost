/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class ValueRangeProcessor {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected ValueRangeProcessor(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ValueRangeProcessor obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      XapianJNI.delete_ValueRangeProcessor(swigCPtr);
    }
    swigCPtr = 0;
  }

  protected void swigDirectorDisconnect() {
    swigCMemOwn = false;
    delete();
  }

  public void swigReleaseOwnership() {
    swigCMemOwn = false;
    XapianJNI.ValueRangeProcessor_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    XapianJNI.ValueRangeProcessor_change_ownership(this, swigCPtr, true);
  }

  public int apply(SWIGTYPE_p_std__string begin, SWIGTYPE_p_std__string end) {
    return XapianJNI.ValueRangeProcessor_apply(swigCPtr, this, SWIGTYPE_p_std__string.getCPtr(begin), SWIGTYPE_p_std__string.getCPtr(end));
  }

  public ValueRangeProcessor() {
    this(XapianJNI.new_ValueRangeProcessor(), true);
    XapianJNI.ValueRangeProcessor_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

}
