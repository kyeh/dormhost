/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class ValueIterator {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected ValueIterator(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ValueIterator obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      XapianJNI.delete_ValueIterator(swigCPtr);
    }
    swigCPtr = 0;
  }

  public ValueIterator() {
    this(XapianJNI.new_ValueIterator__SWIG_0(), true);
  }

  public ValueIterator(ValueIterator other) {
    this(XapianJNI.new_ValueIterator__SWIG_1(ValueIterator.getCPtr(other), other), true);
  }

  public String getValue() {
    return XapianJNI.ValueIterator_getValue(swigCPtr, this);
  }

  public String next() {
    return XapianJNI.ValueIterator_next(swigCPtr, this);
  }

  public boolean equals(ValueIterator other) {
    return XapianJNI.ValueIterator_equals(swigCPtr, this, ValueIterator.getCPtr(other), other);
  }

  public int getValueNo() {
    return XapianJNI.ValueIterator_getValueNo(swigCPtr, this);
  }

  public String toString() {
    return XapianJNI.ValueIterator_toString(swigCPtr, this);
  }

  public boolean hasNext() {
    return XapianJNI.ValueIterator_hasNext(swigCPtr, this);
  }

}
