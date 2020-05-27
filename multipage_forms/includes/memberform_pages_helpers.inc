<?php

/**
 * @file Helperfunctions for memberform pages.inc.
 */

/**
 * Splits name in first, middle, and last name part.
 *
 * @param string $name
 *   The name variable.
 *
 * @return array
 *   Split name parts.
 */
function memberform_split_name($name) {
  $matches = array();
  $name = trim($name);
  $name = trim($name, '.,');
  // Remove comma from name.
  $comma_pos = strpos($name, ',');
  if ($comma_pos !== FALSE) {
    $first_half = trim(substr($name, 0, $comma_pos));
    $last_half = trim(substr($name, $comma_pos + 1));
    $name = $last_half . ' ' . $first_half;
  }
  // Replace terms.
  $replace = array(
    'v' => 'van',
    'v.' => 'van',
    'vd' => 'van de',
    'vd.' => 'van de',
    'v.d.' => 'van de',
    'v/d' => 'van de',
  );
  foreach ($replace as $orig => $new) {
    if (strpos($name, ' ' . $orig . ' ') !== FALSE) {
      $name = str_replace($orig, $new, $name);
    }
  }
  // Regexes for title and middle names.
  $titles_regex = 'Mr|mr|Mevr|mevr|Dhr|dhr|Ir|ir|Ing|ing|Bc|bc|Ms|ms|Drs|drs|Dr|dr';
  $middle_names_array = memberform_middle_names();
  $middle_names_regex = implode('|', $middle_names_array);
  // Split name on middle_name.
  $regex = "@^(?'title'(\b(" . $titles_regex . ")\b\.?\s)?)(?'first_name'(([A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-\.]+)\s*)+?)(?'middle_name'(\b(" . $middle_names_regex . ")\b\s*))(?'last_name'(\b[A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'][a-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-]+\s?)+)$@";
  $result = preg_match($regex, $name, $matches);
  if (!preg_match($regex, $name, $matches)) {
    // Split name on middle name, last name first.
    $regex = "@^(?'title'(\b(" . $titles_regex . ")\b\.?\s)?)(?'last_name'(\b[A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'][A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-]+\s?)+),?\s(?'first_name'(([A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-\.]+)\s*)+?)(?'middle_name'(\b(" . $middle_names_regex . ")\b\s*))$@";
    $result = preg_match($regex, $name, $matches);
    if (!preg_match($regex, $name, $matches)) {
      // Split name without middle_name in first name and last name(s).
      $regex = "@^(?'title'(\b(" . $titles_regex . ")\b\.?\s)?)(?'first_name'(([A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-\.]+)\s*)+?)(?'last_name'(\b[A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'][A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-]+\s?)+)$@";
      $result = preg_match($regex, $name, $matches);
      if (!preg_match($regex, $name, $matches)) {
        // Split name in first name and last name, last name first.
        $regex = "@^(?'title'(\b(" . $titles_regex . ")\b\.?\s)?)(?'last_name'(\b[A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'][A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-]+\s?)+),?\s+(?'first_name'((([A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ']\.)+)\s*)+?)$@";
        $result = preg_match($regex, $name, $matches);
        if (!preg_match($regex, $name, $matches)) {
          // Check if only first name.
          $regex = "@^(?'first_name'([A-ZÀÈÌÒÙÁÉÍÓÚÝÂÊÎÔÛÃÑÕÄËÏÖÜŸÇÅa-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'][a-zàèìòùáéíóúýâêîôûãñõäëïöü¡¿çãñõ'\-]+))$@";
          $result = preg_match($regex, $name, $matches);
          if (!preg_match($regex, $name, $matches)) {
            // No match found.
            return FALSE;
          }
        }
      }
    }
  }
  // Set first character uppercase.
  $parts = array(
    'first_name',
    'last_name',
  );
  foreach ($parts as $part) {
    if (!empty($matches[$part])) {
      $matches[$part] = ucfirst($matches[$part]);
    }
  }
  foreach ($matches as $part => $value) {
    $matches[$part] = trim($value);
  }
  return $matches;
}

/**
 * Return array with middle names.
 *
 * @return array
 *   Middle names.
 */
function memberform_middle_names() {
  return array(
    "Uijt te de", "van van de", "uijt te de", "Voor in ‘t", "De van der", "de van der", "Van van de", "voor in ‘t", "Uit te de", "onder den", "Voor in t", "onder het", "van de l’", "De die le", "de die le", "Van de l’", "voor in t", "Onder het", "Onder den", "uit te de", "over het", "uijt den", "Onder de", "boven d’", "uijt ten", "Uijt ten", "over den", "Over den", "onder de", "onder ‘t", "Onder ‘t", "Van de l", "Boven d’", "van de l", "voor den", "Over het", "Uijt den", "Voor den", "Over ‘t", "Boven d", "Von dem", "boven d", "Von der", "von den", "Von den", "von der", "von dem", "Onder t", "van der", "Van ter", "Van gen", "van het", "van gen", "Van het", "van ter", "over ‘t", "van den", "Van den", "Van Der", "Van der", "onder t", "Van Den", "bij den", "auf dem", "Auf den", "Auf dem", "voor de", "uit het", "Uit het", "auf den", "Auf der", "Voor de", "voor ‘t", "auf ter", "Auf ter", "auf der", "aan het", "Aan het", "uijt de", "vor der", "Uijt de", "uijt ‘t", "Uijt ‘t", "Vor der", "Uit ten", "Aan der", "aan der", "aan den", "Aan den", "uit ten", "uit den", "Uit den", "aus der", "Bij het", "Bij den", "Voor ‘t", "over de", "Over de", "bij het", "Aus dem", "aus dem", "Aus den", "Aus der", "aus den", "in het", "die le", "In der", "in der", "In het", "Uit de", "van ‘t", "Van de", "van De", "van de", "Van ‘t", "Uit ‘t", "In den", "uit de", "uit ‘t", "in den", "Op ten", "op het", "Over t", "Op het", "Die le", "op gen", "over t", "op ten", "Von ‘t", "Bij ‘t", "aus ‘m", "bij ‘t", "Bij de", "von ‘t", "bij de", "Op gen", "op der", "Aan ‘t", "aan ‘t", "De las", "de las", "Van la", "van la", "Aan de", "aan de", "op den", "Op der", "De die", "de die", "Op den", "Aus ‘m", "Onder", "op ‘t", "Op ‘t", "Op de", "op de", "onder", "Uit t", "Van t", "De l’", "uit t", "Dalla", "de l’", "De la", "de le", "De le", "de la", "Von t", "bij t", "Am de", "aan t", "Aan t", "am de", "Aus m", "Bij t", "von t", "aus m", "Degli", "dalla", "van t", "In de", "in ‘t", "In ‘t", "della", "in de", "unter", "Unter", "Della", "degli", "deca", "uijt", "Over", "over", "Voor", "in t", "In t", "Uijt", "voor", "op t", "De l", "thoe", "de l", "thor", "Deca", "Thor", "Dal’", "Thoe", "Op t", "dal’", "zum", "Zur", "toe", "zur", "tot", "Toe", "Tot", "uit", "vom", "Von", "Vom", "ver", "Ver", "van", "Van", "vor", "Vor", "Uit", "von", "Zum", "Los", "Den", "den", "del", "Del", "Dei", "dei", "Der", "der", "dos", "Het", "Dos", "don", "Des", "des", "das", "Das", "Aus", "aus", "auf", "Auf", "Aan", "aan", "Ben", "ben", "Dal", "dal", "bin", "Bin", "Bij", "bij", "het", "Don", "Ter", "las", "ter", "ten", "Ten", "los", "les", "Les", "Tho", "Las", "tho", "d’", "‘T", "‘t", "D’", "Da", "da", "l’", "op", "S’", "‘s", "s’", "te", "af", "am", "Am", "al", "Af", "de", "Al", "zu", "Zu", "to", "Te", "De", "du", "El", "Du", "Le", "do", "Op", "el", "la", "in", "L’", "In", "im", "La", "Im", "Do", "To", "lo", "di", "‘S", "Of", "Lo", "le", "of", "Di", "A", "l", "S", "s", "L", "I", "T", "t", "D", "d", "i", "a",
  );
}

/**
 * Doe een postcode-lookup op basis van postcode en huisnummer.
 *
 * API-method van Jaap voor Pro6pp
 *
 * @param string $postcode
 *   Nederlandse postcode.
 * @param string $huisnummer
 *   Huisnummer.
 *
 * @return object
 *   Blah.
 */
function memberform_postcode_lookup($postcode, $huisnummer = '') {

  $api = spciviapi_get_api();
  $api->PostcodeNL->get([
    'postcode'   => $postcode,
    'huisnummer' => $huisnummer,
  ]);
  $res = $api->result();

  if ($res && !$res->is_error) {
    if (count($res->values) > 0) {
      return array_shift($res->values);
    }
    else {
      return 'Deze combinatie van postcode en huisnummer kon niet worden gevonden! Weet je zeker dat deze correct is?';
    }
  }
}


/**
 * Iban check.
 *
 *  From:
 *  http://monshouwer.org/code-snipets/check-iban-bank-account-number-in-php/
 *
 * @param string $iban
 *   Iban number to check.
 *
 * @return bool
 *   True if is valid, else false.
 */
function memberform_check_iban($iban) {
  // Normalize input (remove spaces and make upcase).
  $iban = strtoupper(str_replace(' ', '', $iban));

  if (preg_match('/^[A-Z]{2}[0-9]{2}[A-Z0-9]{1,30}$/', $iban)) {
    $country = substr($iban, 0, 2);
    $check = intval(substr($iban, 2, 2));
    $account = substr($iban, 4);

    // To numeric representation.
    $search = range('A', 'Z');
    foreach (range(10, 35) as $tmp) {
      $replace[] = strval($tmp);
    }
    $numstr = str_replace($search, $replace, $account . $country . '00');

    // Calculate checksum.
    $checksum = intval(substr($numstr, 0, 1));
    for ($pos = 1; $pos < strlen($numstr); $pos++) {
      $checksum *= 10;
      $checksum += intval(substr($numstr, $pos, 1));
      $checksum %= 97;
    }

    return ((98 - $checksum) == $check);
  }
  else {
    return FALSE;
  }
}

/**
 * Returns age.
 */
function memberform_get_age($birthdate) {
  $birthday_date = new DateTime($birthdate);
  $today   = new DateTime('today');
  $age = $birthday_date->diff($today)->y;
  return $age;
}


/**
 * Helper function to cleanup user input amount.
 */
function memberform_clean_amount($amount) {
  $amount = preg_replace('/[^0-9,.]/', '', $amount);
  $amount = str_replace(',', '.', $amount);
  if (!is_numeric($amount)) {
    return FALSE;
  }
  else {
    $amount = number_format($amount, 2, '.', '');
  }
  return $amount;
}

/**
 * Returns unique hash.
 */
function memberform_get_hash() {
  return md5(openssl_random_pseudo_bytes(6) . time());
}

/**
 * Helper function.
 */
function _hv($value) {
  return empty($value) ? NULL : $value;
}

/**
 * Maps form values to memberships array.
 */
function memberform_shared_page_map_memberships($form_state, $mapping_type = 'array') {
  switch ($mapping_type) {
    case 'array':
      $memberships = array();
      break;

    case 'string':
      $memberships = '';
      break;
  }

  if (!empty($form_state['storage']['membership_type'])) {
    $storage = $form_state['storage'];
    $memberships = array();
    if (isset($storage['membership_type'])) {
      switch ($storage['membership_type']) {
        case 'sp_and_red_member':
          switch ($mapping_type) {
            case 'array':
              $memberships = array(
                'lid_rood',
                'lid_sp',
              );
              break;

            case 'string':
              $memberships = 'lid_sprood';
              break;
          }
          break;

        case 'red_member':
          switch ($mapping_type) {
            case 'array':
              $memberships = array(
                'lid_rood',
              );
              break;

            case 'string':
              $memberships = 'lid_rood';
              break;
          }
          break;

        case 'sp_member':
          switch ($mapping_type) {
            case 'array':
              $memberships = array(
                'lid_sp',
              );
              break;

            case 'string':
              $memberships = 'lid_sp';
              break;
          }
          break;

        case 'tribune':
          switch ($mapping_type) {
            case 'array':
              $memberships = array(
                'tribune',
              );
              break;

            case 'string':
              $memberships = 'tribune';
              break;
          }
          break;

        case 'spanning':
          switch ($mapping_type) {
            case 'array':
              $memberships = array(
                'spanning',
              );
              break;

            case 'string':
              $memberships = 'spanning';
              break;
          }
          break;
      }
    }
  }
  return $memberships;
}

/**
 * Join a string with a natural language conjunction at the end.
 *
 * Https://gist.github.com/angry-dan/e01b8712d6538510dd9c.
 */
function memberform_natural_language_join(array $list, $conjunction = 'and') {
  $last = array_pop($list);
  if ($list) {
    return implode(', ', $list) . ' ' . $conjunction . ' ' . $last;
  }
  return $last;
}

/**
 * Creates overview table.
 *
 * @return string
 *   Table markup.
 */
function memberform_overview_table($form_state) {
  $storage = $form_state['storage'];
  $values = $storage['values'];
  $settings_callback = 'memberform_' . $storage['form_name'] . '_settings';
  $pages = $settings_callback('pages');

  $markup = '';
  $rows = array();

  // Loop through pages.
  $values = array_merge($storage['pages'], $values);
  foreach ($values as $page => $items) {
    if (in_array($page, $storage['nav_history'])) {
      $page_title = (empty($pages[$page])) ? '' : $pages[$page];
      $rows_tmp[] = array(
        'data' => array(
          0 => array(
            'data' => $page_title,
            'header' => TRUE,
            'colspan' => 2,
          ),
        ),
        'no_striping' => TRUE,
      );
      // Loop through form elements.
      if (is_array($items)) {
        foreach ($items as $key => $value) {
          if (!empty($storage['form_element_definitions'][$key])) {
            $form_element = $storage['form_element_definitions'][$key];
            $title = (empty($form_element['#title'])) ? '' : $form_element['#title'];
            if (strpos($key, 'page_') === 0) {
              // Parse output of different form element types.
              switch ($form_element['#type']) {
                case 'textfield':
                case 'emailfield':
                case 'telfield':
                  $rows_tmp[] = array($title, check_plain($value));
                  break;
                case 'numberfield':
                  $rows_tmp[] = array($title, str_replace('.', ',', memberform_clean_amount($value)));
                  break;

                case 'date_select':
                  $date = date("d-m-Y", strtotime($value));
                  $rows_tmp[] = array($title, $date);
                  break;

                case 'select':
                case 'radios':
                  $display = $form_element['#options'][$value];
                  $rows_tmp[] = array($title, preg_replace('/<img[^>]+\>/i', '', $display));
                  break;
              }
            }
          }
        }
      }
      // Do not output pages without interactive elements.
      if (count($rows_tmp) > 1) {
        $rows = array_merge($rows, $rows_tmp);
      }
      $rows_tmp = array();
    }
  }

  // Add membership info.
  if (!empty($storage['membership_type'])) {
    $memberships = memberform_shared_page_map_memberships($form_state);
    if (!empty($memberships)) {
      $rows[] = array(
        'data' => array(
          0 => array(
            'data' => 'Lidmaatschappen',
            'header' => TRUE,
            'colspan' => 2,
          ),
        ),
        'no_striping' => TRUE,
      );
      $titles = memberform_memberships();
        foreach ($memberships as $membership) {
        $membership_titles[] = $titles[$membership];
      }
      $memberships_line = memberform_natural_language_join($membership_titles, 'en');
      $rows[] = array('', $memberships_line);
    }
  }

  // Create table.
  $markup = theme('table', array('rows' => $rows));

  return $markup;
}

/**
 * Check postal code.
 */
function memberform_valid_pc($postal_code) {
  return preg_match('/^[1-9][0-9]{3}\s?[a-zA-Z]{2}$/', $postal_code);
}

/**
 * Check house number.
 */
function memberform_valid_nr($house_number) {
  return preg_match('/^[1-9][0-9]*$/', $house_number);
}
