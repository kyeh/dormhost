/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


public class Enquire {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected Enquire(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Enquire obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      XapianJNI.delete_Enquire(swigCPtr);
    }
    swigCPtr = 0;
  }

  public Enquire(Database databases) {
    this(XapianJNI.new_Enquire(Database.getCPtr(databases), databases), true);
  }

  public void setQuery(Query query, long qlen) {
    XapianJNI.Enquire_setQuery__SWIG_0(swigCPtr, this, Query.getCPtr(query), query, qlen);
  }

  public void setQuery(Query query) {
    XapianJNI.Enquire_setQuery__SWIG_1(swigCPtr, this, Query.getCPtr(query), query);
  }

  public Query getQuery() {
    return new Query(XapianJNI.Enquire_getQuery(swigCPtr, this), false);
  }

  public void setWeightingScheme(Weight weight) {
    XapianJNI.Enquire_setWeightingScheme(swigCPtr, this, Weight.getCPtr(weight), weight);
  }

  public void setCollapseKey(int collapse_key) {
    XapianJNI.Enquire_setCollapseKey(swigCPtr, this, collapse_key);
  }

  public void setDocidOrder(Enquire.docid_order order) {
    XapianJNI.Enquire_setDocidOrder(swigCPtr, this, order.swigValue());
  }

  public void setCutoff(int percent_cutoff, double weight_cutoff) {
    XapianJNI.Enquire_setCutoff__SWIG_0(swigCPtr, this, percent_cutoff, weight_cutoff);
  }

  public void setCutoff(int percent_cutoff) {
    XapianJNI.Enquire_setCutoff__SWIG_1(swigCPtr, this, percent_cutoff);
  }

  public void setSortByRelevance() {
    XapianJNI.Enquire_setSortByRelevance(swigCPtr, this);
  }

  public void setSortByValue(int sort_key, boolean ascending) {
    XapianJNI.Enquire_setSortByValue__SWIG_0(swigCPtr, this, sort_key, ascending);
  }

  public void setSortByValue(int sort_key) {
    XapianJNI.Enquire_setSortByValue__SWIG_1(swigCPtr, this, sort_key);
  }

  public void setSortByValueThenRelevance(int sort_key, boolean ascending) {
    XapianJNI.Enquire_setSortByValueThenRelevance__SWIG_0(swigCPtr, this, sort_key, ascending);
  }

  public void setSortByValueThenRelevance(int sort_key) {
    XapianJNI.Enquire_setSortByValueThenRelevance__SWIG_1(swigCPtr, this, sort_key);
  }

  public void setSortByRelevanceThenValue(int sort_key, boolean ascending) {
    XapianJNI.Enquire_setSortByRelevanceThenValue__SWIG_0(swigCPtr, this, sort_key, ascending);
  }

  public void setSortByRelevanceThenValue(int sort_key) {
    XapianJNI.Enquire_setSortByRelevanceThenValue__SWIG_1(swigCPtr, this, sort_key);
  }

  public void setSortByKey(Sorter sorter, boolean ascending) {
    XapianJNI.Enquire_setSortByKey__SWIG_0(swigCPtr, this, Sorter.getCPtr(sorter), sorter, ascending);
  }

  public void setSortByKey(Sorter sorter) {
    XapianJNI.Enquire_setSortByKey__SWIG_1(swigCPtr, this, Sorter.getCPtr(sorter), sorter);
  }

  public void setSortByKeyThenRelevance(Sorter sorter, boolean ascending) {
    XapianJNI.Enquire_setSortByKeyThenRelevance__SWIG_0(swigCPtr, this, Sorter.getCPtr(sorter), sorter, ascending);
  }

  public void setSortByKeyThenRelevance(Sorter sorter) {
    XapianJNI.Enquire_setSortByKeyThenRelevance__SWIG_1(swigCPtr, this, Sorter.getCPtr(sorter), sorter);
  }

  public void setSortByRelevanceThenKey(Sorter sorter, boolean ascending) {
    XapianJNI.Enquire_setSortByRelevanceThenKey__SWIG_0(swigCPtr, this, Sorter.getCPtr(sorter), sorter, ascending);
  }

  public void setSortByRelevanceThenKey(Sorter sorter) {
    XapianJNI.Enquire_setSortByRelevanceThenKey__SWIG_1(swigCPtr, this, Sorter.getCPtr(sorter), sorter);
  }

  public MSet getMSet(long first, long maxitems, long checkatleast, RSet omrset, MatchDecider mdecider) {
    return new MSet(XapianJNI.Enquire_getMSet__SWIG_0(swigCPtr, this, first, maxitems, checkatleast, RSet.getCPtr(omrset), omrset, MatchDecider.getCPtr(mdecider), mdecider), true);
  }

  public MSet getMSet(long first, long maxitems, long checkatleast, RSet omrset) {
    return new MSet(XapianJNI.Enquire_getMSet__SWIG_1(swigCPtr, this, first, maxitems, checkatleast, RSet.getCPtr(omrset), omrset), true);
  }

  public MSet getMSet(long first, long maxitems, long checkatleast) {
    return new MSet(XapianJNI.Enquire_getMSet__SWIG_2(swigCPtr, this, first, maxitems, checkatleast), true);
  }

  public MSet getMSet(long first, long maxitems) {
    return new MSet(XapianJNI.Enquire_getMSet__SWIG_3(swigCPtr, this, first, maxitems), true);
  }

  public MSet getMSet(long first, long maxitems, long checkatleast, RSet omrset, MatchDecider mdecider, MatchDecider matchspy) {
    return new MSet(XapianJNI.Enquire_getMSet__SWIG_4(swigCPtr, this, first, maxitems, checkatleast, RSet.getCPtr(omrset), omrset, MatchDecider.getCPtr(mdecider), mdecider, MatchDecider.getCPtr(matchspy), matchspy), true);
  }

  public MSet getMSet(long first, long maxitems, RSet omrset, MatchDecider mdecider) {
    return new MSet(XapianJNI.Enquire_getMSet__SWIG_5(swigCPtr, this, first, maxitems, RSet.getCPtr(omrset), omrset, MatchDecider.getCPtr(mdecider), mdecider), true);
  }

  public MSet getMSet(long first, long maxitems, RSet omrset) {
    return new MSet(XapianJNI.Enquire_getMSet__SWIG_6(swigCPtr, this, first, maxitems, RSet.getCPtr(omrset), omrset), true);
  }

  public ESet getESet(long maxitems, RSet omrset, int flags, double k, ExpandDecider edecider) {
    return new ESet(XapianJNI.Enquire_getESet__SWIG_0(swigCPtr, this, maxitems, RSet.getCPtr(omrset), omrset, flags, k, ExpandDecider.getCPtr(edecider), edecider), true);
  }

  public ESet getESet(long maxitems, RSet omrset, int flags, double k) {
    return new ESet(XapianJNI.Enquire_getESet__SWIG_1(swigCPtr, this, maxitems, RSet.getCPtr(omrset), omrset, flags, k), true);
  }

  public ESet getESet(long maxitems, RSet omrset, int flags) {
    return new ESet(XapianJNI.Enquire_getESet__SWIG_2(swigCPtr, this, maxitems, RSet.getCPtr(omrset), omrset, flags), true);
  }

  public ESet getESet(long maxitems, RSet omrset) {
    return new ESet(XapianJNI.Enquire_getESet__SWIG_3(swigCPtr, this, maxitems, RSet.getCPtr(omrset), omrset), true);
  }

  public ESet getESet(long maxitems, RSet omrset, ExpandDecider edecider) {
    return new ESet(XapianJNI.Enquire_getESet__SWIG_4(swigCPtr, this, maxitems, RSet.getCPtr(omrset), omrset, ExpandDecider.getCPtr(edecider), edecider), true);
  }

  public TermIterator getMatchingTermsBegin(long did) {
    return new TermIterator(XapianJNI.Enquire_getMatchingTermsBegin__SWIG_0(swigCPtr, this, did), true);
  }

  public TermIterator getMatchingTermsEnd(long did) {
    return new TermIterator(XapianJNI.Enquire_getMatchingTermsEnd__SWIG_0(swigCPtr, this, did), true);
  }

  public TermIterator getMatchingTermsBegin(MSetIterator i) {
    return new TermIterator(XapianJNI.Enquire_getMatchingTermsBegin__SWIG_1(swigCPtr, this, MSetIterator.getCPtr(i), i), true);
  }

  public TermIterator getMatchingTermsEnd(MSetIterator i) {
    return new TermIterator(XapianJNI.Enquire_getMatchingTermsEnd__SWIG_1(swigCPtr, this, MSetIterator.getCPtr(i), i), true);
  }

  public void registerMatchDecider(String name, MatchDecider mdecider) {
    XapianJNI.Enquire_registerMatchDecider__SWIG_0(swigCPtr, this, name, MatchDecider.getCPtr(mdecider), mdecider);
  }

  public void registerMatchDecider(String name) {
    XapianJNI.Enquire_registerMatchDecider__SWIG_1(swigCPtr, this, name);
  }

  public String toString() {
    return XapianJNI.Enquire_toString(swigCPtr, this);
  }

  public final static class docid_order {
    public final static docid_order ASCENDING = new docid_order("ASCENDING", XapianJNI.Enquire_ASCENDING_get());
    public final static docid_order DESCENDING = new docid_order("DESCENDING", XapianJNI.Enquire_DESCENDING_get());
    public final static docid_order DONT_CARE = new docid_order("DONT_CARE", XapianJNI.Enquire_DONT_CARE_get());

    public final int swigValue() {
      return swigValue;
    }

    public String toString() {
      return swigName;
    }

    public static docid_order swigToEnum(int swigValue) {
      if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
        return swigValues[swigValue];
      for (int i = 0; i < swigValues.length; i++)
        if (swigValues[i].swigValue == swigValue)
          return swigValues[i];
      throw new IllegalArgumentException("No enum " + docid_order.class + " with value " + swigValue);
    }

    private docid_order(String swigName) {
      this.swigName = swigName;
      this.swigValue = swigNext++;
    }

    private docid_order(String swigName, int swigValue) {
      this.swigName = swigName;
      this.swigValue = swigValue;
      swigNext = swigValue+1;
    }

    private docid_order(String swigName, docid_order swigEnum) {
      this.swigName = swigName;
      this.swigValue = swigEnum.swigValue;
      swigNext = this.swigValue+1;
    }

    private static docid_order[] swigValues = { ASCENDING, DESCENDING, DONT_CARE };
    private static int swigNext = 0;
    private final int swigValue;
    private final String swigName;
  }

  public final static int INCLUDE_QUERY_TERMS = XapianJNI.Enquire_INCLUDE_QUERY_TERMS_get();
  public final static int USE_EXACT_TERMFREQ = XapianJNI.Enquire_USE_EXACT_TERMFREQ_get();
}
