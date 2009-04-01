/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.32
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */



#ifndef PHP_XAPIAN_H
#define PHP_XAPIAN_H

extern zend_module_entry xapian_module_entry;
#define phpext_xapian_ptr &xapian_module_entry

#ifdef PHP_WIN32
# define PHP_XAPIAN_API __declspec(dllexport)
#else
# define PHP_XAPIAN_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(xapian);
PHP_MSHUTDOWN_FUNCTION(xapian);
PHP_RINIT_FUNCTION(xapian);
PHP_RSHUTDOWN_FUNCTION(xapian);
PHP_MINFO_FUNCTION(xapian);

ZEND_NAMED_FUNCTION(_wrap_BAD_VALUENO_get);
ZEND_NAMED_FUNCTION(_wrap_version_string);
ZEND_NAMED_FUNCTION(_wrap_major_version);
ZEND_NAMED_FUNCTION(_wrap_minor_version);
ZEND_NAMED_FUNCTION(_wrap_revision);
ZEND_NAMED_FUNCTION(_wrap_new_PositionIterator);
ZEND_NAMED_FUNCTION(_wrap_PositionIterator_get_termpos);
ZEND_NAMED_FUNCTION(_wrap_PositionIterator_next);
ZEND_NAMED_FUNCTION(_wrap_PositionIterator_equals);
ZEND_NAMED_FUNCTION(_wrap_PositionIterator_skip_to);
ZEND_NAMED_FUNCTION(_wrap_PositionIterator_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_PostingIterator);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_skip_to);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_get_doclength);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_get_wdf);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_positionlist_begin);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_positionlist_end);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_get_description);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_get_docid);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_next);
ZEND_NAMED_FUNCTION(_wrap_PostingIterator_equals);
ZEND_NAMED_FUNCTION(_wrap_new_TermIterator);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_get_term);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_next);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_equals);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_skip_to);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_get_wdf);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_get_termfreq);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_positionlist_begin);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_positionlist_end);
ZEND_NAMED_FUNCTION(_wrap_TermIterator_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_ValueIterator);
ZEND_NAMED_FUNCTION(_wrap_ValueIterator_get_value);
ZEND_NAMED_FUNCTION(_wrap_ValueIterator_next);
ZEND_NAMED_FUNCTION(_wrap_ValueIterator_equals);
ZEND_NAMED_FUNCTION(_wrap_ValueIterator_get_valueno);
ZEND_NAMED_FUNCTION(_wrap_ValueIterator_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_Document);
ZEND_NAMED_FUNCTION(_wrap_Document_get_value);
ZEND_NAMED_FUNCTION(_wrap_Document_add_value);
ZEND_NAMED_FUNCTION(_wrap_Document_remove_value);
ZEND_NAMED_FUNCTION(_wrap_Document_clear_values);
ZEND_NAMED_FUNCTION(_wrap_Document_get_data);
ZEND_NAMED_FUNCTION(_wrap_Document_set_data);
ZEND_NAMED_FUNCTION(_wrap_Document_add_posting);
ZEND_NAMED_FUNCTION(_wrap_Document_add_term);
ZEND_NAMED_FUNCTION(_wrap_Document_remove_posting);
ZEND_NAMED_FUNCTION(_wrap_Document_remove_term);
ZEND_NAMED_FUNCTION(_wrap_Document_clear_terms);
ZEND_NAMED_FUNCTION(_wrap_Document_termlist_count);
ZEND_NAMED_FUNCTION(_wrap_Document_termlist_begin);
ZEND_NAMED_FUNCTION(_wrap_Document_termlist_end);
ZEND_NAMED_FUNCTION(_wrap_Document_values_count);
ZEND_NAMED_FUNCTION(_wrap_Document_values_begin);
ZEND_NAMED_FUNCTION(_wrap_Document_values_end);
ZEND_NAMED_FUNCTION(_wrap_Document_get_docid);
ZEND_NAMED_FUNCTION(_wrap_Document_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_MSet);
ZEND_NAMED_FUNCTION(_wrap_MSet_fetch);
ZEND_NAMED_FUNCTION(_wrap_MSet_convert_to_percent);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_termfreq);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_termweight);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_firstitem);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_matches_lower_bound);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_matches_estimated);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_matches_upper_bound);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_max_possible);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_max_attained);
ZEND_NAMED_FUNCTION(_wrap_MSet_size);
ZEND_NAMED_FUNCTION(_wrap_MSet_is_empty);
ZEND_NAMED_FUNCTION(_wrap_MSet_begin);
ZEND_NAMED_FUNCTION(_wrap_MSet_end);
ZEND_NAMED_FUNCTION(_wrap_MSet_back);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_hit);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_document_percentage);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_document);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_docid);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_document_id);
ZEND_NAMED_FUNCTION(_wrap_MSet_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_MSetIterator);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_docid);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_next);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_prev);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_equals);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_document);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_rank);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_weight);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_collapse_key);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_collapse_count);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_percent);
ZEND_NAMED_FUNCTION(_wrap_MSetIterator_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_ESet);
ZEND_NAMED_FUNCTION(_wrap_ESet_get_ebound);
ZEND_NAMED_FUNCTION(_wrap_ESet_size);
ZEND_NAMED_FUNCTION(_wrap_ESet_is_empty);
ZEND_NAMED_FUNCTION(_wrap_ESet_begin);
ZEND_NAMED_FUNCTION(_wrap_ESet_end);
ZEND_NAMED_FUNCTION(_wrap_ESet_back);
ZEND_NAMED_FUNCTION(_wrap_ESet_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_ESetIterator);
ZEND_NAMED_FUNCTION(_wrap_ESetIterator_get_termname);
ZEND_NAMED_FUNCTION(_wrap_ESetIterator_get_term);
ZEND_NAMED_FUNCTION(_wrap_ESetIterator_next);
ZEND_NAMED_FUNCTION(_wrap_ESetIterator_prev);
ZEND_NAMED_FUNCTION(_wrap_ESetIterator_equals);
ZEND_NAMED_FUNCTION(_wrap_ESetIterator_get_weight);
ZEND_NAMED_FUNCTION(_wrap_ESetIterator_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_RSet);
ZEND_NAMED_FUNCTION(_wrap_RSet_size);
ZEND_NAMED_FUNCTION(_wrap_RSet_is_empty);
ZEND_NAMED_FUNCTION(_wrap_RSet_add_document);
ZEND_NAMED_FUNCTION(_wrap_RSet_remove_document);
ZEND_NAMED_FUNCTION(_wrap_RSet_contains);
ZEND_NAMED_FUNCTION(_wrap_RSet_get_description);
ZEND_NAMED_FUNCTION(_wrap_MatchDecider_apply);
ZEND_NAMED_FUNCTION(_wrap_new_Enquire);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_query);
ZEND_NAMED_FUNCTION(_wrap_Enquire_get_query);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_weighting_scheme);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_collapse_key);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_docid_order);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_cutoff);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_sort_by_relevance);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_sort_by_value);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_sort_by_value_then_relevance);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_sort_by_relevance_then_value);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_sort_by_key);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_sort_by_key_then_relevance);
ZEND_NAMED_FUNCTION(_wrap_Enquire_set_sort_by_relevance_then_key);
ZEND_NAMED_FUNCTION(_wrap_Enquire_get_mset);
ZEND_NAMED_FUNCTION(_wrap_Enquire_get_eset);
ZEND_NAMED_FUNCTION(_wrap_Enquire_get_matching_terms_begin);
ZEND_NAMED_FUNCTION(_wrap_Enquire_get_matching_terms_end);
ZEND_NAMED_FUNCTION(_wrap_Enquire_get_matching_terms);
ZEND_NAMED_FUNCTION(_wrap_Enquire_get_description);
ZEND_NAMED_FUNCTION(_wrap_Weight_name);
ZEND_NAMED_FUNCTION(_wrap_Weight_serialise);
ZEND_NAMED_FUNCTION(_wrap_Weight_unserialise);
ZEND_NAMED_FUNCTION(_wrap_Weight_get_sumpart);
ZEND_NAMED_FUNCTION(_wrap_Weight_get_maxpart);
ZEND_NAMED_FUNCTION(_wrap_Weight_get_sumextra);
ZEND_NAMED_FUNCTION(_wrap_Weight_get_maxextra);
ZEND_NAMED_FUNCTION(_wrap_Weight_get_sumpart_needs_doclength);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_clone_object);
ZEND_NAMED_FUNCTION(_wrap_new_BoolWeight);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_name);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_serialise);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_unserialise);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_get_sumpart);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_get_maxpart);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_get_sumextra);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_get_maxextra);
ZEND_NAMED_FUNCTION(_wrap_BoolWeight_get_sumpart_needs_doclength);
ZEND_NAMED_FUNCTION(_wrap_new_BM25Weight);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_clone_object);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_name);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_serialise);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_unserialise);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_get_sumpart);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_get_maxpart);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_get_sumextra);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_get_maxextra);
ZEND_NAMED_FUNCTION(_wrap_BM25Weight_get_sumpart_needs_doclength);
ZEND_NAMED_FUNCTION(_wrap_new_TradWeight);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_clone_object);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_name);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_serialise);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_unserialise);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_get_sumpart);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_get_maxpart);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_get_sumextra);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_get_maxextra);
ZEND_NAMED_FUNCTION(_wrap_TradWeight_get_sumpart_needs_doclength);
ZEND_NAMED_FUNCTION(_wrap_Database_add_database);
ZEND_NAMED_FUNCTION(_wrap_new_Database);
ZEND_NAMED_FUNCTION(_wrap_Database_reopen);
ZEND_NAMED_FUNCTION(_wrap_Database_get_description);
ZEND_NAMED_FUNCTION(_wrap_Database_postlist_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_postlist_end);
ZEND_NAMED_FUNCTION(_wrap_Database_termlist_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_termlist_end);
ZEND_NAMED_FUNCTION(_wrap_Database_positionlist_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_positionlist_end);
ZEND_NAMED_FUNCTION(_wrap_Database_allterms_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_allterms_end);
ZEND_NAMED_FUNCTION(_wrap_Database_get_doccount);
ZEND_NAMED_FUNCTION(_wrap_Database_get_lastdocid);
ZEND_NAMED_FUNCTION(_wrap_Database_get_avlength);
ZEND_NAMED_FUNCTION(_wrap_Database_get_termfreq);
ZEND_NAMED_FUNCTION(_wrap_Database_term_exists);
ZEND_NAMED_FUNCTION(_wrap_Database_get_collection_freq);
ZEND_NAMED_FUNCTION(_wrap_Database_get_doclength);
ZEND_NAMED_FUNCTION(_wrap_Database_keep_alive);
ZEND_NAMED_FUNCTION(_wrap_Database_get_document);
ZEND_NAMED_FUNCTION(_wrap_Database_get_spelling_suggestion);
ZEND_NAMED_FUNCTION(_wrap_Database_spellings_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_spellings_end);
ZEND_NAMED_FUNCTION(_wrap_Database_synonyms_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_synonyms_end);
ZEND_NAMED_FUNCTION(_wrap_Database_synonym_keys_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_synonym_keys_end);
ZEND_NAMED_FUNCTION(_wrap_Database_get_metadata);
ZEND_NAMED_FUNCTION(_wrap_Database_metadata_keys_begin);
ZEND_NAMED_FUNCTION(_wrap_Database_metadata_keys_end);
ZEND_NAMED_FUNCTION(_wrap_new_WritableDatabase);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_flush);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_begin_transaction);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_commit_transaction);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_cancel_transaction);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_add_document);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_delete_document);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_replace_document);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_add_spelling);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_remove_spelling);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_add_synonym);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_remove_synonym);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_clear_synonyms);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_set_metadata);
ZEND_NAMED_FUNCTION(_wrap_WritableDatabase_get_description);
ZEND_NAMED_FUNCTION(_wrap_auto_open_stub);
ZEND_NAMED_FUNCTION(_wrap_quartz_open);
ZEND_NAMED_FUNCTION(_wrap_flint_open);
ZEND_NAMED_FUNCTION(_wrap_inmemory_open);
ZEND_NAMED_FUNCTION(_wrap_remote_open);
ZEND_NAMED_FUNCTION(_wrap_remote_open_writable);
ZEND_NAMED_FUNCTION(_wrap_new_Query);
ZEND_NAMED_FUNCTION(_wrap_Query_get_length);
ZEND_NAMED_FUNCTION(_wrap_Query_get_terms_begin);
ZEND_NAMED_FUNCTION(_wrap_Query_get_terms_end);
ZEND_NAMED_FUNCTION(_wrap_Query_is_empty);
ZEND_NAMED_FUNCTION(_wrap_Query_get_description);
ZEND_NAMED_FUNCTION(_wrap_Stopper_apply);
ZEND_NAMED_FUNCTION(_wrap_Stopper_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_SimpleStopper);
ZEND_NAMED_FUNCTION(_wrap_SimpleStopper_add);
ZEND_NAMED_FUNCTION(_wrap_SimpleStopper_apply);
ZEND_NAMED_FUNCTION(_wrap_SimpleStopper_get_description);
ZEND_NAMED_FUNCTION(_wrap_ValueRangeProcessor_apply);
ZEND_NAMED_FUNCTION(_wrap_new_StringValueRangeProcessor);
ZEND_NAMED_FUNCTION(_wrap_StringValueRangeProcessor_apply);
ZEND_NAMED_FUNCTION(_wrap_new_DateValueRangeProcessor);
ZEND_NAMED_FUNCTION(_wrap_DateValueRangeProcessor_apply);
ZEND_NAMED_FUNCTION(_wrap_new_NumberValueRangeProcessor);
ZEND_NAMED_FUNCTION(_wrap_NumberValueRangeProcessor_apply);
ZEND_NAMED_FUNCTION(_wrap_new_QueryParser);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_set_stemmer);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_set_stemming_strategy);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_set_stopper);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_set_default_op);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_get_default_op);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_set_database);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_parse_query);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_add_prefix);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_add_boolean_prefix);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_stoplist_begin);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_stoplist_end);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_unstem_begin);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_unstem_end);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_add_valuerangeprocessor);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_get_corrected_query_string);
ZEND_NAMED_FUNCTION(_wrap_QueryParser_get_description);
ZEND_NAMED_FUNCTION(_wrap_sortable_serialise);
ZEND_NAMED_FUNCTION(_wrap_sortable_unserialise);
ZEND_NAMED_FUNCTION(_wrap_new_Stem);
ZEND_NAMED_FUNCTION(_wrap_Stem_apply);
ZEND_NAMED_FUNCTION(_wrap_Stem_get_description);
ZEND_NAMED_FUNCTION(_wrap_Stem_get_available_languages);
ZEND_NAMED_FUNCTION(_wrap_new_TermGenerator);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_set_stemmer);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_set_stopper);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_set_document);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_get_document);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_set_database);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_set_flags);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_index_text);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_index_text_without_positions);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_increase_termpos);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_get_termpos);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_set_termpos);
ZEND_NAMED_FUNCTION(_wrap_TermGenerator_get_description);
ZEND_NAMED_FUNCTION(_wrap_new_MultiValueSorter);
ZEND_NAMED_FUNCTION(_wrap_MultiValueSorter_apply);
ZEND_NAMED_FUNCTION(_wrap_MultiValueSorter_add);
#endif /* PHP_XAPIAN_H */