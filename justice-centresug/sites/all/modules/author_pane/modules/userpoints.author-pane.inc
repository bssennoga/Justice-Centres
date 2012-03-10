<?php
/**
 * @file
 * This file provides a preprocess function on behalf of the User Points module.
 */

/**
 * Implements hook_preprocess_author_pane().
 */
function userpoints_preprocess_author_pane(&$variables) {
  // Check if this preprocess needs to be run given who's calling it.
  if (!author_pane_run_preprocess('userpoints', $variables['caller'])) {
    return;
  }

  // Anonymous users can not have user points and also if
  // userpoints_get_current_points() gets $uid = 0, it returns the points
  // of the global $user.
  if ($variables['account']->uid != 0) {
  	// Total of amount of points (userpoints_points) is static cached in
  	// userpoints_get_current_points() so no need to use static cache for that.
  	// But amount of points for each category (userpoints_categories) is also
  	// cached in userpoints_get_current_points() but to save count(userpoints_get_categories())
  	// function calls to userpoints_get_current_points() a static cache is used.
    $userpoints_categories_cache = &drupal_static(__FUNCTION__, array());

    if (!isset($userpoints_categories_cache[$variables['account']->uid])) {
      $userpoints_categories_cache[$variables['account']->uid] = array();
      foreach (userpoints_get_categories() AS $tid => $category) {
        $userpoints_categories_cache[$variables['account']->uid][$category] = userpoints_get_current_points($variables['account']->uid, $tid);
      }
    }

    $variables['userpoints_points'] = userpoints_get_current_points($variables['account']->uid, 'all');
    $variables['userpoints_categories'] = $userpoints_categories_cache[$variables['account']->uid];
  }
}

/**
 * Implements hook_author_pane_allow_preprocess_disable().
 */
function userpoints_author_pane_allow_preprocess_disable() {
  return array('userpoints' => 'User Points');
}
