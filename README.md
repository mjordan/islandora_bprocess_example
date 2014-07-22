## Introduction

Example Islandora background process module. Adds a datastream comprised of a robots.txt to objects when they are first ingested into Islandora, and when they are updated. 
 
Inline comments in the code provide details, but see https://github.com/Islandora/Islandora-Preservation-Interest-Group/tree/master/background_services_discussion_paper for background. This module illustrates how a very simple "external service" works (in this case, the silly task of fetching a robots.txt file and adding it to an object as a datastream). Since this module extends the [Background Process Integration module](https://github.com/mjordan/islandora_background_process), you may also want to consult that module's README.

## Dependencies

[Background Process Integration Framework for Islandora](https://github.com/mjordan/islandora_background_process).

## Installation

Install this module as usual; see [this](https://drupal.org/documentation/install/modules-themes/modules-7) for further information.

## Configuration

Configure the URL of the robots.txt file you want to use as a datastream, and some additional options, at admin/islandora/tools/bprocessexample.

The "Additional time limit" configuration option lets you set how long to increase PHP's maximum execution time for the task performed by this module.

##  Logging

Errors encountered while retrieving the robots.txt file are logged in the Drupal watchdog, as are the start and completion of the retrieval process.

