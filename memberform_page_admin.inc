<?php

/**
 * Admin forms.
 */

/**
 * Civicrm settings form definition.
 */
function memberform_admin_settings_form() {

  $form = array();

  $form['memberform_settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => '<p>Algemene instellingen</p>',
  );

  $image_styles = image_style_options();
  $form['memberform_settings_general']['memberform_gift_image_style'] = array(
    '#type' => 'select',
    '#title' => 'Kies de afbeeldingsstijl voor de welkomstgeschenken',
    '#options' => $image_styles,
    '#default_value' => variable_get('memberform_gift_image_style', 'thumbnail'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm'] = array(
    '#type' => 'fieldset',
    '#title' => '<p>CiviCRM instellingen.</p>',
  );

  $form['memberform_settings_civicrm']['memberform_id_lid_sp'] = array(
    '#type'          => 'textfield',
    '#title'         => 'ID lidmaatschap SP',
    '#default_value' => variable_get('memberform_id_lid_sp', 1),
    '#required'      => true,
  );

  $form['memberform_settings_civicrm']['memberform_id_fintype_sp'] = array(
    '#type' => 'textfield',
    '#title' => 'ID financieel type SP',
    '#default_value' => variable_get('memberform_id_fintype_sp', 6),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_lid_sprood'] = array(
    '#type' => 'textfield',
    '#title' => 'ID lidmaatschap SP en ROOD',
    '#default_value' => variable_get('memberform_id_lid_sprood', 2),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_fintype_sprood'] = array(
    '#type' => 'textfield',
    '#title' => 'ID financieel type SP en ROOD',
    '#default_value' => variable_get('memberform_id_fintype_sp', 7),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_lid_rood'] = array(
    '#type' => 'textfield',
    '#title' => 'ID lidmaatschap ROOD',
    '#default_value' => variable_get('memberform_id_lid_rood', 3),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_fintype_rood'] = array(
    '#type' => 'textfield',
    '#title' => 'ID financieel type ROOD',
    '#default_value' => variable_get('memberform_id_fintype_rood', 5),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_abo_tribune'] = array(
    '#type' => 'textfield',
    '#title' => 'ID abonnement Tribune',
    '#default_value' => variable_get('memberform_id_abo_tribune', 4),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_fintype_tribune'] = array(
    '#type' => 'textfield',
    '#title' => 'ID financieel type Tribune',
    '#default_value' => variable_get('memberform_id_fintype_tribune', 10),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_abo_spanning'] = array(
    '#type' => 'textfield',
    '#title' => 'ID abonnement Spanning',
    '#default_value' => variable_get('memberform_id_abo_spanning', 10),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_fintype_spanning'] = array(
    '#type' => 'textfield',
    '#title' => 'ID financieel type Spanning',
    '#default_value' => variable_get('memberform_id_fintype_spanning', 8),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_status'] = array(
    '#type' => 'textfield',
    '#title' => 'Status-ID voor lidmaatschappen',
    '#default_value' => variable_get('memberform_id_status', 5),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_payment_incasso'] = array(
    '#type' => 'textfield',
    '#title' => 'ID betaalwijze incasso',
    '#default_value' => variable_get('memberform_id_payment_incasso', 10),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_payment_accept'] = array(
    '#type' => 'textfield',
    '#title' => 'ID betaalwijze acceptgiro',
    '#default_value' => variable_get('memberform_id_payment_accept', 9),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_activity'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam toe te voegen activiteit',
    '#default_value' => variable_get('memberform_id_activity', 'Aanmelding via website'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_activity_contact'] = array(
    '#type' => 'textfield',
    '#title' => 'ID Contact voor activiteit',
    '#default_value' => variable_get('memberform_id_activity_contact', 7),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_initials'] = array(
    '#type' => 'textfield',
    '#title' => 'Voorletters',
    '#default_value' => variable_get('memberform_id_custom_initials', 'custom_111'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_gift_sp'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam welkomstcadeau SP',
    '#default_value' => variable_get('memberform_id_custom_gift_sp', 'custom_119'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_gift_rood'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam welkomstcadeau ROOD',
    '#default_value' => variable_get('memberform_id_custom_gift_rood', 'custom_122'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_gift_sp_received'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam welkomstcadeau SP ontvangen',
    '#default_value' => variable_get('memberform_id_custom_gift_sp_received', 'custom_120'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_gift_rood_received'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam welkomstcadeau ROOD ontvangen',
    '#default_value' => variable_get('memberform_id_custom_gift_rood_received', 'custom_123'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_source'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam bron bij contact',
    '#default_value' => variable_get('memberform_id_custom_source', 'custom_112'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_source_membership'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam bron bij lidmaatschap',
    '#default_value' => variable_get('memberform_id_custom_source_membership', 'custom_114'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_gezinslid'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam gezinslid',
    '#default_value' => variable_get('memberform_id_custom_gezinslid', 'custom_127'),
    '#required' => TRUE,
  );

  $form['memberform_settings_civicrm']['memberform_id_custom_welkomstbrief'] = array(
    '#type' => 'textfield',
    '#title' => 'Veldnaam welkomstbrief',
    '#default_value' => variable_get('memberform_id_custom_welkomstbrief', 'custom_125'),
    '#required' => TRUE,
  );

  $form['spaanmelden_id_custom_reden_lidmaatschap'] = array(
    '#type'          => 'textfield',
    '#title'         => 'Veldnaam reden lidmaatschap',
    '#default_value' => variable_get('spaanmelden_id_custom_reden_lidmaatschap', 'custom_218'),
    '#required'      => true,
  );

  return system_settings_form($form);
}

/**
 * View submissions table definition.
 */
function memberform_view_submissions() {

  $header = array(
    'id' => 'ID',
    'name' => 'Naam',
    'type' => 'Type',
    'created' => 'Aangemeld',
    'verified' => 'Geverifieerd',
    'submitted' => 'Toegevoegd',
    'form' => 'Formulier',
    'form_page' => 'Ingevuld t/m',
    'options' => 'Opties',
  );

  $data = array();

  $submissions = db_select('memberform_submissions', 's')->fields('s')->orderBy('created', 'DESC')->execute()->fetchAll();
  foreach ($submissions as $s) {

    $form_title = '';
    $form_page_title = '';
    if (!empty($s->form)) {
      $page_settings_callback = 'memberform_' . $s->form . '_settings';
      $pages = $page_settings_callback('pages');
      $form_title = $page_settings_callback('form_title');
      if (!empty($s->form_page)) {
        if ($s->form_page === 'quickfinish') {
          $form_page_title = 'Terugbellen';
        }
        else {
          $form_page_title = $pages[$s->form_page];
        }
      }
    }
    $data[] = array(
      'id' => $s->id,
      'name' => ($s->firstname ? $s->firstname : $s->initials) . ' ' . ($s->middlename ? $s->middlename . ' ' : '') . $s->lastname,
      'type' => memberform_membership_description($s->membership_type, TRUE),
      'created' => date('d-m-Y H:i', $s->created),
      'verified' => ($s->verified ? date('d-m-Y H:i', $s->verified) : '-'),
      'submitted' => ($s->errormsg ? date('d-m-Y H:i', $s->submitted) . ' - <span style="color:#c00;font-weight:bold;">ERROR</span>' : ($s->submitted ? date('d-m-Y H:i', $s->submitted) . ' - <span style="color:#0c0;font-weight:bold;">' . ($s->contact_id ? $s->contact_id : 'OK') . '</span>' : '-')),
      'form' => $form_title,
      'form_page' => $form_page_title,
      'options' => '<ul><li>' . l(t('Details'), 'admin/config/sp/aanmelden/submission/' . $s->id, array('attributes' => array('target' => '_blank'))) . '</li><li>' . l(t('Bewerk'), 'aanmelden/intern/' . $s->id, array('attributes' => array('target' => '_blank'))) . '</li></ul>',
    );
  }

  if (!empty($data)) {
    $output['data'] = array(
      '#prefix' => '<a href="' . url('admin/config/sp/aanmelden/submit') . '">Handmatig synchroniseren</a><br /><br />',
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $data,
    );
  }
  else {
    $output = 'Er zijn nog geen inschrijvingen aanwezig.';
  }

  return $output;
}

/**
 * Submissions details output.
 */
function memberform_submission_details($id) {

  $header = array(
    'key' => 'Veld',
    'value' => 'Waarde',
  );
  $data = array();

  $s = db_select('memberform_submissions', 's')
    ->fields('s')
    ->condition('s.id', $id)
    ->execute()
    ->fetchAssoc();

  if ($s) {
    $rows = array();
    foreach ($s as $key => &$value) {

      list($key, $value) = memberform_friendly_field_data($key, $value);
      if ($key) {
        $rows[] = array('key' => $key, 'value' => check_plain($value));
      }
    }

    $output['data'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  else {
    $output = 'Geen record gevonden met deze ID.';
  }

  return $output;
}

/**
 * Defines memberform gifts view.
 */
function memberform_gifts_view($action = NULL, $id = NULL) {

  // Delete gift.
  if ($action == 'delete') {
    if (is_numeric($id)) {

      $chk = db_select('memberform_submissions', 's')
        ->condition(db_or()->condition('welcome_present_sp', $id)->condition('welcome_present_rood', $id))
        ->countQuery()->execute()->fetchField();
      if ($chk > 0) {
        drupal_set_message(t('Dit welkomstgeschenk is gekoppeld aan eerdere aanmeldingen, en is daarom niet verwijderd maar op inactief gezet.'));
        db_update('memberform_gifts')
          ->fields(array('show_website' => 0))
          ->condition('id', $id)
          ->execute();
      }
      else {
        $delete = db_delete('memberform_gifts')
          ->condition('id', $id)
          ->execute();
        if ($delete) {
          drupal_set_message(t('Welkomstgeschenk !1 is verwijderd.', array('!1' => $id)));
        }
        else {
          drupal_set_message(t('Het welkomstgeschenk kon niet worden verwijderd. Controleer de link en database-instellingen.'), $type = 'error');
        }
      }
    }
  }

  // View gifts.
  $header = array(
    'id' => 'ID',
    'image' => '',
    'name' => 'Naam',
    'membership_type' => 'Lidmaatschap',
    'show_website' => 'Getoond op site',
    'options' => 'Opties',
  );
  $data = array();

  $gifts = db_select('memberform_gifts', 'sg')->fields('sg')->orderBy('membership_type', 'DESC')->orderBy('position', 'ASC')->execute()->fetchAll();
  foreach ($gifts as $g) {

    if (empty($g->image)) {
      $image_styled = '';
    }
    else {
      $image = file_load($g->image);
      $image_styled = empty($image) ? '' : theme('image_style', array('style_name' => 'thumbnail', 'path' => $image->uri));
    }

    $data[] = array(
      'id' => $g->id,
      'image' => $image_styled,
      'name' => check_plain($g->name),
      'type' => memberform_membership_description($g->membership_type, TRUE),
      'show_website' => ($g->show_website ? 'ja' : 'nee'),
      'options' =>
      l(t('Bewerken'), 'admin/config/sp/aanmelden/gifts/edit/' . $g->id) . '&nbsp;&nbsp;&nbsp;' .
      l(t('Verwijderen'), 'admin/config/sp/aanmelden/gifts/delete/' . $g->id),
    );
  }

  $add_link = '<a href="' . url('admin/config/sp/aanmelden/gifts/edit') . '">Nieuw welkomstgeschenk</a><br /><br />';
  if (!empty($data)) {
    $output['data'] = array(
      '#prefix' => $add_link,
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $data,
    );
  }
  else {
    $output['data'] = array(
      '#prefix' => $add_link . 'Er zijn nog geen welkomstcadeaus toegevoegd.',
      '#theme' => 'table',
    );
  }

  return $output;
}

/**
 * Definition gift form.
 */
function memberform_gifts_form($form = NULL, $form_state = NULL, $id = NULL) {

  // Begin add form.
  $form = array();

  $form['add_gift'] = array(
    '#type' => 'fieldset',
    '#title' => t('Welkomstgeschenk'),
    '#description' => t('Kies een naam voor het welkomstgeschenk en voor welke lidmaatschappen het geschenk van toepassing is. De interne naam wordt doorgegeven aan CiviCRM.'),
    '#tree' => TRUE,
    '#parents' => array('add_gift'),
  );
  $form['add_gift']['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam',
    '#description' => 'Deze naam wordt getoond op het formulier.',
    '#required' => TRUE,
  );
  $form['add_gift']['internal_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Interne naam',
    '#description' => 'Deze naam wordt doorgegeven aan CiviCRM.',
    '#required' => TRUE,
  );
  $form['add_gift']['image'] = array(
    '#type' => 'media',
    '#title' => 'Afbeelding',
    '#description' => 'Deze afbeelding wordt weergegeven in de aanmeldingsformulieren.',
  );
  $form['add_gift']['membership_type'] = array(
    '#type' => 'select',
    '#title' => 'Lidmaatschap',
    '#options' => array('lid_sp' => 'SP', 'lid_rood' => 'ROOD'),
    '#description' => 'Kies of dit cadeau voor SP of ROOD wordt getoond.',
    '#required' => TRUE,
  );
  $form['add_gift']['show_website'] = array(
    '#type' => 'select',
    '#title' => 'Tonen op website?',
    '#options' => array(1 => 'ja', 0 => 'nee'),
    '#default_value' => 1,
    '#description' => 'Kies of dit cadeau op de website moet worden getoond. Op het interne formulier worden alle cadeaus getoond.',
    '#required' => TRUE,
  );

  $weight_options = array();
  for ($i = -20; $i <= 20; $i++) {
    $weight_options[] = $i;
  }
  $form['add_gift']['position'] = array(
    '#type' => 'select',
    '#options' => $weight_options,
    '#title' => t('Weight'),
    '#default_value' => 0,
    '#description' => 'Optioneel. Hoe lager het gewicht, hoe eerder het geschenk wordt getoond.',
    '#required' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Toevoegen',
  );

  // Begin edit form.
  if (!empty($id) && is_numeric($id)) {

    $default_values = db_select('memberform_gifts', 'sg')->fields('sg')->condition('id', $id)->execute()->fetchObject();
    if (!$default_values) {
      drupal_set_message('Het welkomstgeschenk kon niet opgehaald worden uit de database. Controleer de link en de database-instellingen.');
      return $form;
    }

    // Alter above add form with default values.
    $form['add_gift']['name']['#default_value'] = $default_values->name;
    $form['add_gift']['internal_name']['#default_value'] = $default_values->internal_name;
    $form['add_gift']['image']['#default_value'] = $default_values->image;
    $form['add_gift']['membership_type']['#default_value'] = $default_values->membership_type;
    $form['add_gift']['position']['#default_value'] = $default_values->position;
    $form['add_gift']['show_website']['#default_value'] = $default_values->show_website;
    $form['edit_flag'] = array('#type' => 'hidden', '#value' => check_plain($id));
    $form['submit']['#value'] = 'Opslaan';
  }

  return $form;
}

/**
 * Gifts form validation.
 */
function memberform_gifts_form_validate($form, &$form_state) {
  if (
    !$form_state['values']['add_gift']['name'] ||
    !$form_state['values']['add_gift']['internal_name'] ||
    !$form_state['values']['add_gift']['membership_type']
  ) {
    form_set_error('add_gift', 'U hebt niet alle verplichte velden ingevuld.');
  }
}

/**
 * Giftsform submit function.
 */
function memberform_gifts_form_submit($form, &$form_state) {

  if (isset($form_state['values']['edit_flag'])) {
    $edit_gift = db_update('memberform_gifts')->fields(array(
      'name' => $form_state['values']['add_gift']['name'],
      'internal_name' => $form_state['values']['add_gift']['internal_name'],
      'image' => $form_state['values']['add_gift']['image'],
      'membership_type' => $form_state['values']['add_gift']['membership_type'],
      'position' => (int) $form_state['values']['add_gift']['position'],
      'show_website' => (int) $form_state['values']['add_gift']['show_website'],
    ))->condition('id', (int) $form_state['values']['edit_flag'])->execute();

    if ($edit_gift) {
      drupal_set_message('Het geschenk is opgeslagen.');
      drupal_goto('admin/config/sp/aanmelden/gifts');
    }
  }
  else {
    $add_gift = db_insert('memberform_gifts')->fields(array(
      'name' => $form_state['values']['add_gift']['name'],
      'internal_name' => $form_state['values']['add_gift']['internal_name'],
      'image' => $form_state['values']['add_gift']['image'],
      'membership_type' => $form_state['values']['add_gift']['membership_type'],
      'position' => (int) $form_state['values']['add_gift']['position'],
      'show_website' => (int) $form_state['values']['add_gift']['show_website'],
    ))->execute();
    if ($add_gift) {
      drupal_set_message('Het geschenk is toegevoegd.');
      drupal_goto('admin/config/sp/aanmelden/gifts');
    }
    else {
      drupal_set_message('Het is niet gelukt het geschenk toe te voegen aan de database.', $type = 'error');
    }
  }
}

/**
 * Pending submissions submit.
 */
function memberform_admin_submit_pending() {

  memberform_submit_pending();

  drupal_set_message('Wachtende inschrijvingen zijn gesynchroniseerd met CiviCRM.');
  drupal_goto('admin/config/sp/aanmelden');
}
