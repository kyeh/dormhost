<?php
/* Simple command-line search PHP4 script.
 *
 * Copyright (C) 2007 Olly Betts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

if (php_sapi_name() != "cli") {
    print "This example script is written to run under the command line ('cli') version of\n";
    print "the PHP interpreter, but you're using the '".php_sapi_name()."' version\n";
    exit(1);
}

include "php4/xapian.php";

// PHP < 4.3.0 only sets $argc and $argv if 'register_globals' is on.
if (!isset($argc)) $argc = $_SERVER['argc'];
if (!isset($argv)) $argv = $_SERVER['argv'];

if ($argc < 3) {
    print "Usage: {$argv[0]} PATH_TO_DATABASE QUERY\n";
    exit(1);
}

// Open the database for searching.
$database = new XapianDatabase($argv[1]);
if (!$database) {
    print "Couldn't open database '{$argv[1]}'\n";
    exit(1);
}

// Start an enquire session.
$enquire = new XapianEnquire($database);

// Combine the rest of the command line arguments with spaces between
// them, so that simple queries don't have to be quoted at the shell
// level.
$query_string = join(" ", array_slice($argv, 2));

$qp = new XapianQueryParser();
$stemmer = new XapianStem("english");
$qp->set_stemmer($stemmer);
$qp->set_database($database);
$qp->set_stemming_strategy(XapianQueryParser_STEM_SOME);
$query = $qp->parse_query($query_string);
print "Parsed query is: {$query->get_description()}\n";

// Find the top 10 results for the query.
$enquire->set_query($query);
$matches = $enquire->get_mset(0, 10);

// Display the results.
print "{$matches->get_matches_estimated()} results found:\n";

$i = $matches->begin();
while (!$i->equals($matches->end())) {
    $n = $i->get_rank() + 1;
    $data = $i->get_document()->get_data();
    print "$n: {$i->get_percent()}% docid={$i->get_docid()} [$data]\n\n";
    $i->next();
}
?>
