<?php

/**
 * @file Helperfunctions for memberform module.
 */

/**
 * Removes error messages, and stores errors in form state.
 */
function memberform_remove_errors(&$form_state) {
  // Remove form error messages.
  if (!empty($form_state['form_errors'])) {
    $errors = $form_state['form_errors'];
    $error_messages = &$_SESSION['messages']['error'];
    if (!empty($error_messages)) {
      foreach ($errors as $error) {
        if (($key = array_search($error, $error_messages)) !== FALSE) {
          unset($error_messages[$key]);
          if (empty($error_messages)) {
            unset($_SESSION['messages']['error']);
          }
        }
      }
    }
    // Store errors for display in form elements.
    $form_state['form_errors'] = $errors;
    // Get the array of errors and reset them so form will be submitted.
    $errors = &drupal_static("form_set_error", array());
    // Make sure that each error should be applied to the field.
    foreach (array_keys($errors) as $field) {
      unset($errors[$field]);
    }
  }
}

/**
 * Gets stored form data from database.
 *
 * @param int $sid
 *   Form submission id.
 *
 * @return array $storage
 *   Stored form data.
 */
function memberform_get_storage($sid = NULL, $form_name = '') {
  $storage = array();
  if (!isset($sid) && isset($_SESSION['memberform'][$form_name]['sid'])) {
    $sid = $_SESSION['memberform'][$form_name]['sid'];
  }

  if (isset($sid)) {
    // Get storage from database if available.
    $query = "SELECT form_values FROM {memberform_storage} WHERE sid = :sid";
    $result = db_query($query, array(':sid' => $sid))->fetchAssoc();
    if (!empty($result['form_values'])) {
      $storage = unserialize($result['form_values']);
      $storage['sid'] = $sid;
    }
  }
  return $storage;
}

/**
 * Stores database storage variable in database.
 *
 * @param array $storage
 *   Database storage variable.
 */
function memberform_store_form_data(array &$storage = NULL) {
  if (isset($storage['sid'])) {
    // Update database storage.
    db_merge('memberform_storage')
    ->key(array('sid' => $storage['sid']))
    ->fields(array(
      'form_values' => serialize($storage),
    ))
    ->execute();
  }
}

/**
 * Return the names of the different forms defined in /etc/pages.
 *
 * @return array
 *   List of pagenames.
 */
function memberform_get_formnames() {
  $paths = glob(drupal_get_path('module', 'memberform') . '/multipage_forms/*');
  foreach ($paths as $path) {
    $result = preg_match('/^.*\/memberform_([^.]*)_pages.inc$/', $path, $matches);
    if (!empty($matches[1])) {
      $form_names[] = $matches[1];
    }
  }
  return $form_names;
}

/**
 * Submit pending submissions.
 */
function memberform_submit_pending() {

  $pending = db_select('memberform_submissions', 's')
    ->fields('s')
    ->isNotNULL('verified')
    ->isNULL('submitted')
    ->execute();

  if ($pending) {
    while ($record = $pending->fetchAssoc()) {

      try {
        $contact_id = memberform_add_membership($record);
        $error = NULL;
      }
      catch (\Exception $e) {
        $error = $e->getMessage();
        $contact_id = NULL;
      }

      db_update('memberform_submissions')
        ->fields([
          'submitted'  => time(),
          'errormsg'   => $error,
          'contact_id' => $contact_id,
        ])
        ->condition('id', $record['id'])
        ->execute();
    }
  }
  return TRUE;
}

/**
 * Discription missing.
 */
function memberform_friendly_field_data($key, $value, $activity = FALSE) {
  $value = check_plain($value);
  switch ($key) {
    case 'hash':
      return [NULL, NULL];

    case 'errormsg':
      if ($value != '') {
        return ['Error', '<span style="color:#f00;font-weight:bold;">' . $value . '</span>'];
      }
      return [NULL, NULL];

    case 'id':
      return ['Website-ID', $value];

    case 'membership_type':
      return ['Lidmaatschapstype', memberform_membership_description($value, TRUE)];

    case 'initials':
      return ['Voorletters', $value];

    case 'firstname':
      return ['Voornaam', $value];

    case 'middlename':
      return ['Tussenvoegsel', $value];

    case 'lastname':
      return ['Achternaam', $value];

    case 'gender':
      return ['Geslacht', strtoupper($value)];

    case 'birthdate':
      return ['Geboortedatum', $value];

    case 'email':
      return ['E-mail', $value];

    case 'phone':
      return ['Telefoon', $value];

    case 'phone_mobile':
      return ['Mobiel', $value];

    case 'country':
      return ['Land', $value];

    case 'zipcode':
      return ['Postcode', $value];

    case 'housenumber':
      return ['Huisnummer', $value];

    case 'housenumber_suffix':
      return ['Huisnummertoevoeging', $value];

    case 'street':
      return ['Straat', $value];

    case 'city':
      return ['Plaats', $value];

    case 'contribution':
      return ['Bijdrage', number_format($value, 2, ',', '.')];

    case 'bank_account':
      return ['IBAN', $value];

    case 'payment_method':
      return ['Betaalwijze', ucfirst($value)];

    case 'welcome_present_sp':
      return ['Welkomstcadeau SP', memberform_get_gift_internal_name($value)];

    case 'welcome_present_rood':
      return ['Welkomstcadeau ROOD', memberform_get_gift_internal_name($value)];

    case 'source':
      return ['Bron', $value];

    case 'created':
      return ['Aangemeld', $value ? date('d-m-Y H:i:s', $value) : '-'];

    case 'flags':
      if ($value) {
        return ['Type', $value == 'INTERN' ? '<em>Interne aanmelding</em>' : ''];
      }
      return [NULL, NULL];

    case 'verified':
      return ['Bevestigd', $value ? date('d-m-Y H:i:s', $value) : '-'];

    case 'reminded':
      return ['Herinnerd', $value ? date('d-m-Y H:i:s', $value) : '-'];

    case 'submitted':
      return ['Toegevoegd', $value ? date('d-m-Y H:i:s', $value) : '-'];

    case 'ref_campaign':
      return ['Ref: campagne', $value];

    case 'ref_source':
      return ['Ref: bron', $value];

    case 'contact_id':
      if ($value > 0 && !$activity) {
        return ['CiviCRM-ID', $value . '&nbsp;&nbsp;&nbsp; <a href="' . variable_get('spciviapi_civicrm_server') . '/civicrm/contact/view?reset=1&cid=' . $value . '">Bekijk contact in CiviCRM</a>'];
      }
      return [NULL, NULL];
  }
}

/**
 * Missing description.
 */
function memberform_add_membership($data = []) {

  // Create contact.
  $contact_data = [
    'contact_type' => 'Individual',
    'first_name'   => $data['firstname'],
    'middle_name'  => $data['middlename'],
    'last_name'    => $data['lastname'],
    'birth_date'   => $data['birthdate'],
    'gender'       => $data['gender'],
    'initials'     => $data['initials'],
    'source'       => 'Lidworden formulier',
    'street'       => $data['street'],
    'house_number' => $data['housenumber'],
    'house_number_addition'=> $data['housenumber_suffix'],
    'locality'             => $data['city'],
    'postal_code'          => $data['zipcode'],
    'country'              => $data['country'],
    'email'                => $data['email'],
    'telephone'            => $data['phone'],
    'phone_mobile'         => $data['phone_mobile'],
    'overwrite'            => 1,
  ];

  $contact_id = spcivipush_push_contact($contact_data, 'Lidworden formulier');

  if ($contact_id !== FALSE && $contact_id !== 'insufficient data') {
    // Get contact ID and location type ID.
    $location_type_id = memberform_get_location_type_id();

    // Get civiapi api.
    $spcivi = \SPCivi::getInstance();

    // Add membership.
    switch ($data['membership_type']) {

      case 'lid_sp':
        $membership_type_id = variable_get('memberform_id_lid_sp');
        $financial_type_id = variable_get('memberform_id_fintype_sp');
        $contribution = (float) $data['contribution'];
        if ($data['payment_method'] != 'incasso') {
          $contribution += 1;
        }
        break;

      case 'lid_sprood':
        $membership_type_id = variable_get('memberform_id_lid_sprood');
        $financial_type_id = variable_get('memberform_id_fintype_sprood');
        $contribution = (float) $data['contribution'];
        if ($data['payment_method'] != 'incasso') {
          $contribution += 1;
        }
        break;

      case 'lid_rood':
        $membership_type_id = variable_get('memberform_id_lid_rood');
        $financial_type_id = variable_get('memberform_id_fintype_rood');
        $contribution = round((float) $data['contribution'] / 4, 2);
        break;

      case 'tribune':
        $membership_type_id = variable_get('memberform_id_abo_tribune');
        $financial_type_id = variable_get('memberform_id_fintype_tribune');
        $contribution = ($data['payment_method'] == 'incasso') ? 5 : 6;
        break;

      case 'spanning':
        $membership_type_id = variable_get('memberform_id_abo_spanning');
        $financial_type_id = variable_get('memberform_id_fintype_spanning');
        $contribution = 6;
        break;

      default:
        throw new \Exception('CiviCRM API error: invalid membership type: ' . $data['membership_type'] . '.');
    }

    if ($data['payment_method'] == 'incasso') {
      $payment_instrument = variable_get('memberform_id_payment_incasso');
    }
    else {
      $payment_instrument = variable_get('memberform_id_payment_accept');
    }

    $membership_data = [
      'contact_id'             => $contact_id,
      'membership_type_id'     => $membership_type_id,
      'membership_start_date'  => date('Ymdhis', $data['verified']),
      'join_date'              => date('Ymdhis', $data['verified']),
      'is_override'            => 1,
      'status_id'              => variable_get('memberform_id_status'),
      'total_amount'           => number_format($contribution, 2, '.', ''),
      'financial_type_id'      => $financial_type_id,
      'contribution_status_id' => 2,
      'payment_instrument_id'  => $payment_instrument,
      'new_mandaat'            => 0,
      'iban'                   => $data['bank_account'],
    ];

    // Get bic from iban.
    if (!empty($data['bank_account'])) {
      $result = $spcivi->api('Bic', 'getfromiban', array(
        'sequential' => 1,
        'iban' => $data['bank_account'],
      ));
      if (!$result || $result['is_error']) {
        throw new \Exception('CiviCRM API error (Bic): ' . $result['error_message']);
      }
      if (isset($result['bic'])) {
        $membership_data['bic'] = $result['bic'];
      }
    }

    if ($data['payment_method'] == 'incasso' && $data['bank_account']) {
      $membership_data['new_mandaat'] = 1;
      $membership_data['mandaat_status'] = 'RCUR';
      $membership_data['mandaat_datum'] = date('Ymdhis', $data['verified']);
      $membership_data['mandaat_plaats'] = ($data['city'] ? $data['city'] : '[Onbekend]');
    }

    if (in_array($data['membership_type'], ['lid_sp', 'lid_sprood'])) {
      $sp_gift_field = variable_get('memberform_id_custom_gift_sp');
      $membership_data[$sp_gift_field] = memberform_get_gift_internal_name($data['welcome_present_sp']);

      $sp_gift_field_received = variable_get('memberform_id_custom_gift_sp_received');
      $membership_data[$sp_gift_field_received] = 0;
    }
    if (in_array($data['membership_type'], ['lid_sprood', 'lid_rood'])) {
      $rood_gift_field = variable_get('memberform_id_custom_gift_rood');
      $membership_data[$rood_gift_field] = memberform_get_gift_internal_name($data['welcome_present_rood']);

      $rood_gift_field_received = variable_get('memberform_id_custom_gift_rood_received');
      $membership_data[$rood_gift_field_received] = 0;
    }

    $membership_source_field = variable_get('memberform_id_custom_source_membership');
    if ($data['source']) {
      $membership_data[$membership_source_field] = $data['source'];
    }
    else {
      $membership_data[$membership_source_field] = memberform_get_source_website();
    }

    $gezinslid_field = variable_get('memberform_id_custom_gezinslid');
    $membership_data[$gezinslid_field] = 0;
    $welkomstbrief_field = variable_get('memberform_id_custom_welkomstbrief');
    $membership_data[$welkomstbrief_field] = 0;

    $membership_ref_source_field = variable_get('memberform_id_custom_lidmaatschap_referentie_bron');
    $membership_data[$membership_ref_source_field] = $data['ref_source'];

    $membership_ref_campaign_field = variable_get('memberform_id_custom_lidmaatschap_referentie_campagne');
    $membership_data[$membership_ref_campaign_field] = $data['ref_campaign'];

    $res = $spcivi->api('Membership', 'spcreate', $membership_data);

    if (!$res || $res['is_error']) {
      throw new \Exception('CiviCRM API error (Membership): ' . $res['error_message'] . ' ' . print_r($membership_data, TRUE));
    }

    // Add activity record.
    $data['submitted'] = time();
    $activity_data = '';
    foreach ($data as $key => $value) {
      list($key, $value) = memberform_friendly_field_data($key, $value, TRUE);
      if ($key) {
        $activity_data .= $key . ': ' . $value . '<br />';
      }
    }

    $params = [
      'source_contact_id'  => variable_get('memberform_id_activity_contact'),
      'target_contact_id'  => $contact_id,
      'activity_name'      => variable_get('memberform_id_activity'),
      'activity_subject'   => 'Aanmelding als ' . memberform_membership_description($data['membership_type']),
      'activity_date_time' => date('YmdHis', $data['verified']),
      'details'            => 'De volgende aanmelding is via de website ontvangen op ' . date('d-m-Y H:i:s', $data['created']) . ' en bevestigd op ' . date('d-m-Y H:i:s', $data['verified']) . ':<br /><br />' . $activity_data,
    ];
    $res = $spcivi->api('Activity', 'create', $params);
    if (!$res || $res['is_error']) {
      throw new \Exception('CiviCRM API error (Activity): ' . $res['error_message']);
    }

    // Add note.
    if (!empty($data['note'])) {
      memberform_create_note($contact_id, $data['note']);
    }

    return $contact_id;
  }
}

/**
 * Description missing.
 */
function memberform_membership_description($membership_type, $short = FALSE) {
  switch ($membership_type) {
    case 'lid_sp':
      return ($short ? 'SP' : 'lid van de SP');

    case 'lid_sprood':
      return ($short ? 'SP + ROOD' : 'lid van de SP en ROOD');

    case 'lid_rood':
      return ($short ? 'ROOD' : 'lid van ROOD');

    case 'tribune':
      return ($short ? 'Tribune' : 'abonnee van de Tribune');

    case 'spanning':
      return ($short ? 'Spanning' : 'abonnee van Spanning');

    default:
      return '[' . ($short ? 'onbekend' : 'onbekend lidmaatschap') . ']';
  }
}

/**
 * Description missing.
 */
function memberform_get_gift_internal_name($gift_id) {
  return db_select('memberform_gifts', 'sg')
    ->fields('sg', ['internal_name'])
    ->condition('id', $gift_id, '=')
    ->execute()
    ->fetchField(0);
}

/**
 * Herinneringsmails sturen aan aanmelders die nog niet bevestigd hebben.
 */
function memberform_send_reminders() {

  $unverified = db_select('memberform_submissions', 's')
    ->fields('s')
    ->condition('created', time() - 86400, '<')
    ->condition('form_page', 'summary')
    ->isNotNull('email')
    ->isNull('verified')
    ->isNull('reminded')
    ->execute();

  if ($unverified) {

    while ($record = $unverified->fetchAssoc()) {

      $mail = drupal_mail(
        'memberform', 'reminder', $record['email'], LANGUAGE_NONE,
        [
          'hash'            => $record['hash'],
          'membership_type' => $record['membership_type'],
          'format'          => SWIFTMAILER_FORMAT_HTML,
        ]
      );

      if (!empty($mail['send'])) {

        db_update('memberform_submissions')
          ->fields([
            'reminded' => time(),
          ])
          ->condition('id', $record['id'])
          ->execute();
      }
    }
  }
}

/**
 * Defines e-mail.
 */
function memberform_get_mail(&$message, $params) {

  if (!$params['hash'] || !$params['membership_type'] || !$params['form_state']) {
    throw new \Exception('Params hash, membership_type or form_state missing.');
  }

  $table = memberform_overview_table($params['form_state']);
  $descr = memberform_membership_description($params['membership_type']);

  global $base_url;
  $validate_url = $base_url . '/aanmelden/bevestig/' . $params['hash'];

  $message['subject'] = 'Bevestig je aanmelding als ' . $descr;
  $message['body'] = [];
  $message['body'][] = '<p>Bedankt voor je aanmelding als ' . $descr . '!<br />Klik op onderstaande link om je aanmelding te bevestigen. Als je niets doet wordt je aanmelding vanzelf verwijderd.</p>';
  $message['body'][] = '<p>' . l($validate_url, $validate_url) . '</p>';
  $message['body'][] = $table;
  $message['body'][] = memberform_get_qline();
}

/**
 * Defines reminder mail.
 */
function memberform_get_reminder_mail(&$message, $params) {

  if (!$params['hash'] || !$params['membership_type']) {
    throw new \Exception('Params hash or membership_type missing.');
  }

  $descr = memberform_membership_description($params['membership_type']);

  global $base_url;
  $validate_url = $base_url . '/aanmelden/bevestig/' . $params['hash'];

  $message['subject'] = 'Herinnering: bevestig je aanmelding als ' . $descr;
  $message['body'] = [];
  $message['body'][] = '<p>Gisteren heb je je aangemeld als ' . $descr . '.<br />Je hebt je aanmelding echter nog niet bevestigd. Klik op onderstaande link om je aanmelding te bevestigen en deze door te geven aan de SP-ledenadministratie.</p>';
  $message['body'][] = '<p>' . l($validate_url, $validate_url) . '</p>';
  $message['body'][] = memberform_get_qline();
}

/**
 * Defines call back mail.
 */
function memberform_get_call_back_mail(&$message, $params) {

  if (!$params['hash']) {
    throw new \Exception('Params hash missing.');
  }

  $descr = memberform_membership_description($params['membership_type']);

  global $base_url;
  $intern_form_url = $base_url . '/aanmelden/intern/' . $params['form_state']['storage']['sid'];

  $name = $params['form_state']['storage']['values']['start']['page_start_name'];
  $phone = $params['form_state']['storage']['values']['start']['page_start_phone'];
  $message['subject'] = 'Lid worden formulier terugbellen ' . $name . ': ' . $phone;
  $message['body'] = [];
  $message['body'][] = '<p>' . $name . ' heeft zich aangemeld als ' . $descr . ' en wil teruggebeld worden om e.e.a. af te ronden.</p>';
  $message['body'][] = '<p><strong>' . $descr . '<br/>' . $name  . '<br/>' . $phone . '</strong></p>';
  $message['body'][] = '<p>' . l('Afronden aanmelding', $intern_form_url, array('target' => '_blank')) . '</p>';
  $message['body'][] = '<p>(automatisch gegenereerd bericht)</p>';
}

/**
 * Beschrijving ontbreekt.
 */
function memberform_get_qline() {
  return '<p>Voor verdere vragen over je aanmelding kun je contact opnemen met de
  ledenadministratie: 088-243 55 40 of <a href="mailto:administratie@sp.nl">administratie@sp.nl</a>.</p>';
}

/**
 * Haal de ID van de bron Website op.
 */
function memberform_get_source_website() {

  $sources = memberform_get_source_options();
  if (in_array('Website', $sources)) {
    return array_search('Website', $sources);
  }
  return '';
}

/**
 * Haal een lijst van mogelijke bronnen op (quick and dirty uit de database).
 */
function memberform_get_source_options() {

  $source_field = variable_get('memberform_id_custom_source');
  $q = db_select('civicrm_option_value', 'cov')
    ->fields('cov', ['label', 'value'])
    ->condition('ccf.id', str_replace('custom_', '', $source_field), '=')
    ->orderBy('cov.weight');
  $q->join('civicrm_custom_field', 'ccf', 'ccf.option_group_id=cov.option_group_id');
  $sources = $q->execute()->fetchAll();

  $ret = [];
  if ($sources) {
    $ret[] = 'Kies...';
    foreach ($sources as $s) {
      $ret[$s->value] = $s->label;
    }
  }

  return $ret;
}

/**
 * Creates note.
 */
function memberform_create_note($contact_id, $note) {
  $note_field = variable_get('memberform_id_custom_reden_lidmaatschap', 'custom_218');
  $api = spciviapi_get_api();
  $params = array(
    'version' => 3,
    'entity_id' => $contact_id,
    $note_field => $note,
  );
  $api->CustomValue->create($params);
  $result = $api->result();
  if (!$result || $result->is_error || !$result->values) {
    drupal_set_message('Er is iets misgegaan met het opslaan van de informatie. Neem contact op met webmaster@sp.nl.', 'error');
    throw new \Exception('CiviCRM API error (Note): ' . $result->error_message);
  }
}

/**
 * Stores note.
 */
function memberform_store_note($hash, $note) {
  $num = db_update('memberform_submissions')
    ->fields(array(
      'note' => $note,
    ))
    ->condition('hash', $hash)
    ->execute();
}

/**
 * Haal de ID op die bij het locatietype 'Thuis' hoort.
 */
function memberform_get_location_type_id() {

  return memberform_cache_data('memberform_location_type', function () {

    $api = spciviapi_get_api();
    try {
      $api->LocationType->Get([
        'name' => 'Thuis',
      ]);
      $lt = $api->result();
      if ($lt) {
        return $lt->id;
      }
    }
    catch (\Exception $e) {
      return FALSE;
    }
  });
}

/**
 * Cache data.
 */
function memberform_cache_data($cache_name, $callback, $limit = 28800) {
  $data = cache_get($cache_name);
  if ($data) {
    return $data->data;
  }

  if (is_callable($callback)) {
    $data = call_user_func($callback);
    if ($data) {
      cache_set($cache_name, $data, 'cache', $limit);
    }
    return $data;
  }
  return FALSE;
}

function memberform_memberships() {
  $memberships = array(
    'lid_sp' => 'SP',
    'lid_rood' => 'ROOD',
    'tribune' => 'Tribune',
    'spanning' => 'Spanning',
  );
  return $memberships;
}
