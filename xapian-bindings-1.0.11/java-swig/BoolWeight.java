/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class BoolWeight extends Weight {
  private long swigCPtr;

  protected BoolWeight(long cPtr, boolean cMemoryOwn) {
    super(XapianJNI.SWIGBoolWeightUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(BoolWeight obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      XapianJNI.delete_BoolWeight(swigCPtr);
    }
    swigCPtr = 0;
    super.delete();
  }

  public BoolWeight cloneWeight() {
    long cPtr = XapianJNI.BoolWeight_cloneWeight(swigCPtr, this);
    return (cPtr == 0) ? null : new BoolWeight(cPtr, false);
  }

  public BoolWeight() {
    this(XapianJNI.new_BoolWeight(), true);
  }

  public String name() {
    return XapianJNI.BoolWeight_name(swigCPtr, this);
  }

  public String serialise() {
    return XapianJNI.BoolWeight_serialise(swigCPtr, this);
  }

  public Weight unserialise(String s) {
    long cPtr = XapianJNI.BoolWeight_unserialise(swigCPtr, this, s);
    return (cPtr == 0) ? null : new BoolWeight(cPtr, false);
  }

  public double getSumPart(long wdf, double len) {
    return XapianJNI.BoolWeight_getSumPart(swigCPtr, this, wdf, len);
  }

  public double getMaxPart() {
    return XapianJNI.BoolWeight_getMaxPart(swigCPtr, this);
  }

  public double getSumExtra(double len) {
    return XapianJNI.BoolWeight_getSumExtra(swigCPtr, this, len);
  }

  public double getMaxExtra() {
    return XapianJNI.BoolWeight_getMaxExtra(swigCPtr, this);
  }

  public boolean getSumPartNeedsDocLength() {
    return XapianJNI.BoolWeight_getSumPartNeedsDocLength(swigCPtr, this);
  }

}
