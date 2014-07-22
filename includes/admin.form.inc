<?php

/**
 * @file
 * The admin form for the Islandora Background Process Example module.
 */

/**
 * Admin settings form builder.
 */
function islandora_bprocess_example_admin_settings() {
  $form['islandora_bprocess_example_robots_txt_url'] = array(
    '#title' => t('URL to the robots.txt you want to add as a datastream.'),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('islandora_bprocess_example_robots_txt_url', 'http://google.com/robots.txt'),
    '#description' => t('The full path to the robots.txt file.'),
    '#required' => TRUE,
  );
  $form['islandora_bprocess_example_robots_txt_dsid'] = array(
    '#title' => t('Datastream ID to use for the robots.txt file.'),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('islandora_bprocess_example_robots_txt_dsid', 'ROBOTS'),
    '#required' => TRUE,
  );
  $form['islandora_bprocess_example_robots_txt_dslabel'] = array(
    '#title' => t('Datastream label to use for the robots.txt file.'),
    '#type' => 'textfield',
    '#size' => 60,
    '#default_value' => variable_get('islandora_bprocess_example_robots_txt_dslabel', 'Robots.txt file'),
    '#required' => TRUE,
  );
  $form['islandora_bprocess_example_additional_time_limit'] = array(
    '#title' => t('Additional time limit'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => variable_get('islandora_bprocess_example_additional_time_limit', '30'),
    '#description' => t('Number of seconds to pass to <a href="@drupal_set_time_limit">drupal_set_time_limit</a>.
      Leave blank to not add additional time.', array("@drupal_set_time_limit" =>
      'https://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_set_time_limit/7')),
  );
  $form['islandora_bprocess_example_max_attempts'] = array(
    '#title' => t('Maximum attempts'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => variable_get('islandora_bprocess_example_max_attempts', '3'),
    '#description' => t('Maximum number of attempts to retrieve the robots.txt file.'),
    '#required' => TRUE,
  );
  $form['islandora_bprocess_example_target_cmodels'] = array(
    '#title' => t('Target content models'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => islandora_background_process_get_collections_cmodels('info:fedora/fedora-system:ContentModel-3.0'),
    '#default_value' => variable_get('islandora_bprocess_example_target_cmodels', array()),
    '#description' => t('Apply this service to objects with any of these content models.'),
    '#required' => TRUE,
  );
  $form['islandora_bprocess_example_target_collections'] = array(
    '#title' => t('Target collections'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => islandora_background_process_get_collections_cmodels('info:fedora/islandora:collectionCModel'),
    '#default_value' => variable_get('islandora_bprocess_example_target_collections', array()),
    '#description' => t('Apply this service to objects in any of these collections.'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}



