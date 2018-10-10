<?php

/**
 * @file Form definitions for memberform pages.
 *
 *   Define page form functions like 'memberform_x_page_x_form' where the
 *   first x is to be substituted with the form name and the second x with the
 *   page name used in $storage['current_page'] and $storage['next_page'].
 *
 *   Example:
 *
 *   function memberform_membership_page_start_form(&$form, $storage, $values) {
 *     $form['page_start_choice'] = array(
 *       '#type' => 'radios',
 *       '#title' => 'Maak een keuze',
 *       '#options' => array('colors' => 'Kleuren', 'tastes' => 'Smaken'),
 *       '#required' => TRUE,
 *     );
 *   }
 */

/**
 * Form settings.
 *
 * Add 'no_nav_pages' variable (array) with page names of pages that should not
 * show a back button.
 *
 * Add 'pages' array with page name to page titles.
 *
 * @param string $variable_name
 *   Name of the variable to return.
 *
 * @return Variable value.
 *   Returns variable value.
 */
function memberform_membership_settings($variable_name) {
  $settings = array(
    'path_name' => 'aanmelden/lid',
    'form_title' => 'Lid worden van de SP',
    'no_nav_pages' => array(
      //'quickfinish',
      'intro',
      'thanks',
    ),
    'pages' => array(
      // 'intro' => '',
      'start' => 'Contactgegevens',
      'quickfinish' => '',
      'person_details' => 'Persoonsgegevens',
      'red' => 'ROOD lidmaatschap',
      'address' => 'Adresgegevens',
      'contribution' => 'Persoonlijke bijdrage',
      'gift' => 'Welkomstgeschenk',
      'summary' => 'Controleer uw gegevens',
      'thanks' => 'Bedankt',
    ),
    'final_submit_page' => 'summary',
    'feedback_page' => 'thanks'
  );
  return $settings[$variable_name];
}

/**
 * Form definition for start page.
 */
function memberform_membership_page_start_form(&$form, &$form_state) {
  $form['intro'] = array(
    '#markup' => "<div class=\"introduction\"><p>Iedereen die <em>16 jaar of ouder</em> is kan lid worden van de SP. Vul hieronder uw contactgegevens in. We gebruiken deze gegevens voor uw inschrijving en om u te informeren over nieuws en activiteiten van de SP. Meer informatie staat in ons <a href=\"https://www.sp.nl/privacy\" target=\"_blank\">privacy reglement</a></p></div>",
  );
  memberform_shared_page_start_form($form, $form_state);
}

/**
 * Form definition for thanks phone page.
 */
function memberform_membership_page_quickfinish_form(&$form, &$form_state) {
  memberform_shared_page_quickfinish_form($form, $form_state);
}

/**
 * Form definition for person details page.
 */
function memberform_membership_page_person_details_form(&$form, &$form_state) {
  memberform_shared_page_person_details_form($form, $form_state);
}

/**
 * Form definition for red membership page.
 */
function memberform_membership_page_red_form(&$form, &$form_state) {
  memberform_shared_page_red_form($form, $form_state);
}

/**
 * Form definition for address page.
 */
function memberform_membership_page_address_form(&$form, &$form_state) {
  memberform_shared_page_address_form($form, $form_state);
}

/**
 * Form definition for contribution page.
 */
function memberform_membership_page_contribution_form(&$form, &$form_state) {
  memberform_shared_page_contribution_form($form, $form_state);
}

/**
 * Form definition for gift page.
 *
 * Add images in module folder gift_images like book.jpg.
 */
function memberform_membership_page_gift_form(&$form, &$form_state) {
  memberform_shared_page_gift_form($form, $form_state);
}

/**
 * Form definition for summary page.
 */
function memberform_membership_page_summary_form(&$form, &$form_state) {
  memberform_shared_page_summary_form($form, $form_state);
}

/**
 * Form definition for thanks page.
 */
function memberform_membership_page_thanks_form(&$form, &$form_state) {
  $storage = $form_state['storage'];
  if (!empty($storage['values']['person_details']['page_person_details_first_name'])) {
    $name = $storage['values']['person_details']['page_person_details_first_name'];
  }
  else {
    $name = $storage['values']['start']['page_start_name'];
  }

  $form['feedback'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-feedback')),
  );

  switch ($storage['previous_page']) {
    case 'quickfinish':
      $form['feedback']['thanks'] = array(
        '#markup' => '<p>Bedankt ' . check_plain($name) . ' voor je aanmelding als lid.</p><p>We bellen je binnenkort om je inschrijving af te ronden.</p>',
      );
      break;
    case 'summary':
      $form['feedback']['thanks'] = array(
        '#markup' => '<p>Bedankt ' . check_plain($name) . ' voor je aanmelding als lid van de SP. Samen werken we aan ons sociaal alternatief.</p><p>Je ontvangt zo een e-mail met daarin een link die je moet aanklikken om de aanmelding te bevestigen.</p><p>Heb je vragen over het lidmaatschap of gegevens? Neem dan contact op met onze <a href="https://www.sp.nl/contact/partijbureau">ledenadministratie</a></p>',
      );
      break;
  }
}

/**
 * Validation for member form pages.
 */
function memberform_membership_page_form_validate($form, &$form_state) {
  memberform_shared_validation($form, $form_state);

  $values = &$form_state['values'];
  $storage = &$form_state['storage'];

  // Process trigger.
  if ($form_state['validate']) {
  }
}

/**
 * Submit function for memberform pages.
 *
 * Set $storage['next_page'] to value of next page.
 */
function memberform_membership_page_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $storage = &$form_state['storage'];

  // Process trigger.
  $trigger = $form_state['triggering_element']['#name'];

  // Store membership.
  switch ($trigger) {
    case 'next_page':
      switch ($storage['current_page']) {
        case 'start':
          // Store membership type.
          $storage['membership_type'] = 'sp_member';
          break;
      }
      break;
  }

  memberform_shared_submit($form, $form_state);
}
