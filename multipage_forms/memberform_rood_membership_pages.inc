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
 *   function memberform_rood_page_start_form(&$form, &$form_state) {
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
function memberform_rood_membership_settings($variable_name) {
  $settings = array(
    'path_name' => 'aanmelden/rood',
    'form_title' => 'ROOD lid worden',
    'no_nav_pages' => array(
      //'quickfinish',
      'thanks',
    ),
    'pages' => array(
      'start' => 'Contactgegevens',
      'quickfinish' => '',
      'person_details' => 'Persoonsgegevens',
      'red' => 'ROOD lidmaatschap',
      'address' => 'Adresgegevens',
      'contribution' => 'Persoonlijke bijdrage',
      'gift' => 'Welkomstgeschenk',
      'summary' => 'Controleer je gegevens',
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
function memberform_rood_membership_page_start_form(&$form, &$form_state) {
  memberform_shared_page_start_form($form, $form_state);
}

/**
 * Form definition for thanks phone page.
 */
function memberform_rood_membership_page_quickfinish_form(&$form, &$form_state) {
  memberform_shared_page_quickfinish_form($form, $form_state);
}

/**
 * Form definition for red membership page.
 */
function memberform_rood_membership_page_red_form(&$form, &$form_state) {
  memberform_shared_page_red_form($form, $form_state);
  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-input')),
  );

  $form['input']['page_red_membership'] = array(
    '#title' => 'Rood lidmaatschap',
    '#type' => 'radios',
    '#options' => array(
      'sp_and_red_member' => 'Ja, ik wil ook lid worden van de SP worden!',
      'red_member' => 'Nee, ik wil alleen lid worden van ROOD',
    ),
    '#required' => TRUE,
  );
}

/**
 * Form definition for person details page.
 */
function memberform_rood_membership_page_person_details_form(&$form, &$form_state) {
  memberform_shared_page_person_details_form($form, $form_state);
  // Custom year range for membership and rood_membership forms.
  $form['input']['page_person_details_birth_date']['#date_year_range'] = '-29:-14';
}

/**
 * Form definition for address page.
 */
function memberform_rood_membership_page_address_form(&$form, &$form_state) {
  memberform_shared_page_address_form($form, $form_state);
}

/**
 * Form definition for contribution page.
 */
function memberform_rood_membership_page_contribution_form(&$form, &$form_state) {
  memberform_shared_page_contribution_form($form, $form_state);
}

/**
 * Form definition for gift page.
 *
 * Add images in module folder gift_images like book.jpg.
 */
function memberform_rood_membership_page_gift_form(&$form, &$form_state) {
  memberform_shared_page_gift_form($form, $form_state);
}

/**
 * Form definition for summary page.
 */
function memberform_rood_membership_page_summary_form(&$form, &$form_state) {
  memberform_shared_page_summary_form($form, $form_state);
}

/**
 * Form definition for thanks page.
 */
function memberform_rood_membership_page_thanks_form(&$form, &$form_state) {
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
        '#markup' => '<p>Welkom bij ROOD ' . check_plain($name) . '!</p><p>We bellen je binnenkort om je inschrijving af te ronden.</p>',
      );
      break;
    case 'summary':
      $form['feedback']['thanks'] = array(
        '#markup' => '<p>Welkom bij ROOD ' . check_plain($name) . '! Je ontvangt zo een e-mail met daarin een link die je moet aanklikken om de aanmelding te bevestigen.</p><p>Heb je vagen over het lidmaatschap of gegevens? Neem dan contact op met onze <a href="https://www.sp.nl/contact/partijbureau">ledenadministratie</a></p>',
      );
      break;
  }
}

/**
 * Validation for member form pages.
 */
function memberform_rood_membership_page_form_validate($form, &$form_state) {
  memberform_shared_validation($form, $form_state);

  $values = &$form_state['values'];
  $storage = &$form_state['storage'];

  // Process trigger.
  if ($form_state['validate']) {

    switch ($storage['current_page']) {
      case 'person_details':
        $age = memberform_get_age($values['page_person_details_birth_date']);
        if ($age > 28) {
          form_set_error('page_person_details_birth_date', 'Je bent ouder dan 28 jaar en kunt daarom geen lid meer worden van ROOD.');
        }
        break;
    }
  }
}

/**
 * Submit function for memberform pages.
 *
 * Set $storage['next_page'] to value of next page.
 */
function memberform_rood_membership_page_form_submit($form, &$form_state) {
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
          $storage['membership_type'] = 'red_member';
          break;
      }
      break;
  }

  memberform_shared_submit($form, $form_state);
}
