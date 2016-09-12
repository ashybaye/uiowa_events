<?php
/**
 * @file
 * Full listing theme functions.
 */

/**
 * Create full listing page.
 *
 * @param array &$variables
 *   Array reference of arguments given to the theme() function.
 */
function template_preprocess_uiowa_events_full_listing(&$variables) {
  drupal_set_title($variables['title']);

  $variables['add_link'] = '';

  if (variable_get('uiowa_events_add_event_link')) {
    $variables['add_link'] = array(
      '#theme' => 'link',
      '#text' => t('Add Your Event'),
      '#path' => 'http://events.uiowa.edu/event/create',
      '#options' => array(
        'attributes' => array(
          'class' => array('uiowa-events-add-link'),
          'target' => '_blank',
        ),
      ),
    );
  }

  $variables['nav'] = theme('uiowa_events_full_listing_date_nav',
    array(
      'date' => $variables['data'],
      'nav_items' => _uiowa_events_nav_control($variables['length'], $variables['date']),
    )
  );

  $external_link = FALSE;
  if (variable_get('uiowa_events_event_link') == 'event-link-external') {
    $external_link = TRUE;
  }

  foreach ($variables['data'] as $e) {
    if (variable_get('uiowa_events_filter_past') == TRUE) {
      $filter_date = isset($e['end']) ? $e['end'] : $e['start'];

      // Skip this event if it occurred in the past.
      if (strtotime($filter_date) < strtotime('midnight')) {
        continue;
      }
    }

    $variables['list'][] = array(
      '#theme' => 'uiowa_events_teaser',
      '#event' => $e,
      '#external_link' => $external_link,
    );
  }
}

/**
 * Theme function for full listing date navigation.
 *
 * @param array $variables
 *    Variables from the event data.
 */
function theme_uiowa_events_full_listing_date_nav($variables) {
  $nav = array();

  $links = array(
   'previous' => array(
     'title' => '<span aria-hidden="true">&laquo;</span>',
     'href' => $variables['nav_items']['previous'],
     'html' => TRUE,
     'attributes' => array(
       'aria-label' => 'Previous',
     ),
   ),
   'active' => array(
     'title' => $variables['nav_items']['label'],
     'attributes' => array(
       'aria-label' => 'Previous',
     ),
   ),
   'next' => array(
     'title' => '<span aria-hidden="true">&raquo;</span>',
     'href' => $variables['nav_items']['next'],
     'html' => TRUE,
     'attributes' => array(
       'aria-label' => 'next',
     ),
   ),
  );

  $nav['pagination'] = array(
    '#theme' => 'links',
    '#links' => $links,
    '#attributes' => array(
      'class' => array('pagination'),
    ),
  );

  return $nav;
}
