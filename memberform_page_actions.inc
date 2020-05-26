<?php

/**
 * Action pages.
 */

/**
 * Thanks page content.
 */
/*
function memberform_page_thanks() {
  $name = base64_decode($_GET['name']);

  if (!drupal_valid_token($_GET['token'], $name, TRUE)) {
    return MENU_ACCESS_DENIED;
  }

  drupal_set_title($name . ', bedankt!');

  $ret = '<div class="lid-worden-bedankt content">';
  $descr = memberform_membership_description($_GET['membership_type']);
  $ret .= '<p>Bedankt voor je aanmelding als ' . $descr . '. ';
  if (!in_array($_GET['membership_type'], array('tribune', 'spanning'))) {
    $ret .= 'Goed dat je ons wilt steunen in de strijd voor een beter Nederland!<br />';
  }
  $ret .= 'Je ontvangt straks per e-mail een overzicht van de gegevens die je hebt opgegeven.<br />
    Let op! De gegevens moet je nog wel even via een link in die e-mail bevestigen.</p>'
    . memberform_get_qline() . '</div>';
  return $ret;
}
*/

/**
 * Error page content.
 */
function memberform_page_error() {
  return '<div class="lid-worden-bedankt content"><h1>Aanmelding niet verwerkt</h1><p>Excuus, maar het is
    niet gelukt om je aanmelding goed te verwerken. Probeer het later nog eens, of neem even contact met ons
    op.' . memberform_get_qline() . '</p></div>';
}

/**
 * Validation function for membership.
 */
function memberform_page_validate($hash) {

  $q = db_select('memberform_submissions', 's')
    ->fields('s')
    ->condition('hash', $hash, '=')
    ->execute();
  if ($q) {
    $record = $q->fetchAssoc();

    if (empty($record['verified'])) {
      $exec = db_update('memberform_submissions')
        ->fields(array(
          'verified' => time(),
        ))
        ->condition('hash', $hash, '=')
        ->execute();

      if ($exec) {
        $output = '<div class="lid-worden-bedankt content"><h1>Aanmelding bevestigd</h1><p>Bedankt voor het bevestigen van je gegevens. Je aanmelding wordt zo spoedig mogelijk verwerkt door onze ledenadministratie.</p>';
        if (in_array($record['membership_type'], array('lid_rood', 'lid_sprood', 'lid_sp'))) {
          $output .= '<p>Zou je ons nog willen vertellen waarom je lid bent geworden?</p>';
          $output .= drupal_render(drupal_get_form('memberform_page_validate_question_form', $record));
        }
        $output .= memberform_get_qline() . '</div>';
        return $output;
      }
    }
    else {
      $output = '<div class="lid-worden-bedankt content"><h1>Aanmelding reeds bevestigd</h1><p>Bedankt, je aanmelding was reeds bevestigd en wordt zo spoedig mogelijk verwerkt door onze ledenadministratie.</p>';
      $output .= memberform_get_qline() . '</div>';
      return $output;
    }
  }

  return '<div class="lid-worden-bedankt content"><h1>Aanmelding niet bevestigd</h1><p>Er is een fout
    opgetreden bij het verwerken van je aanvraag. Controleer of de link die je gevolgd hebt klopt, en kopieer
    deze eventueel handmatig uit de e-mail.</p>' . memberform_get_qline() . '</div>';

}

/**
 * Membership question form.
 */
function memberform_page_validate_question_form($form, &$form_state, $record) {
  // Store membership values.
  if (!empty($record)) {
    $form_state['membership_record'] = $record;
  }

  $form = array(
    '#prefix' => '<div id ="question-form">',
    '#suffix' => '</div>',
  );
  if (empty($form_state['values']['why_member'])) {
    $form['why_member'] = array(
      '#type' => 'textarea',
      '#title' => !empty($record['firstname']) ? check_plain($record['firstname']) . ', waarom ben je juist nu lid geworden?' : 'Waarom ben je juist nu lid geworden?',
    );
    $form['submit'] = array(
      '#type' => 'button',
      '#value' => 'Indienen',
      '#name' => 'submit',
      '#ajax' => array(
        'callback' => 'memberform_page_validate_question_form_ajax_callback',
        'wrapper' => 'question-form',
        'method' => 'replace',
      ),
    );
  }
  else {
    $form['thanks'] = array(
      '#markup' => '<p><strong>Bedankt voor de feedback!</strong></p>',
    );
  }
  return $form;
}

/**
 * Memberform question validation.
 */
function memberform_page_validate_question_form_validate($form, &$form_state) {
  if (!empty($form_state['membership_record'])) {
    $record = $form_state['membership_record'];
    if (empty($form_state['values']['why_member'])) {
      form_set_error('why_member', 'Ben je vergeten iets in te vullen?');
      return;
    }
    if (!empty($form_state['triggering_element']['#name'])) {
      switch ($form_state['triggering_element']['#name']) {
        case 'submit':
          $note = preg_replace('~[\r\n]+~', ' ', check_plain($form_state['values']['why_member']));
          memberform_store_note($record['hash'], $note);
          if (!empty($record['contact_id'])) {
            memberform_create_note($record['contact_id'], $note);
          }
          $form_state['rebuild'] = TRUE;
          break;
      }
    }
  }
  else {
    form_set_error('why_member', 'Je aanmelding is reeds bevestigd. Het doorgeven van je reden om lid te worden is optioneel. Als je dit wil moet je wel iets invullen.');
    return;
  }
}

/**
 * Ajax callback.
 */
function memberform_page_validate_question_form_ajax_callback($form, &$form_state) {
  return $form;
}
