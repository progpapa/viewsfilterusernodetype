<?php

/**
 * @file
 * Views customization for the viewsfilterusernodetype module.
 */

/**
 * Implements hook_views_data_alter().
 */
function viewsfilterusernodetype_views_data_alter(&$data) {
  $data['users']['uid']['filter']['handler'] = 'viewsfilterusernodetype_handler_filter';
}