<?php

/**
 * Shared form elements.
 */

/**
 * Form definition for start page.
 */
function memberform_shared_page_start_form(&$form, &$form_state) {
  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array(
      'class' => array('memberform-input'),
    ),
  );

  $form['input']['page_start_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Naam',
    '#required' => TRUE,
    '#attributes' => array('placeholder' => 'Voornaam Achternaam'),
  );

  $form['input']['page_start_email'] = array(
    '#type' => 'emailfield',
    '#title' => 'E-mailadres',
    '#attributes' => array('placeholder' => 'naam@domein.nl'),
  );

  $form['input']['page_start_phone'] = array(
    '#type' => 'telfield',
    '#title' => 'Telefoonnummer',
    '#attributes' => array('placeholder' => '0612345678'),
  );
}

/**
 * Form definition for thanks phone page.
 */
function memberform_shared_page_quickfinish_form(&$form, &$form_state) {
  if (count($form_state['storage']['nav_history']) < 3) {
    $form['action']['instruction'] = array(
      '#markup' => '<p>U kunt zelf het formulier invullen, of als u weinig tijd hebt, kunnnen wij u bellen om de aanmelding in orde te maken.</p>'
    );
    $form['action']['next_page'] = array(
      '#type' => 'submit',
      '#value' => 'Ik vul mijn gegevens zelf in',
      '#name' => 'next_page',
      '#attributes' => array('class' => array('btn hollow do-self')),
      '#ajax' => array(
        'wrapper' => 'memberform_form_wrapper',
        'callback' => 'memberform_ajax_callback',
      ),
    );
    $form['action']['call_back'] = array(
      '#type' => 'submit',
      '#value' => 'Bel me om lid te worden',
      '#attributes' => array('class' => array('btn do-call')),
      '#name' => 'call_back',
      '#ajax' => array(
        'wrapper' => 'memberform_form_wrapper',
        'callback' => 'memberform_ajax_callback',
      ),
    );
  }
  else {
    $form['input']['instructions'] = array(
      '#markup' => '<p>Komt u er niet uit, of heeft u geen tijd voor het afronden? Geen probleem. Klik op de bel knop hieronder en we bellen u binnenkort om uw aanmelding af te ronden.</p>',
    );
    $form['input']['call_back'] = array(
      '#type' => 'submit',
      '#value' => 'Bel me',
      '#attributes' => array('class' => array('btn do-call-back')),
      '#name' => 'call_back',
      '#ajax' => array(
        'wrapper' => 'memberform_form_wrapper',
        'callback' => 'memberform_ajax_callback',
      ),
      '#prefix' => '<div class="memberform-inform-button">',
      '#suffix' => '</class>',
    );
  }
}

/**
 *
 */
function memberform_shared_page_call_back_feedback(&$form, &$form_state) {
  $form['feedback'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-feedback')),
  );

  $form['feedback']['call_back_feedback'] = array(
      '#markup' => '<p>Wat goed dat je lid wilt worden van de SP! We bellen je binnenkort om de aanmelding af te ronden. Mocht je in de tussentijd vragen hebben kun je altijd contact opnemen met de ledenadminisrtatie van de SP.</p>',
  );
}

/**
 * Form definition for person details page.
 */
function memberform_shared_page_person_details_form(&$form, &$form_state) {
  $storage = &$form_state['storage'];

  // Split name given in start page.
  $parts = array('first_name', 'middle_name', 'last_name');
  if (empty($storage['values']['person_details']['page_person_details_last_name'])) {
    // Prefill name parts.
    $split_name = memberform_split_name($storage['values']['start']['page_start_name']);
    foreach ($parts as $part) {
      $$part = isset($split_name[$part]) ? $split_name[$part] : '';
      $storage['values']['person_details']['page_person_details_' . $part] = $$part;
    }
  }
  else {
    // Show stored name parts.
    foreach ($parts as $part) {
      $$part = isset($storage['values']['person_details']['page_person_details_' . $part]) ? $storage['values']['person_details']['page_person_details_' . $part] : '';
    }
  }

  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-input')),
  );

  $form['input']['page_person_details_first_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Voornaam',
    '#required' => TRUE,
    '#default_value' => $first_name,
  );

  $form['input']['page_person_details_middle_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Tussenvoegsel',
    '#default_value' => $middle_name,
  );

  $form['input']['page_person_details_last_name'] = array(
    '#type' => 'textfield',
    '#title' => 'Achternaam',
    '#required' => TRUE,
    '#default_value' => $last_name,
  );

  $form['input']['page_person_details_birth_date'] = array(
    '#title'           => 'Geboortedatum',
    '#type'            => 'date_select',
    '#date_format'     => 'd-m-Y',
    '#date_year_range' => '-14:-120',
    '#required' => TRUE,
    '#date_label_position' => 'within',
  );

  $form['input']['page_person_details_gender'] = array(
    '#title'         => 'Geslacht',
    '#type'          => 'radios',
    '#options'       => array('m' => 'Man', 'v' => 'Vrouw', 'a' => 'Anders'),
    '#required'      => TRUE,
  );
}

/**
 * Form definition for red intern page.
 */
function memberform_shared_page_red_form(&$form, &$form_state) {
  $storage = $form_state['storage'];


  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-input')),
  );
  if (!empty($storage['membership_type']) && $storage['membership_type'] === 'red_member') {
    $form['input']['page_only_red'] = array(
      '#markup' => '<p>Omdat je nog geen 16 bent wordt je lid van ROOD, de actiefste jongerenorganisatie van Nederland. Zodra je 16 bent kan je ook lid worden van de SP.</p><p>Samen met jongeren gaan we de strijd aan voor een betere wereld. We zijn niet bang om onrecht, discriminatie en ongelijkheid te bestrijden waar dat moet gebeuren.</p>',
    );
  }
  else {
    $form['input']['page_red_intro'] = array(
      '#markup' => '<p><strong>ROOD is de actiefste jongerenorganisatie van Nederland</strong></p><p>Samen met jongeren gaan we de strijd aan voor een betere wereld. We zijn niet bang om onrecht, discriminatie en ongelijkheid te bestrijden waar dat moet gebeuren.</p>',
    );

    $form['input']['page_red_membership'] = array(
      '#title' => 'Rood lidmaatschap (gratis)',
      '#type' => 'radios',
      '#options' => array(
        'sp_and_red_member' => 'Ja, ik wil ook lid worden van ROOD, Jong in de SP!',
        'sp_member' => 'Nee, ik wil alleen lid worden van de SP',
      ),
      '#required' => TRUE,
    );
  }
}

/**
 * Form definition for address page.
 */
function memberform_shared_page_address_form(&$form, &$form_state) {
  $storage = $form_state['storage'];

  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-input')),
  );

  include_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();
  $form['input']['page_address_country'] = array(
    '#title'         => 'Land',
    '#type'          => 'select',
    '#options'       => $countries,
    '#default_value' => 'NL',
    '#required'      => TRUE,
    '#ajax' => array(
      'wrapper' => 'memberform_form_wrapper',
      'callback' => 'memberform_ajax_callback',
    ),
  );

  if (
    empty($storage['values']['address']['page_address_country']) ||
    $storage['values']['address']['page_address_country'] === 'NL'
  ) {
    // Dutch address, autofill street and locality fields.
    $form['input']['page_address_postal_code'] = array(
      '#title'         => 'Postcode',
      '#type'          => 'textfield',
      '#required'      => TRUE,
      '#attributes' => array('placeholder' => '1234AB'),
    );
    $form['input']['page_address_house_number'] = array(
      '#title'         => 'Huisnummer',
      '#type'          => 'textfield',
      '#required'      => TRUE,
      '#ajax' => array(
        'wrapper' => 'memberform_form_wrapper',
        'callback' => 'memberform_ajax_callback',
        'event' => 'change'
      ),
    );
    $form['input']['page_address_house_number_addition'] = array(
      '#title'         => 'Huisnummer toevoeging',
      '#type'          => 'textfield',
    );
    // Set street value.
    $form['input']['page_address_street'] = array(
      '#title'         => 'Straat',
      '#type'          => 'textfield',
      '#required'      => TRUE,
    );
    // Set locality value.
    $form['input']['page_address_locality'] = array(
      '#title'         => 'Plaats',
      '#type'          => 'textfield',
      '#required'      => TRUE,
    );
    // Get address data based on postal code and house number.
    $postal_code = $storage['values']['address']['page_address_postal_code'];
    $house_number = $storage['values']['address']['page_address_house_number'];
    if (empty($storage['address_fixed'])) {
      if (!empty($postal_code) && !empty($house_number)) {
        if (!memberform_valid_pc($postal_code) || !memberform_valid_nr($house_number)) {
          if (!memberform_valid_pc($postal_code)) {
            // Show warning when postalcode not valid.
            $message = '<div class="messages warning">Deze postcode is niet geldig!</div>';
            $form['input']['page_address_postal_code']['#field_suffix'] = empty($form['input']['page_address_postal_code']['#field_suffix']) ? $message : $form['input']['page_address_postal_code']['#field_suffix'] . $message;
          }
          if (!memberform_valid_nr($house_number)) {
            // Show warning when postalcode not valid.
            $message = '<div class="messages warning">Dit huisnummer is niet geldig!</div>';
            $form['input']['page_address_house_number']['#field_suffix'] = empty($form['input']['page_address_house_number']['#field_suffix']) ? $message : $form['input']['page_address_house_number']['#field_suffix'] . $message;
          }
          $form['input']['page_address_street']['#value'] = '';
          $form['input']['page_address_locality']['#value'] = '';
        }
        else {
          $address_data = memberform_postcode_lookup($postal_code, $house_number);
          if (!is_object($address_data) && !empty($address_data)) {
            // Show warning when postalcode housenumber combination is not found.
            $message = '<div class="messages warning">' . $address_data . '</div>';
            $form['input']['page_address_house_number']['#field_suffix'] = empty($form['input']['page_address_house_number']['#field_suffix']) ? $message : $form['input']['page_address_house_number']['#field_suffix'] . $message;
            $form['input']['page_address_street']['#value'] = '';
            $form['input']['page_address_locality']['#value'] = '';
          }
          else {
            // Set values.
            if (!empty($address_data->adres)) {
              //$form['input']['page_address_street']['#default_value'] = $address_data->adres;
              $form['input']['page_address_street']['#value'] = $address_data->adres;
            };
            if (!empty($address_data->woonplaats)) {
              //$form['input']['page_address_locality']['#default_value'] = $address_data->woonplaats;
              $form['input']['page_address_locality']['#value'] = $address_data->woonplaats;
            };
          }
        }
        $form['input']['page_address_postal_code']['#ajax'] = array(
          'wrapper' => 'memberform_form_wrapper',
          'callback' => 'memberform_ajax_callback',
          'event' => 'change'
        );
      }
    }
  }
  else {
    // International address.
    $form['input']['page_address_street'] = array(
      '#title'         => 'Straat',
      '#type'          => 'textfield',
      '#required'      => TRUE,
      '#attributes' => array('placeholder' => 'Straat'),
    );
    $form['input']['page_address_house_number'] = array(
      '#title'         => 'Huisnummer',
      '#type'          => 'textfield',
      '#required'      => TRUE,
    );
    $form['input']['page_address_house_number_addition'] = array(
      '#title'         => 'Huisnummer toevoeging',
      '#type'          => 'textfield',
    );
    $form['input']['page_address_postal_code'] = array(
      '#title'         => 'Postcode',
      '#type'          => 'textfield',
    );
    $form['input']['page_address_locality'] = array(
      '#title'         => 'Plaats',
      '#type'          => 'textfield',
      '#required'      => TRUE,
    );
  }
}

/**
 * Form definition for contribution page.
 */
function memberform_shared_page_contribution_form(&$form, &$form_state) {
  $storage = &$form_state['storage'];

  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('contribution-form-element', 'memberform-input')),
  );

  $form['input']['page_contribution_choice'] = array(
    '#title' => 'Kwartaalbijdrage',
    '#type' => 'radios',
    '#options' => array(
      '5' => '5,-',
      '10' => '10,-',
      '20' => '20,-',
      '50' => '50,-',
      'custom' => 'Anders',
    ),
    '#required' => TRUE,
    '#ajax' => array(
      'wrapper' => 'memberform_form_wrapper',
      'callback' => 'memberform_ajax_callback',
    ),
    '#attributes' => array('class' => array('memberform-radio-button')),
  );

  if ($storage['values']['contribution']['page_contribution_choice'] === 'custom') {
    $form['input']['page_contribution_custom'] = array(
      '#title' => 'Kies een bedrag (minimaal 5 euro)',
      '#type' => 'numberfield',
      '#step' => 'any',
      '#required' => TRUE,
      '#field_prefix' => '<div class="inline">€</div>',
    );
  }

  $form['input']['page_contribution_payment_method'] = array(
    '#title' => 'Hoe wil je je bijdrage voldoen?',
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => array(
      'auto' => 'Automatische incasso',
      'accept' => 'Acceptgiro',
    ),
    '#name' => 'page_contribution_payment_method',
    '#ajax' => array(
      'wrapper' => 'memberform_form_wrapper',
      'callback' => 'memberform_ajax_callback',
    ),
    '#attributes' => array('class' => array('memberform-radio-button')),
  );

  $form['input']['page_contribution_accept_costs'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="page_contribution_payment_method"]' => array(
          'value' => 'accept',
        ),
      ),
    ),
  );

  $form['input']['page_contribution_accept_costs']['accept_consts_message'] = array(
    '#markup' => '<p><strong>Let op! De kosten voor betaling via acceptgiro bedragen 6 euro per kwartaal.</strong></p>',
  );

  $form['input']['page_contribution_iban'] = array(
    '#title' => 'Wat is je bankrekeningnummer?',
    '#type' => 'textfield',
  );
  if ($storage['values']['contribution']['page_contribution_payment_method'] === 'accept') {
    $form['input']['page_contribution_iban']['#required'] = FALSE;
    unset($storage['required_fields']['page_contribution_iban']);
  }
  else {
    $form['input']['page_contribution_iban']['#required'] = TRUE;
  }
}

/**
 * Form definition for payment page.
 */
function memberform_shared_page_payment_form(&$form, &$form_state) {
  $storage = $form_state['storage'];
  $form_name = $storage['form_name'];
  $is_member = $storage['values']['person_details']['page_person_details_spmember']['is_member'];

  switch ($form_name) {
    case 'spanning':
      if ($is_member) {
        $options = array(
          'auto_12' => 'Automatische incasso, € 12 per jaar (niet leden betalen € 20)',
          'accept_12' => 'Acceptgiro, € 12 per jaar (niet leden betalen € 24)',
        );
      }
      else {
        $options = array(
          'auto_20' => 'Automatische incasso, € 20 per jaar (€ 12 voor leden)',
          'accept_24' => 'Acceptgiro, € 24 per jaar (€ 12 voor leden)',
        );
      }
      break;

    case 'tribune':
      $options = array(
        'auto_5' => 'Automatische incasso, € 5 per kwartaal',
        'accept_24' => 'Acceptgiro, € 24 per jaar',
      );
      break;
  }

  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-input')),
  );

  $form['input']['page_payment_payment_method'] = array(
    '#title' => 'Betalingsgegevens',
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => $options,
    '#name' => 'page_payment_payment_method',
    '#ajax' => array(
      'wrapper' => 'memberform_form_wrapper',
      'callback' => 'memberform_ajax_callback',
    ),
    '#attributes' => array('class' => array('memberform-radio-button')),
  );

  $form['input']['page_payment_iban'] = array(
    '#title' => 'Wat is je bankrekeningnummer?',
    '#type' => 'textfield',
  );
}

/**
 * Form definition for gift page.
 *
 * Add images in module folder gift_images like book.jpg.
 */
function memberform_shared_page_gift_form(&$form, &$form_state) {
  $storage = $form_state['storage'];
  $memberships = array();
  $gift_options = array();
  $image_style = variable_get('memberform_gift_image_style', 'thumbnail');

  $memberships = memberform_shared_page_map_memberships($form_state);
  $translate = memberform_memberships();

  $form['input'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => array('memberform-input', 'memberform-input-gift')),
  );

  foreach ($memberships as $membership) {
    $gift_options = array();
    $gift_map = array();
    // Get gifts data.
    $select = "SELECT * FROM {memberform_gifts} WHERE membership_type = :membership AND show_website = 1 ORDER BY position";
    $gifts = db_query($select, array(':membership' => $membership))->fetchAll();
    $form_type = (count($gifts) > 4 ) ? 'select' : 'radios';

    foreach ($gifts as $key => $gift) {
      $image = file_load($gift->image);
      $gift_map[$gift->id] = $gift;
      $image_styled = empty($image) ? '' : theme('image_style', array('style_name' => $image_style, 'path' => $image->uri));
      // Create gift options for select list.
      if ($form_type === 'select') {
        $gift_options[$gift->id] = $gift->name;
      }
      else {
        $gift_options[$gift->id] = $image_styled . '<span>' . $gift->name . '</span>';
      }
    }

    $selected_gift_id = empty($storage['values']['gift']['page_gift_select_' . $membership]) ? '' : $storage['values']['gift']['page_gift_select_' . $membership];
    $selected_gift = $gift_map[$selected_gift_id];

    if ($form_type === 'select') {
      // Create image code.
      $image = file_load($selected_gift->image);
      $image_styled = empty($image) ? '' : theme('image_style', array('style_name' => $image_style, 'path' => $image->uri));
      $form['input']['page_gift_image_' . $selected_gift->id] = array(
        '#markup' => $image_styled,
      );
    }

    // Show gift selection form.
    $form['input']['page_gift_select_' . $membership] = array(
      '#title' => 'Welkomstgeschenk ' . $translate[$membership],
      '#type' => $form_type,
      '#required' => TRUE,
      '#options' => $gift_options,
      '#default_value' => $selected_gift_id,
      '#ajax' => array(
        'wrapper' => 'memberform_form_wrapper',
        'callback' => 'memberform_ajax_callback',
      ),
    );

    switch ($form_type) {
    case 'select':
      $form['input']['page_gift_select_' . $membership]['#empty_option'] = '- Maak een keuze -';
      break;
    case 'radios':
      $form['input']['page_gift_select_' . $membership]['#attributes']['class'][] = 'memberform-radio-button';
      break;
    }
  }
}

/**
 * Form definition for summary page.
 */
function memberform_shared_page_summary_form(&$form, &$form_state) {
  $markup = memberform_overview_table($form_state);
  $form['overview'] = array(
    '#markup' => $markup,
  );
}

/**
 * Shared validation for the diffent shared form elements.
 */
function memberform_shared_validation($form, &$form_state) {
  $values = &$form_state['values'];
  $storage = &$form_state['storage'];
  // Process trigger.
  $trigger = $form_state['triggering_element']['#name'];
  if ($form_state['validate']) {

    switch ($storage['current_page']) {
      case 'start':
        // Validate email.
        if (!empty($values['page_start_email'])) {
          if (!valid_email_address($values['page_start_email'])) {
            form_set_error('page_start_email', 'Geef een geldig e-mailadres op!');
          }
        }
        // Validate phone.
        if (!empty($values['page_start_phone'])) {
          $clean = preg_replace('/[^0-9]/', '', $values['page_start_phone']);
          if (strlen($clean) < 10) {
            form_set_error('page_start_phone', 'Geef een geldig telefoonnummer op!');
          }
        }
        // Validate contact information.
        if (empty($values['page_start_email']) && empty($values['page_start_phone'])) {
          form_set_error('page_start_phone', 'Geef tenminste een e-mailadres of telefoonnummer op zodat we je kunnen bereiken.');
        }
        // Validate name.
        $words_in_name = str_word_count($values['page_start_name'], 1);
        foreach ($words_in_name as $word) {
          if (strlen($word) < 2) {
            form_set_error('page_start_name', 'Vul uw volledige naam in, en niet slechts uw voorletter(s)');
          }
        }
        break;

      case 'person_details':
        if (isset($values['page_person_details_birth_date'])) {
          // Validate names.
          $name_types = array(
            'first' => 'voornaam',
            'middle' => 'tussenvoegsel',
            'last' => 'achternaam',);
          foreach ($name_types as $name_type => $name_type_display) {
            $words_in_name = str_word_count($values['page_person_details_' . $name_type . '_name'], 1);
            foreach ($words_in_name as $word) {
              if (strlen($word) < 2) {
                form_set_error('page_person_details_' . $name_type . '_name', 'Vul uw volledige ' . $name_type_display . ' in, en niet slechts letter(s)');
              }
            }
          }
          // Validate age.
          $age = memberform_get_age($values['page_person_details_birth_date']);
          if ($age < 14) {
            form_set_error('page_person_details_birth_date', 'Je bent jonger dan 14 jaar, en kunt daarom nog geen lid worden van de SP of ROOD.');
          }
        }
        break;

      case 'address':
        $values['page_address_street'] = $form_state['input']['page_address_street'];
        $values['page_address_locality'] = $form_state['input']['page_address_locality'];
        if ($values['page_address_country'] === 'NL') {
          // Cleanup postal code.
          if(preg_match('/^([1-9][0-9]{3})\s?([a-zA-Z]{2})$/', $values['page_address_postal_code'], $number_parts)) {
            $values['page_address_postal_code'] = $number_parts[1] . ' ' . strtoupper($number_parts[2]);
            //$storage['values']['address']['page_address_postal_code'] = $number_parts[1] . ' ' . strtoupper($number_parts[2]);
          }
        }
        $checks = array(
          'postal_code' => array(
            'regex' => '/^[1-9][0-9]{3}\s?[a-zA-Z]{2}$/',
            'message' => 'Vul een correcte postcode in',
          ),
          'house_number' => array(
            'regex' => '/^[1-9][0-9]*$/',
            'message' => 'Vul een geldig huisnummer in, een toevoeging kan in het veld hieronder',
          ),
          'street' => array(
            'regex' => '/[a-zA-Z]{2}/',
            'message' => 'Vul een geldige straatnaam in',
          ),
          'locality' => array(
            'regex' => '/[a-zA-Z]{2}/',
            'message' => 'Vul een geldige plaatsnaam in',
          ),
        );
        // Do no check International postal codes.
        if ($values['page_address_country'] !== 'NL') {
          unset($checks['postal_code']);
        }
        foreach ($checks as $key => $check) {
          if (!preg_match($check['regex'], $values['page_address_' . $key])) {
            form_set_error('page_address_' . $key, $check['message']);
          }
        }
        break;

      case 'contribution':
        if ($values['page_contribution_choice'] === 'custom') {
          $amount = $values['page_contribution_custom'];
          if ($amount === FALSE) {
            form_set_error('page_contribution_custom', 'Vul een getal in.');
          }
          else {
            if ($amount > 999999) {
              form_set_error('page_contribution_custom', 'Vul een kleiner bedrag in.');
            }
            if ($amount < 5) {
              form_set_error('page_contribution_custom', 'De minimumbijdrage is 5,- euro.');
            }
            $values['page_contribution_custom'] = $amount;
          }
        }
        $iban = strtoupper($values['page_contribution_iban']);
        $iban = preg_replace('/[^0-9A-Z]/', '', $iban);
        if ($storage['values']['address']['page_address_country'] === 'NL') {
          if (!empty($values['page_contribution_iban'])) {
            if (!memberform_check_iban($iban)) {
              form_set_error('page_contribution_iban', 'Vul een geldig iban nummer in.');
            }
            else {
              $values['page_contribution_iban'] = $iban;
            }
          }
        }
        break;

      case 'payment':
        $iban = strtoupper($values['page_payment_iban']);
        $iban = preg_replace('/[^0-9A-Z]/', '', $iban);
        if ($storage['values']['address']['page_address_country'] === 'NL') {
          if (!memberform_check_iban($iban)) {
            form_set_error('page_payment_iban', 'Vul een geldig iban nummer in.');
          }
          else {
            $values['page_payment_iban'] = $iban;
          }
        }
        break;
    }
  }
}

/**
 * Shared submit function for memberform pages.
 *
 * Set $storage['next_page'] to value of next page.
 */
function memberform_shared_submit($form, &$form_state) {
  if (isset($form_state['form_errors'])) {
    return;
  }

  $values = &$form_state['values'];
  $storage = &$form_state['storage'];

  // Process trigger.
  $trigger = $form_state['triggering_element']['#name'];

  switch ($storage['current_page']) {
    case 'intro':
      $storage['next_page'] = 'start';
    break;

    case 'start':
      // Set form navigation.
      if (empty($values['page_start_phone'])) {
        $storage['next_page'] = 'person_details';
      }
      else {
        $storage['next_page'] = 'quickfinish';
      }
      // Store hash name, value and created.
      $split_name = memberform_split_name($values['page_start_name']);
      $storage['values']['person_details']['page_person_details_first_name'] = $split_name['first_name'];
      $storage['values']['person_details']['page_person_details_middle_name'] = $split_name['middle_name'];
      $storage['values']['person_details']['page_person_details_last_name'] = $split_name['last_name'];
      $storage['hash'] = empty($storage['hash']) ? memberform_get_hash() : $storage['hash'];
      $storage['created'] = empty($storage['created']) ? time() : $storage['created'];
      break;

    case 'quickfinish':
      switch ($trigger) {
        case 'call_back':
          $storage['next_page'] = 'thanks';
          break;
        case 'next_page':
          $storage['next_page'] = 'person_details';
          break;
      }
      break;

    case 'person_details':
      // Store name.
      $middle_name = empty($values['page_person_details_middle_name']) ? '' : ' ' . $values['page_person_details_middle_name'];
      $last_name = empty($values['page_person_details_last_name']) ? '' : ' ' . $values['page_person_details_last_name'];
      $storage['values']['start']['page_start_name'] = $values['page_person_details_first_name'] . $middle_name . $last_name;
      if (!in_array($storage['form_name'], array('tribune', 'spanning'))) {
        $age = memberform_get_age($values['page_person_details_birth_date']);
        if ($age < 16) {
          $storage['membership_type'] = 'red_member';
          $storage['next_page'] = 'red';
        }
        elseif ($age <= 27) {
          $storage['membership_type'] = '';
          $storage['next_page'] = 'red';
        }
        else {
          $storage['next_page'] = 'address';
          $storage['membership_type'] = 'sp_member';
        }
      }
      else {
        $storage['next_page'] = 'address';
      }
      break;

    case 'red':
    if (empty($storage['membership_type'])) {
        $storage['membership_type'] = $values['page_red_membership'];
      }
      $storage['next_page'] = 'address';
      break;

    case 'address':
      $storage['next_page'] = 'contribution';
      $storage['address_fixed'] = true;
      break;

    case 'contribution':
      $storage['next_page'] = 'gift';
      // Store payment method and amount.
      if ($values['page_contribution_choice'] === 'custom') {
        $storage['payment_amount'] = $values['page_contribution_custom'];
      }
      else {
        $storage['payment_amount'] = $values['page_contribution_choice'];
      }
      $storage['payment_method'] = $values['page_contribution_payment_method'];
      $storage['bank_account'] = $values['page_contribution_iban'];
      break;

    case 'payment':
      $storage['next_page'] = 'summary';
      // Store payment method and amount.
      $payment_info = explode('_', $values['page_payment_payment_method']);
      $storage['payment_method'] = $payment_info[0];
      $storage['payment_amount'] = $payment_info[1];
      $storage['bank_account'] = $values['page_payment_iban'];
      break;

    case 'gift':
      $storage['next_page'] = 'summary';
      break;

    case 'summary':
      $storage['next_page'] = 'thanks';
      break;
  }

  // Store submission.
  $store_result = memberform_store_submission($form_state);

  if (!$store_result['submission_id']) {
    $form_state['redirect'] = array('aanmelden/fout');
  }
  else {
    $storage['sid'] = $store_result['submission_id'];
  }

  $fields = $store_result['fields'];
  if ($storage['current_page'] == 'summary' && !empty($fields['email']) && $trigger === 'next_page') {
    // Send confirmation mail when submitting summary page.
    $mail = drupal_mail(
      'memberform', 'verificatie', $fields['email'], LANGUAGE_NONE,
      array(
        'form_state'      => $form_state,
        'hash'            => $storage['hash'],
        'membership_type' => $fields['membership_type'],
        'format'          => SWIFTMAILER_FORMAT_HTML,
      )
    );
    if (empty($mail['send'])) {
      $storage['next_page'] = $storage['current_page'];
      drupal_set_message('Het is niet gelukt om de bevestigingsmail te versturen. Houdt dit probleem aan, neem dan contact op met webmaster@sp.nl', 'error');
    }
  }
}


/**
 * Store submitted values in database.
 */
function memberform_store_submission($form_state) {
  $storage = $form_state['storage'];
  $values = $storage['values'];
  $created = empty($storage['created']) ? time() : $storage['created'];

  // Set mobile, or phone.
  $phone = _hv($values['start']['page_start_phone']);
  if (preg_match('/^(((\+31|0|0031)6){1}[1-9]{1}[0-9]{7})$/', $phone)) {
    $mobile = $phone;
    $phone = NULL;
  }
  else {
    $mobile = NULL;
  }

  $fields = array(
    'membership_type' => _hv(memberform_shared_page_map_memberships($form_state, 'string')),
    'initials' => NULL,
    'firstname' => _hv($values['person_details']['page_person_details_first_name']),
    'middlename' => _hv($values['person_details']['page_person_details_middle_name']),
    'lastname' => _hv($values['person_details']['page_person_details_last_name']),
    'gender' => _hv($values['person_details']['page_person_details_gender']),
    'birthdate' => _hv($values['person_details']['page_person_details_birth_date']),
    'email' => _hv($values['start']['page_start_email']),
    'phone' => $phone,
    'phone_mobile' => $mobile,
    'country' => _hv($values['address']['page_address_country']),
    'zipcode' => _hv($values['address']['page_address_postal_code']),
    'housenumber' => _hv($values['address']['page_address_house_number']),
    'housenumber_suffix' => _hv($values['address']['page_address_house_number_addition']),
    'street' => _hv($values['address']['page_address_street']),
    'city' => _hv($values['address']['page_address_locality']),
    'contribution' => _hv($storage['payment_amount']),
    'bank_account' => _hv($storage['bank_account']),
    'payment_method' => _hv($storage['payment_method']),
    'welcome_present_sp' => _hv($values['gift']['page_gift_select_lid_sp']),
    'welcome_present_rood' => _hv($values['gift']['page_gift_select_lid_rood']),
    'source' => NULL,
    'created' => $created,
    'verified' => NULL,
    'reminded' => NULL,
    'submitted' => NULL,
    'errormsg' => NULL,
    'flags' => _hv($storage['flags']),
    'contact_id' => NULL,
    'note' => NULL,
    'form' => $storage['form_name'],
    'form_page' => $storage['current_page'],
  );

  db_merge('memberform_submissions')
    ->key(array(
      'hash' => $storage['hash'],
    ))
    ->fields($fields)
    ->execute();
  $submission_id = Database::getConnection()->lastInsertId();

  return array('submission_id' => $submission_id, 'fields' => $fields);
}
