/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class TermGenerator {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected TermGenerator(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TermGenerator obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      XapianJNI.delete_TermGenerator(swigCPtr);
    }
    swigCPtr = 0;
  }

  public TermGenerator() {
    this(XapianJNI.new_TermGenerator(), true);
  }

  public void setStemmer(Stem stemmer) {
    XapianJNI.TermGenerator_setStemmer(swigCPtr, this, Stem.getCPtr(stemmer), stemmer);
  }

  public void setStopper(Stopper stop) {
    XapianJNI.TermGenerator_setStopper__SWIG_0(swigCPtr, this, Stopper.getCPtr(stop), stop);
  }

  public void setStopper() {
    XapianJNI.TermGenerator_setStopper__SWIG_1(swigCPtr, this);
  }

  public void setDocument(Document doc) {
    XapianJNI.TermGenerator_setDocument(swigCPtr, this, Document.getCPtr(doc), doc);
  }

  public Document getDocument() {
    return new Document(XapianJNI.TermGenerator_getDocument(swigCPtr, this), false);
  }

  public void setDatabase(WritableDatabase db) {
    XapianJNI.TermGenerator_setDatabase(swigCPtr, this, WritableDatabase.getCPtr(db), db);
  }

  public TermGenerator.flags setFlags(TermGenerator.flags toggle, TermGenerator.flags mask) {
    return TermGenerator.flags.swigToEnum(XapianJNI.TermGenerator_setFlags__SWIG_0(swigCPtr, this, toggle.swigValue(), mask.swigValue()));
  }

  public TermGenerator.flags setFlags(TermGenerator.flags toggle) {
    return TermGenerator.flags.swigToEnum(XapianJNI.TermGenerator_setFlags__SWIG_1(swigCPtr, this, toggle.swigValue()));
  }

  public void indexText(String text, long weight, String prefix) {
    XapianJNI.TermGenerator_indexText__SWIG_0(swigCPtr, this, text, weight, prefix);
  }

  public void indexText(String text, long weight) {
    XapianJNI.TermGenerator_indexText__SWIG_1(swigCPtr, this, text, weight);
  }

  public void indexText(String text) {
    XapianJNI.TermGenerator_indexText__SWIG_2(swigCPtr, this, text);
  }

  public void indexTextWithoutPositions(String text, long weight, String prefix) {
    XapianJNI.TermGenerator_indexTextWithoutPositions__SWIG_0(swigCPtr, this, text, weight, prefix);
  }

  public void indexTextWithoutPositions(String text, long weight) {
    XapianJNI.TermGenerator_indexTextWithoutPositions__SWIG_1(swigCPtr, this, text, weight);
  }

  public void indexTextWithoutPositions(String text) {
    XapianJNI.TermGenerator_indexTextWithoutPositions__SWIG_2(swigCPtr, this, text);
  }

  public void increaseTermpos(long delta) {
    XapianJNI.TermGenerator_increaseTermpos__SWIG_0(swigCPtr, this, delta);
  }

  public void increaseTermpos() {
    XapianJNI.TermGenerator_increaseTermpos__SWIG_1(swigCPtr, this);
  }

  public long getTermPos() {
    return XapianJNI.TermGenerator_getTermPos(swigCPtr, this);
  }

  public void setTermpos(long termpos) {
    XapianJNI.TermGenerator_setTermpos(swigCPtr, this, termpos);
  }

  public String toString() {
    return XapianJNI.TermGenerator_toString(swigCPtr, this);
  }

  public final static class flags {
    public final static flags FLAG_SPELLING = new flags("FLAG_SPELLING", XapianJNI.TermGenerator_FLAG_SPELLING_get());

    public final int swigValue() {
      return swigValue;
    }

    public String toString() {
      return swigName;
    }

    public static flags swigToEnum(int swigValue) {
      if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
        return swigValues[swigValue];
      for (int i = 0; i < swigValues.length; i++)
        if (swigValues[i].swigValue == swigValue)
          return swigValues[i];
      throw new IllegalArgumentException("No enum " + flags.class + " with value " + swigValue);
    }

    private flags(String swigName) {
      this.swigName = swigName;
      this.swigValue = swigNext++;
    }

    private flags(String swigName, int swigValue) {
      this.swigName = swigName;
      this.swigValue = swigValue;
      swigNext = swigValue+1;
    }

    private flags(String swigName, flags swigEnum) {
      this.swigName = swigName;
      this.swigValue = swigEnum.swigValue;
      swigNext = this.swigValue+1;
    }

    private static flags[] swigValues = { FLAG_SPELLING };
    private static int swigNext = 0;
    private final int swigValue;
    private final String swigName;
  }

}