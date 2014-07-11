<?php

/**
 * @file
 * Page callback for views user autocomplete.
 */

/**
 * Page callback for views user autocomplete.
 *
 * @param string $node_types
 *   A string of node type machine names concatenated by a "+".
 * @param string $string
 *   The first few characters of the username to search for.
 *   Passed from the exposed filter field.
 *
 * @return JSON
 *   Returns user names in JSON format.
 *
 * @see views_ajax_autocomplete_user()
 */
function viewsfilterusernodetype_autocomplete_user($node_types, $string = '') {
  // The user enters a comma-separated list of user name. We only autocomplete the last name.
  $array = drupal_explode_tags($string);
  $types = explode(variable_get('viewsfilterusernodetype_separator', '-'), $node_types);
  // Fetch last name
  $last_string = trim(array_pop($array));
  $matches = array();
  if ($last_string != '') {
    $prefix = count($array) ? implode(', ', $array) . ', ' : '';

    // Anonymous user is skipped.
//    if (strpos('anonymous', strtolower($last_string)) !== FALSE) {
//      $matches[$prefix . 'Anonymous'] = 'Anonymous';
//    }

    $query = db_select('users', 'u');
    $query->join('node', 'n', 'n.uid = u.uid');
    $query->fields('u', array('uid', 'name'))
      ->condition('n.type', $types, 'IN')
      ->condition('n.status', 1)
      ->condition('u.name', db_like($last_string) . '%', 'LIKE')
      ->orderBy('u.name')
      ->range(0, 10);
    $query->distinct();
    $result= $query->execute()->fetchAllKeyed();

    foreach ($result as $account) {
      $n = $account;
      // Commas and quotes in terms are special cases, so encode 'em.
      if (strpos($account, ',') !== FALSE || strpos($account, '"') !== FALSE) {
        $n = '"' . str_replace('"', '""', $account) . '"';
      }
      $matches[$prefix . $n] = check_plain($account);
    }
  }

  drupal_json_output($matches);
}