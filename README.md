## Introduction

Example Islandora background process module. Adds a datastream comprised of a robots.txt to objects when they are first ingested into Islandora, and when they are updated. Developers should consult the inline comments for details.
 
See https://github.com/Islandora/Islandora-Preservation-Interest-Group/tree/master/background_services_discussion_paper for background. This module is demonstrates how a very simple "external service" as described in that document works.

## Dependencies

[Background Process Integration Framework for Islandora](https://github.com/mjordan/islandora_background_process).

## Installation

Install this module as usual; see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the URL of the robots.txt file you want to use as a datastream, and some additional options, at admin/islandora/tools/bprocessexample.

The "Additional time limit" configuration option lets you set how long to increase PHP's maximum execution time for this task.

##  Logging

Errors encountered while retrieving the robots.txt file are logged in the Drupal watchdog, as are the start and completion of the retrieval process.

