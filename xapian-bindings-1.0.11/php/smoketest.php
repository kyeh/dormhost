<?php
// Run this PHP script using 'make check' in the build tree.

/* Simple test to ensure that we can load the xapian module and exercise basic
 * functionality successfully.
 *
 * Copyright (C) 2004,2005,2006,2007 Olly Betts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

$php_version = substr(PHP_VERSION, 0, 1);

include "php$php_version/xapian.php";

# Include PHP version specific code.
include "smoketest$php_version.php";

$stem = new XapianStem("english");
if ($stem->get_description() != "Xapian::Stem(english)") {
    print "Unexpected \$stem->get_description()\n";
    exit(1);
}

$doc = new XapianDocument();
$doc->set_data("a\x00b");
if ($doc->get_data() === "a") {
    print "get_data+set_data truncates at a zero byte\n";
    exit(1);
}
if ($doc->get_data() !== "a\x00b") {
    print "get_data+set_data doesn't transparently handle a zero byte\n";
    exit(1);
}
$doc->set_data("is there anybody out there?");
$doc->add_term("XYzzy");
$doc->add_posting($stem->apply("is"), 1);
$doc->add_posting($stem->apply("there"), 2);
$doc->add_posting($stem->apply("anybody"), 3);
$doc->add_posting($stem->apply("out"), 4);
$doc->add_posting($stem->apply("there"), 5);

// Check virtual function dispatch.
if (substr($db->get_description(), 0, 17) !== "WritableDatabase(") {
    print "Unexpected \$db->get_description()\n";
    exit(1);
}
$db->add_document($doc);
if ($db->get_doccount() != 1) {
    print "Unexpected \$db->get_doccount()\n";
    exit(1);
}

$terms = array("smoke", "test", "terms");
$query = new XapianQuery($op_or, $terms);
if ($query->get_description() != "Xapian::Query((smoke OR test OR terms))") {
    print "Unexpected \$query->get_description()\n";
    exit(1);
}
$query1 = new XapianQuery($op_phrase, array("smoke", "test", "tuple"));
if ($query1->get_description() != "Xapian::Query((smoke PHRASE 3 test PHRASE 3 tuple))") {
    print "Unexpected \$query1->get_description()\n";
    exit(1);
}
$query2 = new XapianQuery($op_xor, array(new XapianQuery("smoke"), $query1, "string"));
if ($query2->get_description() != "Xapian::Query((smoke XOR (smoke PHRASE 3 test PHRASE 3 tuple) XOR string))") {
    print "Unexpected \$query2->get_description()\n";
    exit(1);
}
$subqs = array("a", "b");
$query3 = new XapianQuery($op_or, $subqs);
if ($query3->get_description() != "Xapian::Query((a OR b))") {
    print "Unexpected \$query3->get_description()\n";
    exit(1);
}
$enq = new XapianEnquire($db);
$enq->set_query(new XapianQuery($op_or, "there", "is"));
$mset = $enq->get_mset(0, 10);
if ($mset->size() != 1) {
    print "Unexpected \$mset->size()\n";
    exit(1);
}
$terms = join(" ", $enq->get_matching_terms($mset->get_hit(0)));
if ($terms != "is there") {
    print "Unexpected matching terms: $terms\n";
    exit(1);
}

if ($op_elite_set != 10) {
    print "OP_ELITE_SET is $op_elite_set not 10\n";
    exit(1);
}

# Regression test - overload resolution involving boolean types failed.
$enq->set_sort_by_value(1, TRUE);

# Regression test - fixed in 0.9.10.1.
$oqparser = new XapianQueryParser();
$oquery = $oqparser->parse_query("I like tea");

# Regression test for bug#192 - fixed in 1.0.3.
$enq->set_cutoff(100);

# Check DateValueRangeProcessor works.
$qp = new XapianQueryParser();
$vrpdate = new XapianDateValueRangeProcessor(1, 1, 1960);
$qp->add_valuerangeprocessor($vrpdate);
$query = $qp->parse_query('12/03/99..12/04/01');
if ($query->get_description() !== 'Xapian::Query(VALUE_RANGE 1 19991203 20011204)') {
    print "XapianDateValueRangeProcessor didn't work - result was ".$query->get_description()."\n";
    exit(1);
}

# Test setting and getting metadata
if ($db->get_metadata('Foo') !== '') {
    print "Unexpected value for metadata associated with 'Foo' (expected ''): '".$db->get_metadata('Foo')."'\n";
    exit(1);
}
$db->set_metadata('Foo', 'Foo');
if ($db->get_metadata('Foo') !== 'Foo') {
    print "Unexpected value for metadata associated with 'Foo' (expected 'Foo'): '".$db->get_metadata('Foo')."'\n";
    exit(1);
}

# Test OP_SCALE_WEIGHT and corresponding constructor
$query4 = new XapianQuery($op_scale_weight, new XapianQuery('foo'), 5.0);
if ($query4->get_description() != "Xapian::Query(5 * foo)") {
    print "Unexpected \$query4->get_description()\n";
    exit(1);
}

# Test MultiValueSorter.

$doc = new XapianDocument();
$doc->add_term("foo");
$doc->add_value(0, "ABB");
$db2->add_document($doc);
$doc->add_value(0, "ABC");
$db2->add_document($doc);
$doc->add_value(0, "ABC\0");
$db2->add_document($doc);
$doc->add_value(0, "ABCD");
$db2->add_document($doc);
$doc->add_value(0, "ABC\xff");
$db2->add_document($doc);

$enquire = new XapianEnquire($db2);
$enquire->set_query(new XapianQuery("foo"));

{
    $sorter = new XapianMultiValueSorter();
    $sorter->add(0);
    $enquire->set_sort_by_key($sorter);
    $mset = $enquire->get_mset(0, 10);
    mset_expect_order($mset, array(5, 4, 3, 2, 1));
}

{
    $sorter = new XapianMultiValueSorter();
    $sorter->add(0, false);
    $enquire->set_sort_by_key($sorter);
    $mset = $enquire->get_mset(0, 10);
    mset_expect_order($mset, array(1, 2, 3, 4, 5));
}

{
    $sorter = new XapianMultiValueSorter();
    $sorter->add(0);
    $sorter->add(1);
    $enquire->set_sort_by_key($sorter);
    $mset = $enquire->get_mset(0, 10);
    mset_expect_order($mset, array(5, 4, 3, 2, 1));
}

{
    $sorter = new XapianMultiValueSorter();
    $sorter->add(0, false);
    $sorter->add(1);
    $enquire->set_sort_by_key($sorter);
    $mset = $enquire->get_mset(0, 10);
    mset_expect_order($mset, array(1, 2, 3, 4, 5));
}

{
    $sorter = new XapianMultiValueSorter();
    $sorter->add(0);
    $sorter->add(1, false);
    $enquire->set_sort_by_key($sorter);
    $mset = $enquire->get_mset(0, 10);
    mset_expect_order($mset, array(5, 4, 3, 2, 1));
}

{
    $sorter = new XapianMultiValueSorter();
    $sorter->add(0, false);
    $sorter->add(1, false);
    $enquire->set_sort_by_key($sorter);
    $mset = $enquire->get_mset(0, 10);
    mset_expect_order($mset, array(1, 2, 3, 4, 5));
}

function mset_expect_order($mset, $a) {
    if ($mset->size() != sizeof($a)) {
	print "MSet has ".$mset->size()." entries, expected ".sizeof($a)."\n";
	exit(1);
    }
    for ($j = 0; $j < sizeof($a); ++$j) {
	# PHP4 doesn't cope with: $mset->get_hit($j)->get_docid();
	$hit = $mset->get_hit($j);
	if ($hit->get_docid() != $a[$j]) {
	    print "Expected MSet[$j] to be $a[$j], got ".$hit->get_docid()."\n";
	    exit(1);
	}
    }
}

# Feature tests for Query "term" constructor optional arguments:
$query_wqf = new XapianQuery('wqf', 3);
if ($query_wqf->get_description() != 'Xapian::Query(wqf:(wqf=3))') {
    print "Unexpected \$query_wqf->get_description():\n";
    print $query_wqf->get_description() . "\n";
    exit(1);
}


$query = new XapianQuery($op_value_ge, 0, "100");
if ($query->get_description() != 'Xapian::Query(VALUE_GE 0 100)') {
    print "Unexpected \$query->get_description():\n";
    print $query->get_description() . "\n";
    exit(1);
}

?>
