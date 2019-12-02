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
    'form_title' => 'Lid worden van ROOD',
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
  $form['intro'] = array(
    '#markup' => "<div class=\"introduction\"><p>Iedereen die <em>14 jaar of ouder</em> is kan lid worden van ROOD, jong in de SP. Vul hieronder je contactgegevens in. We gebruiken deze gegevens voor je inschrijving en om je op de hoogte te houden van nieuws en activiteiten van ROOD en de SP. Meer informatie lees je in het <a href=\"https://www.sp.nl/privacy\">privacyreglement</a> van de SP.</p></div>",
  );
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
  $storage = $form_state['storage'];
  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-input')),
  );

  if ($storage['membership_type'] == 'red_member') {
    $form['input']['page_only_red'] = array(
      '#markup' => '<p>Omdat je nog geen 16 bent wordt je lid van ROOD, de actiefste jongerenorganisatie van Nederland. Zodra je 16 bent kan je ook lid worden van de SP.</p><p>Samen met jongeren gaan we de strijd aan voor een betere wereld. We zijn niet bang om onrecht, discriminatie en ongelijkheid te bestrijden waar dat moet gebeuren.</p>',
    );
  }
  else {
    $form['input']['page_red_intro'] = array(
      '#markup' => '<p>Omdat je 16 of ouder bent wordt je lid van ROOD, de actiefste jongerenorganisatie van Nederland en de SP.</p><p><strong>ROOD is de actiefste jongerenorganisatie van Nederland</strong></p><p>Samen met jongeren gaan we de strijd aan voor een betere wereld. We zijn niet bang om onrecht, discriminatie en ongelijkheid te bestrijden waar dat moet gebeuren.</p>',
    );
  }
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
        '#markup' => '<p>Welkom bij ROOD ' . check_plain($name) . '! Je ontvangt zo een e-mail met daarin een link die je moet aanklikken om de aanmelding te bevestigen.</p><p>Heb je vragen over het lidmaatschap of gegevens? Neem dan contact op met onze <a href="https://www.sp.nl/contact/partijbureau">ledenadministratie</a></p><div class="button-link"><a href="https://www.sp.nl">Naar de SP website</a></div>',
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
          drupal_set_message('<strong>Je bent ouder dan 28 jaar en kunt daarom geen lid meer worden van de jongerenorganisatie ROOD.</strong><br/>Ga verder om je aan te melden als lid van de SP.', 'warning');
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
