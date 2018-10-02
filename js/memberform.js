(function($, Drupal) {
  Drupal.ajax.prototype.commands.memberform_ga = function(ajax, response, status) {
    if(window.ga && ga.create) {
      ga('set', { page: response.path, title: response.title });
      ga('send', 'pageview');
    }
  };
  Drupal.behaviors.memberformModule = {
    attach: function (context, settings) {
      // Code to be run on page load, and
      // on ajax load added here

      // Determine scroll
      var scrollPos = $('h1').first().offset().top;

      if(( $( ".messages.error" ).first().length)) {
        scrollPos = $('.messages.error').first().parent().offset().top;
        $('html, body').animate({ scrollTop: scrollPos },300);
      } else {
        // Check if new page.
        path = window.location.pathname;
        if(!path.includes('aanmelden/intern')) {
          if($( ".memberform-navigation" ).length) {
            scrollPos = $('.memberform-navigation').offset().top;
            $('html, body').animate({ scrollTop: scrollPos },300);
          }
        }
      }
    }
  };
})(jQuery, Drupal);
