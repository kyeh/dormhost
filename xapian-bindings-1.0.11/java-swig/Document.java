/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class Document {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected Document(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Document obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      XapianJNI.delete_Document(swigCPtr);
    }
    swigCPtr = 0;
  }

  public Document(Document other) {
    this(XapianJNI.new_Document__SWIG_0(Document.getCPtr(other), other), true);
  }

  public Document() {
    this(XapianJNI.new_Document__SWIG_1(), true);
  }

  public String getValue(int valueno) {
    return XapianJNI.Document_getValue(swigCPtr, this, valueno);
  }

  public void addValue(int valueno, String value) {
    XapianJNI.Document_addValue(swigCPtr, this, valueno, value);
  }

  public void removeValue(int valueno) {
    XapianJNI.Document_removeValue(swigCPtr, this, valueno);
  }

  public void clearValues() {
    XapianJNI.Document_clearValues(swigCPtr, this);
  }

  public String getData() {
    return XapianJNI.Document_getData(swigCPtr, this);
  }

  public void setData(String data) {
    XapianJNI.Document_setData(swigCPtr, this, data);
  }

  public void addPosting(String tname, long tpos, long wdfinc) {
    XapianJNI.Document_addPosting__SWIG_0(swigCPtr, this, tname, tpos, wdfinc);
  }

  public void addPosting(String tname, long tpos) {
    XapianJNI.Document_addPosting__SWIG_1(swigCPtr, this, tname, tpos);
  }

  public void addTerm(String tname, long wdfinc) {
    XapianJNI.Document_addTerm__SWIG_0(swigCPtr, this, tname, wdfinc);
  }

  public void addTerm(String tname) {
    XapianJNI.Document_addTerm__SWIG_1(swigCPtr, this, tname);
  }

  public void removePosting(String tname, long tpos, long wdfdec) {
    XapianJNI.Document_removePosting__SWIG_0(swigCPtr, this, tname, tpos, wdfdec);
  }

  public void removePosting(String tname, long tpos) {
    XapianJNI.Document_removePosting__SWIG_1(swigCPtr, this, tname, tpos);
  }

  public void removeTerm(String tname) {
    XapianJNI.Document_removeTerm(swigCPtr, this, tname);
  }

  public void clearTerms() {
    XapianJNI.Document_clearTerms(swigCPtr, this);
  }

  public long termListCount() {
    return XapianJNI.Document_termListCount(swigCPtr, this);
  }

  public TermIterator termListBegin() {
    return new TermIterator(XapianJNI.Document_termListBegin(swigCPtr, this), true);
  }

  public TermIterator termListEnd() {
    return new TermIterator(XapianJNI.Document_termListEnd(swigCPtr, this), true);
  }

  public long valuesCount() {
    return XapianJNI.Document_valuesCount(swigCPtr, this);
  }

  public ValueIterator valuesBegin() {
    return new ValueIterator(XapianJNI.Document_valuesBegin(swigCPtr, this), true);
  }

  public ValueIterator valuesEnd() {
    return new ValueIterator(XapianJNI.Document_valuesEnd(swigCPtr, this), true);
  }

  public long getDocId() {
    return XapianJNI.Document_getDocId(swigCPtr, this);
  }

  public String toString() {
    return XapianJNI.Document_toString(swigCPtr, this);
  }

}
