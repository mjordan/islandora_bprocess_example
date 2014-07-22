<?php

/**
 * @file
 * Class that illustrates how to extend the abstract class provided by the
 * Islandora Background Process module.
 */

class IslandoraBprocessExample extends IslandoraBackgroundProcess {
  // Members of the $hooks property should be the same strings used as the function
  // names which implements the hooks that this process should be executed in. All
  // classes that extend IslandoraBackgroundProcess should have this property.
  public $hooks = array(
    'islandora_background_process_islandora_object_ingested',
    'islandora_background_process_islandora_object_modified',
  );

  // Counter for number of attempts to perform the work() method before giving up.
  // All classes that extend IslandoraBackgroundProcess should have this property.
  static $numAttempts = 0;

  public function __construct() {
    // Call IslandoraBackgroundProcess's contructor to register the IslandoraBprocessExample
    // object in its list of children. All classes that extend IslandoraBackgroundProcess
    // should call the parent's constructor in this way.
    parent::__construct();
  }

  // All classes that extend IslandoraBackgroundProcess run their
  // code in their work() method. Note: even though hook_islandora_object_ingested()
  // doesn't take a $datastream parameter, we need it here so the work() method is
  // compatible with the work() method in the IslandoraBackgroundProcess abstract
  // class.
  public function work($islandora_object = NULL, $datastream = NULL) {
    $dsid = variable_get('islandora_bprocess_example_robots_txt_dsid', 'ROBOTS');
    $pid_dsid_string = $islandora_object->id . "/" . $dsid; 
    $ds_label = variable_get('islandora_bprocess_example_robots_txt_dslabel', 'Robots.txt file');

    // Check to make sure that the we only perform the work on object that have the
    // configured content models.
    $has_target_cmodel = FALSE;
    $wanted_models = variable_get('islandora_bprocess_example_target_cmodels', array());
    $object_content_models = $islandora_object->relationships->get(FEDORA_MODEL_URI, 'hasModel');
    foreach ($object_content_models as $cmodel) {
      if (($cmodel['predicate']['value'] == 'hasModel') &&
        array_key_exists($cmodel['object']['value'], $wanted_models)) {
        $has_target_cmodel = TRUE;
      }
    }

    // Check to make sure that the we only perform the work on members of the configured
    // collections.
    $in_target_collection = FALSE;
    $wanted_collections = variable_get('islandora_bprocess_example_target_collections', array());
    $object_collections = $islandora_object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOfCollection');
    foreach ($object_collections as $collection) {
      if (($collection['predicate']['value'] == 'isMemberOfCollection') &&
        array_key_exists($collection['object']['value'], $wanted_collections)) {
        $in_target_collection = TRUE;
      }
    }

    if ($has_target_cmodel && $in_target_collection) {
      // Check to see if we've reached our maximum number of attempts. If we have, log the
      // condition and exit. All classes the extend IslandoraBackgroundProcess should
      // increment their numAttempts property like this and check to make sure it does
      // not exceed the configured maximum.
      $this->numAttempts++;
      if ($this->numAttempts > (int) variable_get('islandora_bprocess_example_max_attempts', '3')) {
        islandora_background_process_log("Reached maximum number of attempts trying to perfom work
          for %item, so quiting", array('%item' => $pid_dsid_string));
        exit;
      }

      // Copy $islandora_object to use when we retry work() on error. All classes that extend
      // IslandoraBackgroundProcess should should clone the incoming Islandora object for use
      // in retries, since it is altered by having datastreams added later in this method.
      $islandora_object_copy = clone $islandora_object;

      islandora_background_process_log("Starting remote process for datastream %item",
        array('%item' => $pid_dsid_string), WATCHDOG_INFO);

      $time_pre = microtime(true);
      // Pause execution to simulate a long-running processes.
      $sleep = variable_get('islandora_bprocess_example_sleep_period', '30');
      sleep($sleep);

      // In this example module, we retrieve the robots.txt file at the specified location and
      // add it to the object as a datastream.
      $robots_txt_url = variable_get('islandora_bprocess_example_robots_txt_url', 'http://google.com/robots.txt');
      // We don't use any $options in this example.
      $robots_txt_options = array();
      $robots_txt_result = drupal_http_request($robots_txt_url, $robots_txt_options);
      if (!$robots_txt_result || $robots_txt_result->code != '200') {
        islandora_background_process_log("Retrieval of robots.txt failed on attempt %attempt for %item",
          array('%attempt' => $this->numAttempts, '%item' => $pid_dsid_string));
        // Retry the work() method, using the copy of the Islandora object.
        $this->work($islandora_object_copy);
      }

      // Add the datastream to the object.
      $robots_txt_ds = isset($islandora_object[$dsid]) ? $islandora_object[$dsid] : $islandora_object->constructDatastream($dsid);
      $robots_txt_ds->label = $ds_label;
      $robots_txt_ds->mimeType = 'text/plain';
      $robots_txt_ds->setContentFromString($robots_txt_result->data);
      try {
        $islandora_object->ingestDatastream($robots_txt_ds);
      }
      catch (Exception $e) {
        islandora_background_process_log("Failed to add robots.txt datastream for %item: %error",
          array('%item' => $pid_dsid_string, '%error' => $e->getMessage()));
      }

      // Log completion time.
      $time_post = microtime(true);
      $exec_time = $time_post - $time_pre;
      islandora_background_process_log("Finished remote process for datastream %item, which took %seconds seconds to complete",
        array('%item' => $pid_dsid_string, '%seconds' => $exec_time), WATCHDOG_INFO);
    }
  }
}
