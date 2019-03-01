<?php

/**
 * @file
 * Memberform definition and validation and submit functions.
 */

/**
 * Form constructor for the member form.
 */
function memberform_form($form, &$form_state, $form_name) {
  if (!empty($_GET['prefill_form'])) {
    unset($_SESSION['memberform']);
  }
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  $sid = empty($form_state['storage']['sid']) ? NULL : $form_state['storage']['sid'];
  $storage = memberform_get_storage($sid, $form_name);

  // Store storage data in form.
  $form_state['storage'] = $storage;
  $storage = &$form_state['storage'];

  // Store form name.
  $storage['form_name'] = $form_name;

  // Store settings.
  $settings_callback = 'memberform_' . $form_name . '_settings';
  $storage['pages'] = $settings_callback('pages');
  $storage['no_nav_pages'] = $settings_callback('no_nav_pages');
  $storage['final_submit_page'] = $settings_callback('final_submit_page');
  $storage['feedback_page'] = $settings_callback('feedback_page');


  // Set default page.
  if (empty($storage['current_page'])) {
    $storage['current_page'] = 'start';
    $storage['nav_history'][] = $storage['current_page'];
  }

  // Store params.
  if (empty($storage['params'])) {
    $storage['params'] = $_GET;
  }

  // Store reference.
  foreach (array('ref_campaign', 'ref_source') as $var) {
    $url_params = drupal_get_query_parameters();
    if (!empty($url_params[$var])) {
      if (empty($storage[$var])) {
        $value = $url_params[$var];
        if (preg_match('/^[a-zA-Z_\-0-9]{3,48}$/', $value)) {
          $storage[$var] = $value;
        }
      }
    }
  }

  $form_state['validate'] = TRUE;

  // Create form based on form page.
  $form = array(
    '#validate' => array(
      // This order is needed because of required elements inline message.
      'memberform_form_pre_custom_validate',
      'memberform_' . $form_name . '_page_form_validate',
      'memberform_form_post_custom_validate',
    ),
    '#submit' => array(
      'memberform_' . $form_name . '_page_form_submit',
      'memberform_form_submit',
    ),
    '#prefix' => '<div id="memberform_form_wrapper">',
    '#suffix' => '</div>',
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'memberform') . '/css/memberform.css',
      ),
      'js' => array(
        drupal_get_path('module', 'memberform') . '/js/memberform.js',
      ),
    ),
    '#attributes' => array('class' => array('memberform_' . $form_name)),
  );

  // Add header image.
  $file_path = drupal_get_path('module', 'memberform') . '/header_images/' . $storage['current_page'] . '.jpg';
  if (file_exists($file_path)) {
    global $base_url;
    $form['header'] = array(
      //'#markup' => '<div class="form_header"><img src="' . $base_url . '/' . drupal_get_path('module', 'memberform') . '/header_images/' . $storage['current_page'] . '.jpg" /></div>',
      '#markup' => '<div class="form_header"><img src="' . $base_url . '/' . drupal_get_path('module', 'memberform') . '/header_images/lilian-marijnissen-banner.jpg" /></div>',
    );
  }

  // Add form navigation.
  if ($storage['current_page'] !== $storage['feedback_page'] && count($storage['nav_history']) > 2 && !in_array($storage['current_page'], $storage['no_nav_pages'])) {
    $form['form_navigation'] = array(
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('memberform-navigation')),
    );
    $count = 0;
    $page_names_short = array(
      'intro' => 'Intro',
      'start' => 'Contactgegevens',
      'quickfinish' => 'Bel me',
      'person_details' => 'Persoonsgegevens',
      'red' => 'ROOD',
      'address' => 'Adres',
      'payment' => 'Betaling',
      'contribution' => 'Bijdrage',
      'gift' => 'Geschenk',
      'summary' => 'Controle',
    );
    foreach($storage['pages'] as $form_page => $title) {
      if (!in_array($form_page, $storage['no_nav_pages'])) {
        $title_short = $page_names_short[$form_page];
        $count++;
        $storage['navigation_order'][$count] = $form_page;
        $form['form_navigation']['go_to_' . $form_page] = array(
          '#type' => 'submit',
          '#value' => $title_short,
          '#name' => 'go_to_' . $form_page,
          '#ajax' => array(
            'wrapper' => 'memberform_form_wrapper',
            'callback' => 'memberform_ajax_callback',
          ),
          '#disabled' => !in_array($form_page, $storage['nav_history']) || $form_page === $storage['current_page'],
          '#attributes' => ($form_page === $storage['current_page']) ? array('class' => array('active')) : array(),
        );
      }
    }
  }

  // Add form title.
  if (!empty($storage['pages'][$storage['current_page']])) {
    $form['progress'] = array(
      '#markup' => '<h3>' . $storage['pages'][$storage['current_page']] . '</h3>',
    );
  }

  // Add messages.
  $output = theme_status_messages();
  if (!empty($output)) {
    $form['messages'] = array(
      '#markup' => $output,
    );
  }

  // Get pages forms.
  $form_callback = 'memberform_' . $form_name . '_page_' . $storage['current_page'] . '_form';
  if (function_exists($form_callback)) {

    // Add page specific form elements.
    $form_callback($form, $form_state);

    // Modify form elements (Add default values, error messages, rebuild required functionality).
    foreach ($form as $key => $item) {
      memberform_modify_form_elements($form[$key], $form_state, $key);
    }
  }

  // Add action buttons.
  if ($storage['current_page'] !== $storage['feedback_page'] && !empty($storage['previous_page']) && !in_array($storage['current_page'], $storage['no_nav_pages']) && count($form_state['storage']['nav_history']) > 2)  {
    $form['action']['previous_page'] = array(
      '#type' => 'submit',
      '#value' => 'Terug',
      '#name' => 'previous_page',
      '#attributes' => array('class' => array('btn hollow previous')),
      '#ajax' => array(
        'wrapper' => 'memberform_form_wrapper',
        'callback' => 'memberform_ajax_callback',
      ),
    );
  }

  if ($storage['current_page'] !== $storage['final_submit_page']) {
    $next_button_text = 'Verder';
  }
  else {
    $next_button_text = 'Insturen';
  }
  if ($storage['current_page'] !== $storage['feedback_page'] && empty($form['action']['next_page'])) {
    $form['action']['next_page'] = array(
      '#type' => 'submit',
      '#value' => $next_button_text,
      '#name' => 'next_page',
      '#attributes' => array('class' => array('btn','next')),
      '#ajax' => array(
        'wrapper' => 'memberform_form_wrapper',
        'callback' => 'memberform_ajax_callback',
      ),
    );
  }
  if (!empty($form['action'])) {
    $form['action']['#type'] = 'fieldset';
    $form['action']['#attributes'] = array('class' => array('memberform-action'));
  }

// Clear form history when on final page.
if ($storage['current_page'] === $storage['feedback_page']) {
  memberform_clear_form($form_state);
}

  return $form;
}

/**
 * Validate function for memberform.
 */
function memberform_form_pre_custom_validate($form, &$form_state) {
  $values = &$form_state['values'];
  $storage = &$form_state['storage'];

  // Trim string values.
  foreach ($values as $key => $value) {
    if (!empty($value) && is_string($value)) {
      $values[$key] = trim($value);
    }
  }

  // Implement required fields validation.
  $trigger = $form_state['triggering_element']['#name'];
  preg_match('/^go_to_([a-z_]+)$/', $trigger, $matches);
  if ($storage['current_page'] == end($storage['nav_history']) && ($trigger == 'previous_page' || !empty($matches[1]))) {
    $form_state['validate'] = FALSE;
  }
  if ($form_state['validate']) {
    foreach ($values as $key => $value) {
      if (isset($storage['required_fields'][$key])) {
        if (empty($value)) {
          form_set_error($key, 'Dit moeten we echt weten.');
        }
      }
    }
  }
}

/**
 * Validate function for memberform.
 */
function memberform_form_post_custom_validate($form, &$form_state) {
  $values = $form_state['values'];
  $storage = &$form_state['storage'];

  // Store values.
  $form_value_found = FALSE;
  foreach ($values as $key => $value) {
    $storage['values'][$storage['current_page']][$key] = $value;
    if (strpos($key, 'page_') !== FALSE && !empty($value)) {
      $form_value_found = TRUE;
    }
  }

  // Make sure form data is stored on ajax callback.
  if (empty($storage['sid']) && $form_value_found) {
    $hash = memberform_get_hash();
    $sid = db_insert('memberform_submissions')
    ->fields(array(
      'hash' => $hash,
      'created' => REQUEST_TIME,
    ))
    ->execute();
    $storage['sid'] = $sid;
    $storage['hash'] = $hash;
  }

  // Store data.
  memberform_store_form_data($storage);

  // Store submission id in session.
  $_SESSION['memberform'][$storage['form_name']]['sid'] = $storage['sid'];

  $form_state['form_errors'] = form_get_errors();
  memberform_remove_errors($form_state);
  unset($form_state['input']);
}

/**
 * Submit function for memberform.
 */
function memberform_form_submit($form, &$form_state) {

  // Rebuild form when errors occured.
  if (isset($form_state['form_errors'])) {
    $form_state['rebuild'] = TRUE;
    return;
  }

  $values = $form_state['values'];
  $storage = &$form_state['storage'];

  // Store values.
  unset($storage['values'][$storage['current_page']]);
  foreach ($values as $key => $value) {
    if (!empty($value)) {
      $storage['values'][$storage['current_page']][$key] = $value;
    }
  }

  // Get current naviation state.
  $nav = memberform_get_nav($storage);

  // Clear navigation when new next page.

  // Process previous, next page.
  $trigger = $form_state['triggering_element']['#name'];

  switch ($trigger) {
    case 'previous_page':
      if (!empty($nav['previous_page'])) {
        $storage['current_page'] = $nav['previous_page'];
      }
      break;

    case 'next_page':
    case 'call_back':
      if ($nav['next_page'] && !in_array($storage['next_page'], $storage['no_nav_pages']) && $storage['next_page'] !== $nav['next_page']) {
        $pos = array_search($storage['current_page'], $storage['nav_history']);
        $storage['nav_history'] = array_splice($storage['nav_history'], 0, $pos + 1);
      }
      $storage['current_page'] = $storage['next_page'];
      break;
  }

  // Process form navigation.
  preg_match('/^go_to_([a-z_]+)$/', $trigger, $matches);
  if (!empty($matches[1])) {
    $storage['current_page'] = $matches[1];
  }

  // Add current page to history.
  if (array_search($storage['current_page'], $storage['nav_history']) === FALSE) {
    $storage['nav_history'][] = $storage['current_page'];
  }

  // Store next and previous pages.
  $nav = memberform_get_nav($storage);
  $storage['previous_page'] = $nav['previous_page'];
  $storage['next_page'] = $nav['next_page'];

  // Store form data in database.
  memberform_store_form_data($storage);

  // Store submission id in session.
  $_SESSION['memberform'][$storage['form_name']]['sid'] = $storage['sid'];

  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax callback function for memberform.
 */
function memberform_ajax_callback($form, &$form_state) {
  $storage = $form_state['storage'];
  $trigger = $form_state['triggering_element']['#name'];
  $nav = memberform_get_nav($storage);
  $commands = array();
  $commands[] = ajax_command_html('#memberform_form_wrapper', render($form));
  switch ($trigger) {
    case 'previous_page':
    case 'next_page':
      $title = $storage['pages'][$nav['current_page']];
      $base_path = base_path();
      $settings_callback = 'memberform_' . $storage['form_name'] . '_settings';
      $form_path_name = $settings_callback('path_name');
      $page_name = pathauto_cleanstring($title);
      $path = $base_path . $form_path_name . '/' . $page_name;
      $commands[] = array(
        'command' => 'memberform_ga',
        'title'  => $title,
        'path'  => $path,
      );
      break;
  }
  return array('#type' => 'ajax', '#commands' => $commands);
}

function memberform_clear_form(&$form_state) {
  if (!empty($form_state['storage']['sid'])) {
    $number = db_delete('memberform_storage')
    ->condition('sid', $form_state['storage']['sid'])
    ->execute();
    unset($form_state['storage']['sid']);
  }
  unset($_SESSION['memberform']);
  //unset($form_state['storage']); Removed because it breaks google analytics in ajax callback.
}

function memberform_modify_form_elements(&$item, &$form_state, $key) {
  $item_org = $item;
  $storage = &$form_state['storage'];
  if (is_array($item)) {
    if (preg_match('/^page_' . $storage['current_page'] . '_[a-z0-9_]+$/', $key, $matches)) {
      // Add default values.
      if (isset($storage['values'][$storage['current_page']][$key])) {
        $item['#default_value'] = $storage['values'][$storage['current_page']][$key];
      }
      elseif (!empty($storage['params'][$key])) {
        $item['#default_value'] = $storage['params'][$key];
      }
      else {
        if (empty($item['#default_value'])) {
          $item['#default_value'] = '';
        }
      }
      // Add error messages.
      if (isset($form_state['form_errors'][$key])) {
        $item['#prefix'] = '<div class="memberform validation-error">';
        $item['#suffix'] = '<div class="messages error">' . $form_state['form_errors'][$key] . '</div></div>';
      }
      else {
        $item['#prefix'] = '<div class="memberform">';
        $item['#suffix'] = '</div>';
      }
      // Rebuild required functionality.
      if (!empty($item['#required'])) {
        unset($item['#required']);
        $item['#title'] .= ' <span class="form-required" title="This field is required.">*</span>';
        $storage['required_fields'][$key] = $key;
      }
      // Store form definition for use in other pages
      // (review page for example).
      $storage['form_element_definitions'][$key] = $item;
    }
    else {
      foreach ($item as $key2 => $item2) {
        memberform_modify_form_elements($item[$key2], $form_state, $key2);
      }
    }
  }
}

function memberform_get_nav($storage) {
  if (!empty($storage['current_page'])) {
    $current_nav_history_key = array_search($storage['current_page'], $storage['nav_history']);
    if ($current_nav_history_key !== FALSE) {
      $prev_elements = array_reverse(array_slice($storage['nav_history'], 0, $current_nav_history_key, TRUE));
      $previous_page = false;
      foreach($prev_elements as $key => $page) {
        if (array_search($page, $storage['no_nav_pages']) === FALSE) {
          $previous_page = $page;
          break;
        }
      }
      $next_elements = array_slice($storage['nav_history'], $current_nav_history_key + 1, NULL, TRUE);
      $next_page = false;
      foreach($next_elements as $key => $page) {
        if (array_search($page, $storage['no_nav_pages']) === FALSE) {
          $next_page = $page;
          break;
        }
      }
      $result = array(
        'previous_page' => $previous_page,
        'current_page' => $storage['current_page'],
        'next_page' => $next_page,
      );
      return $result;
    }
  }
  return FALSE;
}
