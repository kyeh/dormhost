/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class TermIterator {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected TermIterator(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TermIterator obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      XapianJNI.delete_TermIterator(swigCPtr);
    }
    swigCPtr = 0;
  }

  public TermIterator() {
    this(XapianJNI.new_TermIterator__SWIG_0(), true);
  }

  public TermIterator(TermIterator other) {
    this(XapianJNI.new_TermIterator__SWIG_1(TermIterator.getCPtr(other), other), true);
  }

  public String getTerm() {
    return XapianJNI.TermIterator_getTerm(swigCPtr, this);
  }

  public String next() {
    return XapianJNI.TermIterator_next(swigCPtr, this);
  }

  public boolean equals(TermIterator other) {
    return XapianJNI.TermIterator_equals(swigCPtr, this, TermIterator.getCPtr(other), other);
  }

  public void skipTo(String tname) {
    XapianJNI.TermIterator_skipTo(swigCPtr, this, tname);
  }

  public long getWdf() {
    return XapianJNI.TermIterator_getWdf(swigCPtr, this);
  }

  public long getTermFreq() {
    return XapianJNI.TermIterator_getTermFreq(swigCPtr, this);
  }

  public PositionIterator positionListBegin() {
    return new PositionIterator(XapianJNI.TermIterator_positionListBegin(swigCPtr, this), true);
  }

  public PositionIterator positionListEnd() {
    return new PositionIterator(XapianJNI.TermIterator_positionListEnd(swigCPtr, this), true);
  }

  public String toString() {
    return XapianJNI.TermIterator_toString(swigCPtr, this);
  }

  public boolean hasNext() {
    return XapianJNI.TermIterator_hasNext(swigCPtr, this);
  }

}
