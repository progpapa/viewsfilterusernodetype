<?php
/**
 * @file
 * Definition of viewsfilterusernodetype_hanler_filter.
 */

/**
 * Extends the username autocomplete filter to filter only users who are
 * authors of selected node types.
 *
 * @ingroup views_filter_handlers
 */
class viewsfilterusernodetype_handler_filter extends views_handler_filter_user_name {

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    // Override the autocomplete path if we want to filter by node type.
    if ($this->options['use_node_type']
        && array_filter($this->options['selected_node_types'])) {
      $node_types = implode(variable_get('viewsfilterusernodetype_separator', '-'),
          array_keys(array_filter($this->options['selected_node_types'])));
      $form['value']['#autocomplete_path']
          = 'viewsfilterusernodetype/autocomplete/user/' . $node_types;
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    // This is required to store our custom values and set their defaults.
    $options['use_node_type'] = array('default' => FALSE);
    $options['selected_node_types'] = array('default' => array());
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['use_node_type'] = array(
      '#type' => 'checkbox',
      '#title' => t('Filter by node type'),
      '#description' => t('Filter only users that are authors of nodes of selected types.'),
      '#default_value' => $this->options['use_node_type'],
      '#weight' => 49,
    );
    $form['selected_node_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select node types'),
      '#options' => _viewsfilterusernodetype_node_types(),
      '#default_value' => $this->options['selected_node_types'],
      '#weight' => 50,
      // Only show the checkboxes if the "Filter by node type" option is selected.
      '#dependency' => array('edit-options-use-node-type' => array(TRUE)),
    );
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);
    $values = $form_state['values']['options'];
    // Ensures that at least one node type is selected if we want to filter by
    // node type.
    if ($values['use_node_type']
        && !array_filter($values['selected_node_types'])) {
      // Checkboxes won't be highlighted: https://www.drupal.org/node/222380
      form_error($form['selected_node_types'],
          t('Please select at least one node type if you want to filter by node type.'));
    }
  }
}

/**
 * Get all available node types.
 *
 * @return array
 *   An associative array of node types. Keys are the machine names of the node
 *   type, values are the human readable names.
 */
function _viewsfilterusernodetype_node_types() {
  $types = node_type_get_types();
  $options = array();
  foreach ($types as $type => $info) {
    $options[$type] = t($info->name);
  }
  asort($options);
  return $options;
}