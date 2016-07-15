<?php
/**
 * @file
 * Ctools content type plugin file for a pane displaying a listing of events.
 */

 $plugin = array(
  'single' => TRUE,
  'title' => t('Events Listing'),
  'description' => t('Shows a listing of events using the settings for the site.'),
  'category' => t('UIowa'),
  'all contexts' => TRUE,
  'render callback' => 'events_pane_render',
  'admin info' => 'events_pane_admin_info',
  'edit form' => 'events_pane_edit_form',
  'defaults' => array(
    'num_of_events' => '5',
  ),
);

/**
 * The 'admin info' callback for panel pane.
 */
function events_pane_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Events listing for department/unit');
    return $block;
  }
}

/**
 * The 'Edit form' callback for the content type.
 */
function events_pane_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  // Set the height of the map.
  $form['num_of_events'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Events'),
    '#description' => t('Set the number of events to show'),
    '#default_value' => $conf['num_of_events'],
  );
  return $form;
}

/**
 * The submit form stores the data in $conf.
 */
function events_pane_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Run-time rendering of the body of the block (content type).
 *
 * See ctools_plugin_examples for more advanced info.
 */
function events_pane_render($subtype, &$conf, $panel_args, $context) {
  $num_of_events = $conf['num_of_events'];
  $events = uiowa_events_pane_listing($num_of_events);

  $events_render = '';

  if(isset($events['data'])){
    $events_render .= "<ul>";

    foreach($events['data'] as $event){
      $events_render .= '<li class="event">';
      $events_render .= '<div class="event-date">'.date('M',strtotime($event['start'])).'<br />'.date('j',strtotime($event['start'])).'</div>';
      $events_render .= '<div class="event-title"><a href="/event/'.$event['id'].'"">'.$event['title'].'</a></div>';
      $events_render .= '<div class="event-time">'.date('g',strtotime($event['start'])).':'.date('i',strtotime($event['start'])).' '.date('A',strtotime($event['start'])).'</div>';
      $events_render .= '<div class="event-location">'.$event['location'].' '.$event['room_number'].'</div>';
      $events_render .= '</li>';
    }

    $events_render .= "</ul>";
  }


  $block = new stdClass();
  $block->title = t('@title', array('@title' => "Events"));
  $block->content = array(
    '#prefix' => '<div class="events-pane">',
    '#markup' => $events_render,
    '#suffix' => '</div>',
  );
  return $block;
}
